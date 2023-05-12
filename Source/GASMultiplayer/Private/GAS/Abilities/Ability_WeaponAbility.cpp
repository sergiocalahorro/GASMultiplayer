// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_WeaponAbility.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "Inventory/Item/Weapon/WeaponItemActor.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region WEAPON

/** Get equipped weapon's static data */
const UWeaponStaticData* UAbility_WeaponAbility::GetEquippedWeaponStaticData() const
{
	return Cast<UWeaponStaticData>(GetEquippedItemStaticData());
}

/** Get equipped weapon item Actor */
AWeaponItemActor* UAbility_WeaponAbility::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}

/** Get damage effect spec handle */
FGameplayEffectSpecHandle UAbility_WeaponAbility::GetWeaponEffectSpec(const FHitResult& InHitResult)
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

/** Get hit from weapon's trace to focus */
const bool UAbility_WeaponAbility::GetWeaponToFocusTraceResult(float TraceDistance, ETraceTypeQuery TraceType, FHitResult& OutHitResult) const
{
	AWeaponItemActor* WeaponItemActor = GetEquippedWeaponItemActor();
	ABaseCharacter* Character = GetBaseCharacterFromActorInfo();

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetAvatarActorFromActorInfo());

	// Make first trace, from camera location forward to obtain the focus hit
	FHitResult FocusHit;
	const FTransform CameraTransform = Character->GetFollowCamera()->GetComponentTransform();
	const FVector FocusTraceEnd = CameraTransform.GetLocation() + CameraTransform.GetRotation().Vector() * TraceDistance;
	UKismetSystemLibrary::LineTraceSingle(this, CameraTransform.GetLocation(), FocusTraceEnd, TraceType, false, IgnoredActors, EDrawDebugTrace::ForDuration, FocusHit, true);

	// Make second trace, from weapon's muzzle location in the direction the focus hit
	const FVector MuzzleLocation = WeaponItemActor->GetMuzzleLocation();
	const FVector WeaponTraceEnd = MuzzleLocation + (FocusHit.Location - MuzzleLocation).GetSafeNormal() * TraceDistance;
	UKismetSystemLibrary::LineTraceSingle(this, MuzzleLocation, WeaponTraceEnd, TraceType, false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHitResult, true);

	return OutHitResult.bBlockingHit;
}

#pragma endregion WEAPON
