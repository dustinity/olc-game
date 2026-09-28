// WP-121 Step 4 — in-engine verification of galaxy cluster generation, warp
// routes, and fuel-paid travel on UOLCNavigationSubsystem.
//
// Run via the MCP AutomationTestToolset (RunTests with filter "OLC.NavStep4").
// Each test builds an isolated standalone game instance (the engine's
// InitializeStandalone pattern) so no live editor session state is touched:
// the dummy world makes GetWorld() resolve for the subsystems, and the whole
// environment is torn down when the test finishes.

#include "Core/OLCNavigationSubsystem.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"

#define LOCTEXT_NAMESPACE "OLCNavStep4"

namespace OLCNavStep4Testing
{
	/** Isolated game-instance environment for subsystem tests. */
	struct FSubsystemEnv
	{
		UGameInstance* GI = nullptr;
		UOLCNavigationSubsystem* Nav = nullptr;
		UOLCUIDataSubsystem* Data = nullptr;

		bool Init(FAutomationTestBase& Test)
		{
			GI = NewObject<UGameInstance>(GEngine);
			GI->AddToRoot();
			GI->InitializeStandalone(FName(TEXT("OLCNavStep4Test")));
			Nav = GI->GetSubsystem<UOLCNavigationSubsystem>();
			Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
			if (!Nav || !Data)
			{
				Test.AddError(TEXT("Failed to initialize OLC navigation/UI-data subsystems in standalone game instance"));
				return false;
			}
			return true;
		}

		float GetFuel() const
		{
			for (const FOLCResourceCounterViewData& Counter : Data->GetResourceCounters())
			{
				if (Counter.ResourceType == EOLCResourceType::Fuel)
				{
					return Counter.Value;
				}
			}
			return -1.0f;
		}

		/** Find the first direct warp route from the current cluster to another cluster. */
		bool FindOutgoingRoute(int32& OutTargetID, int32& OutFuelCost) const
		{
			const TArray<FOLCGalaxyCluster> Clusters = Nav->GetOrGenerateGalaxyClusters();
			const int32 CurrentID = Nav->GetCurrentSystemID();
			if (CurrentID < 0 || CurrentID >= Clusters.Num())
			{
				return false;
			}
			const FText CurrentName = Clusters[CurrentID].ClusterName;

			for (const FOLCSolarWarpRoute& Route : Nav->GetWarpRoutes())
			{
				if (!Route.FromSystem.EqualTo(CurrentName))
				{
					continue;
				}
				for (int32 i = 0; i < Clusters.Num(); i++)
				{
					if (Clusters[i].ClusterName.EqualTo(Route.ToSystem))
					{
						OutTargetID = i;
						OutFuelCost = Route.FuelCost;
						return true;
					}
				}
			}
			return false;
		}

		~FSubsystemEnv()
		{
			if (GI)
			{
				UWorld* World = GI->GetWorld();
				GI->Shutdown();
				GI->RemoveFromRoot();
				GI = nullptr;
				if (World && World->IsValidLowLevel())
				{
					World->DestroyWorld(true);
				}
			}
			Nav = nullptr;
			Data = nullptr;
		}
	};
}

using namespace OLCNavStep4Testing;

// ---------------------------------------------------------------------------
// 1. Nine clusters in a strictly increasing-radius spiral
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCGalaxyClusterSpiralTest, "OLC.NavStep4.GalaxyClustersSpiral", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCGalaxyClusterSpiralTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	const TArray<FOLCGalaxyCluster> Clusters = Env.Nav->GetOrGenerateGalaxyClusters();
	TestEqual(TEXT("cluster count"), Clusters.Num(), UOLCNavigationSubsystem::NumGalaxyClusters);
	TestEqual(TEXT("expected exactly 9 clusters"), Clusters.Num(), 9);

	TSet<FString> SeenNames;
	float PreviousRadius = -1.0f;
	float PreviousAngleDeg = -1.0f;

	for (int32 i = 0; i < Clusters.Num(); i++)
	{
		const FOLCGalaxyCluster& Cluster = Clusters[i];
		TestEqual(TEXT("cluster id matches index"), Cluster.ClusterID, i);
		TestFalse(TEXT("cluster name is empty"), Cluster.ClusterName.IsEmpty());
		bool bNameAlreadySeen = false;
		SeenNames.Add(Cluster.ClusterName.ToString(), &bNameAlreadySeen);
		TestFalse(TEXT("cluster name is unique"), bNameAlreadySeen);

		const float Radius = Cluster.Position.Size();
		if (i > 0)
		{
			// Spiral: radius strictly increases between successive clusters.
			TestTrue(TEXT("radius strictly increases along the spiral"), Radius > PreviousRadius + 1.0f);

			// Angle advances monotonically by a fixed step each cluster.
			const float AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Cluster.Position.Y, Cluster.Position.X));
			const float NormalizedAngle = FMath::Fmod(AngleDeg + 360.0f, 360.0f);
			const float Delta = FMath::Fmod(NormalizedAngle - PreviousAngleDeg + 360.0f, 360.0f);
			TestEqual(TEXT("angle advances by the fixed spiral step"), Delta, 45.0f, 0.1f);
		}

		PreviousRadius = Radius;
		PreviousAngleDeg = FMath::Fmod(FMath::RadiansToDegrees(FMath::Atan2(Cluster.Position.Y, Cluster.Position.X)) + 360.0f, 360.0f);
	}

	return true;
}

// ---------------------------------------------------------------------------
// 2. Repeated calls return identical cluster positions (cached layout)
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCGalaxyClusterRepeatTest, "OLC.NavStep4.GalaxyClustersRepeatIdentical", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCGalaxyClusterRepeatTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	const TArray<FOLCGalaxyCluster> First = Env.Nav->GetOrGenerateGalaxyClusters();
	const TArray<FOLCGalaxyCluster> Second = Env.Nav->GetOrGenerateGalaxyClusters();

	TestEqual(TEXT("repeat call returns same count"), Second.Num(), First.Num());
	for (int32 i = 0; i < First.Num() && i < Second.Num(); i++)
	{
		TestEqual(TEXT("cluster id identical on repeat"), Second[i].ClusterID, First[i].ClusterID);
		TestTrue(TEXT("cluster name identical on repeat"), Second[i].ClusterName.EqualTo(First[i].ClusterName));
		TestTrue(TEXT("cluster position identical on repeat"), Second[i].Position == First[i].Position);
	}

	return true;
}

// ---------------------------------------------------------------------------
// 3. Determinism: same seed -> identical layout across instances; different
//    seed -> identical geometry (by design) but different cluster names
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCGalaxyClusterDeterminismTest, "OLC.NavStep4.GalaxyClustersDeterministicPerSeed", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCGalaxyClusterDeterminismTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	const int32 Seed = Env.Nav->GetGalaxySeed();
	const TArray<FOLCGalaxyCluster> Reference = Env.Nav->GetOrGenerateGalaxyClusters();

	// A brand-new subsystem instance under the same seed must reproduce the
	// exact same layout (positions, names, ids). UOLCNavigationSubsystem is a
	// UGameInstanceSubsystem (ClassWithin=UGameInstance), so it must live on
	// its own isolated game instance rather than being NewObject'd directly.
	FSubsystemEnv SameSeedEnv;
	TestTrue(TEXT("same-seed environment initialized"), SameSeedEnv.Init(*this));
	SameSeedEnv.Nav->SetGalaxySeed(Seed);
	const TArray<FOLCGalaxyCluster> Reproduced = SameSeedEnv.Nav->GetOrGenerateGalaxyClusters();

	TestEqual(TEXT("same seed reproduces cluster count"), Reproduced.Num(), Reference.Num());
	for (int32 i = 0; i < Reference.Num() && i < Reproduced.Num(); i++)
	{
		TestTrue(TEXT("same seed reproduces position"), Reproduced[i].Position == Reference[i].Position);
		TestTrue(TEXT("same seed reproduces name"), Reproduced[i].ClusterName.EqualTo(Reference[i].ClusterName));
	}

	// Per the Step 4 spec, spiral geometry is pure index arithmetic (the random
	// stream must never drive radius/angle ordering), so a different seed keeps
	// the exact same positions. GalaxySeed only drives non-geometric fields —
	// here, the cluster names — which must change with the seed.
	FSubsystemEnv OtherSeedEnv;
	TestTrue(TEXT("other-seed environment initialized"), OtherSeedEnv.Init(*this));
	OtherSeedEnv.Nav->SetGalaxySeed(Seed + 1);
	const TArray<FOLCGalaxyCluster> Different = OtherSeedEnv.Nav->GetOrGenerateGalaxyClusters();

	bool bGeometryIdentical = true;
	bool bNamesDiffer = false;
	for (int32 i = 0; i < Reference.Num() && i < Different.Num(); i++)
	{
		if (Different[i].Position != Reference[i].Position)
		{
			bGeometryIdentical = false;
		}
		if (!Different[i].ClusterName.EqualTo(Reference[i].ClusterName))
		{
			bNamesDiffer = true;
		}
	}
	TestTrue(TEXT("different seed keeps identical spiral geometry (index arithmetic by design)"), bGeometryIdentical);
	TestTrue(TEXT("different seed yields different cluster names"), bNamesDiffer);

	return true;
}

// ---------------------------------------------------------------------------
// 4. Insufficient fuel: travel fails without mutating CurrentSystemID or Fuel
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCTravelInsufficientFuelTest, "OLC.NavStep4.TravelInsufficientFuelNoMutation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCTravelInsufficientFuelTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	const int32 CurrentBefore = Env.Nav->GetCurrentSystemID();

	// Force the Fuel counter below any route cost (AddResource clamps at 0).
	Env.Data->AddResource(EOLCResourceType::Fuel, -1e6f);
	const float FuelBefore = Env.GetFuel();
	TestTrue(TEXT("fuel drained below route cost"), FuelBefore < 50.0f);

	int32 TargetID = INDEX_NONE;
	int32 RouteCost = 0;
	TestTrue(TEXT("outgoing route exists from current cluster"), Env.FindOutgoingRoute(TargetID, RouteCost));
	TestTrue(TEXT("route has a positive fuel cost"), RouteCost > 0);

	TestFalse(TEXT("travel fails when fuel is insufficient"), Env.Nav->TryPayFuelAndTravel(TargetID));

	// No state mutation: fuel counter and current system must be untouched.
	TestEqual(TEXT("fuel counter unchanged after failed travel"), Env.GetFuel(), FuelBefore, 0.001f);
	TestEqual(TEXT("current system unchanged after failed travel"), Env.Nav->GetCurrentSystemID(), CurrentBefore);

	return true;
}

// ---------------------------------------------------------------------------
// 5. Sufficient fuel: travel deducts exactly the route cost and updates the
//    current system (round trip restores the starting cluster)
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCTravelSuccessTest, "OLC.NavStep4.TravelSuccessDeductsAndMoves", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCTravelSuccessTest::RunTest(const FString& Parameters)
{
	FSubsystemEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	const int32 CurrentBefore = Env.Nav->GetCurrentSystemID();

	// Top the Fuel counter up to capacity (AddResource clamps at max).
	Env.Data->AddResource(EOLCResourceType::Fuel, 1e6f);
	const float FuelBefore = Env.GetFuel();

	int32 TargetID = INDEX_NONE;
	int32 RouteCost = 0;
	TestTrue(TEXT("outgoing route exists from current cluster"), Env.FindOutgoingRoute(TargetID, RouteCost));
	TestTrue(TEXT("route is affordable at full capacity"), static_cast<float>(RouteCost) <= FuelBefore);

	TestTrue(TEXT("travel succeeds when fuel is sufficient"), Env.Nav->TryPayFuelAndTravel(TargetID));
	TestEqual(TEXT("current system updated to target"), Env.Nav->GetCurrentSystemID(), TargetID);
	TestEqual(TEXT("fuel deducted by exactly the route cost"), Env.GetFuel(), FuelBefore - static_cast<float>(RouteCost), 0.001f);

	// Round trip: the reverse route exists, so travel back and restore state.
	Env.Data->AddResource(EOLCResourceType::Fuel, 1e6f);
	TestTrue(TEXT("travel back along the reverse route succeeds"), Env.Nav->TryPayFuelAndTravel(CurrentBefore));
	TestEqual(TEXT("current system restored after round trip"), Env.Nav->GetCurrentSystemID(), CurrentBefore);

	return true;
}

#undef LOCTEXT_NAMESPACE
