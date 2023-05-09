// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"

// GASMultiplayer
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
class UFootstepsComponent;
class UCustomMotionWarpingComponent;
class UCustomCharacterMovementComponent;
class UInventoryComponent;

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

public:

	/** Called upon landing when falling, to perform actions based on the Hit result */
	virtual void Landed(const FHitResult& Hit) override;

	/** Request the character to start crouching. The request is processed on the next update of the CharacterMovementComponent */
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/** Request the character to stop crouching. The request is processed on the next update of the CharacterMovementComponent */
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

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

	/** Tell client that the Pawn is begin restarted */
	virtual void PawnClientRestart() override;
	
#pragma endregion OVERRIDES

#pragma region COMPONENTS

public:

	/** Getter of CameraBoom */
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Getter of FollowCamera */
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Getter of CustomMotionWarpingComponent */
	FORCEINLINE UCustomMotionWarpingComponent* GetCustomMotionWarpingComponent() const { return CustomMotionWarpingComponent; }

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AA|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AA|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Footsteps component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UFootstepsComponent> FootstepsComponent;

	/** Custom Motion Warping component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UCustomMotionWarpingComponent> CustomMotionWarpingComponent;

	/** Custom Character Movement component */
	TObjectPtr<UCustomCharacterMovementComponent> CustomCharacterMovementComponent;

	/** Inventory component */ 
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "AA|Components")
	TObjectPtr<UInventoryComponent> InventoryComponent;

#pragma endregion COMPONENTS

#pragma region INPUT

protected:

	/** Allows a Pawn to set up custom input bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called when jump is started */
	void StartJump(const FInputActionValue& Value);

	/** Called when jump is stopped */
	void StopJump(const FInputActionValue& Value);

	/** Called when crouch is started */
	void StartCrouch(const FInputActionValue& Value);

	/** Called when crouch is stopped */
	void StopCrouch(const FInputActionValue& Value);

	/** Called when sprint is started */
	void StartSprint(const FInputActionValue& Value);

	/** Called when sprint is stopped */
	void StopSprint(const FInputActionValue& Value);

	/** Called when item is equipped */
	void EquipNextItem(const FInputActionValue& Value);

	/** Called when item is unequipped */
	void UnequipItem(const FInputActionValue& Value);

	/** Called when item is dropped */
	void DropItem(const FInputActionValue& Value);

private:
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	/** Equip next item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> EquipNextItemAction;

	/** Unequip item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UnequipItemAction;

	/** Drop item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AA|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DropItemAction;

#pragma endregion INPUT

#pragma region GAS

#pragma region GAS_CORE

public:

	/** Returns the ability system component to use for this actor */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Setter of CharacterData */
	UFUNCTION()
	void SetCharacterData(const FCharacterData& NewCharacterData);

	/** Getter of CharacterData */
	UFUNCTION()
	FCharacterData GetCharacterData() const { return CharacterData; }

	/** Apply given gameplay effect to self */
	UFUNCTION()
	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& EffectContext) const;

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

	/** Function bound to the delegate that is called whenever the MaxMovementSpeed attribute is changed */
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
	
protected:

	/** Character's data asset */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Core")
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

#pragma endregion GAS_CORE

#pragma region GAS_EFFECTS

protected:

	/** Crouch state's gameplay effect */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Effects")
	TSubclassOf<UGameplayEffect> CrouchStateEffect;

#pragma endregion GAS_EFFECTS

#pragma region GAS_TAGS

protected:

	/** Gameplay event tag: Jump */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTag JumpEventTag;

	/** Tags applied while in air */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer InAirTags;
	
	/** Tags applied while crouching */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer CrouchTags;

	/** Tags applied while sprinting */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer SprintTags;

#pragma endregion GAS_TAGS

#pragma endregion GAS

#pragma region INVENTORY

	
	
};

