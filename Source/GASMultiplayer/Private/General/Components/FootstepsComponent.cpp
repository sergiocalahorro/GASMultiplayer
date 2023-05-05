// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/FootstepsComponent.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// GASMultiplayer
#include "Character/BaseCharacter.h"
#include "General/PhysicalMaterials/BasePhysicalMaterial.h"

static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootSteps"),
	0,
	TEXT("Draws debug info about footsteps")
	TEXT("	0: off/n")
	TEXT("	1: on/n"),
	ECVF_Cheat);

/** Sets default values for this component's properties */
UFootstepsComponent::UFootstepsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	FeetSocketNames.Add(EFoot::Left, FName("foot_l"));
	FeetSocketNames.Add(EFoot::Right, FName("foot_r"));
}

/** Called when the game starts */
void UFootstepsComponent::BeginPlay()
{
	Super::BeginPlay();
}

/** Handle footstep with given foot */
void UFootstepsComponent::HandleFootstep(EFoot Foot)
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();
		
		FCollisionQueryParams QueryParams;
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.AddIgnoredActor(Character);
		
		FHitResult HitResult;
		const FVector TraceStartLocation = Character->GetMesh()->GetSocketLocation(*FeetSocketNames.Find(Foot));
		const FVector TraceEndLocation = TraceStartLocation - FVector::UpVector * TraceDistance;

		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation,
															   ECC_WorldStatic, QueryParams);

		if (bHit && HitResult.bBlockingHit)
		{
			// Debug
			if (DebugShowFootsteps > 0)
			{
				DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, bHit ? FColor::Green : FColor::Red, false, 1.f, 0, 1.f);
			}
			
			if (UBasePhysicalMaterial* PhysMaterial = Cast<UBasePhysicalMaterial>(HitResult.PhysMaterial.Get()))
			{
				UGameplayStatics::PlaySoundAtLocation(this, PhysMaterial->FootstepSound, HitResult.Location, 1.f);

				// Debug
				if (DebugShowFootsteps > 0)
				{
					DrawDebugString(GetWorld(), HitResult.Location, PhysMaterial->GetName(), nullptr, FColor::White, 4.f);
				}
			}
			
			// Debug
			if (DebugShowFootsteps > 0)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, 16.f, 16, FColor::Blue, false, 4.f);
			}
		}
	}
}