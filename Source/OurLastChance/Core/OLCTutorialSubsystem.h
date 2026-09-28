#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCTutorialTypes.h"
#include "OLCTutorialSubsystem.generated.h"

class AActor;
class UOLCTutorialData;

// ---------------------------------------------------------------------------
// Tutorial delegates (WP-129 Step 1)
// ---------------------------------------------------------------------------

/** Fired exactly once per objective when it transitions from incomplete to complete. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOLCTutorialObjectiveCompleted, EOLCTutorialObjective, Objective);

/**
 * Fired whenever the active objective advances (including to Complete).
 * NewActiveObjective is the objective that is active AFTER the advance;
 * it is EOLCTutorialObjective::Complete once all five are done or skipped.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOLCTutorialAdvanced, EOLCTutorialObjective, NewActiveObjective);

/**
 * GameInstanceSubsystem owning the interactive crash-site tutorial (WP-129 Step 1).
 *
 * Responsibilities:
 * - Owns the FOLCTutorialState and advances it through the five sequential
 *   objectives (Idle → Active → Complete per objective).
 * - Idempotent progression: calling CompleteObjective on an already-complete
 *   (or skipped/None/Complete) objective is a no-op — no double-advance,
 *   no double delegate broadcast.
 * - Persists the full state through a native UE5 SaveGame slot and auto-resumes
 *   on Initialize, so reloading picks up the current objective without
 *   duplicate reward grants (bRewardsGranted guards terminal-state rewards).
 * - Resolves the world target actor for each objective: registered actors win
 *   (RegisterTargetActor — wired to real gameplay actors in later steps), with
 *   built-in fallbacks for the crash-site and dropship objectives.
 *
 * Reward application (WP-129 Step 4): ApplyObjectiveRewards grants a completed
 * objective's UOLCTutorialData reward batch to UOLCUIDataSubsystem exactly once,
 * guarded per-objective by State.bRewardClaimed so neither a skip, a resume, nor
 * a re-triggered completion can double-grant.
 */
UCLASS()
class OURLASTCHANCE_API UOLCTutorialSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// -----------------------------------------------------------------------
	// State queries
	// -----------------------------------------------------------------------

	/**
	 * Currently active objective, or EOLCTutorialObjective::Complete when the
	 * tutorial has ended (all objectives complete or skipped). Never returns
	 * None in a valid state — None is reserved for invalid/corrupt input.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	EOLCTutorialObjective GetActiveObjective() const;

	/** Full persisted state snapshot (value copy). */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	FOLCTutorialState GetState() const { return State; }

	/** True if the given objective has been completed. */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	bool IsObjectiveComplete(EOLCTutorialObjective Objective) const;

	/** True once all five objectives are complete (skipped or not). */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	bool AreAllObjectivesComplete() const { return State.AreAllObjectivesComplete(); }

	/** True if the player skipped the tutorial. */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	bool WasTutorialSkipped() const { return State.bSkipped; }

	// -----------------------------------------------------------------------
	// Progression
	// -----------------------------------------------------------------------

	/**
	 * Mark one objective as complete and advance the active pointer past any
	 * completed objectives. Idempotent: a second call for the same objective
	 * (or a call after skip) is a no-op — OnObjectiveCompleted fires at most
	 * once per objective, and OnTutorialAdvanced only when the active index
	 * actually changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void CompleteObjective(EOLCTutorialObjective Objective);

	/**
	 * End the tutorial immediately: marks every objective complete, sets
	 * bSkipped and bRewardsGranted so no further events can fire or grant
	 * anything, and broadcasts OnTutorialAdvanced(Complete). Idempotent.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void SkipTutorial();

	/**
	 * Reset to a fresh tutorial (all objectives incomplete, index 0, no skip).
	 * Does NOT broadcast delegates or touch the save slot — call SaveProgress()
	 * afterwards if the reset should persist. Intended for new-game setup/tests.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void ResetProgress();

	/** Mark terminal rewards as granted (called by reward-apply code in Step 3). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void MarkRewardsGranted() { State.bRewardsGranted = true; }

	/**
	 * WP-129 Step 3 (test-only): schedule the UOLCTutorialTestConfig auto-complete /
	 * auto-skip timers against World's timer manager. Safe to call multiple times or
	 * from multiple call sites — no-ops after the first successful call per session
	 * (bTestHooksScheduled). PIE worlds are duplicated rather than created, so
	 * FWorldDelegates::OnPostWorldCreation never fires for them; callers that know
	 * they are running under a real game world (e.g. AOLCMenuPlayerController::BeginPlay)
	 * should call this directly rather than relying solely on OnPostWorldCreation.
	 */
	void ScheduleTestHooksIfNeeded(UWorld* World);

	// -----------------------------------------------------------------------
	// Target resolution
	// -----------------------------------------------------------------------

	/**
	 * World actor the player should interact with for the given objective.
	 * Registered actors (RegisterTargetActor) take precedence; otherwise falls
	 * back to built-in resolution: InspectCrashSite → crash-site actor,
	 * BeginDriveRepair → gameplay world actor (crashed dropship). Returns null
	 * when no target is available for the objective yet.
	 */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	AActor* GetTargetActorForObjective(EOLCTutorialObjective Objective) const;

	/** Register (or replace) the world target actor for one objective. Null clears it. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void RegisterTargetActor(EOLCTutorialObjective Objective, AActor* Actor);

	/**
	 * WP-129 Step 3 — move/clear the world highlight component so it matches
	 * the currently active objective (terminal state → no highlight). Called
	 * from OnTutorialAdvanced, after target registration, and shortly after
	 * world begin-play. Safe to call any time; no-op when already in place.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void RefreshObjectiveHighlight();

	// -----------------------------------------------------------------------
	// Data access
	// -----------------------------------------------------------------------

	/** Register the tutorial configuration data asset (null-tolerant; defaults apply). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void SetTutorialData(UOLCTutorialData* InData) { TutorialData = InData; }

	/**
	 * WP-129 Step 5 — register the tutorial configuration DataAsset at
	 * gameplay start and re-apply the world highlight for whichever objective
	 * is currently active (fresh start: objective 1; resumed session: wherever
	 * LoadProgress left off). Null-tolerant: a missing DataAsset leaves the
	 * built-in defaults in place via GetObjectiveRewards/GetDefaultStartingInventory.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void InitializeFromConfig(UOLCTutorialData* InData);

	/** Returns the registered tutorial data asset (may be null). */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	const UOLCTutorialData* GetTutorialData() const { return TutorialData; }

	/** Reward amounts for one objective from the data asset (empty if none/invalid). */
	UFUNCTION(BlueprintPure, Category = "OLC|Tutorial")
	TArray<FOLCResourceAmount> GetObjectiveRewards(EOLCTutorialObjective Objective) const;

	// -----------------------------------------------------------------------
	// Persistence (native UE5 SaveGame slot: Saved/Saves/OLC_TutorialProgress)
	// -----------------------------------------------------------------------

	/** Write the full FOLCTutorialState to the save slot. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	bool SaveProgress();

	/** Read the full FOLCTutorialState from the save slot. Returns true if a valid save was loaded. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	bool LoadProgress();

	// -----------------------------------------------------------------------
	// Delegates
	// -----------------------------------------------------------------------

	UPROPERTY(BlueprintAssignable, Category = "OLC|Tutorial")
	FOnOLCTutorialObjectiveCompleted OnObjectiveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "OLC|Tutorial")
	FOnOLCTutorialAdvanced OnTutorialAdvanced;

private:
	/** Maps an objective to its 0-based sequence index; -1 for None/Complete. */
	static int32 GetObjectiveIndex(EOLCTutorialObjective Objective);

	/** Advances ActiveObjectiveIndex past completed objectives; broadcasts OnTutorialAdvanced if it moved. */
	void AdvanceActiveObjective();

	/** Built-in target resolution used when no actor was registered for the objective. */
	AActor* ResolveDefaultTargetActor(EOLCTutorialObjective Objective) const;

	/**
	 * WP-129 Step 4 — grant this objective's UOLCTutorialData reward batch to
	 * UOLCUIDataSubsystem exactly once. Idempotent via State.bRewardClaimed: safe
	 * to call redundantly (e.g. SkipTutorial calls it for every objective).
	 */
	void ApplyObjectiveRewards(EOLCTutorialObjective Objective);

	/** WP-129 Step 3 — OnTutorialAdvanced handler: re-resolves and moves the world highlight. */
	UFUNCTION()
	void HandleTutorialAdvanced(EOLCTutorialObjective NewActiveObjective);

	/** WP-129 Step 3 — remove the highlight component from its current actor (idempotent). */
	void ClearObjectiveHighlight();

	/** Save slot name (file: Saved/Saves/<slot>). */
	static FString GetSaveSlotName() { return TEXT("OLC_TutorialProgress"); }

	/** Persisted tutorial progress. */
	UPROPERTY(VisibleAnywhere, Category = "OLC|Tutorial")
	FOLCTutorialState State;

	/** Registered tutorial configuration (null → built-in defaults). */
	UPROPERTY()
	TObjectPtr<UOLCTutorialData> TutorialData;

	/** Runtime-registered target actors per objective (not serialized). */
	TMap<EOLCTutorialObjective, TWeakObjectPtr<AActor>> TargetActors;

	// -----------------------------------------------------------------------
	// WP-129 Step 3 — world-target highlight state (not serialized)
	// -----------------------------------------------------------------------

	/** Actor currently carrying the objective-highlight component. */
	TWeakObjectPtr<AActor> HighlightedActor;

	/** Relative offset of the highlight on that actor (e.g. mineral-node offset). */
	FVector HighlightRelativeOffset = FVector::ZeroVector;

	/** FWorldDelegates::OnPostWorldCreation handle for scheduling the deferred highlight apply. */
	FDelegateHandle WorldCreatedHandle;

	/** Timer that applies the highlight shortly after world creation (actors exist by then). */
	FTimerHandle HighlightApplyTimerHandle;

	/** WP-129 Step 3 (test-only): deferred auto-completion timers (UOLCTutorialTestConfig), one per objective. */
	FTimerHandle TestAutoCompleteTimers[NumOLCTutorialObjectives];

	/** WP-129 Step 3 (test-only): deferred auto-skip timer (UOLCTutorialTestConfig). */
	FTimerHandle TestSkipTimer;

	/** WP-129 Step 3 (test-only): guards ScheduleTestHooksIfNeeded against double-scheduling. */
	bool bTestHooksScheduled = false;
};
