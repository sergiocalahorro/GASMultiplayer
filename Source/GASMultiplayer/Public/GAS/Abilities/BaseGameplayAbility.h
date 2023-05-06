// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "BaseGameplayAbility.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region ABILITY

public:

	/** Get character's reference */
	UFUNCTION()
	ABaseCharacter* GetBaseCharacterFromActorInfo() const;

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion ABILITY
	
#pragma region EFFECTS

private:

	/** Apply gameplay effect and returns its active handle */
	FActiveGameplayEffectHandle ApplyActiveGameplayEffect(const TSubclassOf<UGameplayEffect>& GameplayEffect, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEffectContextHandle& EffectContext) const;

protected:

	/** Effects that will be applied when the ability starts */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyOnStart;

	/** Effects that will be removed when the ability ends */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToRemoveOnEnd;

private:

	/** Effects' handles to be removed when the ability ends */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectsHandlesToRemoveOnEnd;

#pragma endregion EFFECTS

};
