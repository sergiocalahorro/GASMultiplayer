// Fill out your copyright notice in the Description page of Project Settings.

#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"

// GASMultiplayer
#include "Inventory/ItemStaticData.h"

/** Get item static data */
const UItemStaticData* UGASMultiplayerStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return GetDefault<UItemStaticData>(ItemDataClass);
	}

	return nullptr;
}