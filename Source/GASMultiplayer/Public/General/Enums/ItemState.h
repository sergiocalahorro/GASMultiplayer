// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "ItemState.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None,
	Equipped,
	Dropped
};
