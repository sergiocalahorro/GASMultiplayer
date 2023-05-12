// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Death.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Death::UAbility_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Actually activate ability, do not call this directly */
void UAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetAvatarActorFromActorInfo()->SetLifeSpan(TimeToDestroyActor);
}

#pragma endregion ABILITY