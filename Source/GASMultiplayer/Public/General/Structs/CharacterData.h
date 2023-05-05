// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "CharacterData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayAbility;
class UGameplayEffect;

// Forward declarations - GASMultiplayer
class UCharacterAnimationDataAsset;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	/** Character's effects */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AA|GAS")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	/** Character's abilities */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AA|GAS")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	/** Animation data asset */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AA|Animation")
	TObjectPtr<UCharacterAnimationDataAsset> CharacterAnimationDataAsset;
};