// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemInstance.h"

// Unreal Engine
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// GASMultiplayer
#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"
#include "Inventory/Item/ItemActor.h"
#include "Inventory/Item/ItemStaticData.h"

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
			ItemActor->OnEquipped();
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

	TryGrantAbilities(InOwner);
	bEquipped = true;
}

/** Functionality performed when the item is unequipped */
void UInventoryItemInstance::OnUnequipped(AActor* InOwner)
{
	if (IsValid(ItemActor))
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}

	TryRemoveAbilities(InOwner);
	bEquipped = false;
}

/** Functionality performed when the item is dropped */
void UInventoryItemInstance::OnDropped(AActor* InOwner)
{
	if (IsValid(ItemActor))
	{
		ItemActor->OnDropped();
	}

	TryRemoveAbilities(InOwner);
	bEquipped = false;
}

/** Get ItemStaticData */
const UItemStaticData* UInventoryItemInstance::GetItemStaticData() const
{
	return UGASMultiplayerStatics::GetItemStaticData(ItemStaticDataClass);
}

/** Get ItemActor */
AItemActor* UInventoryItemInstance::GetItemActor() const
{
	return ItemActor;
}

/** Try granting item's abilities */
void UInventoryItemInstance::TryGrantAbilities(AActor* InOwner)
{
	if (IsValid(InOwner) && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			const UItemStaticData* ItemStaticData = GetItemStaticData();
			for (TSubclassOf<UGameplayAbility> ItemAbilityClass : ItemStaticData->GrantedAbilities)
			{
				GrantedAbilityHandles.Add(AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ItemAbilityClass)));
			}
		}
	}
}

/** Try removing item's abilities */
void UInventoryItemInstance::TryRemoveAbilities(AActor* InOwner)
{
	if (IsValid(InOwner) && InOwner->HasAuthority())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
		{
			for (FGameplayAbilitySpecHandle ItemAbilitySpecHandle : GrantedAbilityHandles)
			{
				AbilitySystemComponent->ClearAbility(ItemAbilitySpecHandle);
			}
			
			GrantedAbilityHandles.Empty();
		}
	}
}

#pragma endregion ITEM