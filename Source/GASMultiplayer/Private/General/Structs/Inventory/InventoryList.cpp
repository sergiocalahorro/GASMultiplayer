// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Structs/Inventory/InventoryList.h"

// GASMultiplayer
#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"
#include "Inventory/InventoryItemInstance.h"
#include "General/Structs/Inventory/InventoryListItem.h"
#include "Inventory/Item/ItemStaticData.h"

/** Add item to inventory */
void FInventoryList::AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(ItemStaticDataClass, UGASMultiplayerStatics::GetItemStaticData(ItemStaticDataClass)->MaxStackCount);
	MarkItemDirty(Item);
}

/** Add item to inventory */
void FInventoryList::AddItem(UInventoryItemInstance* InItemInstance)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = InItemInstance;
	MarkItemDirty(Item);
}

/** Remove item from inventory */
void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance->GetItemStaticData()->IsA(ItemStaticDataClass))
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

/** Remove item from inventory */
void FInventoryList::RemoveItem(UInventoryItemInstance* InItemInstance)
{
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance && Item.ItemInstance == InItemInstance)
		{
			ItemIter.RemoveCurrent();
			MarkArrayDirty();
			break;
		}
	}
}

/** Get all item instances with given tag */
TArray<UInventoryItemInstance*> FInventoryList::GetAllInstancesWithTag(FGameplayTag InTag)
{
	TArray<UInventoryItemInstance*> OutInstances;
	
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;
		if (Item.ItemInstance->GetItemStaticData()->InventoryTags.Contains(InTag))
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}
	
	return OutInstances;
}

/** Get all item instances of given class */
TArray<UInventoryItemInstance*> FInventoryList::GetAllAvailableInstancesOfClass(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	TArray<UInventoryItemInstance*> OutInstances;
	
	for (auto ItemIter = Items.CreateIterator(); ItemIter; ++ItemIter)
	{
		FInventoryListItem& Item = *ItemIter;

		const UItemStaticData* ItemStaticData = Item.ItemInstance->GetItemStaticData();
		const bool bSameClass = ItemStaticData->IsA(InItemStaticDataClass);
		const bool bHasCapacity = ItemStaticData->MaxStackCount > Item.ItemInstance->GetQuantity();

		if (bSameClass && bHasCapacity)
		{
			OutInstances.Add(Item.ItemInstance);
		}
	}
	
	return OutInstances;	
}