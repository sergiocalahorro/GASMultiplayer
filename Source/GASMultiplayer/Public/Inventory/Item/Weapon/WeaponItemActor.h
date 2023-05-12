// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Inventory/Item/ItemActor.h"

#include "WeaponItemActor.generated.h"

// Forward declarations - GASMultiplayer
class UWeaponStaticData;

UCLASS()
class GASMULTIPLAYER_API AWeaponItemActor : public AItemActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	AWeaponItemActor();

#pragma endregion INITIALIZATION

#pragma region WEAPON

public:

	/** Get weapon's static data */
	UFUNCTION()
	const UWeaponStaticData* GetWeaponStaticData() const;

	/** Get weapon's muzzle location */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMuzzleLocation() const;

	/** Play weapon's effects */
	UFUNCTION(BlueprintCallable)
	void PlayWeaponEffects(const FHitResult& InHitResult, bool bUsePhysicalMaterial = true);

protected:

	/** Play weapon's effects (both in clients and server) */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponEffects(const FHitResult& InHitResult, bool bUsePhysicalMaterial = true);
	
	/** Play weapon's effects (internal) */
	UFUNCTION()
	void PlayWeaponEffects_Internal(const FHitResult& InHitResult, bool bUsePhysicalMaterial = true);

	/** Client-side initialization (used for initializing items's visuals) */
	virtual void Init_Internal() override;

protected:

	/** Weapon's mesh */
	UPROPERTY()
	TObjectPtr<UMeshComponent> MeshComponent;

#pragma endregion WEAPON

};
