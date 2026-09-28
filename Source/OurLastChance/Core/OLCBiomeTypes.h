#pragma once

#include "CoreMinimal.h"

#include "OLCBiomeTypes.generated.h"

UENUM(BlueprintType)
enum class EOLCRaceBiomeType : uint8
{
	Jungle              UMETA(DisplayName = "Jungle"),
	Swamp               UMETA(DisplayName = "Swamp"),
	Desert              UMETA(DisplayName = "Desert"),
	Rocky               UMETA(DisplayName = "Rocky"),
	Ice                 UMETA(DisplayName = "Ice"),
	CrystalCaves        UMETA(DisplayName = "Crystal Caves"),
	DarkZones           UMETA(DisplayName = "Dark Zones"),
	Ruins               UMETA(DisplayName = "Ruins"),
	IndustrialWreckage  UMETA(DisplayName = "Industrial Wreckage"),
	LightSnow           UMETA(DisplayName = "Light Snow"),
	CenterGalaxy        UMETA(DisplayName = "Center Galaxy"),
	All                 UMETA(DisplayName = "All")
};
