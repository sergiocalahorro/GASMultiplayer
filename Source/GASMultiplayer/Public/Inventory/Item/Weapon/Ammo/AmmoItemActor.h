// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item/ItemActor.h"
#include "AmmoItemActor.generated.h"

// Forward declarations - GASMultiplayer
class UAmmoStaticData;

UCLASS()
class GASMULTIPLAYER_API AAmmoItemActor : public AItemActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AAmmoItemActor();

#pragma endregion INITIALIZATION

#pragma region AMMO
	
public:
	
	/** Get ammo's static data */
	UFUNCTION()
	const UAmmoStaticData* GetAmmoStaticData() const;

protected:

	/** Client-side initialization (used for initializing items's visuals) */
	virtual void Init_Internal() override;

protected:

	/** Ammo's mesh */
	UPROPERTY()
	TObjectPtr<UMeshComponent> MeshComponent;

#pragma endregion AMMO

};
