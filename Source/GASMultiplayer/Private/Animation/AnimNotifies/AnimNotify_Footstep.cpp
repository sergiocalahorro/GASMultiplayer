// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimNotifies/AnimNotify_Footstep.h"

#include "Character/BaseCharacter.h"

#pragma region INITIALIZATION

/** Constructor */
UAnimNotify_Footstep::UAnimNotify_Footstep(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 255, 0, 255);
#endif // WITH_EDITORONLY_DATA

	bIsNativeBranchingPoint = false;
}

#pragma endregion INITIALIZATION

/** Called when AnimNotify is reached */
void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	check(MeshComp);

	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		Character->HandleFootstep(Foot);
	}
}

/** Get the name for this notify */
FString UAnimNotify_Footstep::GetNotifyName_Implementation() const
{
	return UEnum::GetValueAsString(Foot);
}