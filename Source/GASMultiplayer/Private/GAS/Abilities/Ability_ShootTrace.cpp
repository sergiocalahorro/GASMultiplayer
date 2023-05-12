// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_ShootTrace.h"

// Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/EngineTypes.h"

// GASMultiplayer
#include "Inventory/Item/Weapon/WeaponItemActor.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region SHOOT

/** Shoot */
void UAbility_ShootTrace::Shoot(FGameplayEventData Payload)
{
	const UWeaponStaticData* WeaponData = GetEquippedWeaponStaticData();

	FHitResult TraceHit;
	if (GetWeaponToFocusTraceResult(WeaponData->ShootingDistance, WeaponData->WeaponTraceChannel, TraceHit))
	{
		if (UAbilitySystemComponent* ShotActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TraceHit.GetActor()))
		{
			const FGameplayEffectContextHandle DamageEffectContext = ShotActorAbilitySystemComponent->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = GetWeaponEffectSpec(TraceHit);
			ShotActorAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	GetEquippedWeaponItemActor()->PlayWeaponEffects(TraceHit);
}

#pragma endregion SHOOT
