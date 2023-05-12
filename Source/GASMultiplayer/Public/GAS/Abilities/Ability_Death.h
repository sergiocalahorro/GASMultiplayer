// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "Ability_Death.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_Death : public UBaseGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAbility_Death();

#pragma endregion INITIALIZATION

#pragma region ABILITY

protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

#pragma endregion ABILITY

#pragma region DEATH

protected:

	/** Time to destroy actor */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Death")
	float TimeToDestroyActor = 2.f;

#pragma endregion DEATH
	
};
