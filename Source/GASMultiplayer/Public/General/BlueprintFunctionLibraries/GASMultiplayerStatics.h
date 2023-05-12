// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GASMultiplayerStatics.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

// Forward declarations - GASMultiplayer
class UItemStaticData;
class UProjectileStaticData;
class ABaseProjectile;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UGASMultiplayerStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Get item static data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UItemStaticData* GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass);

	/** Apply radial damage */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ApplyRadialDamage(UObject* WorldContextObject, AActor* DamageCauser, FVector Location, float Radius, float DamageAmount, TArray<TSubclassOf<UGameplayEffect>> DamageEffects, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, ETraceTypeQuery TraceType);

	/** Launch projectile */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static ABaseProjectile* LaunchProjectile(UObject* WorldContextObject, TSubclassOf<UProjectileStaticData> ProjectileDataClass, FTransform Transform, AActor* Owner, APawn* Instigator);
};