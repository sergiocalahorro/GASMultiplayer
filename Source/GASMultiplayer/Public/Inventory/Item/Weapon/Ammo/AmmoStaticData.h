// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Inventory/Item/ItemStaticData.h"

#include "AmmoStaticData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAmmoStaticData : public UItemStaticData
{
	GENERATED_BODY()

public:

	/** Asset for this ammo's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Ammo")
	TObjectPtr<UStaticMesh> StaticMesh;
};
