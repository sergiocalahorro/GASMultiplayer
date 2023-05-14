// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"

#include "GameplayCue_Actor_Burn.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API AGameplayCue_Actor_Burn : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Constructor */
	AGameplayCue_Actor_Burn();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called when a GameplayCue with duration is first activated, this will only be called if the client witnessed the activation */
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

	/** Called when a GameplayCue with duration is removed */
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

#pragma endregion OVERRIDES

#pragma region BURN

protected:

	/** Fire particles */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Burn")
	TObjectPtr<UParticleSystem> FireParticleSystem;

private:

	/** Spawned fire particles' component reference */
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> FireParticleComponent;

#pragma endregion BURN

};
