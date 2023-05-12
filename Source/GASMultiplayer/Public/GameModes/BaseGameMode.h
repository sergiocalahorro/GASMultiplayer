// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Forward declarations - GASMultiplayer
class ABasePlayerController;

#include "BaseGameMode.generated.h"

UCLASS(minimalapi)
class ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	ABaseGameMode();

#pragma endregion INITIALIZATION

#pragma region PLAYER

public:

	/** Notify that the incoming player controller is dead */
	UFUNCTION()
	void NotifyPlayerDied(ABasePlayerController* PlayerController);

protected:

	/** Time to restart player once dead */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Restart")
	float TimeToRestartPlayer = 2.f;

#pragma endregion PLAYER
	
};
