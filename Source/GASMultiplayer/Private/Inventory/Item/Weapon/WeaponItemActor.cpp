// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Item/Weapon/WeaponItemActor.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// GASMultiplayer
#include "General/PhysicalMaterials/BasePhysicalMaterial.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/Weapon/WeaponStaticData.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AWeaponItemActor::AWeaponItemActor()
{

}

#pragma endregion INITIALIZATION

#pragma region WEAPON

/** Get weapon's static data */
const UWeaponStaticData* AWeaponItemActor::GetWeaponStaticData() const
{
	return ItemInstance ? Cast<UWeaponStaticData>(ItemInstance->GetItemStaticData()) : nullptr;
}

/** Get weapon's muzzle location */
FVector AWeaponItemActor::GetMuzzleLocation() const
{
	return MeshComponent ? MeshComponent->GetSocketLocation(TEXT("Muzzle")) : GetActorLocation();
}

/** Play weapon's effects */
void AWeaponItemActor::PlayWeaponEffects(const FHitResult& InHitResult)
{
	if (HasAuthority())
	{
		MulticastPlayWeaponEffects(InHitResult);
	}
	else
	{
		PlayWeaponEffects_Internal(InHitResult);
	}
}

/** Play weapon's effects (both in clients and server) */
void AWeaponItemActor::MulticastPlayWeaponEffects_Implementation(const FHitResult& InHitResult)
{
	if (!Owner || Owner->GetLocalRole() != ROLE_AutonomousProxy)
	{
		PlayWeaponEffects_Internal(InHitResult);
	}
}
	
/** Play weapon's effects (internal) */
void AWeaponItemActor::PlayWeaponEffects_Internal(const FHitResult& InHitResult)
{
	if (UBasePhysicalMaterial* PhysicalMaterial = Cast<UBasePhysicalMaterial>(InHitResult.PhysMaterial.Get()))
	{
		UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->PointImpactSound, InHitResult.ImpactPoint, 1.f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PhysicalMaterial->PointImpactVFX, InHitResult.ImpactPoint);
	}

	if (const UWeaponStaticData* WeaponStaticData = GetWeaponStaticData())
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponStaticData->AttackSound, GetActorLocation(), 1.f);
	}
}

/** Client-side initialization (used for initializing items's visuals) */
void AWeaponItemActor::Init_Internal()
{
	Super::Init_Internal();

	if (const UWeaponStaticData* WeaponStaticData = GetWeaponStaticData())
	{
		if (WeaponStaticData->SkeletalMesh)
		{
			if (USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("MeshComponent")))
			{
				SkeletalMeshComponent->RegisterComponent();
				SkeletalMeshComponent->SetSkeletalMesh(WeaponStaticData->SkeletalMesh);
				SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MeshComponent = SkeletalMeshComponent;
			}
		}
		else if (WeaponStaticData->StaticMesh)
		{
			if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("MeshComponent")))
			{
				StaticMeshComponent->RegisterComponent();
				StaticMeshComponent->SetStaticMesh(WeaponStaticData->StaticMesh);
				StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				MeshComponent = StaticMeshComponent;
			}
		}
	}
}

#pragma endregion WEAPON