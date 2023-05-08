﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "InventoryItemInstance.generated.h"

// Forward declarations - GASMultiplayer
class UItemStaticData;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()

#pragma region OVERRIDES

public:

	/** IsSupportedForNetworking means an object can be referenced over the network */
	virtual bool IsSupportedForNetworking() const override { return true; }

protected:

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region ITEM

public:

	/** Initialize item instance */
	UFUNCTION()
	virtual void Init(TSubclassOf<UItemStaticData> InItemStaticDataClass);

	/** Replicate bEquipped */
	UFUNCTION()
	void OnRep_Equipped();

	/** Functionality performed when the item is equipped */
	UFUNCTION()
	virtual void OnEquipped() PURE_VIRTUAL(UItemStaticData::OnEquipped);

	/** Functionality performed when the item is unequipped */
	UFUNCTION()
	virtual void OnUnequipped() PURE_VIRTUAL(UItemStaticData::OnUnequipped);

public:

	/** Item class */
	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	/** Whether the time is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool bEquipped = false;

#pragma endregion ITEM
	
};