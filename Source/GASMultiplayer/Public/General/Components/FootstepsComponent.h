// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// GASMultiplayer
#include "General/Enums/Foot.h"

#include "FootstepsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASMULTIPLAYER_API UFootstepsComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this component's properties */
	UFootstepsComponent();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region FOOTSTEP
	
public:

	/** Handle footstep with given foot */
	UFUNCTION()
	void HandleFootstep(EFoot Foot) const;

private:

	/** Socket name for left foot */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Footstep")
	FName LeftFootSocketName = FName("foot_l");

	/** Socket name for right foot */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Footstep")
	FName RightFootSocketName = FName("foot_r");

	/** Distance to perform footstep's trace */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Footstep", meta = (ClampMin = 0.f, UIMin = 0.f))
	float TraceDistance = 50.f;

#pragma endregion FOOTSTEP

};
