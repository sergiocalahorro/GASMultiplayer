// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/FootstepsComponent.h"

// Unreal Engine
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// GASMultiplayer
#include "General/PhysicalMaterials/BasePhysicalMaterial.h"

/** Sets default values for this component's properties */
UFootstepsComponent::UFootstepsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/** Called when the game starts */
void UFootstepsComponent::BeginPlay()
{
	Super::BeginPlay();
}

/** Handle footstep with given foot */
void UFootstepsComponent::HandleFootstep(EFoot Foot) const
{
	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		// Debug
		static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
		const bool bShowCustomDebug = CVar->GetInt() > 0;
		
		FCollisionQueryParams QueryParams;
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.AddIgnoredActor(Character);
		
		FHitResult HitResult;
		const FVector TraceStartLocation = Character->GetMesh()->GetSocketLocation(Foot == EFoot::Left ? LeftFootSocketName : RightFootSocketName);
		const FVector TraceEndLocation = TraceStartLocation - FVector::UpVector * TraceDistance;

		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation,
															   ECC_WorldStatic, QueryParams);

		// Debug
		if (bShowCustomDebug)
		{
			DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, bHit ? FColor::Green : FColor::Red, false, 2.f, 0, 3.f);
		}
		
		if (bHit && HitResult.bBlockingHit)
		{
			if (UBasePhysicalMaterial* PhysMaterial = Cast<UBasePhysicalMaterial>(HitResult.PhysMaterial.Get()))
			{
				UGameplayStatics::PlaySoundAtLocation(this, PhysMaterial->FootstepSound, HitResult.Location, 1.f);

				// Debug
				if (bShowCustomDebug)
				{
					DrawDebugString(GetWorld(), HitResult.Location, GetNameSafe(PhysMaterial), nullptr, FColor::White, 4.f);
				}
			}
			
			// Debug
			if (bShowCustomDebug)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, 16.f, 16, FColor::Blue, false, 4.f);
			}
		}
	}
}
