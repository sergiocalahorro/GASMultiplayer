// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// GASMultiplayer
#include "General/Structs/CharacterData/CharacterData.h"

#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASMULTIPLAYER_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Character's data */
	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
};
