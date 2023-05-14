// Fill out your copyright notice in the Description page of Project Settings.

#include "Volumes/AbilitySystemPhysicsVolume.h"

// Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#pragma region INITIALIZATION

/** Sets default values */
AAbilitySystemPhysicsVolume::AAbilitySystemPhysicsVolume()
{
	PrimaryActorTick.bCanEverTick = true;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when actor enters a volume */
void AAbilitySystemPhysicsVolume::ActorEnteredVolume(AActor* Other)
{
	Super::ActorEnteredVolume(Other);

	if (!HasAuthority())
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other))
	{
		FAbilityVolumeEnteredActorInfo ActorInfo;
		ActorInfo.AppliedAbilities.Append(GiveAbilities(AbilitySystemComponent, OngoingAbilitiesToGiveOnEnter));
		ActorInfo.AppliedEffects.Append(ApplyEffects(AbilitySystemComponent, EffectsToApplyOnEnter));

		GiveAbilities(AbilitySystemComponent, PermanentAbilitiesToGiveOnEnter);
		SendEventTags(Other, EventTagsToSendOnEnter);
		EnteredActorsInfoMap.Add(Other, ActorInfo);
	}
}

/** Called when actor leaves a volume */
void AAbilitySystemPhysicsVolume::ActorLeavingVolume(AActor* Other)
{
	Super::ActorLeavingVolume(Other);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other))
	{
		if (FAbilityVolumeEnteredActorInfo* ActorInfo = EnteredActorsInfoMap.Find(Other))
		{
			RemoveAbilities(AbilitySystemComponent, ActorInfo->AppliedAbilities);
			RemoveEffects(AbilitySystemComponent, ActorInfo->AppliedEffects);

			EnteredActorsInfoMap.Remove(Other);
		}
		
		ApplyEffects(AbilitySystemComponent, EffectsToApplyOnExit);
		SendEventTags(Other, EventTagsToSendOnExit);
	}
}

/** Called every frame */
void AAbilitySystemPhysicsVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Debug
	if (bDrawDebug)
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), GetBounds().BoxExtent, FColor::Red, false, 0.f, 5);
	}
}

#pragma endregion OVERRIDES

#pragma region ABILITY_SYSTEM

/** Give abilities */
TArray<FGameplayAbilitySpecHandle> AAbilitySystemPhysicsVolume::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	for (TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpecHandle AbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		AbilityHandles.Add(AbilityHandle);
	}

	return AbilityHandles;
}

/** Remove abilities */
void AAbilitySystemPhysicsVolume::RemoveAbilities(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FGameplayAbilitySpecHandle>& AbilityHandles)
{
	for (FGameplayAbilitySpecHandle AbilityHandle : AbilityHandles)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
	}
}

/** Apply effects */
TArray<FActiveGameplayEffectHandle> AAbilitySystemPhysicsVolume::ApplyEffects(UAbilitySystemComponent* AbilitySystemComponent, const TArray<TSubclassOf<UGameplayEffect>>& Effects)
{
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	for (TSubclassOf<UGameplayEffect> Effect : Effects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddInstigator(AbilitySystemComponent->GetOwnerActor(), AbilitySystemComponent->GetOwnerActor());

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (ActiveEffectHandle.WasSuccessfullyApplied())
			{
				EffectHandles.Add(ActiveEffectHandle);
			}
		}
	}

	return EffectHandles;
}

/** Remove effects */
void AAbilitySystemPhysicsVolume::RemoveEffects(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FActiveGameplayEffectHandle>& EffectHandles)
{
	for (FActiveGameplayEffectHandle EffectHandle : EffectHandles)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
	}
}

/** Send event tags */
void AAbilitySystemPhysicsVolume::SendEventTags(AActor* Other, const TArray<FGameplayTag>& EventTags)
{
	for (FGameplayTag EventTag : EventTags)
	{
		FGameplayEventData EventPayload;
		EventPayload.EventTag = EventTag;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, EventTag, EventPayload);
	}
}

#pragma region ABILITY_SYSTEM
