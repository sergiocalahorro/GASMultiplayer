// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "Inventory/Item/ItemStaticData.h"

#include "WeaponStaticData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UWeaponStaticData : public UItemStaticData
{
	GENERATED_BODY()

public:

	/** Damage gameplay effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Shooting distance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	float ShootingDistance = 10000.f;

	/** Asset for this weapon's skeletal mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	/** Asset for this weapon's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<UStaticMesh> StaticMesh;

	/** Weapon's attack animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<UAnimMontage> AttackMontage;
};
