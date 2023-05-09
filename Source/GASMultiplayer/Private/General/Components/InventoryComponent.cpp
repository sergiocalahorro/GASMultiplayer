// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/InventoryComponent.h"

// Unreal Engine
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

// GASMultiplayer
#include "General/Globals/DebugSystem.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/ItemStaticData.h"

#pragma region INITIALIZATION

/** Sets default values for this component's properties */
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Initializes the component. Occurs at level startup or actor spawn. This is before BeginPlay (Actor or Component) */
void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner()->HasAuthority())
	{
		for (const TSubclassOf<UItemStaticData> ItemClass : DefaultItems)
		{
			InventoryList.AddItem(ItemClass);
		}
	}
}

/** Method that allows an actor to replicate subobjects on its actor channel */
bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance.Get();
		if (IsValid(ItemInstance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		}
	}

	return bWroteSomething;
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, EquippedItem);
}

/** Called every frame */
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug
	const bool bShowDebug = CVarShowInventoryDebug.GetValueOnGameThread() != 0;
	if (!bShowDebug)
	{
		return;
	}
	
	for (FInventoryListItem& Item : InventoryList.GetItemsRef())
	{
		const UInventoryItemInstance* ItemInstance = Item.ItemInstance.Get();
		if (IsValid(ItemInstance))
		{
			if (const UItemStaticData* ItemStaticData = ItemInstance->GetItemStaticData())
			{
				GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString()));
			}
		}
	}
}

#pragma endregion OVERRIDES

#pragma region INVENTORY

/** Add item to inventory */
void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	InventoryList.AddItem(ItemStaticDataClass);
}

/** Add item to inventory */
void UInventoryComponent::AddItem(UInventoryItemInstance* InItem)
{
	InventoryList.AddItem(InItem->ItemStaticDataClass);
}

/** Remove item from inventory */
void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	InventoryList.RemoveItem(ItemStaticDataClass);
}

/** Remove item from inventory */
void UInventoryComponent::RemoveItem(UInventoryItemInstance* InItem)
{
	InventoryList.RemoveItem(InItem->ItemStaticDataClass);
}

/** Equip item */
void UInventoryComponent::EquipItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		for (FInventoryListItem& Item : InventoryList.GetItemsRef())
		{
			UInventoryItemInstance* ItemInstance = Item.ItemInstance;
			if (IsValid(ItemInstance) && ItemInstance->ItemStaticDataClass == ItemStaticDataClass)
			{
				ItemInstance->OnEquipped(GetOwner());
				EquippedItem = ItemInstance;
				break;
			}
		}
	}
}

/** Unequip item */
void UInventoryComponent::UnequipItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(EquippedItem))
		{
			EquippedItem->OnUnequipped();
			EquippedItem = nullptr;
		}
	}
}

/** Drop item */
void UInventoryComponent::DropItem()
{
	if (GetOwner()->HasAuthority())
	{
		if (IsValid(EquippedItem))
		{
			RemoveItem(EquippedItem);
			EquippedItem->OnDropped();
			EquippedItem = nullptr;
		}
	}
}

#pragma endregion INVENTORY
