#pragma once

#include "CoreMinimal.h"
#include "Core/OLCTerrainTypes.h"
#include "OLCPlanetTerrainGenerator.generated.h"

class UOLCPlanetTerrainProfile;

UCLASS()
class OURLASTCHANCE_API UOLCPlanetTerrainGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	static void GenerateTerrain(const FOLCTerrainGenerationSettings& Settings, const UOLCPlanetTerrainProfile* Profile, TArray<FOLCTerrainTile>& OutTiles);

private:
	static float TileNoise(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y, int32 Salt);
	static float SmoothNoise(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y, int32 Salt, int32 Radius);
	static bool IsInsidePlayableArea(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y);
	static bool IsInsideLandingArea(const FOLCTerrainGenerationSettings& Settings, int32 X, int32 Y);
};
