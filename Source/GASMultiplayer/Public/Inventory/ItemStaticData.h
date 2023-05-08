// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ItemStaticData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASMULTIPLAYER_API UItemStaticData : public UObject
{
	GENERATED_BODY()

public:

	/** Item Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
};
