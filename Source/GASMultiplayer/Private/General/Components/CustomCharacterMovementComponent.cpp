// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/CustomCharacterMovementComponent.h"

// Unreal Engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

#pragma region OVERRIDES

/** Called when the game starts */
void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	HandleMovementDirection();
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TEXT("Movement.Enforced.Strafe")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCustomCharacterMovementComponent::OnEnforcedStrafeTagChanged);
	}
}

#pragma endregion OVERRIDES

#pragma region TRAVERSAL

/** Try traversal ability */
bool UCustomCharacterMovementComponent::TryTraversal(UAbilitySystemComponent* AbilitySystemComponent)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : TraversalAbilitiesOrdered)
	{
		if (AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, true))
		{
			const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass);
			if (AbilitySpec && AbilitySpec->IsActive())
			{
				return true;
			}
		}
	}

	return false;
}

#pragma endregion TRAVERSAL

#pragma region MOVEMENT

/** Update movement direction type */
void UCustomCharacterMovementComponent::UpdateMovementDirectionType(EMovementDirectionType InMovementDirectionType)
{
	MovementDirectionType = InMovementDirectionType;
	HandleMovementDirection();
}

/** Handle movement direction */
void UCustomCharacterMovementComponent::HandleMovementDirection()
{
	switch (MovementDirectionType)
	{
	case EMovementDirectionType::Strafe:
		bUseControllerDesiredRotation = true;
		bOrientRotationToMovement = false;
		CharacterOwner->bUseControllerRotationYaw = true;
		break;
		
	default:
		bUseControllerDesiredRotation = false;
		bOrientRotationToMovement = true;
		CharacterOwner->bUseControllerRotationYaw = false;
		break;
	}
}

/** Function called when tag is changed to handle rotation */
void UCustomCharacterMovementComponent::OnEnforcedStrafeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount != 0)
	{
		UpdateMovementDirectionType(EMovementDirectionType::Strafe);
	}
	else
	{
		UpdateMovementDirectionType(EMovementDirectionType::OrientToMovement);
	}
}

#pragma endregion MOVEMENT
