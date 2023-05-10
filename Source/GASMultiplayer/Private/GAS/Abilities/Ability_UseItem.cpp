// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_UseItem.h"

// GASMultiplayer
#include "General/Components/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/ItemStaticData.h"
#include "Inventory/Item/ItemActor.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"
#include "Inventory/Item/Weapon/WeaponItemActor.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_UseItem::UAbility_UseItem()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Called when the ability is given to an AbilitySystemComponent */
void UAbility_UseItem::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InventoryComponent = ActorInfo->OwnerActor.Get()->FindComponentByClass<UInventoryComponent>();
}

#pragma endregion ABILITY

#pragma region ITEM

/** Getter of InventoryComponent */
UInventoryComponent* UAbility_UseItem::GetInventoryComponent() const
{
	return InventoryComponent;
}

/** Get equipped item instance */
UInventoryItemInstance* UAbility_UseItem::GetEquippedItemInstance() const
{
	return InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
}

/** Get equipped item's static data */
const UItemStaticData* UAbility_UseItem::GetEquippedItemStaticData() const
{
	const UInventoryItemInstance* EquippedItemInstance = GetEquippedItemInstance();
	return EquippedItemInstance ? EquippedItemInstance->GetItemStaticData() : nullptr;
}

/** Get equipped weapon's static data */
const UWeaponStaticData* UAbility_UseItem::GetEquippedWeaponStaticData() const
{
	return Cast<UWeaponStaticData>(GetEquippedItemStaticData());
}

/** Get equipped item Actor */
AItemActor* UAbility_UseItem::GetEquippedItemActor() const
{
	const UInventoryItemInstance* EquippedItemInstance = GetEquippedItemInstance();
	return EquippedItemInstance ? EquippedItemInstance->GetItemActor() : nullptr;
}

/** Get equipped weapon item Actor */
AWeaponItemActor* UAbility_UseItem::GetEquippedWeaponItemActor() const
{
	return Cast<AWeaponItemActor>(GetEquippedItemActor());
}

#pragma endregion ITEM
