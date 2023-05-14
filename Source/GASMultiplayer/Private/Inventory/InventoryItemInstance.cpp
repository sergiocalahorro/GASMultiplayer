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
	DOREPLIFETIME(UInventoryItemInstance, Quantity);
}

#pragma endregion OVERRIDES

#pragma region ITEM

/** Initialize item instance */
void UInventoryItemInstance::Init(TSubclassOf<UItemStaticData> InItemStaticDataClass, int32 InQuantity)
{
	ItemStaticDataClass = InItemStaticDataClass;
	Quantity = InQuantity;
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
	TryApplyEffects(InOwner);
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
	TryRemoveEffects(InOwner);
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
	TryRemoveEffects(InOwner);
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

/** Try applying item's effects */
void UInventoryItemInstance::TryApplyEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		if (const UItemStaticData* ItemStaticData = GetItemStaticData())
		{
			const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			for (const TSubclassOf<UGameplayEffect>& GameplayEffect : ItemStaticData->OngoingEffects)
			{
				if (!GameplayEffect.Get())
				{
					continue;
				}

				FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
				if (EffectSpecHandle.IsValid())
				{
					const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
					if (ActiveEffectHandle.WasSuccessfullyApplied())
					{
						OngoingEffectsHandles.Add(ActiveEffectHandle);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UInventoryItemInstance::TryApplyEffects - Item %s failed to apply runtime effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
					}
				}
			}
		}
	}
}

/** Try removing item's effects */
void UInventoryItemInstance::TryRemoveEffects(AActor* InOwner)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		for (const FActiveGameplayEffectHandle& ActiveEffectHandle : OngoingEffectsHandles)
		{
			if (ActiveEffectHandle.IsValid())
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}
		OngoingEffectsHandles.Empty();
	}
}

/** Add given amount of items */
void UInventoryItemInstance::AddItems(int32 Count)
{
	Quantity += Count;

	if (Quantity < 0)
	{
		Quantity = 0;
	}
}

#pragma endregion ITEM