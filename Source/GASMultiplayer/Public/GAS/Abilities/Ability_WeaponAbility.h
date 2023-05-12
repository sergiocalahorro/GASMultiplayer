// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Ability_ItemAbility.h"

#include "Ability_WeaponAbility.generated.h"

// Forward declarations - GASMultiplayer
class UWeaponStaticData;
class AWeaponItemActor;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_WeaponAbility : public UAbility_ItemAbility
{
	GENERATED_BODY()

#pragma region WEAPON
	
public:

	/** Get equipped weapon's static data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UWeaponStaticData* GetEquippedWeaponStaticData() const;

	/** Get equipped weapon item Actor */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponItemActor* GetEquippedWeaponItemActor() const;

	/** Get damage effect spec handle */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayEffectSpecHandle GetWeaponEffectSpec(const FHitResult& InHitResult) const;

	/** Get hit from weapon's trace to focus */
	UFUNCTION(BlueprintPure)
	const bool GetWeaponToFocusTraceResult(float TraceDistance, ETraceTypeQuery TraceType, FHitResult& OutHitResult) const;

#pragma endregion WEAPON
	
};
