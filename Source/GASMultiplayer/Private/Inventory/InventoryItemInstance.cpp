// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

// GASMultiplayer
#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"
#include "Inventory/ItemActor.h"
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

	DOREPLIFETIME(UInventoryItemInstance, bEquipped);
	DOREPLIFETIME(UInventoryItemInstance, ItemStaticDataClass);
	DOREPLIFETIME(UInventoryItemInstance, ItemActor);
}

#pragma endregion OVERRIDES

#pragma region ITEM

/** Initialize item instance */
void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass)
{
	ItemStaticDataClass = InItemStaticDataClass;
}

/** Functionality performed when the item is equipped */
void UInventoryItemInstance::OnEquipped(AActor* InOwner)
{
	if (UWorld* World = InOwner->GetWorld())
	{
		if (const UItemStaticData* ItemStaticData = GetItemStaticData())
		{
			FTransform SpawnTransform;
			
			ItemActor = World->SpawnActorDeferred<AItemActor>(ItemStaticData->ItemActorClass, SpawnTransform, InOwner);
			ItemActor->Init(this);
			ItemActor->FinishSpawning(SpawnTransform);

			if (IsValid(ItemActor))
			{
				if (const ACharacter* Character = Cast<ACharacter>(InOwner))
				{
					ItemActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemStaticData->AttachmentSocketName);
				}
			}
		}
	}
}

/** Functionality performed when the item is unequipped */
void UInventoryItemInstance::OnUnequipped()
{
	if (IsValid(ItemActor))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
}

/** Functionality performed when the item is dropped */
void UInventoryItemInstance::OnDropped()
{
	if (IsValid(ItemActor))
	{
		ItemActor->OnDropped();
	}
}

/** Get ItemStaticData */
const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UGASMultiplayerStatics::GetItemStaticData(ItemStaticDataClass);
}

#pragma endregion ITEM