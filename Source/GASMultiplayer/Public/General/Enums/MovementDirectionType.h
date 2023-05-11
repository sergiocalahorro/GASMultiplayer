// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "MovementDirectionType.generated.h"

UENUM(BlueprintType)
enum class EMovementDirectionType : uint8
{
	None,
	OrientToMovement,
	Strafe
};
