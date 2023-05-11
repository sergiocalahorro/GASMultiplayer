// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/BaseGameplayAbility.h"

// Unreal Engine
#include "AbilitySystemComponent.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"

#pragma region ABILITY

/** Actually activate ability, do not call this directly */
void UBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();

	// Apply effects on startup
	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : EffectsToApplyOnStart)
	{
		if (!GameplayEffect.Get())
		{
			continue;
		}
		
		const FActiveGameplayEffectHandle ActiveEffectHandle = ApplyActiveGameplayEffect(GameplayEffect, ActorInfo, EffectContext);
		if (!ActiveEffectHandle.WasSuccessfullyApplied())
		{
			UE_LOG(LogTemp, Error, TEXT("UBaseGameplayAbility::ActivateAbility - Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
		}
	}

	// Apply runtime effects and store their active handles for removing them on ability end
	if (IsInstantiated())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : EffectsToRemoveOnEnd)
		{
			if (!GameplayEffect.Get())
			{
				continue;
			}
			
			const FActiveGameplayEffectHandle ActiveEffectHandle = ApplyActiveGameplayEffect(GameplayEffect, ActorInfo, EffectContext);
			if (ActiveEffectHandle.WasSuccessfullyApplied())
			{
				EffectsHandlesToRemoveOnEnd.Add(ActiveEffectHandle);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UBaseGameplayAbility::ActivateAbility - Ability %s failed to apply runtime effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
			}
		}
	}
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsInstantiated())
	{
		for (const FActiveGameplayEffectHandle& ActiveEffectHandle : EffectsHandlesToRemoveOnEnd)
		{
			if (ActiveEffectHandle.IsValid())
			{
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}
		EffectsHandlesToRemoveOnEnd.Empty();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/** Get character's reference */
ABaseCharacter* UBaseGameplayAbility::GetBaseCharacterFromActorInfo() const
{
	return Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
}

#pragma endregion ABILITY

#pragma region EFFECTS

/** Apply gameplay effect and returns its active handle */
FActiveGameplayEffectHandle UBaseGameplayAbility::ApplyActiveGameplayEffect(const TSubclassOf<UGameplayEffect>& GameplayEffect, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEffectContextHandle& EffectContext) const
{
	FActiveGameplayEffectHandle ActiveEffectHandle;
	if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		if (GameplayEffect.Get())
		{
			const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
			if (EffectSpecHandle.IsValid())
			{
				ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			}
		}
	}
	
	return ActiveEffectHandle;
}

#pragma endregion EFFECTS
