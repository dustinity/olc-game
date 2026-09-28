#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCWelcomeScreenWidget.generated.h"

struct FSlateDynamicImageBrush;
class SBorder;
class SImage;
class SWidget;
template <typename OptionType> class SComboBox;

/** Delegate fired when New Campaign is clicked — lets the game mode start the crash sequence. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewCampaignClicked);

UCLASS()
class OURLASTCHANCE_API UOLCWelcomeScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCWelcomeScreenWidget(const FObjectInitializer& ObjectInitializer);

	/** Bind a delegate to handle New Campaign clicks. */
	void SetOnNewCampaignClicked(const FOnNewCampaignClicked& InDelegate) { OnNewCampaign = InDelegate; }

	UPROPERTY(BlueprintAssignable, Category = "OLC|UI")
	FOnNewCampaignClicked OnNewCampaign;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** VFX — smoke drift and flash animation helpers */
	FLinearColor GetSmokeColor(int32 SmokeIndex) const;
	FVector2D GetSmokeOffset(int32 SmokeIndex, float NormalizedTime) const;

	TSharedRef<SWidget> BuildMenuButton(const FText& Label, int32 ActionId);
	TSharedRef<SWidget> BuildSettingsPanel();
	TSharedRef<SWidget> BuildSettingsRow(const FText& Label, const TSharedRef<SWidget>& Control);
	TSharedRef<SWidget> BuildSettingOption(TSharedPtr<FString> Option) const;
	TSharedRef<SWidget> BuildMenuActionButton(const FText& Label, bool bPrimary, const FOnClicked& OnClicked) const;

	void InitializeSettingsOptions();
	void SyncSettingsFromGameUserSettings();
	FText GetSelectedResolutionText() const;
	FText GetSelectedQualityText() const;
	FText GetSelectedShadowText() const;

	FReply HandleContinueClicked();
	FReply HandleNewCampaignClicked();
	FReply HandleLoadGameClicked();
	FReply HandleSettingsClicked();
	FReply HandleExitClicked();
	FReply HandleCloseSettingsClicked();
	FReply HandleApplySettingsClicked();
	FReply HandleResetSettingsClicked();

	void HandleResolutionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void HandleQualityChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void HandleShadowChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	TUniquePtr<FSlateDynamicImageBrush> BackgroundBrush;
	TUniquePtr<FSlateDynamicImageBrush> ButtonActiveBrush;
	TUniquePtr<FSlateDynamicImageBrush> ButtonInactiveBrush;
	TUniquePtr<FSlateDynamicImageBrush> SettingsPanelBrush;
	TUniquePtr<FSlateDynamicImageBrush> SmokeBrush;
	TArray<TSharedPtr<SImage>> SmokeImages;
	bool bSmokeAssetLoaded = false;
	TUniquePtr<FSlateDynamicImageBrush> FlashBrush; // Orange flash VFX (procedural)
	TSharedPtr<SBorder> FlashOverlay;
	bool bFlashAssetLoaded = false;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerTLBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerTRBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerBRBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerBLBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeTopBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeBottomBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeLeftBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeRightBrush;
	TUniquePtr<FSlateDynamicImageBrush> ControlPanelBrush;

	TArray<TSharedPtr<FString>> ResolutionOptions;
	TArray<TSharedPtr<FString>> QualityOptions;
	TArray<TSharedPtr<FString>> ShadowOptions;
	TSharedPtr<FString> SelectedResolution;
	TSharedPtr<FString> SelectedQuality;
	TSharedPtr<FString> SelectedShadow;

	bool bSettingsVisible = false;

	/** VFX animation state — smoke drift and orange flash pulse timers */
	float SmokeTimer = 0.0f;
	float FlashTimer = 0.0f;
};
