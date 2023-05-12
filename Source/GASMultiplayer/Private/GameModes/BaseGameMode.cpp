// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/BaseGameMode.h"

// GASMultiplayer
#include "PlayerControllers/BasePlayerController.h"

#pragma region INITIALIZATION

/** Constructor */
ABaseGameMode::ABaseGameMode()
{
	PlayerControllerClass = ABasePlayerController::StaticClass();
}

#pragma endregion INITIALIZATION

#pragma region PLAYER

/** Notify that the incoming player controller is dead */
void ABaseGameMode::NotifyPlayerDied(ABasePlayerController* PlayerController)
{
	if (PlayerController)
	{
		PlayerController->RestartPlayerInTime(TimeToRestartPlayer);
	}
}

#pragma endregion PLAYER