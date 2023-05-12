// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_SingleShot.h"

// Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// GASMultiplayer
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_SingleShot::UAbility_SingleShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_SingleShot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageAndWaitTask))
	{
		PlayMontageAndWaitTask->EndTask();
	}

	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion ABILITY

#pragma region SINGLE_SHOT

/** Activate ability from event (must be called from Blueprint-side) */
void UAbility_SingleShot::ActivateAbilityFromEvent_Internal(const FGameplayEventData& EventData, FGameplayTag WaitEventTag)
{
	PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GetEquippedWeaponStaticData()->AttackMontage); 
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UAbility_SingleShot::K2_EndAbility);
	PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UAbility_SingleShot::K2_EndAbility);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UAbility_SingleShot::K2_EndAbility);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UAbility_SingleShot::K2_EndAbility);
	PlayMontageAndWaitTask->ReadyForActivation();

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WaitEventTag, nullptr, true, true);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UAbility_SingleShot::Shoot);
	WaitGameplayEventTask->ReadyForActivation();
}

/** Shoot */
void UAbility_SingleShot::Shoot(FGameplayEventData Payload)
{
	Super::Shoot(Payload);
	K2_EndAbility();
}

#pragma endregion SINGLE_SHOT