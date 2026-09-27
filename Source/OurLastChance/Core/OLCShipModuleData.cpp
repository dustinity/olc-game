#include "Core/OLCShipModuleData.h"

UOLCShipModuleData::UOLCShipModuleData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UOLCShipModuleData> UOLCShipModuleData::GetDefault()
{
	return UOLCShipModuleData::StaticClass();
}
