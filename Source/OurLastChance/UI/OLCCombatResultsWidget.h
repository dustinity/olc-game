#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCDungeonData.h"
#include "Core/OLCResourceTypes.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCCombatResultsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScreenClose);

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

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	/** Build the top bar with combat result header. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the loot rewards panel (center). */
	TSharedRef<SWidget> BuildLootPanel();

	/** Build a single loot reward row. */
	TSharedRef<SWidget> BuildLootRow(const FOLCDungeonReward& Reward, int32 Quantity);

	/** Build the unit XP and casualty panel (left side). */
	TSharedRef<SWidget> BuildUnitResults();

	/** Build a single unit result entry. */
	TSharedRef<SWidget> BuildUnitEntry(FText UnitName, float XPReceived, bool bIsCasualty);

	/** Build the artifact found notification (right side). */
	TSharedRef<SWidget> BuildArtifactPanel();

	/** Initialize sample results data from combat. */
	void InitializeResults(bool bVictory);

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
};
