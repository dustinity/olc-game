#include "UOLCTechData.h"

UOLCTechData::UOLCTechData()
{
	ResearchTimeSeconds = 60.0f;
	bAutoUnlock = false;
}

bool UOLCTechData::ArePrerequisitesMet(const TArray<TObjectPtr<UOLCTechData>>& CompletedTechs) const
{
	for (const TObjectPtr<UOLCTechData>& Prereq : Prerequisites)
	{
		if (!Prereq)
		{
			continue;
		}

		bool bFound = false;
		for (const TObjectPtr<UOLCTechData>& Completed : CompletedTechs)
		{
			if (Completed == Prereq)
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			return false;
		}
	}

	return true;
}

FText UOLCTechData::GetCostSummary() const
{
	FString Summary;
	for (const FOLCResourceAmount& Cost : MaterialCost)
	{
		Summary += FString::Printf(
			TEXT("%s: %.0f  "),
			*UEnum::GetValueAsString(Cost.ResourceType),
			Cost.CurrentValue);
	}

	return FText::FromString(Summary);
}

int32 UOLCTechData::GetRingTierIndex() const
{
	switch (RingTier)
	{
		case ERingTier::Core:  return 0;
		case ERingTier::Ring1: return 1;
		case ERingTier::Ring2: return 2;
		case ERingTier::Ring3: return 3;
		case ERingTier::Outer: return 4;
		default:               return 0;
	}
}
