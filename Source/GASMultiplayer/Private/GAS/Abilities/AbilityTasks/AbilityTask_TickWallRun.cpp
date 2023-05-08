// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/AbilityTasks/AbilityTask_TickWallRun.h"

// Unreal Engine
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#pragma region OVERRIDES

/** Activate task */
void UAbilityTask_TickWallRun::Activate()
{
	Super::Activate();

	FHitResult WallHitResult;
	if (!FindRunnableWall(WallHitResult))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			WallRunFinishedDelegate.Broadcast();
		}

		EndTask();
		return;
	}

	WallRunWallSideDeterminedDelegate.Broadcast(IsWallOnTheLeft(WallHitResult));
	CharacterOwner->Landed(WallHitResult);
	CharacterOwner->SetActorLocation(WallHitResult.ImpactPoint + WallHitResult.ImpactNormal * WallCorrectionOffset);
	CharacterMovement->SetMovementMode(MOVE_Flying);
}

/** End and CleanUp the task - may be called by the task itself or by the task owner if the owner is ending */
void UAbilityTask_TickWallRun::OnDestroy(bool bInOwnerFinished)
{
	CharacterMovement->SetPlaneConstraintEnabled(false);
	CharacterMovement->SetMovementMode(MOVE_Falling);
	Super::OnDestroy(bInOwnerFinished);
}

/** Tick task */
void UAbilityTask_TickWallRun::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	FHitResult WallHitResult;

	// Find a suitable wall to run on
	if (!FindRunnableWall(WallHitResult))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			WallRunFinishedDelegate.Broadcast();
		}

		EndTask();
		return;
	}

	const FRotator DirectionRotator = IsWallOnTheLeft(WallHitResult) ? FRotator(0.f, -90.f, 0.f) : FRotator(0.f, 90.f, 0.f);
	const FVector WallRunDirection = DirectionRotator.RotateVector(WallHitResult.ImpactNormal);
	CharacterMovement->Velocity = WallRunDirection * WallRunSpeed;
	CharacterMovement->Velocity.Z = 0.f;
	CharacterOwner->SetActorRotation(WallRunDirection.Rotation());

	CharacterMovement->SetPlaneConstraintEnabled(true);
	CharacterMovement->SetPlaneConstraintOrigin(WallHitResult.ImpactPoint);
	CharacterMovement->SetPlaneConstraintNormal(WallHitResult.ImpactNormal);
}

#pragma endregion OVERRIDES

#pragma region WALLRUN

/** Create task */
UAbilityTask_TickWallRun* UAbilityTask_TickWallRun::CreateTickWallRunTask(UGameplayAbility* OwningAbility, ACharacter* InCharacterOwner, UCharacterMovementComponent* InCharacterMovement, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes)
{
	UAbilityTask_TickWallRun* WallTickRunTask = NewAbilityTask<UAbilityTask_TickWallRun>(OwningAbility);

	WallTickRunTask->bTickingTask = true;
	WallTickRunTask->CharacterMovement = InCharacterMovement;
	WallTickRunTask->CharacterOwner = InCharacterOwner;
	WallTickRunTask->WallRunTraceObjectTypes = TraceObjectTypes;

	return WallTickRunTask;
}

/** Find a wall where character can perform wall run */
bool UAbilityTask_TickWallRun::FindRunnableWall(FHitResult& WallHitResult)
{
	const FVector CharacterLocation = CharacterOwner->GetActorLocation();
	const FVector RightVector = CharacterOwner->GetActorRightVector();
	const FVector ForwardVector = CharacterOwner->GetActorForwardVector();

	const float TraceLength = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + TraceLengthOffset;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(CharacterOwner);

	// Debug
	static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
	const bool bShowTraversal = CVar->GetInt() > 0;
	EDrawDebugTrace::Type DebugDrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	// Can't run on walls that are in front of the character
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + ForwardVector * TraceLength,
		WallRunTraceObjectTypes, true, IgnoredActors, DebugDrawType, WallHitResult, true))
	{
		return false;
	}

	// Can't run on walls that are in front of the character
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + -RightVector * TraceLength,
		WallRunTraceObjectTypes, true, IgnoredActors, DebugDrawType, WallHitResult, true))
	{
		if (FVector::DotProduct(WallHitResult.ImpactNormal, RightVector) > RunnableWallThreshold)
		{
			return true;
		}
	}

	// Can't run on walls that are in front of the character
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), CharacterLocation, CharacterLocation + RightVector * TraceLength,
		WallRunTraceObjectTypes, true, IgnoredActors, DebugDrawType, WallHitResult, true))
	{
		if (FVector::DotProduct(WallHitResult.ImpactNormal, -RightVector) > RunnableWallThreshold)
		{
			return true;
		}
	}

	return false;
}

/** Whether wall is on the left/right */
bool UAbilityTask_TickWallRun::IsWallOnTheLeft(FHitResult& WallHitResult)
{
	return FVector::DotProduct(CharacterOwner->GetActorRightVector(), WallHitResult.ImpactNormal) > 0.f;
}

#pragma endregion WALLRUN