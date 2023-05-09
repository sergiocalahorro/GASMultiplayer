// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

// GASMultiplayer
#include "General/Enums/ItemState.h"

#include "ItemActor.generated.h"

// Forward declarations - Unreal Engine
class USphereComponent;

// Forward declarations - GASMultiplayer
class UInventoryItemInstance;

UCLASS()
class GASMULTIPLAYER_API AItemActor : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this actor's properties */
	AItemActor();

#pragma endregion INITIALIZATION

#pragma region COMPONENTS

protected:

	/** Static mesh */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/** Sphere trigger */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Components")
	TObjectPtr<USphereComponent> SphereComponent;

#pragma endregion COMPONENTS

#pragma region OVERRIDES

public:

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Method that allows an actor to replicate subobjects on its actor channel */
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region ITEM

public:
	
	/** Init ItemInstance */
	UFUNCTION()
	void Init(UInventoryItemInstance* InItemInstance);

	/** Functionality performed when the item is equipped */
	UFUNCTION()
	virtual void OnEquipped();

	/** Functionality performed when the item is unequipped */
	UFUNCTION()
	virtual void OnUnequipped();

	/** Functionality performed when the item is dropped */
	UFUNCTION()
	virtual void OnDropped();

	/** Functionality performed when some Actor enters Item's sphere */
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	/** Item instance */
	UPROPERTY(Replicated)
	TObjectPtr<UInventoryItemInstance> ItemInstance;

	/** Item state */
	UPROPERTY(Replicated)
	EItemState ItemState = EItemState::None;

	/** Event tag for Sphere's begin overlap */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Item|GAS")
	FGameplayTag SphereBeginOverlapEventTag;
	
#pragma endregion ITEM

};
