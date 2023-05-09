// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/ItemActor.h"

// Unreal Engine
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// GASMultiplayer
#include "Inventory/InventoryItemInstance.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
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
}

/** Functionality performed when the item is equipped */
void AItemActor::OnEquipped()
{
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/** Functionality performed when the item is unequipped */
void AItemActor::OnUnequipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/** Functionality performed when the item is dropped */
void AItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

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
		
		if (UKismetSystemLibrary::LineTraceSingleByProfile(this, TraceStart, TraceEnd, TEXT("WorldStatic"), true,
			IgnoredActors, DebugDrawType, Hit, true))
		{
			if (Hit.bBlockingHit)
			{
				SetActorLocation(Hit.Location);
			}
			return;
		}

		SetActorLocation(TraceEnd);
	}
}

/** Functionality performed when some Actor enters Item's sphere */
void AItemActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEventData Payload;
	Payload.OptionalObject = this;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, SphereBeginOverlapEventTag, Payload);
}

#pragma endregion ITEM
