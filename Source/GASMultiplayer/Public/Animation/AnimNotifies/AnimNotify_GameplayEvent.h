// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "AnimNotify_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
public:

	/** Called when AnimNotify is reached */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

#pragma endregion OVERRIDES

protected:

	/** Gameplay event data */
	UPROPERTY(EditAnywhere)
	FGameplayEventData Payload;
};
