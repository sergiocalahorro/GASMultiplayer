// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Aim.h"

// Unreal Engine
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Aim::UAbility_Aim()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = true;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Actually activate ability, do not call this directly */
void UAbility_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (APlayerCameraManager* PlayerCameraManager = GetActorInfo().PlayerController->PlayerCameraManager)
	{
		AimCameraModifier = PlayerCameraManager->AddNewCameraModifier(AimCameraModifierClass);
		
		WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WaitEventTag, nullptr, false, true);
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UAbility_Aim::StopAim);
		WaitGameplayEventTask->ReadyForActivation();
		return;
	}

	K2_EndAbility();
}
	
/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AimCameraModifier)
	{
		if (APlayerCameraManager* PlayerCameraManager = GetActorInfo().PlayerController->PlayerCameraManager)
		{
			PlayerCameraManager->RemoveCameraModifier(AimCameraModifier);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion ABILITY

#pragma region AIM

/** Stop aim */
void UAbility_Aim::StopAim(FGameplayEventData Payload)
{
	K2_EndAbility();
}

#pragma endregion AIM
