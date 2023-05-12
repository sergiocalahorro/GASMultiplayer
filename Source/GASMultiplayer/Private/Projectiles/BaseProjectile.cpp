// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/BaseProjectile.h"

// Unreal Engine
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"

// GASMultiplayer
#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"
#include "Projectiles/ProjectileStaticData.h"

#pragma region INITIALIZATION
	
/** Sets default values for this actor's properties */
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicateMovement(true);
	bReplicates = true;

	// Setup hierarchy
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetIsReplicated(true);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	StaticMeshComponent->SetReceivesDecals(false);

	// Setup components
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMovementComponent->OnProjectileStop.AddUniqueDynamic(this, &ABaseProjectile::OnProjectileStop);
}

/** Called when the game starts or when spawned */
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeProjectile();

	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		StaticMeshComponent->SetStaticMesh(ProjectileData->StaticMesh);
		
		ProjectileMovementComponent->bInitialVelocityInLocalSpace = ProjectileData->bInitialVelocityInLocalSpace;
		ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = ProjectileData->bRotationFollowsVelocity;
		ProjectileMovementComponent->bShouldBounce = ProjectileData->bShouldBounce;
		ProjectileMovementComponent->Bounciness = ProjectileData->Bounciness;
		ProjectileMovementComponent->ProjectileGravityScale = (ProjectileMovementComponent->ProjectileGravityScale == 0.f) ? 0.0001f : ProjectileData->GravityScale;
		
		ProjectileMovementComponent->Velocity = ProjectileData->InitialSpeed * GetActorForwardVector();
	}

	// Debug
	DebugDrawPath();
}

/** Called when the game ends or when destroyed */
void ABaseProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileData->OnStopSFX, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileData->OnStopVFX, GetActorLocation());
	}

	Super::EndPlay(EndPlayReason);
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void ABaseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseProjectile, ProjectileStaticDataClass);
}

#pragma endregion OVERRIDES

#pragma region PROJECTILE

/** Get projectile's static data */
const UProjectileStaticData* ABaseProjectile::GetProjectileStaticData() const
{
	if (IsValid(ProjectileStaticDataClass))
	{
		return GetDefault<UProjectileStaticData>(ProjectileStaticDataClass);
	}

	return nullptr;
}

/** Initialize projectile with data values */
void ABaseProjectile::InitializeProjectile()
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		StaticMeshComponent->SetStaticMesh(ProjectileData->StaticMesh);
		
		ProjectileMovementComponent->bInitialVelocityInLocalSpace = ProjectileData->bInitialVelocityInLocalSpace;
		ProjectileMovementComponent->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = ProjectileData->MaxSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = ProjectileData->bRotationFollowsVelocity;
		ProjectileMovementComponent->bShouldBounce = ProjectileData->bShouldBounce;
		ProjectileMovementComponent->Bounciness = ProjectileData->Bounciness;
		ProjectileMovementComponent->ProjectileGravityScale = (ProjectileMovementComponent->ProjectileGravityScale == 0.f) ? 0.0001f : ProjectileData->GravityScale;
		
		ProjectileMovementComponent->Velocity = ProjectileData->InitialSpeed * GetActorForwardVector();
	}
}

/** Function called on projectile stopped */
void ABaseProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		UGASMultiplayerStatics::ApplyRadialDamage(this, GetOwner(), GetActorLocation(), ProjectileData->DamageRadius,
												  ProjectileData->BaseDamage, ProjectileData->Effects,
												  ProjectileData->RadialDamageQueryTypes, ProjectileData->RadialDamageTraceType);
	}

	Destroy();
}

/** Debug: Draw projectile's path */
void ABaseProjectile::DebugDrawPath() const
{
	static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
	const bool bShowCustomDebug = CVar->GetInt() > 0;
	if (!bShowCustomDebug)
	{
		return;
	}
	
	EDrawDebugTrace::Type DebugDrawType = bShowCustomDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	if (const UProjectileStaticData* ProjectileData = GetProjectileStaticData())
	{
		FPredictProjectilePathParams PredictProjectilePathParams;
		PredictProjectilePathParams.StartLocation = GetActorLocation();
		PredictProjectilePathParams.LaunchVelocity = ProjectileData->InitialSpeed * GetActorForwardVector();
		PredictProjectilePathParams.OverrideGravityZ = ProjectileData->GravityScale;
		PredictProjectilePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
		PredictProjectilePathParams.bTraceComplex = true;
		PredictProjectilePathParams.bTraceWithCollision = true;
		PredictProjectilePathParams.DrawDebugType = DebugDrawType;
		PredictProjectilePathParams.DrawDebugTime = 3.f;

		FPredictProjectilePathResult PredictProjectilePathResult;
		if (UGameplayStatics::PredictProjectilePath(this, PredictProjectilePathParams, PredictProjectilePathResult))
		{
			DrawDebugSphere(GetWorld(), PredictProjectilePathResult.HitResult.Location, 50.f, 10.f, FColor::Red);
		}
	}
}

#pragma endregion PROJECTILE


