// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "General/Enums/AnimLocomotionState.h"

#include "BaseAnimInstance.generated.h"

// Forward declarations - GASMultiplayer
class UCharacterAnimationDataAsset;
class UItemStaticData;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	
	/** Get Locomotion BlendSpace */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UBlendSpace* GetLocomotionBlendSpace(EAnimLocomotionState LocomotionState) const;

	/** Get Idle Animation */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequenceBase* GetIdleAnimation(EAnimLocomotionState LocomotionState) const;
	
	/** Get equipped item's static data */
	const UItemStaticData* GetEquippedItemStaticData() const;

protected:

	/** Default animation data asset, used for preventing the Character from appearing on T-Pose until it's initialized */
	UPROPERTY(EditAnywhere, Category = "AA|Animation")
	TObjectPtr<UCharacterAnimationDataAsset> DefaultCharacterAnimationDataAsset;
};
