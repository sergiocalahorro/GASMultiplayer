// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Unreal Engine
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "General/Structs/CharacterData.h"

#include "BaseCharacter.generated.h"

// Forward declarations - Unreal Engine
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGameplayEffect;
class UGameplayAbility;

// Forward declarations - GASMultiplayer
class UBaseAbilitySystemComponent;
class UBaseAttributeSet;
class UCharacterDataAsset;

UCLASS(config=Game)
class ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:

	/** Constructor for AActor that takes an ObjectInitializer for backward compatibility */
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called when this Pawn is possessed (only called on the server) */
	virtual void PossessedBy(AController* NewController) override;

	/** PlayerState Replication Notification (only called on the client) */
	virtual void OnRep_PlayerState() override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
#pragma endregion OVERRIDES

#pragma region COMPONENTS

public:

	/** Getter of CameraBoom */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Getter of FollowCamera */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AA|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AA|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion COMPONENTS

#pragma region INPUT

protected:

	/** Allows a Pawn to set up custom input bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

#pragma endregion INPUT

#pragma region GAS

public:

	/** Returns the ability system component to use for this actor */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Setter of CharacterData */
	UFUNCTION()
	void SetCharacterData(const FCharacterData& NewCharacterData);

	/** Getter of CharacterData */
	UFUNCTION()
	FCharacterData GetCharacterData() const { return CharacterData; }

protected:

	/** Give abilities */
	UFUNCTION()
	void GiveAbilities();

	/** Apply gameplay effects at startup */
	UFUNCTION()
	void ApplyStartupEffects();

	/** Initialize values from CharacterData */
	UFUNCTION()
	virtual void InitFromCharacterData(const FCharacterData& NewCharacterData, bool bFromReplication = false);

	/** Replicate CharacterData */
	UFUNCTION()
	void OnRep_CharacterData();

private:

	/** Apply given gameplay effect to self */
	UFUNCTION()
	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& EffectContext) const;

protected:

	/** Character's data asset */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS")
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;

private:

	/** Ability system component */
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;

	/** Attributes */
	UPROPERTY(Transient)
	TObjectPtr<UBaseAttributeSet> AttributeSet;
	
	/** Character's data */
	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

#pragma endregion GAS
	
};

