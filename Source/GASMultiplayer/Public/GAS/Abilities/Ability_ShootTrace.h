// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Ability_WeaponAbility.h"

#include "Ability_ShootTrace.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_ShootTrace : public UAbility_WeaponAbility
{
	GENERATED_BODY()

#pragma region SHOOT

protected:

	/** Activate ability from event (must be called from Blueprint-side) */
	UFUNCTION(BlueprintCallable)
	virtual void ActivateAbilityFromEvent_Internal(const FGameplayEventData& EventData, FGameplayTag WaitEventTag) PURE_VIRTUAL(UAbility_WeaponAbility::ActivateAbilityFromEvent_Internal);

	/** Shoot */
	UFUNCTION()
	virtual void Shoot(FGameplayEventData Payload);

#pragma endregion SHOOT
};
