// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_UseWeapon.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "Inventory/Item/Weapon/WeaponItemActor.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region WEAPON

/** Get equipped weapon's static data */
const UWeaponStaticData* UAbility_UseWeapon::GetEquippedWeaponStaticData() const
{
	return Cast<UWeaponStaticData>(GetEquippedItemStaticData());
}

/** Get equipped weapon item Actor */
AWeaponItemActor* UAbility_UseWeapon::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}

/** Get damage effect spec handle */
FGameplayEffectSpecHandle UAbility_UseWeapon::GetWeaponEffectSpec(const FHitResult& InHitResult)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetBaseCharacterFromActorInfo()->GetAbilitySystemComponent())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedWeaponStaticData())
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle OutSpec = AbilitySystemComponent->MakeOutgoingSpec(WeaponStaticData->DamageEffect, 1.f, EffectContext);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(OutSpec, FGameplayTag::RequestGameplayTag(TEXT("Attribute.Health")), -WeaponStaticData->BaseDamage);
			return OutSpec;
		}
	}

	return FGameplayEffectSpecHandle();
}

#pragma endregion WEAPON
