// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_ItemAbility.h"

// GASMultiplayer
#include "General/Components/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/ItemStaticData.h"
#include "Inventory/Item/ItemActor.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_ItemAbility::UAbility_ItemAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Called when the ability is given to an AbilitySystemComponent */
void UAbility_ItemAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
}

#pragma endregion ABILITY

#pragma region ITEM

/** Getter of InventoryComponent */
UInventoryComponent* UAbility_ItemAbility::GetInventoryComponent() const
{
	return InventoryComponent;
}

/** Get equipped item instance */
UInventoryItemInstance* UAbility_ItemAbility::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

/** Get equipped item's static data */
const UItemStaticData* UAbility_ItemAbility::GetEquippedItemStaticData() const
{
	const UInventoryItemInstance* EquippedItemInstance = GetEquippedItemInstance();
	return EquippedItemInstance ? EquippedItemInstance->GetItemStaticData() : nullptr;
}

/** Get equipped item Actor */
AItemActor* UAbility_ItemAbility::GetEquippedItemActor() const
{
	const UInventoryItemInstance* EquippedItemInstance = GetEquippedItemInstance();
	return EquippedItemInstance ? EquippedItemInstance->GetItemActor() : nullptr;
}

#pragma endregion ITEM
