// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_SingleShot.h"

// Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// GASMultiplayer
#include "Inventory/Item/Weapon/WeaponItemActor.h"
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
	// Debug
	static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
	const bool bShowDebug = CVar->GetInt() > 0;
	EDrawDebugTrace::Type DebugDrawType = bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetActorInfo().OwnerActor.Get());

	FHitResult TraceHit;
	const FVector TraceStart = GetEquippedWeaponItemActor()->GetMuzzleLocation();
	const FVector TraceEnd = TraceStart + GetActorInfo().PlayerController->GetControlRotation().Vector() * GetEquippedWeaponStaticData()->ShootingDistance;

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		IgnoredActors, DebugDrawType, TraceHit, true))
	{
		if (UAbilitySystemComponent* ShotActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TraceHit.GetActor()))
		{
			const UGameplayEffect* DamageEffect = Cast<UGameplayEffect>(GetEquippedWeaponStaticData()->DamageEffect->GetDefaultObject());
			FGameplayEffectContextHandle DamageEffectContext = ShotActorAbilitySystemComponent->MakeEffectContext();
			ShotActorAbilitySystemComponent->ApplyGameplayEffectToSelf(DamageEffect, 1.f, DamageEffectContext);
		}
	}
	K2_EndAbility();
}

#pragma endregion SINGLE_SHOT