// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// GASMultiplayer
#include "Inventory/InventoryList.h"

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
	void AddItem(UInventoryItemInstance* InItem);

	/** Remove item from inventory */
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);
	void RemoveItem(UInventoryItemInstance* InItem);

	/** Equip item */
	UFUNCTION(BlueprintCallable)
	void EquipItem(TSubclassOf<UItemStaticData> ItemStaticDataClass);

	/** Unequip item */
	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	/** Drop item */
	UFUNCTION(BlueprintCallable)
	void DropItem();

	/** Getter of EquippedItem */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetEquippedItem() const { return EquippedItem; }

protected:

	/** Inventory list */
	UPROPERTY(Replicated)
	FInventoryList InventoryList;

	/** Default items */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Inventory")
	TArray<TSubclassOf<UItemStaticData>> DefaultItems;

	/** Currently equipped item */
	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> EquippedItem;
	
#pragma endregion INVENTORY

};
