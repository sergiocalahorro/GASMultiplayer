// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "General/Enums/AnimLocomotionState.h"

#include "CharacterAnimationData.generated.h"

// Forward declarations - Unreal Engine
class UBlendSpace;

USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
	GENERATED_USTRUCT_BODY()

public:

	/** Locomotion BlendSpace */
	UPROPERTY(EditDefaultsOnly)
	TMap<EAnimLocomotionState, TObjectPtr<UBlendSpace>> LocomotionBlendSpace;

	/** Idle Animation */
	UPROPERTY(EditDefaultsOnly)
	TMap<EAnimLocomotionState, TObjectPtr<UAnimSequenceBase>> IdleAnimation;
};