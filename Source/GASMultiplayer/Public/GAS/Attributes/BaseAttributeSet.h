// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

// Unreal Engine
#include "AbilitySystemComponent.h"

#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GASMULTIPLAYER_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
protected:

	/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made. */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region CORE

protected:

	/** Replicate Health */
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	/** Replicate MaxHealth */
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	/** Replicate Stamina */
	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	/** Replicate MaxStamina */
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	/** Replicate MaxMovementSpeed */
	UFUNCTION()
	void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed);

public:

	/** Health Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);

	/** MaxHealth Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	/** Stamina Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina);

	/** MaxStamina Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina);

	/** MaxMovementSpeed Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "AA|MovementSpeed", ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMovementSpeed);

#pragma endregion CORE

};
