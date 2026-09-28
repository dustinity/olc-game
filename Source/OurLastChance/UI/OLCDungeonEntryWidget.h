#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCDungeonData.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCDungeonEntryWidget.generated.h"

/** Delegate fired when player clicks Deploy on a selected dungeon. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonDeploy, const UOLCDungeonData*, DungeonData);

/**
 * S07 Dungeon Entry screen — zone list with difficulty badges, reward preview, Deploy button.
 * Displays seven procedural dungeon archetypes with seeded, fully-reachable layouts.
 */
UCLASS()
class OURLASTCHANCE_API UOLCDungeonEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCDungeonEntryWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when player selects a dungeon and clicks Deploy. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Dungeon")
	FOnDungeonDeploy OnDungeonDeployed;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeDestruct() override;

private:
	/** Build the top bar with title and back button. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the dungeon zone list (left panel). */
	TSharedRef<SWidget> BuildDungeonList();

	/** Build a single dungeon entry row. */
	TSharedRef<SWidget> BuildDungeonEntry(const UOLCDungeonData* Dungeon, bool bSelected);

	/** Build the reward preview panel (right side). */
	TSharedRef<SWidget> BuildRewardPreview();

	/** Initialize seven procedural dungeon archetypes from WP-130 DataAssets. */
	void InitializeDungeons();

	/** Display rarity tier badge color and tooltip for an archetype. */
	FLinearColor GetRarityColor(EOLCDungeonRarityTier Tier);

	/** Display rarity tier display name for an archetype. */
	FText GetRarityDisplayName(EOLCDungeonRarityTier Tier);

	/** Display boss phase indicator text for an archetype. */
	FText GetPhaseIndicator(const UOLCDungeonData* Dungeon);

	/** Display layout seed text for an archetype. */
	FText GetLayoutSeedText(int32 Seed);

	TArray<TObjectPtr<UOLCDungeonData>> SampleDungeons;
	int32 SelectedIndex = -1;
};
