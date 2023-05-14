// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "FastArrayTagCounterRecord.generated.h"

USTRUCT(BlueprintType)
struct FFastArrayTagCounterRecord : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()

	/** Item tag */
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	/** Item count */
	UPROPERTY(EditAnywhere)
	int32 Count;
};