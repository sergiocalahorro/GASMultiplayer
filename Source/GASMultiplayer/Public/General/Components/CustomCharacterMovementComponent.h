// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"

// GASMultiplayer
#include "General/Enums/MovementDirectionType.h"

#include "CustomCharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASMULTIPLAYER_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region TRAVERSAL

public:

	/** Try traversal ability */
	UFUNCTION()
	bool TryTraversal(UAbilitySystemComponent* AbilitySystemComponent);

protected:

	/** Traversal abilities in priority order */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Traversal")
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;
	
#pragma endregion TRAVERSAL

#pragma region MOVEMENT

public:

	/** Getter of MovementDirectionType */
	UFUNCTION()
	EMovementDirectionType GetMovementDirectionType() const	{ return MovementDirectionType;	}

	/** Update movement direction type */
	UFUNCTION()
	void UpdateMovementDirectionType(EMovementDirectionType InMovementDirectionType);
	
	/** Function called when tag is changed to handle rotation */
	UFUNCTION()
	void OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
protected:

	/** Handle movement direction */
	UFUNCTION()
	void HandleMovementDirection();
	
protected:

	/** Movement direction type */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Movement")
	EMovementDirectionType MovementDirectionType = EMovementDirectionType::OrientToMovement;

#pragma endregion MOVEMENT
	
};
