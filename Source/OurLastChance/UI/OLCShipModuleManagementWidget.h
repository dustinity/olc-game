#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCShipModuleData.h"
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCShipModuleManagementWidget.generated.h"

/** Delegate fired when a module is attached to the hull. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModuleAttached, const FText&, ModuleName, int32, SlotIndex);

/** Delegate fired when a module is detached from the hull. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModuleDetached, const FText&, ModuleName);

/**
 * S14 Ship Module Management — extends S13 with module grid by category,
 * drag-to-attach on hull schematic, TIR upgrade buttons per module.
 * 7 categories: Drives, Storage, Protection, Scanning, Weapons, Labs, Support.
 */
UCLASS()
class OURLASTCHANCE_API UOLCShipModuleManagementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCShipModuleManagementWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when a module is attached to the hull. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Modules")
	FOnModuleAttached OnModuleAttached;

	/** Fired when a module is detached from the hull. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Modules")
	FOnModuleDetached OnModuleDetached;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	/** Build the top bar with title and back button. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the left panel: module grid by category tabs. */
	TSharedRef<SWidget> BuildModuleGrid();

	/** Build a single module card in the grid. */
	TSharedRef<SWidget> BuildModuleCard(UOLCShipModuleData* Module, bool bSelected);

	/** Build the right panel: ship hull schematic with attachment points. */
	TSharedRef<SWidget> BuildHullManagement();

	/** Build a hull slot indicator (occupied or empty). */
	TSharedRef<SWidget> BuildHullSlot(int32 Index, UOLCShipModuleData* ModuleInSlot);

	/** Build the upgrade panel for selected module. */
	TSharedRef<SWidget> BuildUpgradePanel();

	/** Initialize sample modules from Briefing data (46 modules across 7 categories). */
	void InitializeModules();

	/** Get modules filtered by category. */
	TArray<UOLCShipModuleData*> GetModulesByCategory(EOLCShipModuleCategory Category) const;

	// ---------------------------------------------------------------------------
	// Module state
	// ---------------------------------------------------------------------------

	/** All available module DataAssets. */
	TArray<TObjectPtr<UOLCShipModuleData>> AllModules;

	/** Currently selected category tab. */
	EOLCShipModuleCategory SelectedCategory = EOLCShipModuleCategory::Drives;

	/** Index of selected module in the filtered grid. */
	int32 SelectedGridIndex = -1;

	/** Hull slots: each slot tracks which module is attached (null = empty). */
	TArray<TObjectPtr<UOLCShipModuleData>> HullSlots;

	/** Total hull slots on the dropship. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Ship", meta = (AllowPrivateAccess = "true"))
	int32 TotalHullSlots = 40;

	/** Slots currently occupied. */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Ship", meta = (AllowPrivateAccess = "true"))
	int32 UsedHullSlots = 0;

	/** Module TIR upgrade levels per hull slot (index = slot index). */
	TArray<int32> ModuleTIRLevels;
};
