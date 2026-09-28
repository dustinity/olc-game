#include "Core/OLCNavigationSubsystem.h"
#include "OurLastChance.h"

#include "Math/RandomStream.h"
#include "Core/OLCScanTierData.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"

// ---------------------------------------------------------------------------
// Drive fuel efficiency — fuel units consumed per unit distance.
// Each step up the tech ladder halves the cost (twice as efficient).
// ---------------------------------------------------------------------------
float UOLCNavigationSubsystem::GetDriveFuelEfficiency(EOLCDriveType DriveType)
{
	switch (DriveType)
	{
	case EOLCDriveType::Chemical:   return 1.0f;
	case EOLCDriveType::Ion:        return 0.5f;
	case EOLCDriveType::Fusion:     return 0.25f;
	case EOLCDriveType::Antimatter: return 0.125f;
	}

	// Unknown / out-of-range value — fall back to the baseline chemical drive.
	return 1.0f;
}

void UOLCNavigationSubsystem::SetGalaxySeed(int32 InGalaxySeed)
{
	GalaxySeed = InGalaxySeed;

	// Cached layouts were rolled under the previous seed.
	CachedPlanetsBySystem.Reset();
	CachedGalaxyClusters.Reset();
}

float UOLCNavigationSubsystem::CalculateFuelCost(EOLCDriveType DriveType, float Distance) const
{
	if (Distance <= 0.0f)
		return 0.0f;

	return Distance * GetDriveFuelEfficiency(DriveType);
}

TArray<FOLCPlanetInfo> UOLCNavigationSubsystem::GetOrGeneratePlanetsForSystem(int32 SystemID) const
{
	if (const TArray<FOLCPlanetInfo>* Cached = CachedPlanetsBySystem.Find(SystemID))
	{
		return *Cached;
	}

	// Deterministic per-system stream: GalaxySeed ^ SystemID, combined into one uint32 seed.
	const uint32 CombinedSeed = (uint32)GalaxySeed ^ (uint32)SystemID;
	FRandomStream RandomStream(CombinedSeed);

	static const TCHAR* const NamePrefixes[] = {
		TEXT("Ae"), TEXT("Vor"), TEXT("Kae"), TEXT("Thy"), TEXT("Zar"), TEXT("Umb"), TEXT("Hel"), TEXT("Oss")
	};
	static const TCHAR* const NameSuffixes[] = {
		TEXT("thel"), TEXT("nax"), TEXT("lis"), TEXT("ron"), TEXT("ara"), TEXT("dyn"), TEXT("os"), TEXT("ra")
	};

	const float InnermostOrbit = 150.0f; // closest orbit to the sun
	const float OrbitSpacing   = 70.0f;  // radial spacing between successive orbits

	TArray<FOLCPlanetInfo> Planets;
	Planets.Reserve(NumPlanetsPerSystem);

	for (int32 i = 0; i < NumPlanetsPerSystem; i++)
	{
		FOLCPlanetInfo Planet;
		Planet.PlanetName = FText::FromString(FString::Printf(TEXT("%s%s"),
			NamePrefixes[RandomStream.RandRange(0, (int32)UE_ARRAY_COUNT(NamePrefixes))],
			NameSuffixes[RandomStream.RandRange(0, (int32)UE_ARRAY_COUNT(NameSuffixes))]));

		// Varying orbits: base radius grows with index; jitter stays below the
		// spacing so no two orbit rings cross.
		Planet.OrbitRadius = InnermostOrbit + i * OrbitSpacing + RandomStream.FRandRange(0.0f, 40.0f);
		Planet.OrbitAngle  = RandomStream.FRandRange(0.0f, 360.0f);
		Planet.Radius      = RandomStream.FRandRange(18.0f, 34.0f);
		Planet.TIR         = RandomStream.RandRange(1, 5); // TIR 1-4

		Planet.bIsCurrentPlanet = (i == 0); // first planet is the current location
		Planet.FuelCost = 0;                // filled in below for non-current planets
		Planets.Add(Planet);
	}

	// Fuel cost from the current planet (index 0) to each other planet, using the
	// straight-line distance between their orbital positions and the baseline drive.
	const FOLCPlanetInfo& Origin = Planets[0];
	for (int32 i = 1; i < Planets.Num(); i++)
	{
		FOLCPlanetInfo& Planet = Planets[i];
		const float DTheta = FMath::DegreesToRadians(Planet.OrbitAngle - Origin.OrbitAngle);
		const float StraightLine = FMath::Sqrt(
			FMath::Square(Origin.OrbitRadius) + FMath::Square(Planet.OrbitRadius) -
			2.0f * Origin.OrbitRadius * Planet.OrbitRadius * FMath::Cos(DTheta));
		Planet.FuelCost = FMath::RoundToInt(CalculateFuelCost(EOLCDriveType::Chemical, StraightLine));
	}

	// Cache the freshly rolled layout so repeated queries never re-roll.
	CachedPlanetsBySystem.Add(SystemID, Planets);
	return Planets;
}

// ---------------------------------------------------------------------------
// Scan system (4-tier)
// ---------------------------------------------------------------------------

void UOLCNavigationSubsystem::SetScanTierData(UOLCScanTierData* InData)
{
	ScanTierData = InData;
}

const FOLCScanTierConfig* UOLCNavigationSubsystem::FindTierConfig(EOLCScanTier Tier) const
{
	if (!ScanTierData) return nullptr;

	for (const FOLCScanTierConfig& Config : ScanTierData->ScanTiers)
	{
		if (Config.ScanTier == Tier)
		{
			return &Config;
		}
	}
	return nullptr;
}

bool UOLCNavigationSubsystem::StartScan(int32 SystemIndex, int32 PlanetIndex, EOLCScanTier Tier)
{
	// Need a registered tier config to know the cost/duration.
	const FOLCScanTierConfig* Config = FindTierConfig(Tier);
	if (!Config)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartScan: no config for tier %d (ScanTierData not set?)"),
			static_cast<int32>(Tier));
		return false;
	}

	// Resolve the planet so we can validate indices and later reveal it.
	TArray<FOLCPlanetInfo> Planets = GetOrGeneratePlanetsForSystem(SystemIndex);
	if (PlanetIndex < 0 || PlanetIndex >= Planets.Num())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartScan: invalid planet index %d for system %d"),
			PlanetIndex, SystemIndex);
		return false;
	}

	// Two-phase Energy deduction — mirrors OLCResearchSubsystem::DeductMaterialCosts:
	// first verify affordability (abort all if unaffordable), then deduct.
	if (!GetWorld()) return false;
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (!GI) return false;
	UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
	if (!Data) return false;

	const float EnergyCost = static_cast<float>(Config->EnergyCost);

	// Phase 1: check we can afford the energy.
	bool bHasEnough = false;
	for (const auto& Res : Data->GetResourceCounters())
	{
		if (Res.ResourceType == EOLCResourceType::Energy && Res.Value >= EnergyCost)
		{
			bHasEnough = true;
			break;
		}
	}

	if (!bHasEnough)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartScan: insufficient energy (need %d)"), Config->EnergyCost);
		return false; // Abort — deduct nothing.
	}

	// Phase 2: deduct exactly the tier's energy cost.
	Data->AddResource(EOLCResourceType::Energy, -EnergyCost);

	// Record the active scan.
	FOLCActiveScan Scan;
	Scan.SystemIndex = SystemIndex;
	Scan.PlanetIndex = PlanetIndex;
	Scan.Tier = Tier;
	Scan.TimeRemaining = Config->ScanDuration;
	ActiveScans.Add(Scan);

	UE_LOG(LogOLC, Display, TEXT("[OLC] Started scan: system %d planet %d tier %d (cost %d energy, %.1fs)"),
		SystemIndex, PlanetIndex, static_cast<int32>(Tier), Config->EnergyCost, Config->ScanDuration);

	return true;
}

void UOLCNavigationSubsystem::TickScans(float DeltaTime)
{
	if (ActiveScans.Num() == 0) return;

	// Collect completed scans first so we can remove them while iterating.
	TArray<int32> CompletedIndices;
	for (int32 i = 0; i < ActiveScans.Num(); i++)
	{
		FOLCActiveScan& Scan = ActiveScans[i];
		Scan.TimeRemaining -= DeltaTime;
		if (Scan.TimeRemaining <= 0.0f)
		{
			CompletedIndices.Add(i);
		}
	}

	// Process completions in reverse so removals don't invalidate earlier indices.
	for (int32 k = CompletedIndices.Num() - 1; k >= 0; k--)
	{
		const int32 Index = CompletedIndices[k];
		const FOLCActiveScan& Scan = ActiveScans[Index];

		TArray<FOLCPlanetInfo> Planets = GetOrGeneratePlanetsForSystem(Scan.SystemIndex);
		if (Scan.PlanetIndex >= 0 && Scan.PlanetIndex < Planets.Num())
		{
			FOLCPlanetInfo& Planet = Planets[Scan.PlanetIndex];
			ApplyScanReveal(Planet, Scan.Tier);

			// Write the revealed planet back into the per-system cache so the
			// reveal persists across subsequent GetOrGeneratePlanetsForSystem calls.
			CachedPlanetsBySystem.Add(Scan.SystemIndex, Planets);
		}

		ActiveScans.RemoveAt(Index);

		UE_LOG(LogOLC, Display, TEXT("[OLC] Scan completed: system %d planet %d tier %d"),
			Scan.SystemIndex, Scan.PlanetIndex, static_cast<int32>(Scan.Tier));

		// Broadcast exactly once per completed scan.
		OnScanCompleted.Broadcast(Scan.SystemIndex, Scan.PlanetIndex, Scan.Tier);
	}
}

void UOLCNavigationSubsystem::ApplyScanReveal(FOLCPlanetInfo& Planet, EOLCScanTier Tier) const
{
	// Every completed scan marks the planet as scanned and records the tier reached.
	Planet.bScanned = true;
	Planet.ScanTierAchieved = Tier;

	// Biome is revealed at every tier (Basic and above).
	static const FText Biomes[] = {
		FText::FromString(TEXT("Desert")), FText::FromString(TEXT("Dusty")),
		FText::FromString(TEXT("Rocky")), FText::FromString(TEXT("Water")),
		FText::FromString(TEXT("Swamp")), FText::FromString(TEXT("Jungle")),
		FText::FromString(TEXT("LightSnow")), FText::FromString(TEXT("Ice"))
	};
	if (Planet.BiomeType.IsEmpty())
	{
		// Deterministic biome derived from the planet's TIR so it is stable.
		const int32 BiomeIdx = (Planet.TIR * 3 + 7) % UE_ARRAY_COUNT(Biomes);
		Planet.BiomeType = Biomes[BiomeIdx];
	}

	// Resources are revealed at Detailed and above.
	if (Tier >= EOLCScanTier::Detailed && Planet.Resources.IsEmpty())
	{
		const int32 ResIdx = Planet.TIR % 3;
		switch (ResIdx)
		{
		case 0:  Planet.Resources = FText::FromString(TEXT("Minerals, Construction Material")); break;
		case 1:  Planet.Resources = FText::FromString(TEXT("Fuel, Energy Cells")); break;
		default: Planet.Resources = FText::FromString(TEXT("Minerals, Hull Parts, Survival")); break;
		}
	}

	// Dungeon locations + discovered features are revealed at Deep and above.
	if (Tier >= EOLCScanTier::Deep)
	{
		if (Planet.DungeonLocations.Num() == 0)
		{
			const int32 NumDungeons = FMath::Max(1, Planet.TIR - 1);
			for (int32 d = 0; d < NumDungeons; d++)
			{
				Planet.DungeonLocations.Add(FVector2D(
					FMath::FRandRange(-Planet.Radius, Planet.Radius),
					FMath::FRandRange(-Planet.Radius, Planet.Radius)));
			}
		}

		if (Planet.DiscoveredFeatures.Num() == 0)
		{
			static const TCHAR* const FeatureNames[] = {
				TEXT("Ancient Ruins"), TEXT("Energy Anomaly"), TEXT("Resource Node"),
				TEXT("Alien Beacon"), TEXT("Crater Field")
			};
			const int32 NumFeatures = FMath::Max(1, Planet.TIR - 1);
			for (int32 f = 0; f < NumFeatures; f++)
			{
				Planet.DiscoveredFeatures.Add(FText::FromString(
					FeatureNames[(Planet.TIR + f) % UE_ARRAY_COUNT(FeatureNames)]));
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Galaxy clusters, warp routes, and inter-system travel (WP-121 Step 4)
// ---------------------------------------------------------------------------

TArray<FOLCGalaxyCluster> UOLCNavigationSubsystem::GetOrGenerateGalaxyClusters() const
{
	if (CachedGalaxyClusters.Num() > 0)
	{
		return CachedGalaxyClusters;
	}

	// Spiral geometry is pure arithmetic on the index: the angle increases
	// monotonically and the radius strictly increases with each cluster, so
	// the layout ordering never depends on the random stream.
	const float BaseAngleDeg = 0.0f;  // starting angle of the spiral
	const float AngleStepDeg = 45.0f; // angle advance per successive cluster
	const float BaseRadius   = 80.0f; // innermost cluster radius from galactic center
	const float RadiusStep   = 60.0f; // radial advance per successive cluster

	// Deterministic stream seeded on GalaxySeed — used only for non-geometric
	// fields (cluster names), never for the radius/angle ordering.
	FRandomStream RandomStream((uint32)GalaxySeed);

	static const TCHAR* const NamePrefixes[] = {
		TEXT("Ae"), TEXT("Vor"), TEXT("Kae"), TEXT("Thy"), TEXT("Zar"), TEXT("Umb"), TEXT("Hel"), TEXT("Oss")
	};
	static const TCHAR* const NameSuffixes[] = {
		TEXT("thel"), TEXT("nax"), TEXT("lis"), TEXT("ron"), TEXT("ara"), TEXT("dyn"), TEXT("os"), TEXT("ra")
	};

	TArray<FOLCGalaxyCluster> Clusters;
	Clusters.Reserve(NumGalaxyClusters);

	for (int32 i = 0; i < NumGalaxyClusters; i++)
	{
		FOLCGalaxyCluster Cluster;
		Cluster.ClusterID = i;
		Cluster.ClusterName = FText::FromString(FString::Printf(TEXT("%s%s"),
			NamePrefixes[RandomStream.RandRange(0, (int32)UE_ARRAY_COUNT(NamePrefixes))],
			NameSuffixes[RandomStream.RandRange(0, (int32)UE_ARRAY_COUNT(NameSuffixes))]));

		const float AngleRad = FMath::DegreesToRadians(BaseAngleDeg + i * AngleStepDeg);
		const float Radius   = BaseRadius + i * RadiusStep;
		Cluster.Position = FVector2D(Radius * FMath::Cos(AngleRad), Radius * FMath::Sin(AngleRad));

		Clusters.Add(Cluster);
	}

	CachedGalaxyClusters = Clusters;
	return Clusters;
}

TArray<FOLCSolarWarpRoute> UOLCNavigationSubsystem::GetWarpRoutes() const
{
	const TArray<FOLCGalaxyCluster> Clusters = GetOrGenerateGalaxyClusters();

	TArray<FOLCSolarWarpRoute> Routes;

	// Connected but not all-to-all: adjacent clusters (i <-> i+1) plus a
	// skip-one mesh (i <-> i+2), each pair in both directions so the player
	// can always travel back along a route they used to arrive.
	for (int32 i = 0; i < Clusters.Num(); i++)
	{
		const int32 JumpTargets[] = { i + 1, i + 2 };
		for (const int32 j : JumpTargets)
		{
			if (j >= Clusters.Num())
			{
				break;
			}

			const float Distance = FVector2D::Distance(Clusters[i].Position, Clusters[j].Position);
			const int32 FuelCost = FMath::RoundToInt(CalculateFuelCost(EOLCDriveType::Chemical, Distance));

			FOLCSolarWarpRoute Outbound;
			Outbound.FromSystem = Clusters[i].ClusterName;
			Outbound.ToSystem   = Clusters[j].ClusterName;
			Outbound.FuelCost   = FuelCost;
			Routes.Add(Outbound);

			FOLCSolarWarpRoute Inbound;
			Inbound.FromSystem = Clusters[j].ClusterName;
			Inbound.ToSystem   = Clusters[i].ClusterName;
			Inbound.FuelCost   = FuelCost;
			Routes.Add(Inbound);
		}
	}

	return Routes;
}

bool UOLCNavigationSubsystem::TryPayFuelAndTravel(int32 TargetClusterID) const
{
	const TArray<FOLCGalaxyCluster> Clusters = GetOrGenerateGalaxyClusters();
	if (TargetClusterID < 0 || TargetClusterID >= Clusters.Num())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryPayFuelAndTravel: invalid cluster id %d"), TargetClusterID);
		return false;
	}
	if (CurrentSystemID < 0 || CurrentSystemID >= Clusters.Num())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryPayFuelAndTravel: current system id %d out of range"), CurrentSystemID);
		return false;
	}

	const FOLCGalaxyCluster& Current = Clusters[CurrentSystemID];
	const FOLCGalaxyCluster& Target  = Clusters[TargetClusterID];

	// Find the direct warp route from the current cluster to the target.
	const FOLCSolarWarpRoute* Route = nullptr;
	for (const FOLCSolarWarpRoute& Candidate : GetWarpRoutes())
	{
		if (Candidate.FromSystem.EqualTo(Current.ClusterName) && Candidate.ToSystem.EqualTo(Target.ClusterName))
		{
			Route = &Candidate;
			break;
		}
	}

	if (!Route)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryPayFuelAndTravel: no warp route from %s to %s"),
			*Current.ClusterName.ToString(), *Target.ClusterName.ToString());
		return false;
	}

	// Resolve the resource subsystem (same path as StartScan).
	if (!GetWorld()) return false;
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (!GI) return false;
	UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
	if (!Data) return false;

	const float FuelCost = static_cast<float>(Route->FuelCost);

	// Phase 1: check we can afford the fuel.
	bool bHasEnough = false;
	for (const auto& Res : Data->GetResourceCounters())
	{
		if (Res.ResourceType == EOLCResourceType::Fuel && Res.Value >= FuelCost)
		{
			bHasEnough = true;
			break;
		}
	}

	if (!bHasEnough)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryPayFuelAndTravel: Insufficient Fuel (need %d)"), Route->FuelCost);
		return false; // Abort — deduct nothing, move nowhere.
	}

	// Phase 2: deduct exactly the route's fuel cost and update position.
	Data->AddResource(EOLCResourceType::Fuel, -FuelCost);
	CurrentSystemID = TargetClusterID;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Traveled from %s to %s (cost %d fuel)"),
		*Current.ClusterName.ToString(), *Target.ClusterName.ToString(), Route->FuelCost);

	return true;
}

// ---------------------------------------------------------------------------
// Void Warp Drive — instant travel within 5-system radius (WP-125 Step 2)
// ---------------------------------------------------------------------------

bool UOLCNavigationSubsystem::TryVoidWarp(int32 TargetClusterID) const
{
	// Validate target cluster ID range.
	if (TargetClusterID < 0 || TargetClusterID >= NumGalaxyClusters)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryVoidWarp: invalid target cluster id %d"), TargetClusterID);
		return false;
	}

	// Validate current system ID range.
	if (CurrentSystemID < 0 || CurrentSystemID >= NumGalaxyClusters)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryVoidWarp: current system id %d out of range"), CurrentSystemID);
		return false;
	}

	// Compute cluster distance as absolute difference of IDs.
	const int32 Distance = FMath::Abs(CurrentSystemID - TargetClusterID);

	// Reject if beyond the 5-system radius.
	if (Distance > 5)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryVoidWarp: target %d is %d clusters away (max 5)"), TargetClusterID, Distance);
		return false;
	}

	// Instant warp — no fuel cost, no route lookup.
	const int32 FromID = CurrentSystemID;
	CurrentSystemID = TargetClusterID;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Void Warp: instant travel from cluster %d to cluster %d (0 fuel)"), FromID, TargetClusterID);

	return true;
}

// ---------------------------------------------------------------------------
// Alien Warp Integration — unlimited range at 0.01x fuel cost (WP-125 Step 3)
// ---------------------------------------------------------------------------

bool UOLCNavigationSubsystem::TryAlienWarp(int32 TargetClusterID) const
{
	// Validate target cluster ID range.
	if (TargetClusterID < 0 || TargetClusterID >= NumGalaxyClusters)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryAlienWarp: invalid target cluster id %d"), TargetClusterID);
		return false;
	}

	// Validate current system ID range.
	if (CurrentSystemID < 0 || CurrentSystemID >= NumGalaxyClusters)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] TryAlienWarp: current system id %d out of range"), CurrentSystemID);
		return false;
	}

	// Same system — no travel needed, zero cost.
	if (TargetClusterID == CurrentSystemID)
	{
		UE_LOG(LogOLC, Display, TEXT("[OLC] Alien Warp: already at cluster %d (0 fuel)"), TargetClusterID);
		return true;
	}

	// Compute normal fuel cost from cluster positions (same formula as GetWarpRoutes).
	const TArray<FOLCGalaxyCluster> Clusters = GetOrGenerateGalaxyClusters();
	const float Distance = FVector2D::Distance(Clusters[CurrentSystemID].Position, Clusters[TargetClusterID].Position);
	const int32 NormalFuelCost = FMath::RoundToInt(CalculateFuelCost(EOLCDriveType::Chemical, Distance));

	// Apply 0.01x multiplier; round up to minimum 1 if normal cost > 0.
	int32 AlienFuelCost = 0;
	if (NormalFuelCost > 0)
	{
		AlienFuelCost = FMath::Max(1, FMath::CeilToInt(static_cast<float>(NormalFuelCost) * 0.01f));
	}

	// Deduct fuel via UOLCUIDataSubsystem (two-phase: check then deduct).
	if (AlienFuelCost > 0)
	{
		if (!GetWorld()) return false;
		UGameInstance* GI = GetWorld()->GetGameInstance();
		if (!GI) return false;
		UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
		if (!Data) return false;

		bool bHasEnough = false;
		for (const auto& Res : Data->GetResourceCounters())
		{
			if (Res.ResourceType == EOLCResourceType::Fuel && Res.Value >= AlienFuelCost)
			{
				bHasEnough = true;
				break;
			}
		}

		if (!bHasEnough)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] TryAlienWarp: Insufficient Fuel (need %d)"), AlienFuelCost);
			return false;
		}

		Data->AddResource(EOLCResourceType::Fuel, -AlienFuelCost);
	}

	// Update position.
	const int32 FromID = CurrentSystemID;
	CurrentSystemID = TargetClusterID;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Alien Warp: travel from cluster %d to cluster %d (cost %d fuel, normal would be %d)"),
		FromID, TargetClusterID, AlienFuelCost, NormalFuelCost);

	return true;
}
