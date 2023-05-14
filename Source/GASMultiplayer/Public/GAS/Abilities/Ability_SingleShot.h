// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Ability_ShootTrace.h"

#include "Ability_SingleShot.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_SingleShot : public UAbility_ShootTrace
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Constructor */
	UAbility_SingleShot();

#pragma endregion INITIALIZATION

#pragma region ABILITY

protected:

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion ABILITY

#pragma region SINGLE_SHOT

protected:

	/** Activate ability from event (must be called from Blueprint-side) */
	virtual void ActivateAbilityFromEvent_Internal(const FGameplayEventData& EventData, FGameplayTag WaitEventTag) override;

	/** Shoot */
	virtual void Shoot(FGameplayEventData Payload) override;

private:

	/** Play montage and wait task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageAndWaitTask;

	/** Wait gameplay event task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;

#pragma endregion SINGLE_SHOT

};
