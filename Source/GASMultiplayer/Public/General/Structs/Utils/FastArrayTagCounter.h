// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// GASMultiplayer
#include "FastArrayTagCounterRecord.h"

#include "FastArrayTagCounter.generated.h"

USTRUCT(BlueprintType)
struct FFastArrayTagCounter : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()

public:

	/** Net serialize delta this structure. Serialize a network delta from a base state */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo &DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FFastArrayTagCounterRecord, FFastArrayTagCounter>(TagArray, DeltaParams, *this);
	}

	/** Get number of tags */
	int32 GetTagCount(FGameplayTag InTag) const;

	/** Add tag count */
	void AddTagCount(FGameplayTag InTag, int32 Delta);
	
	/** Getter of TagArray */
	const TArray<FFastArrayTagCounterRecord>& GetTagArray() const { return TagArray; }

public:

	/** List of records */
	UPROPERTY()
	TArray<FFastArrayTagCounterRecord> TagArray;
};

template<>
struct TStructOpsTypeTraits<FFastArrayTagCounter> : public TStructOpsTypeTraitsBase2<FFastArrayTagCounter>
{
	enum { WithNetDeltaSerializer = true };
};