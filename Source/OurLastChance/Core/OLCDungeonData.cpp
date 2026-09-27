#include "Core/OLCDungeonData.h"

UOLCDungeonData::UOLCDungeonData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UOLCDungeonData> UOLCDungeonData::GetDefault()
{
	return UOLCDungeonData::StaticClass();
}
