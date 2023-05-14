// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "InventoryListItem.generated.h"

// Forward declarations - GASMultiplayer
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct FInventoryListItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:

	/** Item instance */
	UPROPERTY()
	TObjectPtr<UInventoryItemInstance> ItemInstance;
};
