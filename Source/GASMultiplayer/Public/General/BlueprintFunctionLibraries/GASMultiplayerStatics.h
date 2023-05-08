// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GASMultiplayerStatics.generated.h"

// Forward declarations - GASMultiplayer
class UItemStaticData;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UGASMultiplayerStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Get item static data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass);
};
