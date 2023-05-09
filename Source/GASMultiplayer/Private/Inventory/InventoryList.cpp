// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryList.h"

// GASMultiplayer
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryListItem.h"
#include "Inventory/ItemStaticData.h"

/** Add item to inventory */
void FInventoryList::AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	FInventoryListItem& Item = Items.AddDefaulted_GetRef();
	Item.ItemInstance = NewObject<UInventoryItemInstance>();
	Item.ItemInstance->Init(ItemStaticDataClass);
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