// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "GAS/Abilities/Ability_WeaponAbility.h"

#include "Ability_ShootProjectile.generated.h"

// Forward declarations - GASMultiplayer
class UProjectileStaticData;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_ShootProjectile : public UAbility_WeaponAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

	/** Constructor */
	UAbility_ShootProjectile();

#pragma endregion INITIALIZATION

#pragma region ABILITY

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

#pragma endregion ABILITY

#pragma region PROJECTILE

protected:

	/** Projectile's static data class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TSubclassOf<UProjectileStaticData> ProjectileStaticDataClass;

#pragma region PROJECTILE
	
};
