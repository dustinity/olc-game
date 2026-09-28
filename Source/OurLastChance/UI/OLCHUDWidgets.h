#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCTutorialTypes.h"
#include "Core/OLCWidgetBase.h"
#include "OLCHUDWidgets.generated.h"

class SWidget;
struct FSlateDynamicImageBrush;
class UOLCToastStackWidget;

// ===================================================================
// WBP_HUD_Root — Main RTS HUD root widget
// ===================================================================
UCLASS()
class OURLASTCHANCE_API UOLCMainRTSHUDWidget : public UOLCWidgetBase
{
	GENERATED_BODY()

public:
	UOLCMainRTSHUDWidget(const FObjectInitializer& ObjectInitializer);

	/** Delegate fired when ship status indicator is clicked — opens ship builder. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipStatusClicked);

	UPROPERTY(BlueprintAssignable, Category = "OLC|UI")
	FOnShipStatusClicked OnShipStatusClicked;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	TSharedRef<SWidget> BuildResourceStrip();
	TSharedRef<SWidget> BuildMissionProgress();
	TSharedRef<SWidget> BuildResearchProgress();
	TSharedRef<SWidget> BuildSelectedEntityPanel();
	TSharedRef<SWidget> BuildQuickBuildTray();
	TSharedRef<SWidget> BuildMinimap();
	TSharedRef<SWidget> BuildTimeSpeedControl();
	TSharedRef<SWidget> BuildShipStatusIndicator();
	TSharedRef<SWidget> BuildBiomeHazardsStrip();

	/** WP-107 Step 7: Toast notification stack (top-right). */
	TSharedRef<SWidget> BuildToastStack();

	/** WP-107 Step 7: Queue a toast notification from gameplay events. */
	UFUNCTION(BlueprintCallable, Category = "OLC|HUD")
	void QueueToastNotification(const FText& InTitle, const FText& InMessage, EOLCColorRole InColor = EOLCColorRole::Default);

	// -----------------------------------------------------------------------
	// WP-129 Step 4: contextual tutorial hint
	// -----------------------------------------------------------------------

	/** Top-center contextual hint bound to the tutorial subsystem's active objective. */
	TSharedRef<SWidget> BuildTutorialHint();

	/** UOLCTutorialSubsystem::OnTutorialAdvanced handler — refreshes the hint text/visibility. */
	UFUNCTION()
	void HandleTutorialAdvanced(EOLCTutorialObjective NewActiveObjective);

	/** Re-applies the current hint text and collapses the container once the tutorial ends. */
	void RefreshTutorialHint();

	/** Short instruction string for the active objective; empty once complete/skipped. */
	FText GetTutorialHintText() const;

	UPROPERTY(Transient)
	TObjectPtr<UOLCToastStackWidget> ToastStackWidget;

	TSharedPtr<class STextBlock> TutorialHintTextBlock;
	TSharedPtr<SWidget> TutorialHintContainer;
};

// ===================================================================
// WBP_ConstructionOverlay — Construction mode overlay
// ===================================================================
UCLASS()
class OURLASTCHANCE_API UOLCConstructionOverlayWidget : public UOLCWidgetBase
{
	GENERATED_BODY()

public:
	UOLCConstructionOverlayWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void RotateBuild();

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void CancelBuild();

	/** Confirm building placement at current cursor position. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void ConfirmBuild();

	/** Check if current mouse position is over valid terrain for placement. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI")
	bool IsPlacementValid() const { return bIsPlacementValid; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void RefreshConstructionData();
	TSharedRef<SWidget> BuildCategoryList();
	TSharedRef<SWidget> BuildCardFor(const FOLCBuildCardViewData& Card, bool bSelected);
	TSharedRef<SWidget> BuildCardThumbnail(const FOLCBuildCardViewData& Card);
	TSharedRef<SWidget> BuildPlacementPreview();
	TSharedRef<SWidget> BuildBuildDetailPanel();

	void OnCategorySelected(int32 CategoryIndex);
	void OnCardSelected(int32 CardIndex);
	FReply OnRotateClicked();
	FReply OnCancelClicked();
	FReply OnConfirmClicked();
	void RefreshConstructionScreen();

	/** Raycast mouse to terrain and update placement validity. */
	void UpdatePlacementValidity();

	TArray<FOLCBuildCardViewData> BuildCards;
	TArray<EOLCConstructionCategory> ConstructionCategories;
	TArray<TUniquePtr<FSlateDynamicImageBrush>> BuildCardBrushes;
	int32 SelectedCategoryIndex = 0;
	int32 SelectedCardIndex = -1;
	int32 BuildRotationDegrees = 0;

	/** Whether current mouse position is valid for building placement. */
	bool bIsPlacementValid = false;

	/** Current grid coordinate under cursor. */
	FIntPoint PlacementGridOrigin = FIntPoint::ZeroValue;
};
