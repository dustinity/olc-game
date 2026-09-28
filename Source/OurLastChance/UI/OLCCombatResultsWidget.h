#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCDungeonData.h"
#include "Core/OLCDungeonGenerationData.h" // FDungeonCompletionResult (WP-130: moved out of this header to avoid a Core -> UI include)
#include "Core/OLCResourceTypes.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCCombatResultsWidget.generated.h"

/** Delegate fired when player clicks Return to Base. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenClose);

/** Delegate fired when dungeon completion results are fed into canonical systems. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonCompletion, const FDungeonCompletionResult&, Result);

/**
 * S10 Combat Results screen — loot inventory, XP bars per unit, casualty list, artifact found notification.
 * Displayed after combat ends (victory or defeat).
 */
UCLASS()
class OURLASTCHANCE_API UOLCCombatResultsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCCombatResultsWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when player clicks Return to Base. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Results")
	FOnScreenClose OnReturnToBase;

	/** Fired when dungeon completion results are fed into canonical systems. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Dungeon")
	FOnDungeonCompletion OnDungeonCompletion;

	/**
	 * WP-130: supply the real completion result from the tactical combat encounter
	 * (loot rolls, resources, casualties). Must be called before RebuildWidget()
	 * runs (i.e. before this widget is added to the viewport). When never called,
	 * RebuildWidget() falls back to the pre-WP-130 prototype data so the existing
	 * AOLCCombatTriggerActor flow keeps working.
	 */
	void InitializeFromResult(const FDungeonCompletionResult& Result);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	/** Build the top bar with combat result header. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the loot rewards panel (center). */
	TSharedRef<SWidget> BuildLootPanel();

	/** Build a single loot reward row. */
	TSharedRef<SWidget> BuildLootRow(const FOLCDungeonReward& Reward, int32 Quantity);

	/** Build a single loot row from a real WP-130 loot roll (rarity-colored, gold highlight for named uniques). */
	TSharedRef<SWidget> BuildLootRollRow(const FOLCDungeonLootRoll& Roll);

	/** Rarity tier -> badge color (same palette convention as S07's GetRarityColor). */
	static FLinearColor GetRarityColor(EOLCDungeonRarityTier Tier);

	/** Build the unit XP and casualty panel (left side). */
	TSharedRef<SWidget> BuildUnitResults();

	/** Build a single unit result entry. */
	TSharedRef<SWidget> BuildUnitEntry(FText UnitName, float XPReceived, bool bIsCasualty);

	/** Build the artifact found notification (right side). */
	TSharedRef<SWidget> BuildArtifactPanel();

	/** Initialize sample results data from combat. */
	void InitializeResults(bool bVictory);

	/** Feed dungeon completion results into canonical faction/progression systems. */
	void FeedCompletionResults();

	// ---------------------------------------------------------------------------
	// Results state
	// ---------------------------------------------------------------------------

	bool bVictory = false;

	TArray<FOLCDungeonReward> LootRewards;
	int32 LootQuantities[7] = {0}; // Per resource type index

	TArray<FText> UnitNames;
	TArray<float> UnitXPReceived;
	TArray<bool> UnitIsCasualty;

	FText ArtifactName;
	bool bFoundArtifact = false;

	// ---------------------------------------------------------------------------
	// WP-130: real completion result, when supplied via InitializeFromResult().
	// ---------------------------------------------------------------------------
	bool bHasRealResult = false;
	FDungeonCompletionResult RealResult;
};
