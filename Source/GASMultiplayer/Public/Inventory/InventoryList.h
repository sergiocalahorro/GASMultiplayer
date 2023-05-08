// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Inventory/InventoryListItem.h"

#include "InventoryList.generated.h"

// Forward declarations - GASMultiplayer
class UItemStaticData;

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	/** Net serialize delta this structure. Serialize a network delta from a base state */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo &DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryListItem, FInventoryList>(Items, DeltaParams, *this);
	}

	/** Add item to inventory */
	void AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

	/** Remove item from inventory */
	void RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

protected:

	/** Item list */
	UPROPERTY()
	TArray<FInventoryListItem> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum {WithNetDeltaSerializer = true};
};
