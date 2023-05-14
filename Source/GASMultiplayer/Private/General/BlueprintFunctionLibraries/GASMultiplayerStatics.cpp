// Fill out your copyright notice in the Description page of Project Settings.

#include "General/BlueprintFunctionLibraries/GASMultiplayerStatics.h"

// Unreal Engine
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// GASMultiplayer
#include "Inventory/Item/ItemStaticData.h"
#include "Projectiles/ProjectileStaticData.h"
#include "Projectiles/BaseProjectile.h"

/** Get item static data */
const UItemStaticData* UGASMultiplayerStatics::GetItemStaticData(TSubclassOf<UItemStaticData> ItemDataClass)
{
	if (IsValid(ItemDataClass))
	{
		return GetDefault<UItemStaticData>(ItemDataClass);
	}

	return nullptr;
}

/** Apply radial damage */
void UGASMultiplayerStatics::ApplyRadialDamage(UObject* WorldContextObject, AActor* DamageCauser, FVector Location, float Radius, float DamageAmount, TArray<TSubclassOf<UGameplayEffect>> DamageEffects, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, ETraceTypeQuery TraceType)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(DamageCauser);

	TArray<AActor*> DamagedActors;
	UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, Location, Radius, ObjectTypes, nullptr, IgnoredActors, DamagedActors);

	// Debug
	static const IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowCustomDebug"));
	const bool bShowCustomDebug = CVar->GetInt() > 0;

	for (AActor* DamagedActor : DamagedActors)
	{
		FHitResult TraceHit;
		if (UKismetSystemLibrary::LineTraceSingle(WorldContextObject, Location, DamagedActor->GetActorLocation(), TraceType, true, IgnoredActors, EDrawDebugTrace::None, TraceHit, true))
		{
			AActor* TraceHitActor = TraceHit.GetActor();
			if (DamagedActor != TraceHitActor)
			{
				// Debug
				if (bShowCustomDebug)
				{
					DrawDebugLine(WorldContextObject->GetWorld(), Location, DamagedActor->GetActorLocation(), FColor::Red, false, 4.f, 0, 1);
					DrawDebugSphere(WorldContextObject->GetWorld(), TraceHit.Location, 16.f, 16, FColor::Red, false, 4.f, 0, 1);
					DrawDebugString(WorldContextObject->GetWorld(), TraceHit.Location, *GetNameSafe(TraceHitActor), nullptr, FColor::Red, 0.f, false, 1.f);
				}
				continue;
			}

			bool bWasApplied = false;
			
			if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TraceHitActor))
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddInstigator(DamageCauser, DamageCauser);

				for (TSubclassOf<UGameplayEffect> DamageEffect : DamageEffects)
				{
					FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.f, EffectContext);
					if (SpecHandle.Data.IsValid())
					{
						UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FGameplayTag::RequestGameplayTag(TEXT("Attribute.Health")), -DamageAmount);
						FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						if (ActiveEffectHandle.IsValid())
						{
							bWasApplied = true;
						}
					}
				}
			}

			// Debug
			if (bShowCustomDebug)
			{
				DrawDebugLine(WorldContextObject->GetWorld(), Location, DamagedActor->GetActorLocation(), bWasApplied ? FColor::Green : FColor::Red, false, 4.f, 0, 1);
				DrawDebugSphere(WorldContextObject->GetWorld(), TraceHit.Location, 16.f, 16, bWasApplied ? FColor::Green : FColor::Red, false, 4.f, 0, 1);
				DrawDebugString(WorldContextObject->GetWorld(), TraceHit.Location, *GetNameSafe(TraceHitActor), nullptr, FColor::White, 0.f, false, 1.f);
			}
		}
		else
		{
			// Debug
			if (bShowCustomDebug)
			{
				DrawDebugLine(WorldContextObject->GetWorld(), Location, DamagedActor->GetActorLocation(), FColor::Red, false, 4.f, 0, 1);
				DrawDebugSphere(WorldContextObject->GetWorld(), TraceHit.Location, 16.f, 16, FColor::Red, false, 4.f, 0, 1);
				DrawDebugString(WorldContextObject->GetWorld(), TraceHit.Location, *GetNameSafe(TraceHit.GetActor()), nullptr, FColor::Red, 0.f, false, 1.f);
			}
		}
	}

	// Debug
	if (bShowCustomDebug)
	{
		DrawDebugSphere(WorldContextObject->GetWorld(), Location, Radius,16, FColor::White, false, 4.f, 0, 1);
	}
}

/** Launch projectile */
ABaseProjectile* UGASMultiplayerStatics::LaunchProjectile(UObject* WorldContextObject, TSubclassOf<UProjectileStaticData> ProjectileDataClass, FTransform Transform, AActor* Owner, APawn* Instigator)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (!World->IsNetMode(NM_Client))
		{
			if (ABaseProjectile* Projectile = World->SpawnActorDeferred<ABaseProjectile>(ABaseProjectile::StaticClass(), Transform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
			{
				Projectile->SetProjectileStaticDataClass(ProjectileDataClass);
				Projectile->FinishSpawning(Transform);
				return Projectile;
			}
		}
	}

	return nullptr;
}