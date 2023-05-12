// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseProjectile.generated.h"

// Forward declarations - Unreal Engine
class UProjectileMovementComponent;

// Forward declarations - GASMultiplayer
class UProjectileStaticData;

UCLASS()
class GASMULTIPLAYER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	ABaseProjectile();
	
#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when the game ends or when destroyed */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region PROJECTILE

public:

	/** Setter of ProjectileStaticDataClass */
	UFUNCTION()
	void SetProjectileStaticDataClass(TSubclassOf<UProjectileStaticData> InProjectileStaticDataClass) { ProjectileStaticDataClass = InProjectileStaticDataClass; }
	
	/** Get projectile's static data */
	UFUNCTION()
	const UProjectileStaticData* GetProjectileStaticData() const;

private:

	/** Initialize projectile with data values */
	UFUNCTION()
	void InitializeProjectile();

	/** Function called on projectile stopped */
	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

	/** Debug: Draw projectile's path */
	UFUNCTION()
	void DebugDrawPath() const;

private:

	/** Projectile's static data class */
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "AA|Projectile")
	TSubclassOf<UProjectileStaticData> ProjectileStaticDataClass;

	/** Projectile's static data class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** Projectile's mesh */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Projectile")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

#pragma endregion PROJECTILE

};
