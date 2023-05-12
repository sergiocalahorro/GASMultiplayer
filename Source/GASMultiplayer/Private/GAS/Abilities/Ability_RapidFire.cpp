// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_RapidFire.h"

// Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// GASMultiplayer
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_RapidFire::UAbility_RapidFire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_RapidFire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageAndWaitTask))
	{
		PlayMontageAndWaitTask->EndTask();
	}

	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EndTask();
	}

	MontageStop();
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion ABILITY

#pragma region RAPID_FIRE

/** Activate ability from event (must be called from Blueprint-side) */
void UAbility_RapidFire::ActivateAbilityFromEvent_Internal(const FGameplayEventData& EventData, FGameplayTag WaitEventTag)
{
	PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, GetEquippedWeaponStaticData()->AttackMontage); 
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UAbility_RapidFire::K2_EndAbility);
	PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UAbility_RapidFire::K2_EndAbility);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UAbility_RapidFire::K2_EndAbility);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UAbility_RapidFire::K2_EndAbility);
	PlayMontageAndWaitTask->ReadyForActivation();

	FTimerDelegate ShootingTimerDelegate;
	ShootingTimerDelegate.BindUFunction(this, FName("Shoot"), FGameplayEventData());
	GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, ShootingTimerDelegate, 1.f / GetEquippedWeaponStaticData()->FireRate, true, 0.f);

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WaitEventTag, nullptr, true, true);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UAbility_RapidFire::StopShooting);
	WaitGameplayEventTask->ReadyForActivation();
}

/** Shoot */
void UAbility_RapidFire::Shoot(FGameplayEventData Payload)
{
	Super::Shoot(Payload);
}

/** Stop shooting */
void UAbility_RapidFire::StopShooting(FGameplayEventData Payload)
{
	K2_EndAbility();
}

#pragma endregion RAPID_FIRE