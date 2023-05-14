// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "GameplayTagContainer.h"
#include "General/Structs/Volumes/AbilityVolumeEnteredActorInfo.h"

#include "AbilitySystemPhysicsVolume.generated.h"

// Forward declarations - Unreal Engine
class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class GASMULTIPLAYER_API AAbilitySystemPhysicsVolume : public APhysicsVolume
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values */
	AAbilitySystemPhysicsVolume();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called when actor enters a volume */
	virtual void ActorEnteredVolume(AActor* Other) override;

	/** Called when actor leaves a volume */
	virtual void ActorLeavingVolume(AActor* Other) override;

	/** Called every frame */
	virtual void Tick(float DeltaSeconds) override;

#pragma endregion OVERRIDES

#pragma region ABILITY_SYSTEM

private:

	/** Give abilities */
	UFUNCTION()
	TArray<FGameplayAbilitySpecHandle> GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	/** Remove abilities */
	UFUNCTION()
	void RemoveAbilities(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FGameplayAbilitySpecHandle>& AbilityHandles);
	
	/** Apply effects */
	UFUNCTION()
	TArray<FActiveGameplayEffectHandle> ApplyEffects(UAbilitySystemComponent* AbilitySystemComponent, const TArray<TSubclassOf<UGameplayEffect>>& Effects);

	/** Remove effects */
	UFUNCTION()
	void RemoveEffects(UAbilitySystemComponent* AbilitySystemComponent, const TArray<FActiveGameplayEffectHandle>& EffectHandles);
	
	/** Send event tags */
	UFUNCTION()
	void SendEventTags(AActor* Other, const TArray<FGameplayTag>& EventTags);
	
protected:

	/** Abilities to give while inside the volume */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> OngoingAbilitiesToGiveOnEnter;

	/** Abilities to give permanently on enter */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> PermanentAbilitiesToGiveOnEnter;

	/** Effects to apply on enter */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyOnEnter;

	/** Effects to apply on exit */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Effects")
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyOnExit;

	/** Event tags to send on enter */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Events")
	TArray<FGameplayTag> EventTagsToSendOnEnter;

	/** Event tags to send on exit */
	UPROPERTY(EditAnywhere, Category = "AA|AbilitySystem|Events")
	TArray<FGameplayTag> EventTagsToSendOnExit;

private:

	/** Map that stores info from Actors that entered the volume */
	UPROPERTY()
	TMap<AActor*, FAbilityVolumeEnteredActorInfo> EnteredActorsInfoMap;

#pragma endregion ABILITY_SYSTEM

#pragma region DEBUG
	
private:
	
	/** Debug: Draw volume */
	UPROPERTY(EditAnywhere, Category = "AA|Debug")
	bool bDrawDebug = false;

#pragma endregion DEBUG
};
