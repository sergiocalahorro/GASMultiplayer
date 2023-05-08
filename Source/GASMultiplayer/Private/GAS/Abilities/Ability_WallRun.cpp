// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_WallRun.h"

// Unreal Engine
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// GASMultiplayer
#include "AbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "GAS/AbilityTasks/AbilityTask_TickWallRun.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_WallRun::UAbility_WallRun()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the ability is given to an AbilitySystemComponent */
void UAbility_WallRun::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (const ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent())
		{
			CapsuleComponent->OnComponentHit.AddUniqueDynamic(this, &UAbility_WallRun::OnCapsuleComponentHit);
		}
	}
}

/** Called when the ability is removed from an AbilitySystemComponent */
void UAbility_WallRun::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (ActorInfo)
	{
		if (const ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor))
		{
			if (UCapsuleComponent* CapsuleComponent = CharacterAvatar->GetCapsuleComponent())
			{
				CapsuleComponent->OnComponentHit.RemoveDynamic(this, &UAbility_WallRun::OnCapsuleComponentHit);
			}
		}
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}

/** Returns true if this ability can be activated right now. Has no side effects */
bool UAbility_WallRun::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const ACharacter* CharacterAvatar = Cast<ACharacter>(ActorInfo->AvatarActor);
	return CharacterAvatar && !CharacterAvatar->GetCharacterMovement()->IsMovingOnGround();
}

/** Actually activate ability, do not call this directly */
void UAbility_WallRun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TickWallRunTask = UAbilityTask_TickWallRun::CreateTickWallRunTask(this, Cast<ACharacter>(GetAvatarActorFromActorInfo()), Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent.Get()), TraceObjectTypes); 
	TickWallRunTask->WallRunFinishedDelegate.AddDynamic(this, &UAbility_WallRun::K2_EndAbility);
	TickWallRunTask->WallRunWallSideDeterminedDelegate.AddDynamic(this, &UAbility_WallRun::UAbility_WallRun::OnWallSideDetermined);
	TickWallRunTask->ReadyForActivation();
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_WallRun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(TickWallRunTask))
	{
		TickWallRunTask->EndTask();
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunLeftSideEffectClass, AbilitySystemComponent);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(WallRunRightSideEffectClass, AbilitySystemComponent);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region WALLRUN

/** Activate ability when character's capsule component hits a wall */
void UAbility_WallRun::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->TryActivateAbility(GetCurrentAbilitySpec()->Handle);
	}
}

/** Function bound to the delegate called when the wall's side is determined */
void UAbility_WallRun::OnWallSideDetermined(bool bLeftSide)
{
	if (const ABaseCharacter* CharacterAvatar = GetBaseCharacterFromActorInfo())
	{
		if (const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
		{
			const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			const TSubclassOf<UGameplayEffect> WallRunSideEffectClass = bLeftSide ? WallRunLeftSideEffectClass : WallRunRightSideEffectClass;
			CharacterAvatar->ApplyGameplayEffectToSelf(WallRunSideEffectClass, EffectContextHandle);
		}
	}
}

#pragma endregion WALLRUN
