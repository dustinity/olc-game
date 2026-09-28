#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCDungeonGenerationData.h" // FOLCDungeonGenerationResult, FDungeonCompletionResult
#include "Core/OLCUIDataSubsystem.h" // FOLCSquadDeploymentData
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCTacticalCombatWidget.generated.h"

class AOLCMenuPlayerController;
class AOLCUnitBase;
class UOLCBossRaceData;
class UOLCRaceSubsystem;

/** Delegate fired when combat ends (victory/defeat). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEnded, bool, bVictory);

/** Delegate fired when boss phase changes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossPhaseChanged, int32, NewPhaseIndex, float, CurrentHPPercent);

/**
 * S08 Tactical Combat View — real-time top-down combat arena.
 * Unit health bars above units, ability hotbar at bottom, pause capability.
 * Combat is real-time with pause (NOT turn-based).
 */
UCLASS()
class OURLASTCHANCE_API UOLCTacticalCombatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCTacticalCombatWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when combat ends. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Combat")
	FOnCombatEnded OnCombatEnded;

	/** Fired when boss phase changes (NewPhaseIndex: 1-4, CurrentHPPercent). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Boss")
	FOnBossPhaseChanged OnBossPhaseChanged;

	/**
	 * WP-130: initialize this encounter from a generated dungeon layout and confirmed squad,
	 * replacing the prototype random-enemy fallback. Must be called before RebuildWidget() runs
	 * (i.e. before this widget is added to the viewport).
	 */
	void InitializeFromExpedition(const UOLCDungeonData* Dungeon, const FOLCDungeonGenerationResult& Layout, const FOLCSquadDeploymentData& Squad);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	/** Build the top bar with combat info and pause button. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the combat arena (center canvas). */
	TSharedRef<SWidget> BuildCombatArena();

	/** Build a unit marker on the arena canvas. */
	TSharedRef<SWidget> BuildUnitMarker(AOLCUnitBase* Unit, bool bPlayerUnit);

	/** Build a health bar above a unit. */
	TSharedRef<SWidget> BuildHealthBar(AOLCUnitBase* Unit);

	/** Build the ability hotbar at bottom. */
	TSharedRef<SWidget> BuildAbilityHotbar();

	/** Build combat log (scrolling text of events). */
	TSharedRef<SWidget> BuildCombatLog();

	/** Toggle pause state. */
	void TogglePause();

	/** Check if all enemy units are dead → victory. */
	bool CheckVictoryCondition();

	/** Check if all player units are dead → defeat. */
	bool CheckDefeatCondition();

	/** Add an entry to the combat log. */
	void LogCombatEvent(const FText& Message);

	/** Populate player and enemy unit lists for the prototype encounter. */
	void InitializeCombatEncounter();

	/** Check for boss phase transitions and apply their effects (rewritten for WP-130 to use real boss data). */
	void CheckBossPhaseTransition();

	/** Project a room's tile-space center onto the arena canvas (180 world units/tile convention, WP-113). */
	FVector2D ProjectRoomToCanvas(const FOLCDungeonRoom& Room) const;

	/** Build the completion result from the current encounter state and feed it via DungeonStateSubsystem. */
	FDungeonCompletionResult BuildCompletionResult(bool bVictory);

	// ---------------------------------------------------------------------------
	// Boss phase tracking
	// ---------------------------------------------------------------------------

	/** Current boss phase (1-4, where 4 is enraged <15% HP). */
	int32 CurrentBossPhase = 1;

	/** Whether a boss is currently active in this encounter. */
	bool bHasActiveBoss = false;

	/** Name of the last phase applied, so CheckBossPhaseTransition only reacts to real transitions. */
	FText LastBossPhaseName;

	/** WP-118 boss race data driving phase thresholds/mechanics (null until a boss encounter resolves). */
	UPROPERTY()
	TObjectPtr<UOLCBossRaceData> BossRaceData;

	/** The spawned boss unit, if any. */
	UPROPERTY()
	TObjectPtr<AOLCUnitBase> BossUnit;

	/** Outgoing boss damage multiplier for the current phase. */
	float CurrentBossDamageMultiplier = 1.0f;

	/** Boss's unscaled base attack damage, captured at spawn time so phase multipliers don't compound. */
	float BossBaseDamage = 0.0f;

	/** Cap on concurrently-summoned adds (WP-118 bSummonsAdds phases). */
	int32 AddCap = 8;

	/** Timer for the final-phase enrage (WP-118 FinalPhaseEnrageTimerSeconds): expiry forces defeat. */
	FTimerHandle BossEnrageTimer;

	UFUNCTION()
	void OnBossEnrageExpired();

	// ---------------------------------------------------------------------------
	// WP-130 expedition state
	// ---------------------------------------------------------------------------

	UPROPERTY()
	TObjectPtr<UOLCDungeonData> ExpeditionDungeon;

	FOLCDungeonGenerationResult ExpeditionLayout;

	UPROPERTY()
	FOLCSquadDeploymentData ExpeditionSquad;

	/** True once InitializeFromExpedition has supplied real layout/squad data. */
	bool bHasExpedition = false;

	// ---------------------------------------------------------------------------
	// Combat state
	// ---------------------------------------------------------------------------

	/** Whether combat is currently paused. */
	bool bIsPaused = false;

	/** Player units in this combat encounter. */
	TArray<AOLCUnitBase*> PlayerUnits;

	/** Enemy units in this combat encounter. */
	TArray<AOLCUnitBase*> EnemyUnits;

	/** Combat log entries (most recent at bottom). */
	TArray<FText> CombatLogEntries;

	/** Canvas size for arena positioning. */
	FVector2D ArenaSize = FVector2D::ZeroVector;

	/** Per-unit canvas position, populated when spawning from a generated layout (WP-130). */
	TMap<TWeakObjectPtr<AOLCUnitBase>, FVector2D> UnitCanvasPositions;

	/** Timer handle for combat tick processing. */
	FTimerHandle CombatTickTimer;

	/** Time between AI decision ticks (seconds). */
	float CombatTickInterval = 1.0f;

	/** Last time a combat tick was processed. */
	float LastCombatTickTime = 0.0f;

	/** Whether combat has ended. */
	bool bCombatEnded = false;

	/** Reference to the player controller for input handling. */
	UPROPERTY()
	TObjectPtr<AOLCMenuPlayerController> MenuPC;
};
