// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimNotifies/AnimNotify_GameplayEvent.h"

// Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"

#pragma region OVERRIDES
	
/** Called when AnimNotify is reached */
void UAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), Payload.EventTag, Payload);
}

#pragma endregion OVERRIDES