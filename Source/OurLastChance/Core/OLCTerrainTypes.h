#pragma once

#include "CoreMinimal.h"
#include "Core/OLCBuildingData.h"
#include "Core/OLCResourceTypes.h"
#include "OLCTerrainTypes.generated.h"

UENUM(BlueprintType)
enum class EOLCTerrainTileRole : uint8
{
	Buildable  UMETA(DisplayName = "Buildable"),
	Restricted UMETA(DisplayName = "Restricted"),
	Blocked    UMETA(DisplayName = "Blocked"),
	Water      UMETA(DisplayName = "Water"),
	Resource   UMETA(DisplayName = "Resource"),
	Dungeon    UMETA(DisplayName = "Dungeon"),
	Landing    UMETA(DisplayName = "Landing"),
	Road       UMETA(DisplayName = "Road"),
	Outside    UMETA(DisplayName = "Outside"),
};

UENUM(BlueprintType)
enum class EOLCTerrainRenderMode : uint8
{
	SurfaceMesh UMETA(DisplayName = "Surface Mesh"),
	DebugTiles  UMETA(DisplayName = "Debug Tiles"),
};

USTRUCT(BlueprintType)
struct FOLCTerrainTile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	FIntPoint Coord = FIntPoint::ZeroValue;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	float Height = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	EOLCTerrainTileRole Role = EOLCTerrainTileRole::Buildable;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	EOLCResourceType ResourceType = EOLCResourceType::Minerals;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	float ResourceRichness = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	float DecorationDensity = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	bool bDiscovered = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Terrain")
	bool bOccupied = false;

	bool IsBuildable() const
	{
		return Role == EOLCTerrainTileRole::Buildable || Role == EOLCTerrainTileRole::Landing;
	}
};

USTRUCT(BlueprintType)
struct FOLCTerrainGenerationSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	int32 MapWidth = 128;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	int32 MapHeight = 128;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	float TileSize = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	float HeightScale = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	int32 Seed = 184736;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;
};
