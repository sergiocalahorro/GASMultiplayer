// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

static TAutoConsoleVariable<int32> CVarShowDebug(
	TEXT("ShowCustomDebug"),
	0,
	TEXT("Draws custom debug info")
	TEXT(" 0: Off |")
	TEXT(" 1: On"),
	ECVF_Cheat);