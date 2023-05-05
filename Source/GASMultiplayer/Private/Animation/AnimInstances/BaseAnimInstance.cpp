// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimInstances/BaseAnimInstance.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "General/DataAssets/CharacterAnimationDataAsset.h"

/** Get Locomotion BlendSpace's reference */
UBlendSpace* UBaseAnimInstance::GetLocomotionBlendSpace() const
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			return CharacterAnimationDataAsset->CharacterAnimationData.LocomotionBlendSpace;
		}
	}

	return DefaultCharacterAnimationDataAsset ? DefaultCharacterAnimationDataAsset->CharacterAnimationData.LocomotionBlendSpace : nullptr;
}

/** Get Idle Animation asset */
UAnimSequenceBase* UBaseAnimInstance::GetIdleAnimationAsset() const
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			return CharacterAnimationDataAsset->CharacterAnimationData.IdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimationDataAsset ? DefaultCharacterAnimationDataAsset->CharacterAnimationData.IdleAnimationAsset : nullptr;
}