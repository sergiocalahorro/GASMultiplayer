// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "General/Enums/WeaponType.h"
#include "Inventory/Item/ItemStaticData.h"

#include "WeaponStaticData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UNiagaraSystem;

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
	EWeaponType WeaponType = EWeaponType::Trace;
	
	/** Weapon trace channel to apply damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TEnumAsByte<ETraceTypeQuery> WeaponTraceChannel = TraceTypeQuery3;

	/** Damage gameplay effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Base damage value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	float BaseDamage;

	/** Fire rate (amount of shots performed in 1s) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon", meta = (UIMin = 1.f, ClampMin = 1.f, EditCondition = "WeaponType == EWeaponType::Trace", EditConditionHides))
	float FireRate = 1.f;

	/** Shooting distance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon", meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "WeaponType == EWeaponType::Trace", EditConditionHides, Units = "cm"))
	float ShootingDistance = 10000.f;

	/** Projectile's spawn offset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon", meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "WeaponType == EWeaponType::Projectile", EditConditionHides, Units = "cm"))
	float SpawnOffset = 150.f;

	/** Asset for this weapon's skeletal mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	/** Asset for this weapon's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<UStaticMesh> StaticMesh;

	/** Weapon's attack animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Weapon's attack sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<USoundBase> AttackSound;

	/** Weapon's attack VFX */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Item|Weapon")
	TObjectPtr<UNiagaraSystem> AttackVFX;
};
