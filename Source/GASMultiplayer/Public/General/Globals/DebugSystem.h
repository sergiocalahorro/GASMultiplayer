// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/** Footstep debug */
static TAutoConsoleVariable<int32> CVarShowFootstepDebug(
	TEXT("ShowFootstepDebug"),
	0,
	TEXT("Draws footstep debug info")
	TEXT(" 0: Off |")
	TEXT(" 1: On"),
	ECVF_Cheat);

/** Traversal debug */
static TAutoConsoleVariable<int32> CVarShowTraversalDebug(
	TEXT("ShowTraversalDebug"),
	0,
	TEXT("Draws traversal debug info")
	TEXT(" 0: Off |")
	TEXT(" 1: On"),
	ECVF_Cheat);