// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_WeaponAbility.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
FGameplayEffectSpecHandle UAbility_WeaponAbility::GetWeaponEffectSpec(const FHitResult& InHitResult) const
{
	if (const UAbilitySystemComponent* AbilitySystemComponent = GetBaseCharacterFromActorInfo()->GetAbilitySystemComponent())
	{
		if (const UWeaponStaticData* WeaponStaticData = GetEquippedWeaponStaticData())
		{
			const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
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
	if (APlayerCameraManager* PlayerCameraManager = GetActorInfo().PlayerController->PlayerCameraManager)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(GetAvatarActorFromActorInfo());

		// Debug
		static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
		const bool bShowCustomDebug = CVar->GetInt() > 0;
		EDrawDebugTrace::Type DebugDrawType = bShowCustomDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		
		// Make first trace, from camera location to the direction it is looking at, to obtain the focus hit
		FHitResult FocusHit;
		const FVector FocusTraceStart = PlayerCameraManager->GetCameraLocation();
		const FVector FocusTraceEnd = FocusTraceStart + PlayerCameraManager->GetCameraRotation().Vector() * TraceDistance;
		UKismetSystemLibrary::LineTraceSingle(this, FocusTraceStart, FocusTraceEnd, TraceType, false, IgnoredActors, DebugDrawType, FocusHit, true);

		// Make second trace, from weapon's muzzle location in the direction the focus hit
		const FVector WeaponTraceStart = GetEquippedWeaponItemActor()->GetMuzzleLocation();
		const FVector WeaponTraceEnd = WeaponTraceStart + (FocusHit.Location - WeaponTraceStart).GetSafeNormal() * TraceDistance;
		UKismetSystemLibrary::LineTraceSingle(this, WeaponTraceStart, WeaponTraceEnd, TraceType, false, IgnoredActors, DebugDrawType, OutHitResult, true);

		return OutHitResult.bBlockingHit;
	}

	return false;
}

#pragma endregion WEAPON
