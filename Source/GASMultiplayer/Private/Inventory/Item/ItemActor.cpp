// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Item/ItemActor.h"

// Unreal Engine
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// GASMultiplayer
#include "General/Components/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Item/ItemStaticData.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void AItemActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItemActor::OnSphereBeginOverlap);
}

/** Called when the game starts or when spawned */
void AItemActor::BeginPlay() 
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!IsValid(ItemInstance) && IsValid(ItemStaticDataClass))
		{
			ItemInstance = NewObject<UInventoryItemInstance>();
			ItemInstance->Init(ItemStaticDataClass);
			
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SphereComponent->SetGenerateOverlapEvents(true);

			Init_Internal();
		}
	}
}

/** Called every frame */
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/** Method that allows an actor to replicate subobjects on its actor channel */
bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bWroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
	return bWroteSomething;
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemInstance);
	DOREPLIFETIME(AItemActor, ItemState);
}

#pragma endregion OVERRIDES

#pragma region ITEM

/** Init ItemInstance */
void AItemActor::Init(UInventoryItemInstance* InItemInstance)
{
	ItemInstance = InItemInstance;
	Init_Internal();
}

/** Functionality performed when the item is equipped */
void AItemActor::OnEquipped()
{
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

/** Functionality performed when the item is unequipped */
void AItemActor::OnUnequipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

/** Functionality performed when the item is dropped */
void AItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector ForwardVector = ActorOwner->GetActorForwardVector();

		const float DroppedItemOffset = 100.f;
		const float DroppedItemTraceDistance = 1000.f;

		const FVector TraceStart = Location + ForwardVector * DroppedItemOffset;
		const FVector TraceEnd = TraceStart - FVector::UpVector * DroppedItemTraceDistance;

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		IgnoredActors.Add(ActorOwner);
		
		FHitResult Hit;

		// Debug
		static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowInventoryDebug"));
		const bool bShowTraversal = CVar->GetInt() > 0;
		EDrawDebugTrace::Type DebugDrawType = bShowTraversal ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		
		FVector TargetLocation = TraceEnd;
		
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true,
			IgnoredActors, DebugDrawType, Hit, true))
		{
			if (Hit.bBlockingHit)
			{
				TargetLocation = Hit.Location;
			}
		}

		SetActorLocation(TargetLocation);

		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
	}
}

/** Client-side initialization (used for initializing items's visuals) */
void AItemActor::Init_Internal()
{
	
}

/** Functionality performed when some Actor enters Item's sphere */
void AItemActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		FGameplayEventData EventPayload;
		EventPayload.Instigator = this;
		EventPayload.OptionalObject = ItemInstance;
		EventPayload.EventTag = UInventoryComponent::EquipItemActorTag;
	
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, UInventoryComponent::EquipItemActorTag, EventPayload);
	}
}

/** Replicate ItemState */
void AItemActor::OnRep_ItemState()
{
	const bool bEnableCollision = ItemState == EItemState::Dropped;
	const ECollisionEnabled::Type CollisionEnabled = bEnableCollision ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
	SphereComponent->SetCollisionEnabled(CollisionEnabled);
	SphereComponent->SetGenerateOverlapEvents(bEnableCollision);
}

/** Replicate ItemInstance */
void AItemActor::OnRep_ItemInstance(UInventoryItemInstance* OldItemInstance)
{
	if (IsValid(ItemInstance) && !IsValid(OldItemInstance))
	{
		Init_Internal();
	}
}

#pragma endregion ITEM
