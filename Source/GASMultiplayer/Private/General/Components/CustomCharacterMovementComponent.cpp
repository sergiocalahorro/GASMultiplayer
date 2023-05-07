// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/CustomCharacterMovementComponent.h"

// Unreal Engine
#include "AbilitySystemComponent.h"

// GASMultiplayer
#include "General/Globals/DebugSystem.h"

#pragma region TRAVERSAL

/** Try traversal ability */
bool UCustomCharacterMovementComponent::TryTraversal(UAbilitySystemComponent* AbilitySystemComponent)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : TraversalAbilitiesOrdered)
	{
		if (AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, true))
		{
			FGameplayAbilitySpec* AbilitySpec;
			
			AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass);
			if (AbilitySpec && AbilitySpec->IsActive())
			{
				return true;
			}
		}
	}

	return false;
}

#pragma endregion TRAVERSAL
