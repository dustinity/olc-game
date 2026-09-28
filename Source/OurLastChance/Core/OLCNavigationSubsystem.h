#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCNavigationTypes.h"
#include "OLCNavigationSubsystem.generated.h"

class UOLCScanTierData;

// ---------------------------------------------------------------------------
// EOLCDriveType — drive technology installed on the dropship.
// Pre-checked: ship module code (OLCShipModuleData.h) defines no drive-type
// enum with per-type fuel efficiency, so this is the canonical definition.
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCDriveType : uint8
{
	Chemical   UMETA(DisplayName = "Chemical Drive"),
	Ion        UMETA(DisplayName = "Ion Drive"),
	Fusion     UMETA(DisplayName = "Fusion Drive"),
	Antimatter UMETA(DisplayName = "Antimatter Drive"),
};

/** Delegate fired when a planet scan completes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScanCompleted, int32, SystemIndex, int32, PlanetIndex, EOLCScanTier, Tier);

/** A single in-flight scan tracked by the navigation subsystem. */
USTRUCT()
struct FOLCActiveScan
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SystemIndex = 0;

	UPROPERTY()
	int32 PlanetIndex = 0;

	UPROPERTY()
	EOLCScanTier Tier = EOLCScanTier::Basic;

	/** Remaining seconds until the scan completes. */
	UPROPERTY()
	float TimeRemaining = 0.0f;

	FOLCActiveScan() {}
};

/**
 * Navigation subsystem: deterministic solar-system generation, fuel math,
 * and the 4-tier planet scan system (energy cost + timed reveal).
 */
UCLASS()
class OURLASTCHANCE_API UOLCNavigationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Planets generated per solar system. */
	static constexpr int32 NumPlanetsPerSystem = 6;

	/** Get (or deterministically generate on first call) the planet layout for a solar system. */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	TArray<FOLCPlanetInfo> GetOrGeneratePlanetsForSystem(int32 SystemID) const;

	/** Fuel cost to travel Distance units with DriveType — linear in Distance. */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	float CalculateFuelCost(EOLCDriveType DriveType, float Distance) const;

	/** Galaxy-wide seed; per-system streams are seeded on GalaxySeed ^ SystemID. */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	int32 GetGalaxySeed() const { return GalaxySeed; }

	/** Set the galaxy seed. Clears cached layouts so they regenerate under the new seed. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	void SetGalaxySeed(int32 InGalaxySeed);

	// -----------------------------------------------------------------------
	// Scan system (4-tier)
	// -----------------------------------------------------------------------

	/** Register the scan tier DataAsset that holds per-tier cost/duration config. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	void SetScanTierData(UOLCScanTierData* InData);

	/** Look up a tier's config from the registered data asset (null if missing). */
	const FOLCScanTierConfig* FindTierConfig(EOLCScanTier Tier) const;

	/** The registered scan tier DataAsset itself, for UI that needs every configured tier entry (not just one per EOLCScanTier value). */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	UOLCScanTierData* GetScanTierData() const { return ScanTierData; }

	/**
	 * Start a scan of a planet at the given tier. Checks and deducts Energy via
	 * UOLCUIDataSubsystem (two-phase: verify affordability first, then deduct).
	 * Returns false without deducting anything when Energy is insufficient.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	bool StartScan(int32 SystemIndex, int32 PlanetIndex, EOLCScanTier Tier);

	/** Advance all active scans by DeltaTime; on completion reveal planet info and broadcast. */
	void TickScans(float DeltaTime);

	/** Fired exactly once per completed scan with the system/planet index and tier. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Navigation")
	FOnScanCompleted OnScanCompleted;

	// -----------------------------------------------------------------------
	// Galaxy clusters, warp routes, and inter-system travel (WP-121 Step 4)
	// -----------------------------------------------------------------------

	/** Solar system clusters on the galaxy map (S12). */
	static constexpr int32 NumGalaxyClusters = 9;

	/** Get (or deterministically generate on first call) the 9 galaxy clusters in spiral layout. */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	TArray<FOLCGalaxyCluster> GetOrGenerateGalaxyClusters() const;

	/** Warp routes between adjacent clusters with fuel costs from CalculateFuelCost. */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	TArray<FOLCSolarWarpRoute> GetWarpRoutes() const;

	/**
	 * Pay the route's fuel cost and travel to TargetClusterID. Two-phase like
	 * StartScan: verify Fuel affordability first, then deduct. Returns false
	 * without deducting fuel or moving when Fuel is insufficient.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	bool TryPayFuelAndTravel(int32 TargetClusterID) const;

	/** ID of the cluster the player currently occupies (0-based cluster ID). */
	UFUNCTION(BlueprintPure, Category = "OLC|Navigation")
	int32 GetCurrentSystemID() const { return CurrentSystemID; }

	// -----------------------------------------------------------------------
	// Void Warp Drive — instant travel within 5-system radius (WP-125 Step 2)
	// -----------------------------------------------------------------------

	/**
	 * Instant warp to TargetClusterID with zero fuel cost, allowed only when the
	 * cluster distance |CurrentSystemID - TargetClusterID| is <= 5. Bypasses the
	 * warp-route graph entirely. Returns false for out-of-range targets or when
	 * the distance exceeds the 5-system radius.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	bool TryVoidWarp(int32 TargetClusterID) const;

	// -----------------------------------------------------------------------
	// Alien Warp Integration — unlimited range at 0.01x fuel cost (WP-125 Step 3)
	// -----------------------------------------------------------------------

	/**
	 * Wormhole transit to any TargetClusterID regardless of distance. Charges
	 * 0.01x the normal route fuel cost (minimum 1 fuel if normal cost > 0).
	 * Returns false for out-of-range targets or when fuel is insufficient.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Navigation")
	bool TryAlienWarp(int32 TargetClusterID) const;

private:
	/** Fuel consumed per unit distance for a drive type (lower = more efficient). */
	static float GetDriveFuelEfficiency(EOLCDriveType DriveType);

	/** Apply the tier's reveal flags to a planet (biome/resources/dungeons/features). */
	void ApplyScanReveal(FOLCPlanetInfo& Planet, EOLCScanTier Tier) const;

	/** Deterministic planet layout cache, keyed by system ID. */
	mutable TMap<int32, TArray<FOLCPlanetInfo>> CachedPlanetsBySystem;

	/** Cached spiral galaxy layout (regenerated when GalaxySeed changes). */
	mutable TArray<FOLCGalaxyCluster> CachedGalaxyClusters;

	/** Cluster the player currently occupies (0-based cluster ID). */
	mutable int32 CurrentSystemID = 0;

	/** Galaxy-wide determinism seed (default until a campaign sets one). */
	int32 GalaxySeed = 20260827;

	/** Registered scan tier configuration asset. */
	UPROPERTY()
	TObjectPtr<UOLCScanTierData> ScanTierData;

	/** In-flight scans advanced by TickScans. */
	TArray<FOLCActiveScan> ActiveScans;
};
