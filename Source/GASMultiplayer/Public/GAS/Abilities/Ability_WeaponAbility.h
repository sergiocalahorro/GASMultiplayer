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

#pragma region ABILITY

public:

	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:

	/** Commits resources/cooldowns etc. */
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr);

#pragma endregion ABILITY

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
	
	/** Check whether owner of the ability has enough ammo to perform ability */
	UFUNCTION(BlueprintPure)
	bool HasEnoughAmmo() const;

	/** Decrease ammo after using weapon */
	UFUNCTION()
	void DecreaseAmmo();
	
#pragma endregion WEAPON
	
};
