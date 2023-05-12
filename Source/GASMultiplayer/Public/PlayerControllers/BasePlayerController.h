// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"

#include "BasePlayerController.generated.h"

UCLASS()
class GASMULTIPLAYER_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:

	/** Overridable native function for when this controller is asked to possess a pawn */
	virtual void OnPossess(APawn* InPawn) override;

	/** Overridable native function for when this controller unpossesses its pawn */
	virtual void OnUnPossess() override;

#pragma endregion OVERRIDES

#pragma region DEATH
	
public:

	/** Restart player after the given time */
	UFUNCTION()
	void RestartPlayerInTime(float InTime);

protected:

	/** Restart player */
	UFUNCTION()
	void RestartPlayer();

	/** Callback called when death state is changed */
	UFUNCTION()
	void OnPawnDeathStateChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	/** Death state's tag delegate handle */
	FDelegateHandle DeathStateTagDelegate;

private:

	/** Timer handle's for restarting player */
	UPROPERTY()
	FTimerHandle RestartPlayerTimerHandle;

#pragma endregion DEATH

};
