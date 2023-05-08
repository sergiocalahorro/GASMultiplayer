// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"

#include "Ability_WallRun.generated.h"

class UAbilityTask_TickWallRun;
/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_WallRun : public UBaseGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAbility_WallRun();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Called when the ability is removed from an AbilitySystemComponent */
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion OVERRIDES

#pragma region WALLRUN

protected:

	/** Activate ability when character's capsule component hits a wall */
	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Function bound to the delegate called when the wall's side is determined */
	UFUNCTION()
	void OnWallSideDetermined(bool bLeftSide);

protected:

	/** List of object types for traces */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	/** Wall run's left side gameplay effect */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun")
	TSubclassOf<UGameplayEffect> WallRunLeftSideEffectClass;

	/** Wall run's right side gameplay effect */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun")
	TSubclassOf<UGameplayEffect> WallRunRightSideEffectClass;

	/** Task's reference */
	UPROPERTY()
	TObjectPtr<UAbilityTask_TickWallRun> TickWallRunTask;

#pragma endregion WALLRUN

};
