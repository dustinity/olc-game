#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCShipModuleData.h"
#include "OLCShipBuilderWidget.generated.h"

class UOLCUIDataSubsystem;

/**
 * Ship Builder / Dropship Repair overlay widget.
 * Opens with M key, shows 2-column layout: left = hull with slots, right = module catalog.
 */
UCLASS()
class OURLASTCHANCE_API UOLCShipBuilderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCShipBuilderWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Toggle ship builder visibility. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ship")
	void ToggleVisibility();

	/** Close the ship builder. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ship")
	void CloseShipBuilder();

	/** Install selected module. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ship")
	void OnInstallClicked();

	/** Swap selected module (remove old, install new). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ship")
	void OnSwapClicked();

	/** Repair damaged module in selected category. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Ship")
	void OnRepairClicked();

protected:
	/** Build the left column (hull with slot indicators). */
	TSharedRef<SWidget> BuildHullPanel();

	/** Build the right column (module catalog with details). */
	TSharedRef<SWidget> BuildModuleCatalog();

	/** Build category tabs. */
	TSharedRef<SWidget> BuildCategoryTabs();

	/** Build module list for selected category. */
	TSharedRef<SWidget> BuildModuleList();

	/** Build module card for a single module. */
	TSharedRef<SWidget> BuildModuleCard(const FOLCShipModuleViewData& Module, bool bSelected);

	/** Build details panel for selected module. */
	TSharedRef<SWidget> BuildDetailsPanel();

	/** Update hull slot indicators based on current state. */
	void UpdateHullSlots();

	/** Refresh module catalog display. */
	void RefreshModuleCatalog();

	/** Update details panel with selected module info. */
	void UpdateDetailsPanel();

private:
	/** Current selected category. */
	EOLCShipModuleCategory SelectedCategory = EOLCShipModuleCategory::Drives;

	/** Index of selected module in current category list. */
	int32 SelectedModuleIndex = -1;

	/** Whether ship builder is visible. */
	bool bIsVisible = false;

	/** Cached subsystem reference. */
	UPROPERTY()
	TObjectPtr<UOLCUIDataSubsystem> DataSubsystem;

	/** Module cards for current category. */
	TArray<FOLCShipModuleViewData> CurrentCategoryModules;

	/** Slot indicator colors (green=installed, yellow=damaged, red=empty). */
	FLinearColor GetSlotColor(EOLCShipModuleCategory Category) const;

	/** Get module state text. */
	FText GetStateText(EOLCModuleState State) const;
};
