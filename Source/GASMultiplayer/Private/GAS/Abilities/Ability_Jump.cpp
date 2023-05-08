// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Ability_Jump.h"

// Unreal Engine
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#pragma region INITIALIZATION

/** Constructor */
UAbility_Jump::UAbility_Jump()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

#pragma endregion INITIALIZATION

#pragma region ABILITY

/** Returns true if this ability can be activated right now. Has no side effects */
bool UAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed))
	{
		const bool bMovementAllowJump = Character->GetCharacterMovement()->IsJumpAllowed();
		if (const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character))
		{
			return Character->CanJump() || (bMovementAllowJump && AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateTag));
		}
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/** Actually activate ability, do not call this directly */
void UAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			return;
		}

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		if (ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get()))
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character))
			{
				if (AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateTag))
				{
					const FGameplayTagContainer WallRunTags(WallRunStateTag);
					AbilitySystemComponent->CancelAbilities(&WallRunTags);

					FVector InputDirection = Character->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal();
					const FRotator ForwardRotation = Character->GetActorForwardVector().Rotation();
					InputDirection = ForwardRotation.RotateVector(InputDirection);
					
					if (InputDirection.IsZero()) 
					{
						// If there's no input direction, then use a perpendicular direction for the jump
						InputDirection = Character->GetActorRightVector();
						if (!AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateLeftTag))
						{
							InputDirection *= -1.f;
						}
					}
					else
					{
						// If the input direction is similar to the side the wall is regarding the player, then negate the direction
						if ((InputDirection.Equals(Character->GetActorRightVector(), 0.2) && !AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateLeftTag)) ||
							(InputDirection.Equals(-Character->GetActorRightVector(), 0.2) && AbilitySystemComponent->HasMatchingGameplayTag(WallRunStateLeftTag)))
						{
							InputDirection *= -1.f;
						}
					}

					// Jump in the direction of the input
					const FVector JumpOffDirection = InputDirection + FVector::UpVector;
					Character->LaunchCharacter(JumpOffDirection * OffWallJumpStrength, false, true);
				}
				else
				{
					Character->Jump();
				}
			}
		}
	}
}

#pragma endregion ABILITY
