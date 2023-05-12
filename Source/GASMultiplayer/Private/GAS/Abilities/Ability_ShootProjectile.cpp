// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_ShootProjectile.h"

// Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/EngineTypes.h"

// GASMultiplayer
#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"
#include "Inventory/Item/Weapon/WeaponItemActor.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"
#include "Character/BaseCharacter.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_ShootProjectile::UAbility_ShootProjectile()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Actually activate ability, do not call this directly */
void UAbility_ShootProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UWeaponStaticData* WeaponData = GetEquippedWeaponStaticData();
	AWeaponItemActor* WeaponActor = GetEquippedWeaponItemActor();

	FHitResult TraceHit;
	GetWeaponToFocusTraceResult(WeaponData->ShootingDistance, WeaponData->WeaponTraceChannel, TraceHit);

	WeaponActor->PlayWeaponEffects(TraceHit, false);

	const FVector TraceDirection = (TraceHit.TraceEnd - TraceHit.TraceStart).GetSafeNormal();
	const FVector SpawnLocation = WeaponActor->GetMuzzleLocation() + (TraceDirection * WeaponData->SpawnOffset);
	const FRotator SpawnRotator = TraceDirection.Rotation();
	UGASMultiplayerStatics::LaunchProjectile(this, ProjectileStaticDataClass, FTransform(SpawnRotator, SpawnLocation), GetBaseCharacterFromActorInfo(), GetBaseCharacterFromActorInfo());

	K2_EndAbility();
}

#pragma endregion ABILITY