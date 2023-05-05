// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

// GASMultiplayer
#include "General/Enums/Foot.h"

#include "AnimNotify_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:

	/** Constructor */
	UAnimNotify_Footstep(const FObjectInitializer& ObjectInitializer);

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
public:

	/** Called when AnimNotify is reached */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** Get the name for this notify */
	virtual FString GetNotifyName_Implementation() const override;

private:

	/** Foot used on this step */
	UPROPERTY(EditAnywhere)
	EFoot Foot;

#pragma endregion OVERRIDES

};
