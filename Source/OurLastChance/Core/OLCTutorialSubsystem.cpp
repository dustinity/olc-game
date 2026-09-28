#include "Core/OLCTutorialSubsystem.h"
#include "OurLastChance.h"

#include "Engine/Engine.h"
#include "Engine/World.h" // FWorldDelegates (no standalone WorldDelegates.h in UE 5.8)
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameModes/OLCMenuGameMode.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/OLCTutorialTestConfig.h"
#include "Core/OLCUIDataSubsystem.h"
#include "World/OLCCrashSitePrototypeActor.h"
#include "World/OLCGameplayWorldActor.h"
#include "World/OLCObjectiveHighlightComponent.h"

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UOLCTutorialSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Auto-resume: pick up persisted progress from the last session, if any.
	// Read-only — a missing/corrupt save simply leaves the fresh default state.
	if (LoadProgress())
	{
		UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: resumed — active objective index %d (skipped=%d)"),
			State.ActiveObjectiveIndex, State.bSkipped);
	}
	else
	{
		UE_LOG(LogOLC, Verbose, TEXT("[OLC] Tutorial: no saved progress found — starting fresh"));
	}

	// WP-129 Step 3: keep the world highlight in sync with progression.
	OnTutorialAdvanced.AddDynamic(this, &UOLCTutorialSubsystem::HandleTutorialAdvanced);

	// GameInstance init happens before any PIE world exists, so the initial
	// apply is deferred to shortly after world creation (begin-play and actor
	// registration are done by then). RegisterTargetActor also triggers an
	// immediate apply for flows that register targets later
	// (e.g. OLCMenuGameMode::TransitionToGameplay spawns the crash-site actor).
	WorldCreatedHandle = FWorldDelegates::OnPostWorldCreation.AddLambda([this](UWorld* World)
	{
		if (!World)
		{
			return;
		}

		World->GetTimerManager().SetTimer(
			HighlightApplyTimerHandle, this, &UOLCTutorialSubsystem::RefreshObjectiveHighlight, 0.5f, false);

		// Standalone (non-PIE) game launches create their world via UWorld::CreateWorld,
		// so OnPostWorldCreation fires reliably there. PIE worlds are duplicated from the
		// editor world instead (see ScheduleTestHooksIfNeeded's callers for the PIE path).
		ScheduleTestHooksIfNeeded(World);
	});
}

void UOLCTutorialSubsystem::ScheduleTestHooksIfNeeded(UWorld* World)
{
	// Game worlds only: never fire test hooks against the editor world.
	if (bTestHooksScheduled || !World || !World->IsGameWorld())
	{
		return;
	}

	const UOLCTutorialTestConfig* Test = UOLCTutorialTestConfig::Load();
	if (!Test)
	{
		return;
	}

	const int32 AutoComplete = FMath::Clamp(Test->AutoCompleteObjectives, 0, NumOLCTutorialObjectives);
	if (AutoComplete <= 0 && !Test->bSkipOnStart)
	{
		return;
	}

	bTestHooksScheduled = true;

	for (int32 i = 0; i < AutoComplete; ++i)
	{
		const EOLCTutorialObjective Objective = static_cast<EOLCTutorialObjective>(
			static_cast<uint8>(EOLCTutorialObjective::InspectCrashSite) + i);
		const float Delay = Test->AutoCompleteBaseDelaySeconds + 0.75f * static_cast<float>(i);
		World->GetTimerManager().SetTimer(
			TestAutoCompleteTimers[i],
			FTimerDelegate::CreateUObject(this, &UOLCTutorialSubsystem::CompleteObjective, Objective),
			Delay, false);
	}

	if (Test->bSkipOnStart)
	{
		World->GetTimerManager().SetTimer(
			TestSkipTimer, this, &UOLCTutorialSubsystem::SkipTutorial, Test->SkipDelaySeconds, false);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial test hook: auto-complete=%d skip=%d"), AutoComplete, Test->bSkipOnStart ? 1 : 0);
}

void UOLCTutorialSubsystem::Deinitialize()
{
	FWorldDelegates::OnPostWorldCreation.Remove(WorldCreatedHandle);
	OnTutorialAdvanced.RemoveDynamic(this, &UOLCTutorialSubsystem::HandleTutorialAdvanced);

	ClearObjectiveHighlight();
	TargetActors.Reset();
	TutorialData = nullptr;
	Super::Deinitialize();
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------

EOLCTutorialObjective UOLCTutorialSubsystem::GetActiveObjective() const
{
	if (State.bSkipped || State.ActiveObjectiveIndex >= NumOLCTutorialObjectives)
	{
		return EOLCTutorialObjective::Complete;
	}

	if (!State.bObjectiveCompleted.IsValidIndex(State.ActiveObjectiveIndex))
	{
		// Corrupt state guard — should never happen after LoadProgress sanitization.
		return EOLCTutorialObjective::None;
	}

	// Enum ordering is load-bearing: objectives occupy values 1..5 in sequence order.
	return static_cast<EOLCTutorialObjective>(
		static_cast<uint8>(EOLCTutorialObjective::InspectCrashSite) + static_cast<uint8>(State.ActiveObjectiveIndex));
}

bool UOLCTutorialSubsystem::IsObjectiveComplete(EOLCTutorialObjective Objective) const
{
	const int32 Index = GetObjectiveIndex(Objective);
	return Index >= 0 && State.IsObjectiveComplete(Index);
}

// ---------------------------------------------------------------------------
// Progression
// ---------------------------------------------------------------------------

void UOLCTutorialSubsystem::CompleteObjective(EOLCTutorialObjective Objective)
{
	const int32 Index = GetObjectiveIndex(Objective);
	if (Index < 0 || State.bSkipped)
	{
		return; // None/Complete, or tutorial already skipped — no-op.
	}

	if (State.IsObjectiveComplete(Index))
	{
		return; // Idempotent: already complete — no double-advance, no double broadcast.
	}

	State.bObjectiveCompleted[Index] = true;
	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: objective completed — %s"),
		*UEnum::GetDisplayValueAsText(Objective).ToString());

	OnObjectiveCompleted.Broadcast(Objective);
	ApplyObjectiveRewards(Objective);
	AdvanceActiveObjective();

	// WP-129 Step 4: persist after every completion so a mid-tutorial quit
	// resumes at the correct objective with no duplicate reward grants.
	SaveProgress();
}

void UOLCTutorialSubsystem::SkipTutorial()
{
	if (State.bSkipped)
	{
		return; // Idempotent.
	}

	// WP-129 Step 4: grant any unclaimed per-objective rewards before sealing the
	// terminal state. ApplyObjectiveRewards is itself idempotent (bRewardClaimed),
	// so objectives already completed/rewarded via normal play are skipped here —
	// only objectives the player never reached get their reward granted now.
	for (int32 i = 0; i < NumOLCTutorialObjectives; ++i)
	{
		const EOLCTutorialObjective Objective = static_cast<EOLCTutorialObjective>(
			static_cast<uint8>(EOLCTutorialObjective::InspectCrashSite) + i);
		ApplyObjectiveRewards(Objective);
	}

	State.bSkipped = true;
	for (bool& bComplete : State.bObjectiveCompleted)
	{
		bComplete = true;
	}
	State.ActiveObjectiveIndex = NumOLCTutorialObjectives;
	State.bRewardsGranted = true;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: skipped — all objectives marked complete, unclaimed rewards granted"));
	OnTutorialAdvanced.Broadcast(EOLCTutorialObjective::Complete);
}

void UOLCTutorialSubsystem::ResetProgress()
{
	State = FOLCTutorialState();
	TargetActors.Reset();

	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: progress reset to fresh state"));
}

// ---------------------------------------------------------------------------
// Target resolution
// ---------------------------------------------------------------------------

AActor* UOLCTutorialSubsystem::GetTargetActorForObjective(EOLCTutorialObjective Objective) const
{
	if (const TWeakObjectPtr<AActor>* Registered = TargetActors.Find(Objective))
	{
		if (Registered->IsValid())
		{
			return Registered->Get();
		}
	}

	return ResolveDefaultTargetActor(Objective);
}

void UOLCTutorialSubsystem::RegisterTargetActor(EOLCTutorialObjective Objective, AActor* Actor)
{
	if (!Actor)
	{
		TargetActors.Remove(Objective);
	}
	else
	{
		TargetActors.Add(Objective, Actor);
	}

	// WP-129 Step 3: targets may be registered after world begin-play (e.g.
	// OLCMenuGameMode::TransitionToGameplay spawns the crash-site actor) —
	// re-resolve the highlight so the active objective is marked immediately.
	RefreshObjectiveHighlight();
}

AActor* UOLCTutorialSubsystem::ResolveDefaultTargetActor(EOLCTutorialObjective Objective) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	switch (Objective)
	{
	case EOLCTutorialObjective::InspectCrashSite:
		for (TActorIterator<AOLCCrashSitePrototypeActor> It(World); It; ++It)
		{
			return *It;
		}
		// Fall back to the gameplay world actor when no dedicated crash-site actor exists.
		for (TActorIterator<AOLCGameplayWorldActor> It(World); It; ++It)
		{
			return *It;
		}
		return nullptr;

	case EOLCTutorialObjective::BeginDriveRepair:
		// The crashed dropship lives on the gameplay world actor (CrashedShipComponent).
		for (TActorIterator<AOLCGameplayWorldActor> It(World); It; ++It)
		{
			return *It;
		}
		return nullptr;

	default:
		// CollectMaterials / DiscoverDeposit / PlaceSolarArray targets are resolved by
		// RefreshObjectiveHighlight (WP-129 Step 3) via class search.
		return nullptr;
	}
}

// ---------------------------------------------------------------------------
// WP-129 Step 3 — world-target highlight
// ---------------------------------------------------------------------------

void UOLCTutorialSubsystem::HandleTutorialAdvanced(EOLCTutorialObjective NewActiveObjective)
{
	RefreshObjectiveHighlight();
}

void UOLCTutorialSubsystem::ClearObjectiveHighlight()
{
	if (AActor* Actor = HighlightedActor.Get())
	{
		if (UOLCObjectiveHighlightComponent* Highlight = Actor->FindComponentByClass<UOLCObjectiveHighlightComponent>())
		{
			Highlight->DestroyComponent();
			UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: objective highlight removed from %s"), *Actor->GetName());
		}
	}

	HighlightedActor = nullptr;
	HighlightRelativeOffset = FVector::ZeroVector;
}

void UOLCTutorialSubsystem::RefreshObjectiveHighlight()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const EOLCTutorialObjective Active = GetActiveObjective();

	AActor* Target = nullptr;
	FVector RelativeOffset = FVector::ZeroVector;

	switch (Active)
	{
	case EOLCTutorialObjective::InspectCrashSite:
		// Crash-site wreck — registered actor first, then class search.
		Target = GetTargetActorForObjective(Active);
		break;

	case EOLCTutorialObjective::CollectMaterials:
		// Manual mining happens anywhere — no world target to highlight.
		Target = nullptr;
		break;

	case EOLCTutorialObjective::DiscoverDeposit:
	{
		// Nearest mineral-node cluster to the player's current view (wreck if no PC yet).
		for (TActorIterator<AOLCCrashSitePrototypeActor> It(World); It; ++It)
		{
			FVector From = It->GetActorLocation();
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				From = PC->GetFocalLocation();
			}

			FVector NodeLocation;
			if (It->GetNearestMineralNodeLocation(From, NodeLocation))
			{
				Target = *It;
				RelativeOffset = NodeLocation - It->GetActorLocation();
			}
			break; // First crash-site actor wins.
		}
		break;
	}

	case EOLCTutorialObjective::PlaceSolarArray:
	{
		// Build-tile grid origin — the crash-site actor's root (tile 0,0).
		for (TActorIterator<AOLCCrashSitePrototypeActor> It(World); It; ++It)
		{
			Target = *It;
			break;
		}
		if (!Target)
		{
			Target = GetTargetActorForObjective(EOLCTutorialObjective::InspectCrashSite);
		}
		break;
	}

	case EOLCTutorialObjective::BeginDriveRepair:
		// Dropship drive module — the crashed ship (registered or built-in resolution).
		Target = GetTargetActorForObjective(Active);
		break;

	default:
		// None/Complete (terminal) — nothing to highlight.
		Target = nullptr;
		break;
	}

	if (!Target)
	{
		ClearObjectiveHighlight();
		return;
	}

	if (HighlightedActor.Get() == Target &&
		FVector::DistSquared(HighlightRelativeOffset, RelativeOffset) < KINDA_SMALL_NUMBER)
	{
		return; // Already highlighted in place — no churn.
	}

	ClearObjectiveHighlight();

	UOLCObjectiveHighlightComponent* Highlight = NewObject<UOLCObjectiveHighlightComponent>(Target);
	Target->AddInstanceComponent(Highlight);
	Highlight->RegisterComponent();
	Highlight->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Highlight->SetRelativeLocation(RelativeOffset);

	HighlightedActor = Target;
	HighlightRelativeOffset = RelativeOffset;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: objective highlight attached to %s (objective index %d)"),
		*Target->GetName(), static_cast<int32>(Active));
}

// ---------------------------------------------------------------------------
// WP-129 Step 3 — dev console commands (runtime verification hooks)
// ---------------------------------------------------------------------------

namespace
{
	/**
	 * Tutorial subsystem for console/dev commands. Prefers a PIE (or packaged
	 * game) world over the editor world, since both exist while PIE runs and
	 * verification targets the playing world.
	 */
	UOLCTutorialSubsystem* FindTutorialSubsystemAnyWorld()
	{
		UOLCTutorialSubsystem* Fallback = nullptr;
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			const UWorld* World = Context.World();
			if (!World || !World->GetGameInstance())
			{
				continue;
			}
			UOLCTutorialSubsystem* Tutorial = World->GetGameInstance()->GetSubsystem<UOLCTutorialSubsystem>();
			if (!Tutorial)
			{
				continue;
			}
			if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
			{
				return Tutorial;
			}
			if (!Fallback)
			{
				Fallback = Tutorial;
			}
		}
		return Fallback;
	}
}

static FAutoConsoleCommand GOLCTutorialSkipCmd(
	TEXT("OLCTutorial.Skip"),
	TEXT("Skip the crash-site tutorial: all objectives complete, highlights cleared, progress saved."),
	FConsoleCommandDelegate::CreateStatic([]()
	{
		if (UOLCTutorialSubsystem* Tutorial = FindTutorialSubsystemAnyWorld())
		{
			Tutorial->SkipTutorial(); // broadcasts OnTutorialAdvanced → highlight cleared
			Tutorial->SaveProgress();
		}
	}));

static FAutoConsoleCommand GOLCTutorialRefreshCmd(
	TEXT("OLCTutorial.Refresh"),
	TEXT("Re-apply the active tutorial objective's world highlight immediately."),
	FConsoleCommandDelegate::CreateStatic([]()
	{
		if (UOLCTutorialSubsystem* Tutorial = FindTutorialSubsystemAnyWorld())
		{
			Tutorial->RefreshObjectiveHighlight();
		}
	}));

static FAutoConsoleCommand GOLCTutorialResetCmd(
	TEXT("OLCTutorial.Reset"),
	TEXT("Reset tutorial progress to a fresh state (saved) and re-apply the highlight."),
	FConsoleCommandDelegate::CreateStatic([]()
	{
		if (UOLCTutorialSubsystem* Tutorial = FindTutorialSubsystemAnyWorld())
		{
			Tutorial->ResetProgress();
			Tutorial->SaveProgress();
			Tutorial->RefreshObjectiveHighlight();
		}
	}));

static FAutoConsoleCommand GOLCTutorialTransitionCmd(
	TEXT("OLCTutorial.Transition"),
	TEXT("Dev hook: trigger OLCMenuGameMode::TransitionToGameplay directly (skips the crash sequence)."),
	FConsoleCommandDelegate::CreateStatic([]()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			const UWorld* World = Context.World();
			if (!World)
			{
				continue;
			}
			if (AOLCMenuGameMode* MenuGM = Cast<AOLCMenuGameMode>(World->GetAuthGameMode()))
			{
				MenuGM->TransitionToGameplay();
				return;
			}
		}
	}));

// UE 5.8: no FAutoConsoleCommandWithArgs — plain FAutoConsoleCommand accepts the args delegate.
static FAutoConsoleCommand GOLCTutorialCompleteCmd(
	TEXT("OLCTutorial.Complete"),
	TEXT("Dev hook: complete tutorial objective <1-5> (1=InspectCrashSite, 2=CollectMaterials, 3=DiscoverDeposit, 4=PlaceSolarArray, 5=BeginDriveRepair)."),
	FConsoleCommandWithArgsDelegate::CreateStatic([](const TArray<FString>& Args)
	{
		if (Args.Num() < 1)
		{
			return;
		}

		const int32 Value = FCString::Atoi(*Args[0]);
		const uint8 First = static_cast<uint8>(EOLCTutorialObjective::InspectCrashSite);
		const uint8 Last = static_cast<uint8>(EOLCTutorialObjective::BeginDriveRepair);
		if (Value < First || Value > Last)
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] OLCTutorial.Complete: invalid objective %d (expected 1-5)"), Value);
			return;
		}

		if (UOLCTutorialSubsystem* Tutorial = FindTutorialSubsystemAnyWorld())
		{
			Tutorial->CompleteObjective(static_cast<EOLCTutorialObjective>(Value));
		}
	}));

// ---------------------------------------------------------------------------
// Data access
// ---------------------------------------------------------------------------

void UOLCTutorialSubsystem::InitializeFromConfig(UOLCTutorialData* InData)
{
	SetTutorialData(InData);

	// Re-apply the world highlight for whichever objective is currently active
	// (fresh start: objective 1 / InspectCrashSite; resumed session: wherever
	// Initialize()'s LoadProgress left off) now that config-driven visuals
	// (HighlightParams) are available.
	RefreshObjectiveHighlight();

	UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: initialized from config (%s), active objective index %d"),
		InData ? *InData->GetName() : TEXT("none — using built-in defaults"), State.ActiveObjectiveIndex);
}

TArray<FOLCResourceAmount> UOLCTutorialSubsystem::GetObjectiveRewards(EOLCTutorialObjective Objective) const
{
	const int32 Index = GetObjectiveIndex(Objective);
	if (!TutorialData || !TutorialData->ObjectiveRewards.IsValidIndex(Index))
	{
		return TArray<FOLCResourceAmount>();
	}

	return TutorialData->ObjectiveRewards[Index].Rewards;
}

void UOLCTutorialSubsystem::ApplyObjectiveRewards(EOLCTutorialObjective Objective)
{
	const int32 Index = GetObjectiveIndex(Objective);
	if (Index < 0 || !State.bRewardClaimed.IsValidIndex(Index) || State.bRewardClaimed[Index])
	{
		return; // None/Complete, or already granted — idempotent.
	}

	const TArray<FOLCResourceAmount> Rewards = GetObjectiveRewards(Objective);
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCUIDataSubsystem* UIData = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			for (const FOLCResourceAmount& Reward : Rewards)
			{
				UIData->AddResource(Reward.ResourceType, Reward.CurrentValue);
			}
		}
	}

	// Set the claim flag regardless of whether there was inventory to add to —
	// it marks "this objective's reward path has run", not "resources were added".
	State.bRewardClaimed[Index] = true;

	if (Rewards.Num() > 0)
	{
		UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: granted %d reward entries for objective %s"),
			Rewards.Num(), *UEnum::GetDisplayValueAsText(Objective).ToString());
	}
}

// ---------------------------------------------------------------------------
// Persistence (native UE5 SaveGame slot)
// ---------------------------------------------------------------------------

bool UOLCTutorialSubsystem::SaveProgress()
{
	UOLCTutorialSaveData* SaveObject = Cast<UOLCTutorialSaveData>(
		UGameplayStatics::CreateSaveGameObject(UOLCTutorialSaveData::StaticClass()));
	if (!SaveObject)
	{
		UE_LOG(LogOLC, Error, TEXT("[OLC] Tutorial: failed to create save game object"));
		return false;
	}

	SaveObject->State = State;

	const FString SlotName = GetSaveSlotName();
	const bool bSaved = UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, 0);

	if (bSaved)
	{
		UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: progress saved to slot %s (active index %d, skipped=%d)"),
			*SlotName, State.ActiveObjectiveIndex, State.bSkipped);
	}
	else
	{
		UE_LOG(LogOLC, Error, TEXT("[OLC] Tutorial: SaveGameToSlot failed for slot %s"), *SlotName);
	}

	return bSaved;
}

bool UOLCTutorialSubsystem::LoadProgress()
{
	const FString SlotName = GetSaveSlotName();
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return false;
	}

	UOLCTutorialSaveData* SaveObject = Cast<UOLCTutorialSaveData>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!SaveObject)
	{
		UE_LOG(LogOLC, Error, TEXT("[OLC] Tutorial: failed to load save game from slot %s"), *SlotName);
		return false;
	}

	State = SaveObject->State;

	// Defensive sanitization for older/corrupt saves.
	if (State.bObjectiveCompleted.Num() != NumOLCTutorialObjectives)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Tutorial: save had %d completion entries — resetting to default"),
			State.bObjectiveCompleted.Num());
		State.bObjectiveCompleted.Init(false, NumOLCTutorialObjectives);
	}
	if (State.bRewardClaimed.Num() != NumOLCTutorialObjectives)
	{
		// Older saves predating WP-129 Step 4 won't have this field — default to
		// unclaimed rather than dropping any already-completed objective's reward.
		State.bRewardClaimed.Init(false, NumOLCTutorialObjectives);
	}
	if (State.ActiveObjectiveIndex < 0)
	{
		State.ActiveObjectiveIndex = 0;
	}
	else if (State.ActiveObjectiveIndex > NumOLCTutorialObjectives)
	{
		State.ActiveObjectiveIndex = NumOLCTutorialObjectives;
	}

	return true;
}

// ---------------------------------------------------------------------------
// Internals
// ---------------------------------------------------------------------------

int32 UOLCTutorialSubsystem::GetObjectiveIndex(EOLCTutorialObjective Objective)
{
	const uint8 Value = static_cast<uint8>(Objective);
	const uint8 First = static_cast<uint8>(EOLCTutorialObjective::InspectCrashSite);
	const uint8 Last = static_cast<uint8>(EOLCTutorialObjective::BeginDriveRepair);

	if (Value < First || Value > Last)
	{
		return -1; // None or Complete — not a completable objective.
	}

	return static_cast<int32>(Value) - static_cast<int32>(First);
}

void UOLCTutorialSubsystem::AdvanceActiveObjective()
{
	const int32 PreviousIndex = State.ActiveObjectiveIndex;

	while (State.ActiveObjectiveIndex < NumOLCTutorialObjectives &&
		State.IsObjectiveComplete(State.ActiveObjectiveIndex))
	{
		++State.ActiveObjectiveIndex;
	}

	if (State.ActiveObjectiveIndex >= NumOLCTutorialObjectives)
	{
		// Terminal state: no further tutorial rewards may be granted. This runs
		// AFTER the final OnObjectiveCompleted broadcast, so per-objective reward
		// handlers for the last objective still see bRewardsGranted == false.
		State.ActiveObjectiveIndex = NumOLCTutorialObjectives;
		State.bRewardsGranted = true;
	}

	if (State.ActiveObjectiveIndex != PreviousIndex)
	{
		const EOLCTutorialObjective NewActive = GetActiveObjective();
		UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: advanced — active objective %d → %d"),
			PreviousIndex, State.ActiveObjectiveIndex);
		OnTutorialAdvanced.Broadcast(NewActive);
	}
}
