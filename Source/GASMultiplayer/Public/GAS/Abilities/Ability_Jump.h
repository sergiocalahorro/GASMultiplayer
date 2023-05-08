// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"

#include "Ability_Jump.generated.h"

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_Jump : public UBaseGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAbility_Jump();

#pragma endregion INITIALIZATION

#pragma region ABILITY

public:

	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

#pragma endregion ABILITY

#pragma region JUMP

protected:

	/** Wall Run state tag */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Jump|WallRun")
	FGameplayTag WallRunStateTag;

	/** Wall Run state left tag */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Jump|WallRun")
	FGameplayTag WallRunStateLeftTag;

	/** Jump strength when jumping off wall */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Jump|WallRun")
	float OffWallJumpStrength = 300.f;

#pragma endregion JUMP
	
};
