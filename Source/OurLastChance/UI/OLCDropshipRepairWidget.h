#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCShipModuleData.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCDropshipRepairWidget.generated.h"

/** Delegate fired when player starts a repair action. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRepairStarted, const FText&, ModuleName, float, RepairDuration);

/** Delegate fired when repair completes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRepairComplete, const FText&, ModuleName);

/**
 * S13 Dropship Repair View — blueprint-style 2-column layout.
 * Left panel: available modules/replacements list with repair costs.
 * Right panel: ship hull schematic with attachment point highlights.
 * Uses UOLCProgressBarWidget for repair progress bars.
 */
UCLASS()
class OURLASTCHANCE_API UOLCDropshipRepairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCDropshipRepairWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when player clicks Start Repair on a module. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Repair")
	FOnRepairStarted OnRepairStarted;

	/** Fired when repair completes. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Repair")
	FOnRepairComplete OnRepairComplete;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	/** Build the top bar with title and back button. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the left panel: available modules/replacements list. */
	TSharedRef<SWidget> BuildModuleList();

	/** Build a single module entry in the list. */
	TSharedRef<SWidget> BuildModuleEntry(const FOLCShipModuleViewData& Module, bool bSelected);

	/** Build the right panel: ship hull schematic with attachment points. */
	TSharedRef<SWidget> BuildHullSchematic();

	/** Build a hull attachment point indicator. */
	TSharedRef<SWidget> BuildAttachmentPoint(const FText& Name, EOLCModuleState State, bool bHasModule);

	/** Initialize sample ship modules from Briefing data. */
	void InitializeShipModules();

	/** Check if player has resources for repair. */
	bool CanRepair(const TArray<FOLCResourceAmount>& Cost);

	// ---------------------------------------------------------------------------
	// Ship state
	// ---------------------------------------------------------------------------

	/** Hull integrity percentage (0.0–1.0). */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Ship", meta = (AllowPrivateAccess = "true"))
	float HullIntegrityPercent = 1.0f;

	/** Total hull slots available on the dropship. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Ship", meta = (AllowPrivateAccess = "true"))
	int32 TotalHullSlots = 40;

	/** Hull slots currently occupied by modules. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Ship", meta = (AllowPrivateAccess = "true"))
	int32 UsedHullSlots = 0;

	TArray<FOLCShipModuleViewData> AvailableModules;
	int32 SelectedIndex = -1;

	/** Currently repairing module (if any). */
	FText CurrentRepairingModule;
	float CurrentRepairProgress = 0.0f;
	float CurrentRepairDuration = 0.0f;
};
