// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GameplayCues/NotifyActor/GameplayCue_Actor_Burn.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#pragma region INITIALIZATION

/** Constructor */
AGameplayCue_Actor_Burn::AGameplayCue_Actor_Burn()
{
	bAutoDestroyOnRemove = true;
	bAllowMultipleOnActiveEvents = false;
	bAllowMultipleWhileActiveEvents = false;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when a GameplayCue with duration is first activated, this will only be called if the client witnessed the activation */
bool AGameplayCue_Actor_Burn::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	FireParticleComponent = UGameplayStatics::SpawnEmitterAttached(FireParticleSystem, MyTarget->GetRootComponent(), NAME_None,
																   FVector::ZeroVector, FRotator::ZeroRotator, FVector::OneVector, EAttachLocation::SnapToTarget);

	return Super::OnActive_Implementation(MyTarget, Parameters);
}

/** Called when a GameplayCue with duration is removed */
bool AGameplayCue_Actor_Burn::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (FireParticleComponent)
	{
		FireParticleComponent->Deactivate();
	}

	return Super::OnRemove_Implementation(MyTarget, Parameters);
}

#pragma endregion OVERRIDES