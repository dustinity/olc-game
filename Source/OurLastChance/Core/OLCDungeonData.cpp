#include "Core/OLCDungeonData.h"

UOLCDungeonData::UOLCDungeonData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UOLCDungeonData> UOLCDungeonData::GetDefault()
{
	return UOLCDungeonData::StaticClass();
}

int32 UOLCDungeonData::GetRecommendedSquadMin(EOLCDungeonSize Size)
{
	switch (Size)
	{
		case EOLCDungeonSize::Tiny:     return 2;
		case EOLCDungeonSize::Small:    return 4;
		case EOLCDungeonSize::Medium:   return 6;
		case EOLCDungeonSize::Large:    return 10;
		case EOLCDungeonSize::Fortress: return 12;
		default:                        return 2;
	}
}

int32 UOLCDungeonData::GetRecommendedSquadMax(EOLCDungeonSize Size)
{
	switch (Size)
	{
		case EOLCDungeonSize::Tiny:     return 4;
		case EOLCDungeonSize::Small:    return 8;
		case EOLCDungeonSize::Medium:   return 12;
		case EOLCDungeonSize::Large:    return 16;
		case EOLCDungeonSize::Fortress: return 16;
		default:                        return 4;
	}
}

TArray<FOLCDungeonRespawnRule> UOLCDungeonData::GetDefaultRespawnRules()
{
	return TArray<FOLCDungeonRespawnRule>{
		FOLCDungeonRespawnRule(EOLCDungeonContentCategory::Enemies, 48.0f, true),
		FOLCDungeonRespawnRule(EOLCDungeonContentCategory::Materials, 72.0f, true),
		FOLCDungeonRespawnRule(EOLCDungeonContentCategory::Bosses, 168.0f, true),
		FOLCDungeonRespawnRule(EOLCDungeonContentCategory::Blueprints, 0.0f, false),
	};
}
