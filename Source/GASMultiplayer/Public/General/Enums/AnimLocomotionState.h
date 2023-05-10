// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "AnimLocomotionState.generated.h"

UENUM(BlueprintType)
enum class EAnimLocomotionState : uint8
{
	Default,
	Crouch
};
