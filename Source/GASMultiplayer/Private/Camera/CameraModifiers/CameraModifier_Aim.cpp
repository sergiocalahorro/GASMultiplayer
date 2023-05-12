// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraModifiers/CameraModifier_Aim.h"

#pragma region OVERRIDES

/** Directly modifies variables in the owning camera */
bool UCameraModifier_Aim::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	const FVector InitialViewLocation = InOutPOV.Location;
	const FVector FinalViewLocation = InitialViewLocation + InOutPOV.Rotation.Vector() * AimDistance;
	InOutPOV.Location = FinalViewLocation;

	return true;
}

#pragma endregion OVERRIDES