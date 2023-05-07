// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"

#include "Ability_Vault.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_Vault : public UBaseGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	UAbility_Vault();

#pragma endregion INITIALIZATION

#pragma region ABILITY

public:

	/** The last chance to fail before committing, this will usually be the same as CanActivateAbility. Some abilities may need to do extra checks here if they are consuming extra stuff in CommitExecute */
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
#pragma endregion ABILITY

#pragma region VAULT

protected:

	/** Vault anim montage */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault")
	UAnimMontage* VaultMontage;

	/** Collision channels to ignore when vaulting */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToIgnore;

	/** Trace channels to use on traces */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	/** Horizontal trace radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|HorizontalTrace")
	float HorizontalTraceRadius = 30.f;

	/** Horizontal trace length */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|HorizontalTrace")
	float HorizontalTraceLength = 500.f;

	/** Horizontal trace count */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|HorizontalTrace")
	float HorizontalTraceCount = 5.f;

	/** Horizontal trace step */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|HorizontalTrace")
	float HorizontalTraceStep = 60.f;

	/** Vertical trace radius */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|VerticalTrace")
	float VerticalTraceRadius = 30.f;

	/** Vertical trace step */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Vault|VerticalTrace")
	float VerticalTraceStep = 30.f;

private:

	/** Location to jump to */
	FVector JumpToLocation;

	/** Location to jump over */
	FVector JumpOverLocation;

	/** Montage task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
#pragma endregion VAULT
	
};
