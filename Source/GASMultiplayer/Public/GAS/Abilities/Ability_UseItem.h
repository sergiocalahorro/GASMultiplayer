// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "BaseGameplayAbility.h"

#include "Ability_UseItem.generated.h"

// Forward declarations - GASMultiplayer
class UInventoryComponent;
class UInventoryItemInstance;
class UItemStaticData;
class AItemActor;

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UAbility_UseItem : public UBaseGameplayAbility
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Constructor */
	UAbility_UseItem();

#pragma endregion INITIALIZATION

#pragma region ABILITY

public:

	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

#pragma endregion ABILITY
	
#pragma region ITEM

	/** Getter of InventoryComponent */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	/** Get equipped item instance */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryItemInstance* GetEquippedItemInstance() const;

	/** Get equipped item's static data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticData* GetEquippedItemStaticData() const;

	/** Get equipped item Actor */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AItemActor* GetEquippedItemActor() const;

protected:

	/** Owning ability Actor's inventory component */
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
#pragma endregion ITEM
	
};
