// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "InventoryItemInstance.generated.h"

// Forward declarations - GASMultiplayer
class UItemStaticData;
class AItemActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
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

	/** Functionality performed when the item is equipped */
	UFUNCTION()
	virtual void OnEquipped(AActor* InOwner = nullptr);

	/** Functionality performed when the item is unequipped */
	UFUNCTION()
	virtual void OnUnequipped(AActor* InOwner = nullptr);

	/** Functionality performed when the item is dropped */
	UFUNCTION()
	virtual void OnDropped(AActor* InOwner = nullptr);

	/** Get ItemStaticData */
	UFUNCTION()
	const UItemStaticData* GetItemStaticData() const;

	/** Get ItemActor */
	UFUNCTION()
	AItemActor* GetItemActor() const;

protected:
	
	/** Replicate bEquipped */
	UFUNCTION()
	void OnRep_Equipped();

	/** Try granting item's abilities */
	UFUNCTION()
	void TryGrantAbilities(AActor* InOwner);

	/** Try removing item's abilities */
	UFUNCTION()
	void TryRemoveAbilities(AActor* InOwner);

	/** Try applying item's effects */
	UFUNCTION()
	void TryApplyEffects(AActor* InOwner);

	/** Try removing item's effects */
	UFUNCTION()
	void TryRemoveEffects(AActor* InOwner);
	
public:

	/** Item class */
	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticData> ItemStaticDataClass;

	/** Whether the time is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_Equipped)
	bool bEquipped = false;

protected:

	/** Item Actor's reference */
	UPROPERTY(Replicated)
	TObjectPtr<AItemActor> ItemActor;

	/** Handles for granted abilities */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	
	/** Effects' handles to be removed when the item is unequipped */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> OngoingEffectsHandles;

#pragma endregion ITEM
	
};
