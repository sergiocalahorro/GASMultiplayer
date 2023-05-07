// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CustomCharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASMULTIPLAYER_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

#pragma region TRAVERSAL

public:

	/** Try traversal ability */
	UFUNCTION()
	bool TryTraversal(UAbilitySystemComponent* AbilitySystemComponent);

protected:

	/** Traversal abilities in priority order */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> TraversalAbilitiesOrdered;
	
#pragma endregion TRAVERSAL
};
