// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"

#include "AbilityVolumeEnteredActorInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityVolumeEnteredActorInfo
{
	GENERATED_USTRUCT_BODY()

	/** Abilities applied to Actor that entered the volume */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AppliedAbilities;

	/** Effects applied to Actor that entered the volume */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> AppliedEffects;
};