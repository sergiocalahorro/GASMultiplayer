// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllers/BasePlayerController.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/BaseGameMode.h"

#pragma region OVERRIDES
	
/** Overridable native function for when this controller is asked to possess a pawn */
void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InPawn))
	{
		DeathStateTagDelegate = AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("State.Dead")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ABasePlayerController::OnPawnDeathStateChanged);
	}
}

/** Overridable native function for when this controller unpossesses its pawn */
void ABasePlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (!DeathStateTagDelegate.IsValid())
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
	{
		AbilitySystemComponent->UnregisterGameplayTagEvent(DeathStateTagDelegate, FGameplayTag::RequestGameplayTag(TEXT("State.Dead")), EGameplayTagEventType::NewOrRemoved);
	}
}

#pragma endregion OVERRIDES

#pragma region DEATH
	
/** Restart player after the given time */
void ABasePlayerController::RestartPlayerInTime(float InTime)
{
	ChangeState(NAME_Spectating);
	
	GetWorld()->GetTimerManager().SetTimer(RestartPlayerTimerHandle, this, &ABasePlayerController::RestartPlayer, InTime, false);
}

/** Restart player */
void ABasePlayerController::RestartPlayer()
{
	if (ABaseGameMode* GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RestartPlayer(this);	
	}
}

/** Callback called when death state is changed */
void ABasePlayerController::OnPawnDeathStateChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (ABaseGameMode* GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->NotifyPlayerDied(this);
		}

		if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()))
		{
			AbilitySystemComponent->UnregisterGameplayTagEvent(DeathStateTagDelegate, FGameplayTag::RequestGameplayTag(TEXT("State.Dead")), EGameplayTagEventType::NewOrRemoved);
		}
	}
}

#pragma endregion DEATH
