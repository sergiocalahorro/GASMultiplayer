// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Ability_WeaponAbility.h"

#include "Ability_Aim.generated.h"

// Forward declarations - Unreal Engine
class UCameraModifier;
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_Aim : public UAbility_WeaponAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAbility_Aim();

#pragma endregion INITIALIZATION

#pragma region ABILITY

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion ABILITY

#pragma region AIM

private:

	/** Stop aim */
	UFUNCTION()
	void StopAim(FGameplayEventData Payload);

protected:

	/** Aim camera modifier's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Aim")
	TSubclassOf<UCameraModifier> AimCameraModifierClass;

	/** Tag to wait for in order to end aim ability */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Aim")
	FGameplayTag WaitEventTag;

	/** Aim camera modifier's reference */
	UPROPERTY()
	TObjectPtr<UCameraModifier> AimCameraModifier;

	/** Wait gameplay event task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;

#pragma endregion AIM
	
};
