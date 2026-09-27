#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCTacticalCombatWidget.generated.h"

class AOLCMenuPlayerController;
class AOLCUnitBase;

/** Delegate fired when combat ends (victory/defeat). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatEnded, bool, bVictory);

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
