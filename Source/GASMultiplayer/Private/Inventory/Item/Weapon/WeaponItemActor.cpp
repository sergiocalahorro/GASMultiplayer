// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Item/Weapon/WeaponItemActor.h"

// GASMultiplayer
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