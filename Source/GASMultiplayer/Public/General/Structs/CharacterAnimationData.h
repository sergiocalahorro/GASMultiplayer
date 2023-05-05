// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "CharacterAnimationData.generated.h"

// Forward declarations - Unreal Engine
class UBlendSpace;

USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
	GENERATED_USTRUCT_BODY()

	/** Locomotion BlendSpace's reference */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBlendSpace> LocomotionBlendSpace;

	/** Idle Animation asset */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequenceBase> IdleAnimationAsset;
};