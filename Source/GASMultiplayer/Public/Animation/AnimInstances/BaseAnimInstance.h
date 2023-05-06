// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "BaseAnimInstance.generated.h"

class UCharacterAnimationDataAsset;
/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	/** Get Locomotion BlendSpace's reference */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UBlendSpace* GetLocomotionBlendSpace() const;

	/** Get Idle Animation sequence */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequenceBase* GetIdleAnimationAsset() const;

	/** Get Crouch Locomotion BlendSpace's reference */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UBlendSpace* GetCrouchLocomotionBlendSpace() const;

	/** Get Crouch Idle Animation sequence */
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequenceBase* GetCrouchIdleAnimationAsset() const;

protected:

	/** Default animation data asset, used for preventing the Character from appearing on T-Pose until it's initialized */
	UPROPERTY(EditAnywhere, Category = "AA|Animation")
	TObjectPtr<UCharacterAnimationDataAsset> DefaultCharacterAnimationDataAsset;
};
