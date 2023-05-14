// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Structs/Utils/FastArrayTagCounter.h"

/** Get number of tags */
int32 FFastArrayTagCounter::GetTagCount(FGameplayTag InTag) const
{
	for (auto ItemIter = TagArray.CreateConstIterator(); ItemIter; ++ItemIter)
	{
		const FFastArrayTagCounterRecord& TagRecord = *ItemIter;
		if (TagRecord.Tag == InTag)
		{
			return TagRecord.Count;
		}
	}

	return 0;
}

/** Add tag count */
void FFastArrayTagCounter::AddTagCount(FGameplayTag InTag, int32 Delta)
{
	for (auto ItemIter = TagArray.CreateIterator(); ItemIter; ++ItemIter)
	{
		FFastArrayTagCounterRecord& TagRecord = *ItemIter;
		if (TagRecord.Tag == InTag)
		{
			TagRecord.Count += Delta;

			if (TagRecord.Count <= 0)
			{
				ItemIter.RemoveCurrent();
				MarkArrayDirty();
			}
			else
			{
				MarkItemDirty(TagRecord);
			}

			return;
		}
	}

	// Create item if it doesn't exist in the list
	FFastArrayTagCounterRecord& TagRecord = TagArray.AddDefaulted_GetRef();
	TagRecord.Tag = InTag;
	TagRecord.Count = Delta;
	MarkItemDirty(TagRecord);
}