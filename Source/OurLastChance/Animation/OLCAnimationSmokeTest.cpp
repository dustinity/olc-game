// WP-128 Step 9 — end-to-end automation smoke test for the animation system.
//
// Run via the MCP AutomationTestToolset (RunTests with filter "OLC.Animation.Smoke").
// Follows the project convention from Core/OLCNavigationStep4Tests.cpp: each test
// builds an isolated standalone game instance so no live editor session state is
// touched, and tears it down when finished. Failed Test* assertions record errors
// via AddError (UE 5.8 API), which the runner reports as test failures.
//
// Coverage:
//  - DataAssets: every asset from steps 6-8 exists with the exact slot population.
//  - SubsystemAttach: UOLCAnimationSubsystem attaches exactly one player component
//    to a spawned representative actor whose class matches a set's UnitTypeId.
//  - StateTransitions: per-category slot drives (infantry/vehicle/aerial/ship)
//    update the exposed CurrentSlot flag.
//  - BuildingSequence: staged assembly over the tier-mapped duration, then collapse.
//  - ChampionAbilityPlayback: every (ChampionId, AbilityId) pair resolves and
//    PlayAbilityAnimation runs without crashing (VFX fallback path).

#include "Animation/OLCAnimationPlayerComponent.h"
#include "Animation/OLCAnimationSmokeTestHook.h"
#include "Animation/OLCAnimationSubsystem.h"
#include "Animation/OLCBuildingSequenceComponent.h"
#include "Components/SceneComponent.h"
#include "Core/OLCChampionAnimationData.h"
#include "Core/OLCUnitAnimationData.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "World/OLCAerialUnit.h"
#include "World/OLCGroundUnit.h"
#include "World/OLCVehicleUnit.h"

#define LOCTEXT_NAMESPACE "OLCAnimSmoke"

namespace OLCAnimSmokeTesting
{
	/** Folder scanned by UOLCAnimationSubsystem (step-1 findings convention). */
	static const TCHAR* AnimFolder = TEXT("/Game/OurLastChance/Animation");

	/** Isolated game-instance environment (same pattern as the WP-121 nav tests). */
	struct FAnimEnv
	{
		UGameInstance* GI = nullptr;
		UWorld* World = nullptr;

		bool Init(FAutomationTestBase& Test)
		{
			GI = NewObject<UGameInstance>(GEngine);
			GI->AddToRoot();
			GI->InitializeStandalone(FName(TEXT("OLCAnimSmokeTest")));
			World = GI->GetWorld();
			if (!World)
			{
				Test.AddError(TEXT("Failed to create standalone world for animation smoke test"));
				return false;
			}
			return true;
		}

		~FAnimEnv()
		{
			if (GI)
			{
				GI->Shutdown();
				GI->RemoveFromRoot();
				GI = nullptr;
				if (World && World->IsValidLowLevel())
				{
					World->DestroyWorld(true);
				}
				World = nullptr;
			}
		}
	};

	template <typename T>
	static T* LoadDA(FAutomationTestBase& Test, const TCHAR* AssetName)
	{
		const FString Path = FString::Printf(TEXT("%s/%s.%s"), AnimFolder, AssetName, AssetName);
		T* DA = LoadObject<T>(nullptr, *Path);
		if (!DA)
		{
			Test.AddError(FString::Printf(TEXT("DataAsset not found: %s"), *Path));
		}
		return DA;
	}

	/** Assert the set exists with exactly the expected slots (and UnitTypeId). */
	static void CheckUnitSet(FAutomationTestBase& Test, const TCHAR* AssetName, const TCHAR* ExpectedUnitTypeId, const TArray<EOLCAnimSlot>& ExpectedSlots)
	{
		UOLCUnitAnimationSet* Set = LoadDA<UOLCUnitAnimationSet>(Test, AssetName);
		Test.TestTrue(FString::Printf(TEXT("%s exists"), AssetName), static_cast<bool>(Set));
		if (!Set)
		{
			return;
		}
		Test.TestEqual(FString::Printf(TEXT("%s UnitTypeId"), AssetName), *Set->UnitTypeId, ExpectedUnitTypeId);
		Test.TestEqual(FString::Printf(TEXT("%s entry count"), AssetName), Set->Entries.Num(), ExpectedSlots.Num());
		for (const EOLCAnimSlot Slot : ExpectedSlots)
		{
			FOLCAnimationEntry Entry;
			const bool bFound = Set->FindEntry(Slot, Entry);
			Test.TestTrue(FString::Printf(TEXT("%s has slot %d"), AssetName, static_cast<int32>(Slot)), bFound);
		}
	}
}

using namespace OLCAnimSmokeTesting;

// ---------------------------------------------------------------------------
// 1. Every DataAsset from steps 6-8 exists with the correct slot population
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCAnimationDataAssetsTest, "OLC.Animation.Smoke.DataAssets", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCAnimationDataAssetsTest::RunTest(const FString& Parameters)
{
	// Step 6 — unit sets (5 / 4 / 4).
	CheckUnitSet(*this, TEXT("DA_AnimSet_Infantry_BasicSoldier"), TEXT("BasicSoldier"),
		{ EOLCAnimSlot::Idle, EOLCAnimSlot::Walk, EOLCAnimSlot::Run, EOLCAnimSlot::Attack, EOLCAnimSlot::Heal });
	CheckUnitSet(*this, TEXT("DA_AnimSet_Vehicle_LightVehicle"), TEXT("LightVehicle"),
		{ EOLCAnimSlot::Idle, EOLCAnimSlot::Move, EOLCAnimSlot::Fire, EOLCAnimSlot::Damage });
	CheckUnitSet(*this, TEXT("DA_AnimSet_Aerial_Aerial"), TEXT("Aerial"),
		{ EOLCAnimSlot::Hover, EOLCAnimSlot::Fly, EOLCAnimSlot::Attack, EOLCAnimSlot::Crash });

	// Step 7 — ship sets (4 / 4).
	const TArray<EOLCAnimSlot> ShipSlots = { EOLCAnimSlot::Takeoff, EOLCAnimSlot::Cruise, EOLCAnimSlot::Descent, EOLCAnimSlot::Touchdown };
	CheckUnitSet(*this, TEXT("DA_AnimSet_Ship_Dropship"), TEXT("Dropship"), ShipSlots);
	CheckUnitSet(*this, TEXT("DA_AnimSet_Ship_Mothership"), TEXT("Mothership"), ShipSlots);

	// Step 8 — one champion anim asset per champion (12 × 3 entries).
	const TCHAR* const ChampionIds[12] = {
		TEXT("CH-NP-01"), TEXT("CH-NP-02"), TEXT("CH-NP-03"),
		TEXT("CH-DR-01"), TEXT("CH-DR-02"), TEXT("CH-DR-03"),
		TEXT("CH-CS-01"), TEXT("CH-CS-02"), TEXT("CH-CS-03"),
		TEXT("CH-BR-01"), TEXT("CH-BR-02"), TEXT("CH-BR-03")
	};
	for (const TCHAR* ChampionId : ChampionIds)
	{
		const FString AssetName = FString::Printf(TEXT("DA_ChampionAbilityAnims_%s"), ChampionId);
		UOLCChampionAnimationData* DA = LoadDA<UOLCChampionAnimationData>(*this, *AssetName);
		TestTrue(FString::Printf(TEXT("%s exists"), *AssetName), static_cast<bool>(DA));
		if (!DA)
		{
			continue;
		}
		TestEqual(FString::Printf(TEXT("%s ChampionId"), *AssetName), *DA->ChampionId, ChampionId);
		TestEqual(FString::Printf(TEXT("%s entry count"), *AssetName), DA->AbilityAnims.Num(), 3);
		for (const FOLCChampionAbilityAnim& Anim : DA->AbilityAnims)
		{
			TestFalse(FString::Printf(TEXT("%s empty AbilityId"), *AssetName), Anim.AbilityId.IsEmpty());
			FOLCChampionAbilityAnim Resolved;
			TestTrue(FString::Printf(TEXT("%s resolves %s"), *AssetName, *Anim.AbilityId), DA->FindAbilityAnim(Anim.AbilityId, Resolved));
		}
	}

	return true; // Failures are recorded via AddError by the Test* helpers.
}

// ---------------------------------------------------------------------------
// 2. Subsystem attaches exactly one player component to a matching actor
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCAnimationSubsystemAttachTest, "OLC.Animation.Smoke.SubsystemAttach", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCAnimationSubsystemAttachTest::RunTest(const FString& Parameters)
{
	FAnimEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));

	UOLCAnimationSubsystem* Subsystem = Env.World ? Env.World->GetSubsystem<UOLCAnimationSubsystem>() : nullptr;
	TestTrue(TEXT("animation subsystem available in standalone world"), static_cast<bool>(Subsystem));
	if (!Subsystem)
	{
		return false;
	}

	// Representative aerial actor: class name "OLCAerialUnit" contains the
	// aerial set's UnitTypeId "Aerial".
	AOLCAerialUnit* Aerial = Env.World->SpawnActor<AOLCAerialUnit>();
	TestTrue(TEXT("aerial unit spawned"), static_cast<bool>(Aerial));
	if (!Aerial)
	{
		return false;
	}

	Subsystem->RescanNow();

	TArray<UOLCAnimationPlayerComponent*> Players;
	Aerial->GetComponents<UOLCAnimationPlayerComponent>(Players);
	TestEqual(TEXT("exactly one animation player component attached"), Players.Num(), 1);

	TArray<UOLCBuildingSequenceComponent*> Sequences;
	Aerial->GetComponents<UOLCBuildingSequenceComponent>(Sequences);
	TestEqual(TEXT("no building sequence component for a non-building set"), Sequences.Num(), 0);

	if (Players.Num() == 1)
	{
		UOLCUnitAnimationSet* Expected = LoadDA<UOLCUnitAnimationSet>(*this, TEXT("DA_AnimSet_Aerial_Aerial"));
		TestTrue(TEXT("attached player references the aerial set"),
			static_cast<bool>(Expected) && Players[0]->AnimationSet == Expected);
	}

	// Exactly-once: a second scan must not attach a duplicate.
	Subsystem->RescanNow();
	Aerial->GetComponents<UOLCAnimationPlayerComponent>(Players);
	TestEqual(TEXT("still exactly one player component after re-scan"), Players.Num(), 1);

	return true;
}

// ---------------------------------------------------------------------------
// 3. Per-category slot transitions update the exposed CurrentSlot flag
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCAnimationStateTransitionsTest, "OLC.Animation.Smoke.StateTransitions", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCAnimationStateTransitionsTest::RunTest(const FString& Parameters)
{
	FAnimEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));
	if (!Env.World)
	{
		return false;
	}

	auto DriveSlots = [this, &Env](const TCHAR* Label, AActor* Actor, UOLCUnitAnimationSet* Set, const TArray<EOLCAnimSlot>& Slots)
	{
		TestTrue(FString::Printf(TEXT("%s actor spawned"), Label), static_cast<bool>(Actor));
		TestTrue(FString::Printf(TEXT("%s set loaded"), Label), static_cast<bool>(Set));
		if (!Actor || !Set)
		{
			return;
		}

		UOLCAnimationPlayerComponent* Player = NewObject<UOLCAnimationPlayerComponent>(Actor);
		Player->RegisterComponent();
		Player->AnimationSet = Set;
		if (!Actor->HasActorBegunPlay())
		{
			Actor->DispatchBeginPlay();
		}

		for (const EOLCAnimSlot Slot : Slots)
		{
			Player->SetSlot(Slot, /*BlendTime=*/0.0f);
			TestEqual(FString::Printf(TEXT("%s slot -> %d"), Label, static_cast<int32>(Slot)),
				static_cast<int32>(Player->GetCurrentSlot()), static_cast<int32>(Slot));
		}
	};

	// Infantry: Idle -> Walk -> Run -> Attack -> Heal.
	AOLCGroundUnit* Infantry = Env.World->SpawnActor<AOLCGroundUnit>();
	UOLCUnitAnimationSet* InfantrySet = LoadDA<UOLCUnitAnimationSet>(*this, TEXT("DA_AnimSet_Infantry_BasicSoldier"));
	DriveSlots(TEXT("infantry"), Infantry, InfantrySet,
		{ EOLCAnimSlot::Idle, EOLCAnimSlot::Walk, EOLCAnimSlot::Run, EOLCAnimSlot::Attack, EOLCAnimSlot::Heal });

	// Vehicle: Idle -> Move -> Fire -> Damage.
	AOLCVehicleUnit* Vehicle = Env.World->SpawnActor<AOLCVehicleUnit>();
	UOLCUnitAnimationSet* VehicleSet = LoadDA<UOLCUnitAnimationSet>(*this, TEXT("DA_AnimSet_Vehicle_LightVehicle"));
	DriveSlots(TEXT("vehicle"), Vehicle, VehicleSet,
		{ EOLCAnimSlot::Idle, EOLCAnimSlot::Move, EOLCAnimSlot::Fire, EOLCAnimSlot::Damage });

	// Aerial: Hover -> Fly -> Crash.
	AOLCAerialUnit* Aerial = Env.World->SpawnActor<AOLCAerialUnit>();
	UOLCUnitAnimationSet* AerialSet = LoadDA<UOLCUnitAnimationSet>(*this, TEXT("DA_AnimSet_Aerial_Aerial"));
	DriveSlots(TEXT("aerial"), Aerial, AerialSet,
		{ EOLCAnimSlot::Hover, EOLCAnimSlot::Fly, EOLCAnimSlot::Crash });

	// Ship: Takeoff -> Cruise -> Descent -> Touchdown (no ship actor class exists
	// yet — spec drift recorded in OPEN.md; the component path is exercised on a
	// plain actor with the dropship set).
	AActor* ShipProxy = Env.World->SpawnActor<AActor>();
	UOLCUnitAnimationSet* ShipSet = LoadDA<UOLCUnitAnimationSet>(*this, TEXT("DA_AnimSet_Ship_Dropship"));
	DriveSlots(TEXT("ship"), ShipProxy, ShipSet,
		{ EOLCAnimSlot::Takeoff, EOLCAnimSlot::Cruise, EOLCAnimSlot::Descent, EOLCAnimSlot::Touchdown });

	return true;
}

// ---------------------------------------------------------------------------
// 4. Building assembly over the tier-mapped duration, then collapse
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCAnimationBuildingSequenceTest, "OLC.Animation.Smoke.BuildingSequence", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCAnimationBuildingSequenceTest::RunTest(const FString& Parameters)
{
	FAnimEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));
	if (!Env.World)
	{
		return false;
	}

	// Building stand-in: root + two child parts (CollectParts needs children).
	AActor* Building = Env.World->SpawnActor<AActor>();
	TestTrue(TEXT("building actor spawned"), static_cast<bool>(Building));
	if (!Building)
	{
		return false;
	}
	USceneComponent* Root = NewObject<USceneComponent>(Building);
	Root->RegisterComponent();
	Building->SetRootComponent(Root);
	for (int32 i = 0; i < 2; ++i)
	{
		USceneComponent* Part = NewObject<USceneComponent>(Building);
		Part->SetupAttachment(Root);
		Part->RegisterComponent();
	}

	UOLCAnimSmokeTestBuildingSeq* Seq = NewObject<UOLCAnimSmokeTestBuildingSeq>(Building);
	TestTrue(TEXT("building sequence component created"), static_cast<bool>(Seq));
	Seq->RegisterComponent();
	if (!Building->HasActorBegunPlay())
	{
		Building->DispatchBeginPlay();
	}

	auto TickSeq = [Seq](float Seconds)
	{
		const int32 Steps = FMath::CeilToInt(Seconds / 0.1f);
		for (int32 i = 0; i < Steps; ++i)
		{
			Seq->TickForTest(0.1f);
		}
	};

	// Assembly: Duration <= 0 picks TierAssemblyDurations[BuildingTierIndex=0]
	// (Default tier, ~2 s in the default table) — tick until complete.
	Seq->PlayAssemblySequence(/*Duration=*/-1.0f);
	float Elapsed = 0.0f;
	while (Elapsed < 5.0f && !Seq->IsAssembled())
	{
		TickSeq(0.1f);
		Elapsed += 0.1f;
	}
	const bool bAssembled = Seq->IsAssembled();
	TestTrue(TEXT("assembly completes within the 5 s simulated budget"), bAssembled);
	if (bAssembled)
	{
		TestEqual(TEXT("assembly progress reached 1.0"), Seq->GetAssemblyProgress(), 1.0f, 0.001f);

		// Collapse: shake + staggered scale-down, then hidden + collapsed flag.
		Seq->PlayCollapseSequence();
		Elapsed = 0.0f;
		while (Elapsed < 5.0f && !Seq->IsCollapsed())
		{
			TickSeq(0.1f);
			Elapsed += 0.1f;
		}
		TestTrue(TEXT("collapse completes within the 5 s simulated budget"), Seq->IsCollapsed());
	}

	return true;
}

// ---------------------------------------------------------------------------
// 5. Every champion (ChampionId, AbilityId) pair resolves and plays without crash
// ---------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOLCAnimationChampionPlaybackTest, "OLC.Animation.Smoke.ChampionAbilityPlayback", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FOLCAnimationChampionPlaybackTest::RunTest(const FString& Parameters)
{
	FAnimEnv Env;
	TestTrue(TEXT("standalone environment initialized"), Env.Init(*this));
	if (!Env.World)
	{
		return false;
	}

	AActor* Target = Env.World->SpawnActor<AActor>();
	TestTrue(TEXT("playback target spawned"), static_cast<bool>(Target));
	if (!Target)
	{
		return false;
	}

	const TCHAR* const ChampionIds[12] = {
		TEXT("CH-NP-01"), TEXT("CH-NP-02"), TEXT("CH-NP-03"),
		TEXT("CH-DR-01"), TEXT("CH-DR-02"), TEXT("CH-DR-03"),
		TEXT("CH-CS-01"), TEXT("CH-CS-02"), TEXT("CH-CS-03"),
		TEXT("CH-BR-01"), TEXT("CH-BR-02"), TEXT("CH-BR-03")
	};

	int32 PairsPlayed = 0;
	for (const TCHAR* ChampionId : ChampionIds)
	{
		const FString AssetName = FString::Printf(TEXT("DA_ChampionAbilityAnims_%s"), ChampionId);
		UOLCChampionAnimationData* DA = LoadDA<UOLCChampionAnimationData>(*this, *AssetName);
		if (!DA)
		{
			continue; // Existence failures already reported by the DataAssets test.
		}
		for (const FOLCChampionAbilityAnim& Anim : DA->AbilityAnims)
		{
			FOLCChampionAbilityAnim Resolved;
			TestTrue(FString::Printf(TEXT("%s / %s resolves"), ChampionId, *Anim.AbilityId),
				DA->FindAbilityAnim(Anim.AbilityId, Resolved));

			// VFX fallback path (no animation assets exist yet) — must not crash.
			UOLCAnimationPlayerComponent::PlayAbilityAnimation(Target, Resolved);
			++PairsPlayed;
		}
	}

	TestEqual(TEXT("all 36 champion ability pairs played"), PairsPlayed, 36);

	return true;
}

#undef LOCTEXT_NAMESPACE
