// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/Inventory/InventoryComponent.h"

// Unreal Engine
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

// GASMultiplayer
#include "General/Globals/DebugSystem.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/ItemStaticData.h"

// Static variables
FGameplayTag UInventoryComponent::EquipItemActorTag;
FGameplayTag UInventoryComponent::UnequipItemTag;
FGameplayTag UInventoryComponent::DropItemTag;
FGameplayTag UInventoryComponent::EquipNextItemTag;

#pragma region INITIALIZATION

/** Sets default values for this component's properties */
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static bool bAddedTags = false;
	if (!bAddedTags)
	{
		bAddedTags = true;
		UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddUObject(this, &UInventoryComponent::AddInventoryTags);
	}
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

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipItemActorTag).AddUObject(this, &UInventoryComponent::GameplayEventCallBack);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::UnequipItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallBack);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::DropItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallBack);
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(UInventoryComponent::EquipNextItemTag).AddUObject(this, &UInventoryComponent::GameplayEventCallBack);
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
	DOREPLIFETIME(UInventoryComponent, InventoryTags);
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
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0, FColor::Blue, FString::Printf(TEXT("Item: %s"), *ItemStaticData->Name.ToString()));
			}
		}
	}

	const TArray<FFastArrayTagCounterRecord>& InventoryTagArray = InventoryTags.TagArray;
	for (FFastArrayTagCounterRecord TagRecord : InventoryTagArray)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0, FColor::Purple, FString::Printf(TEXT("Tag: %s | Count: %d"), *TagRecord.Tag.ToString(), TagRecord.Count));
	}
}

#pragma endregion OVERRIDES

#pragma region INVENTORY

/** Add item to inventory */
void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(ItemStaticDataClass);
	}
}

/** Add item instance to inventory */
void UInventoryComponent::AddItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		TArray<UInventoryItemInstance*> Items = InventoryList.GetAllAvailableInstancesOfClass(InItemInstance->ItemStaticDataClass);
		Algo::Sort(Items, [](const UInventoryItemInstance* InA, const UInventoryItemInstance* InB)
		{
			return InA->GetQuantity() < InB->GetQuantity();
		});

		const int32 MaxItemStackCount = InItemInstance->GetItemStaticData()->MaxStackCount;
		int32 ItemsLeft = InItemInstance->GetQuantity();

		for (UInventoryItemInstance* Item : Items)
		{
			const int32 EmptySlots = MaxItemStackCount - Item->GetQuantity();
			int32 SlotsToAdd = ItemsLeft;
			if (ItemsLeft > EmptySlots)
			{
				ItemsLeft = EmptySlots;
			}

			ItemsLeft -= SlotsToAdd;
			Item->AddItems(SlotsToAdd);
			InItemInstance->AddItems(-SlotsToAdd);

			for (FGameplayTag InventoryTag : Item->GetItemStaticData()->InventoryTags)
			{
				InventoryTags.AddTagCount(InventoryTag, SlotsToAdd);
			}

			if (ItemsLeft <= 0)
			{
				ItemsLeft = 0;
				return;
			}
		}

		while (ItemsLeft > MaxItemStackCount)
		{
			AddItem(InItemInstance->GetItemStaticData()->GetClass());
			for (FGameplayTag InventoryTag : InItemInstance->GetItemStaticData()->InventoryTags)
			{
				InventoryTags.AddTagCount(InventoryTag, MaxItemStackCount);
			}

			ItemsLeft -= MaxItemStackCount;
			InItemInstance->AddItems(-MaxItemStackCount);
		}
		
		InventoryList.AddItem(InItemInstance);
		
		for (FGameplayTag InventoryTag : InItemInstance->GetItemStaticData()->InventoryTags)
		{
			InventoryTags.AddTagCount(InventoryTag, InItemInstance->GetQuantity());
		}
	}
}

/** Remove item from inventory */
void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(ItemStaticDataClass);
	}
}

/** Remove item instance from inventory */
void UInventoryComponent::RemoveItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(InItemInstance);

		for (FGameplayTag InventoryTag : InItemInstance->GetItemStaticData()->InventoryTags)
		{
			InventoryTags.AddTagCount(InventoryTag, InItemInstance->GetQuantity());
		}
	}
}

/** Remove item from inventory by given tag */
void UInventoryComponent::RemoveItemWithInventoryTag(FGameplayTag Tag, int32 Count)
{
	if (GetOwner()->HasAuthority())
	{
		int32 CountLeft = Count;
		TArray<UInventoryItemInstance*> Items = InventoryList.GetAllInstancesWithTag(Tag);
		Algo::Sort(Items, [](const UInventoryItemInstance* InA, const UInventoryItemInstance* InB)
		{
			return InA->GetQuantity() < InB->GetQuantity();
		});

		for (UInventoryItemInstance* Item : Items)
		{
			int32 AvailableCount = Item->GetQuantity();
			int32 ItemsToRemove = CountLeft;

			if (ItemsToRemove >= AvailableCount)
			{
				ItemsToRemove = AvailableCount;
				RemoveItemInstance(Item);
			}
			else
			{
				Item->AddItems(-ItemsToRemove);

				for (FGameplayTag InventoryTag : Item->GetItemStaticData()->InventoryTags)
				{
					InventoryTags.AddTagCount(InventoryTag, -ItemsToRemove);
				}
			}

			CountLeft -= ItemsToRemove;
		}
	}
}

/** Get number of items in inventory with given tag */
int32 UInventoryComponent::GetInventoryTagCount(FGameplayTag Tag) const
{
	return InventoryTags.GetTagCount(Tag);
}

/** Add number of items in inventory by given tag */
void UInventoryComponent::AddInventoryTagCount(FGameplayTag Tag, int32 CountDelta)
{
	InventoryTags.AddTagCount(Tag, CountDelta);
}

/** Get list of item instances with given tag */
TArray<UInventoryItemInstance*> UInventoryComponent::GetAllInstancesWithTag(FGameplayTag Tag)
{
	return InventoryList.GetAllInstancesWithTag(Tag);
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

/** Equip item instance */
void UInventoryComponent::EquipItemInstance(UInventoryItemInstance* InItemInstance)
{
	if (GetOwner()->HasAuthority())
	{
		for (FInventoryListItem& Item : InventoryList.GetItemsRef())
		{
			UInventoryItemInstance* ItemInstance = Item.ItemInstance;
			if (IsValid(ItemInstance) && ItemInstance == InItemInstance)
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
			EquippedItem->OnUnequipped(GetOwner());
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
			RemoveItemInstance(EquippedItem);
			EquippedItem->OnDropped(GetOwner());
			EquippedItem = nullptr;
		}
	}
}

/** Equip next item */
void UInventoryComponent::EquipNextItem()
{
	TArray<FInventoryListItem>& Items = InventoryList.GetItemsRef();

	if (Items.IsEmpty() || (Items.Num() == 1 && EquippedItem))
	{
		return;
	}

	UInventoryItemInstance* NextItem = EquippedItem;

	for (FInventoryListItem Item : Items)
	{
		UInventoryItemInstance* ItemInstance = Item.ItemInstance;
		if (IsValid(ItemInstance) && ItemInstance->GetItemStaticData()->bCanBeEquipped)
		{
			if (ItemInstance != EquippedItem)
			{
				NextItem = ItemInstance;
				break;
			}
		}
	}

	if (EquippedItem)
	{
		if (NextItem == EquippedItem)
		{
			return;
		}
		UnequipItem();
	}

	EquipItemInstance(NextItem);
}

/** Gameplay Event callback */
void UInventoryComponent::GameplayEventCallBack(const FGameplayEventData* Payload)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		HandleGameplayEvent_Internal(*Payload);
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		ServerHandleGameplayEvent(*Payload);
	}
}

/** Add Gameplay tags */
void UInventoryComponent::AddInventoryTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	UInventoryComponent::EquipItemActorTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipItemActor"), TEXT("Equip item from item actor event"));
	UInventoryComponent::UnequipItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.UnequipItem"), TEXT("Drop equipped item"));
	UInventoryComponent::DropItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.DropItem"), TEXT("Unequip equipped item"));
	UInventoryComponent::EquipNextItemTag = TagsManager.AddNativeGameplayTag(TEXT("Event.Inventory.EquipNextItem"), TEXT("Try equipping next item"));
	TagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}

/** Handle Gameplay event */
void UInventoryComponent::HandleGameplayEvent_Internal(FGameplayEventData Payload)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (Payload.EventTag.MatchesTag(UInventoryComponent::EquipItemActorTag))
		{
			if (const UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Payload.OptionalObject))
			{
				AddItemInstance(const_cast<UInventoryItemInstance*>(ItemInstance));
				if (Payload.Instigator)
				{
					const_cast<AActor*>(Payload.Instigator.Get())->Destroy();
				}
			}
		}
		else if (Payload.EventTag.MatchesTag(UInventoryComponent::UnequipItemTag))
		{
			UnequipItem();
		}
		else if (Payload.EventTag.MatchesTag(UInventoryComponent::DropItemTag))
		{
			DropItem();
		}
		else if (Payload.EventTag.MatchesTag(UInventoryComponent::EquipNextItemTag))
		{
			EquipNextItem();
		}
	}
}

/** Handle Gameplay event (Server) */
void UInventoryComponent::ServerHandleGameplayEvent_Implementation(FGameplayEventData Payload)
{
	HandleGameplayEvent_Internal(Payload);
}

#pragma endregion INVENTORY
