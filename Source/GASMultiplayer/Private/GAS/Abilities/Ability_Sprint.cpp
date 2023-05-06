// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Sprint.h"

// Unreal Engine
#include "AbilitySystemComponent.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Sprint::UAbility_Sprint()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

#pragma endregion INITIALIZATION
