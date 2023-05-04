// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstances/BaseGameInstance.h"

// Unreal Engine
#include "AbilitySystemGlobals.h"

#pragma region OVERRIDES

/** Virtual function to allow custom GameInstances an opportunity to set up what it needs */
void UBaseGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

#pragma endregion OVERRIDES
