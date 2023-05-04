// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	
};
