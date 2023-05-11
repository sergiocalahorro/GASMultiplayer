// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Ability_UseWeapon.h"

#include "Ability_RapidFire.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_RapidFire : public UAbility_UseWeapon
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Constructor */
	UAbility_RapidFire();

#pragma endregion INITIALIZATION

#pragma region ABILITY

protected:

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion ABILITY

#pragma region RAPID_FIRE

protected:

	/** Activate ability from event (must be called from Blueprint-side) */
	UFUNCTION(BlueprintCallable)
	void ActivateAbilityFromEvent_Internal(const FGameplayEventData& EventData, FGameplayTag WaitEventTag);

	/** Shoot */
	UFUNCTION()
	void Shoot();

	/** Stop shooting */
	UFUNCTION()
	void StopShooting(FGameplayEventData Payload);

private:

	/** Play montage and wait task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageAndWaitTask;

	/** Wait gameplay event task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;

	/** Shooting timer handle */
	UPROPERTY()
	FTimerHandle ShootingTimerHandle;

#pragma endregion RAPID_FIRE

};
