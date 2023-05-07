﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Jump.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Jump::UAbility_Jump()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Returns true if this ability can be activated right now. Has no side effects */
bool UAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed))
	{
		return Character->CanJump();
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/** Actually activate ability, do not call this directly */
void UAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		if (ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
		{
			Character->Jump();
		}
	}
}

#pragma endregion ABILITY
