#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "OLCSharedWidgets.generated.h"

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
class SWidget;
class SBorder;
class SButton;
class STextBlock;
class SImage;
class SOverlay;
class SVerticalBox;
class SHorizontalBox;
class SBox;
class SScaleBox;
class SCanvasPanel;

struct FSlateDynamicImageBrush;
struct FSlateBrush;

// ---------------------------------------------------------------------------
// Style colors (Stil-1 hard sci-fi)
// ---------------------------------------------------------------------------
namespace OLCStyleColors
{
	constexpr FLinearColor GunmetalBlack  = FLinearColor(0.102f, 0.114f, 0.129f, 1.0f);
	constexpr FLinearColor DarkSteel      = FLinearColor(0.141f, 0.157f, 0.188f, 1.0f);
	constexpr FLinearColor CharcoalGray   = FLinearColor(0.180f, 0.196f, 0.227f, 1.0f);
	constexpr FLinearColor BorderGray     = FLinearColor(0.239f, 0.259f, 0.302f, 1.0f);
	constexpr FLinearColor PrimaryOrange  = FLinearColor(0.910f, 0.522f, 0.165f, 1.0f);
	constexpr FLinearColor HoverOrange    = FLinearColor(0.961f, 0.620f, 0.247f, 1.0f);
	constexpr FLinearColor TacticalBlue   = FLinearColor(0.231f, 0.510f, 0.965f, 1.0f);
	constexpr FLinearColor ValidGreen     = FLinearColor(0.133f, 0.773f, 0.369f, 1.0f);
	constexpr FLinearColor DangerRed      = FLinearColor(0.945f, 0.267f, 0.267f, 1.0f);
	constexpr FLinearColor WarningYellow  = FLinearColor(0.957f, 0.839f, 0.196f, 1.0f);
	constexpr FLinearColor TextWhite      = FLinearColor(0.920f, 0.950f, 0.980f, 1.0f);
	constexpr FLinearColor TextDim        = FLinearColor(0.580f, 0.620f, 0.680f, 1.0f);
	constexpr FLinearColor OverlayBG      = FLinearColor(0.015f, 0.025f, 0.035f, 0.88f);
}

// ---------------------------------------------------------------------------
// WBP_UI_Frame — Angular panel/frame shell
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCFrameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCFrameWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedRef<SWidget> BuildFrame();
};

// ---------------------------------------------------------------------------
// WBP_UI_Button — Primary, secondary, danger, disabled button variants
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCButtonWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = "OLC|UI")
	void OnButtonClick();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText ButtonText = FText::FromString(TEXT("BUTTON"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bPrimary = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bDanger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bDisabled = false;

private:
	FReply OnClicked();
};

// ---------------------------------------------------------------------------
// WBP_UI_IconButton — Settings, close, pause, scan, filter, rotate
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCIconButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCIconButtonWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = "OLC|UI")
	void OnIconClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText IconLabel = FText::FromString(TEXT("X"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText IconTooltipText = FText::FromString(TEXT("Close"));

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FReply OnClicked();
};

// ---------------------------------------------------------------------------
// WBP_UI_TabButton — Tab strip buttons
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCTabButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCTabButtonWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = "OLC|UI")
	void OnTabClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText TabLabel = FText::FromString(TEXT("TAB"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	bool bActive = false;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FReply OnClicked();
};

// ---------------------------------------------------------------------------
// WBP_UI_ResourceCounter — One resource with icon, value, capacity, delta, pressure state
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCResourceCounterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCResourceCounterWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void SetViewData(const FOLCResourceCounterViewData& Data);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FOLCResourceCounterViewData CurrentData;
};

// ---------------------------------------------------------------------------
// WBP_UI_ResourceStrip — Adaptive strip of resource counters
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCResourceStripWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCResourceStripWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void SetResources(const TArray<FOLCResourceCounterViewData>& Resources);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TArray<TObjectPtr<UOLCResourceCounterWidget>> CounterWidgets;
};

// ---------------------------------------------------------------------------
// WBP_UI_Badge — TIR, biome, hazard, faction, scan, status chips
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCBadgeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCBadgeWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void SetViewData(const FOLCBadgeViewData& Data);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FOLCBadgeViewData CurrentData;
};

// ---------------------------------------------------------------------------
// WBP_UI_ProgressBar — Health, research, repair, construction, route progress
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCProgressBarWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void SetViewData(const FOLCProgressViewData& Data);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FOLCProgressViewData CurrentData;
};

// ---------------------------------------------------------------------------
// WBP_UI_DetailPanel — Shared right-side entity/planet/system/module detail panel
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCDetailPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCDetailPanelWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedRef<SWidget> BuildDetailRow(const FText& Label, const FText& Value, const FLinearColor& Color = OLCStyleColors::TextWhite);
};

// ---------------------------------------------------------------------------
// WBP_UI_Tooltip — Hover/click details
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCTooltipWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void SetTooltipContent(const FText& Title, const FText& Body);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FText TooltipTitle;
	FText TooltipBody;
};

// ---------------------------------------------------------------------------
// WBP_UI_ModalOverlay — Settings, confirmations, screen popups
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCModalOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCModalOverlayWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = "OLC|UI")
	void OnClose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|UI")
	FText ModalTitle = FText::FromString(TEXT("MODAL"));

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FReply OnCloseClicked();
};

// ---------------------------------------------------------------------------
// WBP_UI_TestSwitcher — Developer-only screen selector
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCTestSwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCTestSwitcherWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	FReply OnScreenSelect(int32 ScreenIndex);
	TSharedRef<SWidget> BuildScreenButton(const FText& Label, const FText& Hotkey, int32 Index);
};

// ---------------------------------------------------------------------------
// WBP_UI_Keymap — Player-facing shortcut reference overlay
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCKeymapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCKeymapWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedRef<SWidget> BuildKeyRow(const FText& KeyLabel, const FText& ActionLabel, const FText& DetailLabel);
};
