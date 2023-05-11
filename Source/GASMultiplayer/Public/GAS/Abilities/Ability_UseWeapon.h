// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Ability_UseItem.h"

#include "Ability_UseWeapon.generated.h"

// Forward declarations - GASMultiplayer
class UWeaponStaticData;
class AWeaponItemActor;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_UseWeapon : public UAbility_UseItem
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
	FGameplayEffectSpecHandle GetWeaponEffectSpec(const FHitResult& InHitResult);

#pragma endregion WEAPON
	
};
