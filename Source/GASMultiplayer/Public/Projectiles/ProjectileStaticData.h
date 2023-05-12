// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "ProjectileStaticData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;
class UNiagaraSystem;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GASMULTIPLAYER_API UProjectileStaticData : public UObject
{
	GENERATED_BODY()

public:

	/** Base damage value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Damage")
	float BaseDamage;
	
	/** Damage radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Damage")
	float DamageRadius;
	
	/** Effects applied on this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Damage")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	/** Object types affected by this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Damage")
	TArray<TEnumAsByte<EObjectTypeQuery>> RadialDamageQueryTypes;

	/** Trace channel used on this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Damage")
	TEnumAsByte<ETraceTypeQuery> RadialDamageTraceType;

	/** Initial speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	float InitialSpeed = 3000.f;

	/** Maximum speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	float MaxSpeed = 3000.f;

	/** Gravity scale */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	float GravityScale = 1.f;

	/** Whether initial velocity is set in local space */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	bool bInitialVelocityInLocalSpace = false;

	/** Whether rotation follows velocity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	bool bRotationFollowsVelocity = true;

	/** Whether projectile should bounce */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement")
	bool bShouldBounce = false;
	
	/** Bounciness */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|Movement", meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "bShouldBounce", EditConditionHides))
	float Bounciness = 0.f;

	/** Asset for this projectile's static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|FX")
	TObjectPtr<UStaticMesh> StaticMesh;
	
	/** Particles spawned when this projectile stops  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|FX")
	TObjectPtr<UNiagaraSystem> OnStopVFX;

	/** Sound effect played when this projectile stops  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AA|Projectile|FX")
	TObjectPtr<USoundBase> OnStopSFX;
};
