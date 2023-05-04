// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASMultiplayerGameMode.h"
#include "GASMultiplayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASMultiplayerGameMode::AGASMultiplayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
