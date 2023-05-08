// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AbilityTask_TickWallRun.generated.h"

// Forward declarations - Unreal Engine
class ACharacter;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWallRunWallSideDeterminedSignature, bool, bLeftSide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWallRunFinishedSignature);

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbilityTask_TickWallRun : public UAbilityTask
{
	GENERATED_BODY()

#pragma region OVERRIDES

public:

	/** Activate task */
	virtual void Activate() override;

	/** End and CleanUp the task - may be called by the task itself or by the task owner if the owner is ending */
	virtual void OnDestroy(bool bInOwnerFinished) override;

	/** Tick task */
	virtual void TickTask(float DeltaTime) override;

#pragma endregion OVERRIDES

#pragma region WALLRUN

public:

	/** Create task */
	UFUNCTION(BlueprintCallable, Category = "AA|WallRun|Task", meta = (HiddenPin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_TickWallRun* CreateTickWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner, UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes);

protected:

	/** Find a wall where character can perform wall run */
	UFUNCTION()
	bool FindRunnableWall(FHitResult& WallHitResult);

	/** Whether wall is on the left/right */
	UFUNCTION()
	bool IsWallOnTheLeft(FHitResult& WallHitResult);
	
public:

	/** Delegate called when a wall side is or not determined for performing wall run */
	UPROPERTY(BlueprintAssignable, Category = "AA|WallRun")
	FWallRunWallSideDeterminedSignature WallRunWallSideDeterminedDelegate;

	/** Delegate called when wall run is finished */
	UPROPERTY(BlueprintAssignable, Category = "AA|WallRun")
	FWallRunFinishedSignature WallRunFinishedDelegate;

protected:

	/** Character movement component's reference */
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	/** Ability's character owner reference */
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

	/** Wall run's speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun")
	float WallRunSpeed = 700.f;

	/** Offset used to set character's location on wall along hit's normal */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun")
	float WallCorrectionOffset = 60.f;

	/** Threshold to compare the dot product's between the character's sides and the wall's normal, so a wall can be runnable */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun|Trace")
	float RunnableWallThreshold = 0.3f;
	
	/** List of object types for traces */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun|Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> WallRunTraceObjectTypes;

	/** Offset used for the trace, to add it up to the height of the character's capsule */
	UPROPERTY(EditDefaultsOnly, Category = "AA|WallRun|Trace")
	float TraceLengthOffset = 30.f;

#pragma endregion WALLRUN

};
