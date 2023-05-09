// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/BaseCharacter.h"

// Unreal Engine
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

// GASMultiplayer
#include "GAS/Attributes/BaseAttributeSet.h"
#include "GAS/AbilitySystem/BaseAbilitySystemComponent.h"
#include "General/Components/CustomCharacterMovementComponent.h"
#include "General/Components/FootstepsComponent.h"
#include "General/Components/CustomMotionWarpingComponent.h"
#include "General/Components/InventoryComponent.h"
#include "General/DataAssets/CharacterDataAsset.h"

#pragma region INITIALIZATION

/** Constructor for AActor that takes an ObjectInitializer for backward compatibility */
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Setup hierarchy
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	FootstepsComponent = CreateDefaultSubobject<UFootstepsComponent>(TEXT("FootstepsComponent"));
	CustomMotionWarpingComponent = CreateDefaultSubobject<UCustomMotionWarpingComponent>(TEXT("CustomMotionWarpingComponent"));
	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);

	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Set default character data: startup abilities and effects, animation data...
	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}

	// Bind MaxMovementSpeed attribute value change
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &ABaseCharacter::OnMaxMovementSpeedChanged);
	}
}

/** Called when the game starts */
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/** Called when this Pawn is possessed (only called on the server) */
void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveAbilities();
	ApplyStartupEffects();
}

/** PlayerState Replication Notification (only called on the client) */
void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CharacterData);
	DOREPLIFETIME(ABaseCharacter, InventoryComponent);
}

/** Tell client that the Pawn is begin restarted */
void ABaseCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

/** Called upon landing when falling, to perform actions based on the Hit result */
void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithTags(InAirTags);
	}
}

/** Request the character to start crouching. The request is processed on the next update of the CharacterMovementComponent */
void ABaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	if (AbilitySystemComponent && CrouchStateEffect.Get())
	{
		const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CrouchStateEffect, 1.f, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle CrouchActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			if (!CrouchActiveEffectHandle.WasSuccessfullyApplied())
			{
				UE_LOG(LogTemp, Error, TEXT("ABaseCharacter::OnStartCrouch - %s | Failed to apply crouch effect %s"), *GetName(), *GetNameSafe(CrouchStateEffect));
			}
		}
	}
}

/** Request the character to stop crouching. The request is processed on the next update of the CharacterMovementComponent */
void ABaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (AbilitySystemComponent && CrouchStateEffect.Get())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CrouchStateEffect, AbilitySystemComponent);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

#pragma endregion OVERRIDES

#pragma region INPUT

/** Allows a Pawn to set up custom input bindings */
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Setup action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		}
		
		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacter::StartJump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopJump);
		}

		// Crouching
		if (CrouchAction)
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABaseCharacter::StartCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopCrouch);
		}

		// Sprinting
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABaseCharacter::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
		}

		// Equip next item
		if (EquipNextItemAction)
		{
			EnhancedInputComponent->BindAction(EquipNextItemAction, ETriggerEvent::Triggered, this, &ABaseCharacter::EquipNextItem);
		}

		// Unequip item
		if (UnequipItemAction)
		{
			EnhancedInputComponent->BindAction(UnequipItemAction, ETriggerEvent::Triggered, this, &ABaseCharacter::UnequipItem);
		}

		// Drop item
		if (DropItemAction)
		{
			EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Triggered, this, &ABaseCharacter::DropItem);
		}
	}
}

/** Called for movement input */
void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	// Find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add movement in both directions
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

/** Called for looking input */
void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}
	
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

/** Called when jump is started */
void ABaseCharacter::StartJump(const FInputActionValue& Value)
{
	CustomCharacterMovementComponent->TryTraversal(AbilitySystemComponent);
}

/** Called when jump is stopped */
void ABaseCharacter::StopJump(const FInputActionValue& Value)
{
	
}

/** Called when crouch is started */
void ABaseCharacter::StartCrouch(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(CrouchTags, true);
	}
}

/** Called when crouch is stopped */
void ABaseCharacter::StopCrouch(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&CrouchTags);
	}
}

/** Called when sprint is started */
void ABaseCharacter::StartSprint(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags, true);
	}
}

/** Called when sprint is stopped */
void ABaseCharacter::StopSprint(const FInputActionValue& Value)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&SprintTags);
	}
}

/** Called when item is equipped */
void ABaseCharacter::EquipNextItem(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::EquipNextItemTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::EquipNextItemTag, EventPayload);
}

/** Called when item is unequipped */
void ABaseCharacter::UnequipItem(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::UnequipItemTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::UnequipItemTag, EventPayload);	
}

/** Called when item is dropped */
void ABaseCharacter::DropItem(const FInputActionValue& Value)
{
	FGameplayEventData EventPayload;
	EventPayload.EventTag = UInventoryComponent::DropItemTag;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UInventoryComponent::DropItemTag, EventPayload);
}

#pragma endregion INPUT

#pragma region GAS

#pragma region GAS_CORE

/** Returns the ability system component to use for this actor */
UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/** Give abilities */
void ABaseCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayAbility> DefaultAbility : CharacterData.Abilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

/** Apply gameplay effects at startup */
void ABaseCharacter::ApplyStartupEffects()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		for (const TSubclassOf<UGameplayEffect> DefaultEffect : CharacterData.Effects)
		{
			ApplyGameplayEffectToSelf(DefaultEffect, EffectContext);
		}
	}
}

/** Apply given gameplay effect to self */
bool ABaseCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& EffectContext) const
{
	if (Effect.Get())
	{
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			return ActiveEffectHandle.WasSuccessfullyApplied();
		}
	}

	return false;
}

/** Setter of CharacterData */
void ABaseCharacter::SetCharacterData(const FCharacterData& NewCharacterData)
{
	CharacterData = NewCharacterData;
	InitFromCharacterData(CharacterData);
}

/** Initialize values from CharacterData */
void ABaseCharacter::InitFromCharacterData(const FCharacterData& NewCharacterData, bool bFromReplication)
{
	
}

/** Replicate CharacterData */
void ABaseCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

/** Function bound to the delegate that is called whenever the MaxMovementSpeed attribute is changed */
void ABaseCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

#pragma endregion GAS_CORE

#pragma endregion GAS
