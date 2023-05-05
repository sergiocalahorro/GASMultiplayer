// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "BasePhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UBasePhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:

	/** Footstep sound */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FootstepSound;
};
