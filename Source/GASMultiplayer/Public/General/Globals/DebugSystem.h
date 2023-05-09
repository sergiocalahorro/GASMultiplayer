// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/** Custom debug */
static TAutoConsoleVariable<int32> CVarShowCustomDebug(
	TEXT("ShowCustomDebug"),
	0,
	TEXT("Draws custom debug info")
	TEXT(" 0: Off |")
	TEXT(" 1: On"),
	ECVF_Cheat);

static TAutoConsoleVariable<int32> CVarShowInventoryDebug(
	TEXT("ShowInventoryDebug"),
	0,
	TEXT("Shows inventory debug info")
	TEXT(" 0: Off |")
	TEXT(" 1: On"),
	ECVF_Cheat);