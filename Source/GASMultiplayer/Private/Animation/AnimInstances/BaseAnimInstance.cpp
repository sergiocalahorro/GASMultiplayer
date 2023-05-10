// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimInstances/BaseAnimInstance.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "General/DataAssets/CharacterAnimationDataAsset.h"
#include "Inventory/Item/ItemStaticData.h"

/** Get Locomotion BlendSpace */
UBlendSpace* UBaseAnimInstance::GetLocomotionBlendSpace(const EAnimLocomotionState LocomotionState) const
{
	UBlendSpace* LocomotionBlendSpace = *DefaultCharacterAnimationDataAsset->CharacterAnimationData.LocomotionBlendSpace.Find(LocomotionState);

	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
		{
			LocomotionBlendSpace = *ItemStaticData->CharacterAnimationData.LocomotionBlendSpace.Find(LocomotionState);
		}
		else if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			LocomotionBlendSpace = *CharacterAnimationDataAsset->CharacterAnimationData.LocomotionBlendSpace.Find(LocomotionState);
		}
	}

	return LocomotionBlendSpace;
}

/** Get Idle Animation */
UAnimSequenceBase* UBaseAnimInstance::GetIdleAnimation(const EAnimLocomotionState LocomotionState) const
{
	UAnimSequenceBase* IdleAnimation = *DefaultCharacterAnimationDataAsset->CharacterAnimationData.IdleAnimation.Find(LocomotionState);

	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		if (const UItemStaticData* ItemStaticData = GetEquippedItemStaticData())
		{
			IdleAnimation = *ItemStaticData->CharacterAnimationData.IdleAnimation.Find(LocomotionState);
		}
		else if (const UCharacterAnimationDataAsset* CharacterAnimationDataAsset = Character->GetCharacterData().CharacterAnimationDataAsset)
		{
			IdleAnimation = *CharacterAnimationDataAsset->CharacterAnimationData.IdleAnimation.Find(LocomotionState);
		}
	}

	return IdleAnimation;
}

/** Get equipped item's static data */
const UItemStaticData* UBaseAnimInstance::GetEquippedItemStaticData() const
{
	const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor());
	const UInventoryComponent* InventoryComponent = Character ? Character->GetInventoryComponent() : nullptr;
	const UInventoryItemInstance* ItemInstance = InventoryComponent ? InventoryComponent->GetEquippedItem() : nullptr;
	
	return ItemInstance ? ItemInstance->GetItemStaticData() : nullptr;
}