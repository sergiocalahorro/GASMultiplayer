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

/** Get Crouch Locomotion BlendSpace's reference */
UBlendSpace* UBaseAnimInstance::GetCrouchLocomotionBlendSpace() const
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			return CharacterAnimationDataAsset->CharacterAnimationData.CrouchLocomotionBlendSpace;
		}
	}

	return DefaultCharacterAnimationDataAsset ? DefaultCharacterAnimationDataAsset->CharacterAnimationData.CrouchLocomotionBlendSpace : nullptr;
}

/** Get Crouch Idle Animation asset */
UAnimSequenceBase* UBaseAnimInstance::GetCrouchIdleAnimationAsset() const
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			return CharacterAnimationDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset;
		}
	}

	return DefaultCharacterAnimationDataAsset ? DefaultCharacterAnimationDataAsset->CharacterAnimationData.CrouchIdleAnimationAsset : nullptr;
}