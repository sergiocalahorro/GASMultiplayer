// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

// GASMultiplayer
#include "General/Structs/Inventory/InventoryList.h"
#include "General/Structs/Utils/FastArrayTagCounter.h"

#include "InventoryComponent.generated.h"

// Forward declarations - GASMultiplayer
class UItemStaticData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASMULTIPLAYER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this component's properties */
	UInventoryComponent();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Initializes the component. Occurs at level startup or actor spawn. This is before BeginPlay (Actor or Component) */
	virtual void InitializeComponent() override;

	/** Method that allows an actor to replicate subobjects on its actor channel */
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region INVENTORY

public:

	/** Add item to inventory */
	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

	/** Add item instance to inventory */
	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UInventoryItemInstance* InItemInstance);

	/** Remove item from inventory */
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

	/** Remove item from inventory */
	UFUNCTION(BlueprintCallable)
	void RemoveItemInstance(UInventoryItemInstance* InItemInstance);

	/** Remove item from inventory by given tag */
	UFUNCTION(BlueprintCallable)
	void RemoveItemWithInventoryTag(FGameplayTag Tag, int32 Count = 1);

	/** Get number of items in inventory with given tag */
	UFUNCTION(BlueprintCallable)
	int32 GetInventoryTagCount(FGameplayTag Tag) const;

	/** Add number of items in inventory by given tag */
	UFUNCTION(BlueprintCallable)
	void AddInventoryTagCount(FGameplayTag Tag, int32 CountDelta);

	/** Get list of item instances with given tag */
	UFUNCTION(BlueprintCallable)
	TArray<UInventoryItemInstance*> GetAllInstancesWithTag(FGameplayTag Tag);
	
	/** Equip item */
	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

	/** Equip item instance */
	UFUNCTION(BlueprintCallable)
	void EquipItemInstance(UInventoryItemInstance* InItemInstance);

	/** Unequip item */
	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	/** Drop item */
	UFUNCTION(BlueprintCallable)
	void DropItem();

	/** Equip next item */
	UFUNCTION(BlueprintCallable)
	void EquipNextItem();

	/** Getter of EquippedItem */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetEquippedItem() const { return EquippedItem; }

	/** Gameplay Event callback */
	virtual void GameplayEventCallBack(const FGameplayEventData* Payload);

protected:

	/** Add Gameplay tags */
	UFUNCTION()
	void AddInventoryTags();

	/** Handle Gameplay event */
	void HandleGameplayEvent_Internal(FGameplayEventData Payload);

	/** Handle Gameplay event (Server) */
	UFUNCTION(Server, Reliable)
	void ServerHandleGameplayEvent(FGameplayEventData Payload);

public:
	
	/** Equip item actor tag */
	static FGameplayTag EquipItemActorTag;

	/** Unequip item tag */
	static FGameplayTag UnequipItemTag;

	/** Drop item tag */
	static FGameplayTag DropItemTag;

	/** Equip next item tag */
	static FGameplayTag EquipNextItemTag;

protected:

	/** Inventory list */
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	/** Inventory tags */
	UPROPERTY(Replicated)
	FFastArrayTagCounter InventoryTags;

	/** Default items */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Inventory")
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	/** Currently equipped item */
	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> EquippedItem;

	/** Gameplay tag delegate handle */
	FDelegateHandle TagDelegateHandle;
	
#pragma endregion INVENTORY

};
