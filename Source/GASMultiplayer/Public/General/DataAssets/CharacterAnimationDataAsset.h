// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// GASMultiplayer
#include "General/Structs/CharacterData/CharacterAnimationData.h"

#include "CharacterAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UCharacterAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/** Character's animation data */
	UPROPERTY(EditDefaultsOnly)
	FCharacterAnimationData CharacterAnimationData;
};
