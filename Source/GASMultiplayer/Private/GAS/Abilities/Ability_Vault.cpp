// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Vault.h"

// Unreal Engine
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "General/Components/Movement/CustomMotionWarpingComponent.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Vault::UAbility_Vault()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** The last chance to fail before committing, this will usually be the same as CanActivateAbility. Some abilities may need to do extra checks here if they are consuming extra stuff in CommitExecute */
bool UAbility_Vault::CommitCheck(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}
	
	ABaseCharacter* Character = GetBaseCharacterFromActorInfo();
	if (!IsValid(Character))
	{
		return false;
	}
	
	const FVector StartLocation = Character->GetActorLocation();
	const FVector ForwardVector = Character->GetActorForwardVector();
	const FVector UpVector = Character->GetActorUpVector();

	TArray<AActor*> IgnoredActors;
	IgnoredActors.AddUnique(Character);

	// Debug
	static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
	const bool bShowCustomDebug = CVar->GetInt() > 0;
	EDrawDebugTrace::Type DebugDrawType = bShowCustomDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	// Jump to location
	int32 JumpToLocationIndex = INDEX_NONE;
	
	// Horizontal traces
	FHitResult TraceHit;
	float MaxJumpDistance = HorizontalTraceLength;
	int i = 0;
	for (; i < HorizontalTraceCount; ++i)
	{
		const FVector TraceStart = StartLocation + i * UpVector * HorizontalTraceStep;
		const FVector TraceEnd = TraceStart + ForwardVector * HorizontalTraceLength;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, HorizontalTraceRadius,
															  TraceObjectTypes, true, IgnoredActors, DebugDrawType, TraceHit, true))
		{
			if (JumpToLocationIndex == INDEX_NONE && (i < HorizontalTraceCount - 1))
			{
				// Set jump location if it wasn't already set and isn't the last trace
				JumpToLocationIndex = i;
				JumpToLocation = TraceHit.Location;
			}
			else if (JumpToLocationIndex == (i - 1))
			{
				// Adjust max jump distance
				MaxJumpDistance = FVector::Dist2D(TraceHit.Location, TraceStart);
				break;
			}
		}
		else
		{
			if (JumpToLocationIndex != INDEX_NONE)
			{
				break;
			}
		}
	}

	if (JumpToLocationIndex == INDEX_NONE)
	{
		return false;
	}

	const float DistanceToJumpTo = FVector::Dist2D(StartLocation, JumpToLocation);
	const float MaxVerticalTraceDistance = MaxJumpDistance - DistanceToJumpTo;

	if (MaxVerticalTraceDistance < 0.f)
	{
		return false;
	}

	if (i == HorizontalTraceCount)
	{
		i = HorizontalTraceCount - 1;
	}

	const float VerticalTraceLength = FMath::Abs(JumpToLocation.Z - (StartLocation + i * UpVector * HorizontalTraceStep).Z);
	FVector VerticalStartLocation = JumpToLocation + UpVector * VerticalTraceLength;
	const float VerticalTraceCount = MaxVerticalTraceDistance / VerticalTraceStep;

	bool bJumpOverLocationSet = false;

	// Vertical traces
	i = 0;
	for (; i <= VerticalTraceCount; ++i)
	{
		const FVector TraceStart = VerticalStartLocation + i * ForwardVector * VerticalTraceStep;
		const FVector TraceEnd = TraceStart + UpVector * VerticalTraceLength * -1.f;

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, TraceEnd, VerticalTraceRadius,
															  TraceObjectTypes, true, IgnoredActors, DebugDrawType, TraceHit, true))
		{
			JumpOverLocation = TraceHit.ImpactPoint;
			if (i == 0)
			{
				JumpToLocation = JumpOverLocation;
			}
		}
		else if (i != 0)
		{
			bJumpOverLocationSet = true;
			break;
		}
	}

	if (!bJumpOverLocationSet)
	{
		return false;
	}

	const FVector TraceStart = JumpOverLocation + ForwardVector * VerticalTraceStep;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(this, TraceStart, JumpOverLocation, VerticalTraceRadius,
														  TraceObjectTypes, true, IgnoredActors, DebugDrawType, TraceHit, true))
	{
		JumpOverLocation = TraceHit.ImpactPoint;
	}

	// Debug
	if (bShowCustomDebug)
	{
		DrawDebugSphere(GetWorld(), JumpToLocation, 15.f, 16, FColor::White, false, 7.f);
		DrawDebugString(GetWorld(), JumpToLocation, FString("JumpToLocation"), nullptr, FColor::Black, 7.f);
		DrawDebugSphere(GetWorld(), JumpOverLocation, 15.f, 16, FColor::White, false, 7.f);
		DrawDebugString(GetWorld(), JumpOverLocation, FString("JumpOverLocation"), nullptr, FColor::Black, 7.f);
	}

	return true;
}

/** Actually activate ability, do not call this directly */
void UAbility_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// End ability if it can't be performed
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		K2_EndAbility();
		return;
	}

	if (const ABaseCharacter* Character = GetBaseCharacterFromActorInfo())
	{
		// Set movement mode to flying
		if (UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
		}

		// Modify capsule's collision channels to be ignored
		if (UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent())
		{
			for (const ECollisionChannel Channel : CollisionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECR_Ignore);
			}
		}
		
		// Set warp targets
		if (UCustomMotionWarpingComponent* MotionWarpingComponent = Character->GetCustomMotionWarpingComponent())
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpToLocation"), JumpToLocation, Character->GetActorRotation());
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("JumpOverLocation"), JumpOverLocation, Character->GetActorRotation());
		}

		// Start montage task
		MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, VaultMontage);
		MontageTask->OnBlendOut.AddUniqueDynamic(this, &UAbility_Vault::K2_EndAbility);
		MontageTask->OnCompleted.AddUniqueDynamic(this, &UAbility_Vault::K2_EndAbility);
		MontageTask->OnInterrupted.AddUniqueDynamic(this, &UAbility_Vault::K2_EndAbility);
		MontageTask->OnCancelled.AddUniqueDynamic(this, &UAbility_Vault::K2_EndAbility);
		MontageTask->ReadyForActivation();
	}
}
	
/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_Vault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	if (const ABaseCharacter* Character = GetBaseCharacterFromActorInfo())
	{
		// Set movement mode to falling
		if (UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
		{
			if (CharacterMovementComponent->IsFlying())
			{
				CharacterMovementComponent->SetMovementMode(MOVE_Falling);
			}
		}
		
		// Modify capsule's collision channels to be blocked
		if (UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent())
		{
			for (const ECollisionChannel Channel : CollisionChannelsToIgnore)
			{
				CapsuleComponent->SetCollisionResponseToChannel(Channel, ECR_Block);
			}
		}

		// Remove warp targets
		if (UCustomMotionWarpingComponent* MotionWarpingComponent = Character->GetCustomMotionWarpingComponent())
		{
			MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpToLocation"));
			MotionWarpingComponent->RemoveWarpTarget(TEXT("JumpOverLocation"));
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion ABILITY