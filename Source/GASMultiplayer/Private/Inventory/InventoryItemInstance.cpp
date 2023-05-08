// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"

// GASMultiplayer
#include "Inventory/ItemStaticData.h"

#pragma region OVERRIDES

/** Replicate bEquipped */
void UInventoryItemInstance::OnRep_Equipped()
{
	
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UInventoryItemInstance, bEquipped);
}

#pragma endregion OVERRIDES

#pragma region ITEM

/** Initialize item instance */
void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

#pragma endregion ITEM