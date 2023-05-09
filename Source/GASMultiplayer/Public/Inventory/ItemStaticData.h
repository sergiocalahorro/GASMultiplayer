﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ItemStaticData.generated.h"

// Forward declarations - GASMultiplayer
class AItemActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASMULTIPLAYER_API UItemStaticData : public UObject
{
	GENERATED_BODY()

public:

	/** Item Name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item")
	FName Name;

	/** Item Actor's class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item")
	TSubclassOf<AItemActor> ItemActorClass;

	/** Component's socket name where the item will be attached */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item")
	FName AttachmentSocketName = NAME_None;
};
