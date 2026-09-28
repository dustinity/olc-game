#pragma once

#include "CoreMinimal.h"
#include "OLCNavigationTypes.generated.h"

// ---------------------------------------------------------------------------
// EOLCScanTier — scan depth reached on a planet, gates how much info is revealed.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCScanTier : uint8
{
	None      UMETA(DisplayName = "Unscanned"),
	Basic     UMETA(DisplayName = "Basic Scan"),
	Detailed  UMETA(DisplayName = "Detailed Scan"),
	Deep      UMETA(DisplayName = "Deep Scan"),
};

/** A planet in the solar system. */
USTRUCT(BlueprintType)
struct FOLCPlanetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText PlanetName;

	/** Orbital angle in degrees (0-360). */
	UPROPERTY()
	float OrbitAngle = 0.0f;

	/** Distance from sun (used for orbital radius). */
	UPROPERTY()
	float OrbitRadius = 200.0f;

	/** Planet radius on screen. */
	UPROPERTY()
	float Radius = 24.0f;

	/** Computed screen position for the current orbital canvas pass. */
	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	/** Biome type (empty until scanned). */
	UPROPERTY()
	FText BiomeType;

	/** Resources found on planet (empty until scanned). */
	UPROPERTY()
	FText Resources;

	/** TIR rating of the planet. */
	UPROPERTY()
	int32 TIR = 1;

	/** Whether this planet has been scanned. */
	UPROPERTY()
	bool bScanned = false;

	/** Whether the player currently occupies this planet. */
	UPROPERTY()
	bool bIsCurrentPlanet = false;

	/** Fuel cost to travel here from current planet. */
	UPROPERTY()
	int32 FuelCost = 100;

	/** Energy cost for basic scan (10 energy). */
	UPROPERTY()
	int32 ScanCost = 10;

	/** Highest scan tier reached so far — gates how much detail is revealed. */
	UPROPERTY()
	EOLCScanTier ScanTierAchieved = EOLCScanTier::None;

	/** Named features (dungeons, anomalies, resource nodes) uncovered by scanning. */
	UPROPERTY()
	TArray<FText> DiscoveredFeatures;

	/** Screen-space locations of dungeons revealed on this planet. */
	UPROPERTY()
	TArray<FVector2D> DungeonLocations;

	FOLCPlanetInfo() {}
};

/** A station or waypoint in the solar system. */
USTRUCT()
struct FOLCStationInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText StationName;

	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	UPROPERTY()
	bool bVisited = false;

	FOLCStationInfo() {}
};

/** A full solar system: its planets, stations, and identity. */
USTRUCT()
struct FOLCSolarSystemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText SystemName;

	UPROPERTY()
	TArray<FOLCPlanetInfo> Planets;

	UPROPERTY()
	TArray<FOLCStationInfo> Stations;

	FOLCSolarSystemInfo() {}
};

/** A warp connection between two solar systems. */
USTRUCT(BlueprintType)
struct FOLCSolarWarpRoute
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	FText FromSystem;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	FText ToSystem;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	int32 FuelCost = 100;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	bool bDiscovered = false;

	FOLCSolarWarpRoute() {}
};

/** Designer-tunable cost/reward configuration for one scan tier. */
USTRUCT(BlueprintType)
struct FOLCScanTierConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan")
	EOLCScanTier ScanTier = EOLCScanTier::Basic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan")
	int32 EnergyCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan")
	float ScanDuration = 300.0f;

	/** Whether this tier reveals BiomeType (matches ApplyScanReveal's BiomeType gate). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan|Reveal")
	bool bRevealBiome = false;

	/** Whether this tier reveals Resources (matches ApplyScanReveal's Tier>=Detailed gate). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan|Reveal")
	bool bRevealResources = false;

	/** Whether this tier reveals DungeonLocations (matches ApplyScanReveal's Tier>=Deep gate). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan|Reveal")
	bool bRevealDungeons = false;

	/** Whether this tier reveals DiscoveredFeatures (matches ApplyScanReveal's Tier>=Deep gate). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan|Reveal")
	bool bRevealFeatures = false;

	FOLCScanTierConfig() {}
};

/** A solar system cluster on the galaxy map (S12), positioned in a spiral layout. */
USTRUCT(BlueprintType)
struct FOLCGalaxyCluster
{
	GENERATED_BODY()

	/** Stable identity of this cluster (0-based index into the spiral). */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	int32 ClusterID = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	FText ClusterName;

	/** Position on the galaxy map canvas, relative to the galactic center. */
	UPROPERTY(BlueprintReadOnly, Category = "OLC|Navigation")
	FVector2D Position = FVector2D::ZeroVector;

	FOLCGalaxyCluster() {}
};
