// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Item/Weapon/Ammo/AmmoItemActor.h"

// GASMultiplayer
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/Weapon/Ammo/AmmoStaticData.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AAmmoItemActor::AAmmoItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

#pragma endregion INITIALIZATION

#pragma region AMMO
	
/** Get ammo's static data */
const UAmmoStaticData* AAmmoItemActor::GetAmmoStaticData() const
{
	return ItemInstance ? Cast<UAmmoStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}

/** Client-side initialization (used for initializing items's visuals) */
void AAmmoItemActor::Init_Internal()
{
	if (const UAmmoStaticData* AmmoStaticData = GetAmmoStaticData())
	{
		if (AmmoStaticData->StaticMesh)
		{
			if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("MeshComponent")))
			{
				StaticMeshComponent->RegisterComponent();
				StaticMeshComponent->SetStaticMesh(AmmoStaticData->StaticMesh);
				StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MeshComponent = StaticMeshComponent;
			}
		}
	}
}

#pragma endregion AMMO