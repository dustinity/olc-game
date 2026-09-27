#include "OLCWelcomeScreenWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Paths.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "OLCWelcomeScreen"

namespace
{
	constexpr float DesignWidth = 1920.0f;
	constexpr float DesignHeight = 1080.0f;

	FString UIFile(const TCHAR* FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/WelcomeScreen") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }

	FSlateColor MenuTextColor()
	{
		return FSlateColor(FLinearColor(0.92f, 0.95f, 0.98f, 1.0f));
	}

	FLinearColor PanelBlack()
	{
		return FLinearColor(0.015f, 0.025f, 0.035f, 0.96f);
	}

	FLinearColor PanelSteel()
	{
		return FLinearColor(0.06f, 0.075f, 0.09f, 0.96f);
	}

	FLinearColor MenuOrange()
	{
		return FLinearColor(0.95f, 0.42f, 0.03f, 1.0f);
	}
}

UOLCWelcomeScreenWidget::UOLCWelcomeScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

// ---------------------------------------------------------------------------
// VFX — Smoke drift and orange flash animation (NativeTick)
// ---------------------------------------------------------------------------
void UOLCWelcomeScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SmokeTimer += InDeltaTime;
	FlashTimer += InDeltaTime;

	// Update smoke positions/opacities via InvalidateLayoutAndVolatility.
	if (SmokeImages.Num() > 0)
	{
		InvalidateLayoutAndVolatility();
	}

	// Flash overlay: pulse every 3 seconds using sin-wave.
	if (FlashOverlay.IsValid() && bFlashAssetLoaded)
	{
		float Phase = FMath::Fmod(FlashTimer, 3.0f);
		// Sin wave 0..PI maps to 0.0 -> 0.15 -> 0.0 over ~1.5s within the 3s cycle.
		float Opacity = FMath::Clamp(FMath::Sin(Phase * (PI / 1.5f)) * 0.15f, 0.0f, 0.15f);
		FlashOverlay->SetColorAndOpacity(FLinearColor(0.95f, 0.42f, 0.03f, Opacity));
	}
}

FLinearColor UOLCWelcomeScreenWidget::GetSmokeColor(int32 SmokeIndex) const
{
	// Warm dark color tint per WP-13 spec: (0.6, 0.35, 0.15) base with animated alpha.
	const float BaseR = 0.6f, BaseG = 0.35f, BaseB = 0.15f;

	float Alpha;
	switch (SmokeIndex)
	{
		case 0:
			// Smoke 1: oscillate between 0.20 and 0.40 alpha over 8s cycle.
			Alpha = 0.30f + FMath::Sin(SmokeTimer * 0.8f) * 0.10f;
			break;
		case 1:
			// Smoke 2: oscillate between 0.16 and 0.32 alpha over 10s cycle (offset).
			Alpha = 0.24f + FMath::Sin(SmokeTimer * 0.6f + 1.5f) * 0.08f;
			break;
		default:
			Alpha = 0.25f;
			break;
	}

	return FLinearColor(BaseR, BaseG, BaseB, Alpha);
}

FVector2D UOLCWelcomeScreenWidget::GetSmokeOffset(int32 SmokeIndex, float NormalizedTime) const
{
	switch (SmokeIndex)
	{
		case 0:
			// Slow drift right-up over 12s cycle.
			return FVector2D(
				FMath::Sin(SmokeTimer * 0.5f) * 30.0f,
				FMath::Cos(SmokeTimer * 0.4f) * 15.0f);
		case 1:
			// Slow drift left-down over 15s cycle (offset).
			return FVector2D(
				FMath::Sin(SmokeTimer * 0.3f + 2.0f) * 25.0f,
				FMath::Cos(SmokeTimer * 0.35f + 1.0f) * 12.0f);
		default:
			return FVector2D::ZeroVector;
	}
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::RebuildWidget()
{
	BackgroundBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Background_3840x2160.png"))), FVector2D(DesignWidth, DesignHeight));
	ButtonActiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Button_Active.png"))), FVector2D(470.0f, 64.0f));
	ButtonInactiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Button_Inactive.png"))), FVector2D(470.0f, 64.0f));

	// Smoke VFX — try actual asset first, then fallback to procedural
	FString SmokePath = UIFile(TEXT("VFX_Smoke_Soft.png"));
	if (!AssetExists(SmokePath))
	{
		SmokePath = UIFile(TEXT("VFX_SmokePink_Soft.png"));
	}
	if (AssetExists(SmokePath))
	{
		SmokeBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*SmokePath), FVector2D(650.0f, 165.0f));
		bSmokeAssetLoaded = true;
		UE_LOG(LogTemp, Log, TEXT("[OLC] WelcomeScreen: Loaded smoke VFX from %s"), *SmokePath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] WelcomeScreen: Smoke VFX not found — using procedural SImage fallback"));
		SmokeBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Background_3840x2160.png"))), FVector2D(650.0f, 165.0f));
		bSmokeAssetLoaded = false;
	}

	// Orange flash overlay — procedural per WP-13 spec (no PNG needed)
	FlashBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Background_3840x2160.png"))), FVector2D(DesignWidth, DesignHeight));
	bFlashAssetLoaded = true;

	FrameCornerTLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Corner_TL.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerTRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Corner_TR.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerBRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Corner_BR.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerBLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Corner_BL.png"))), FVector2D(160.0f, 160.0f));
	FrameEdgeTopBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Edge_Top.png"))), FVector2D(1600.0f, 64.0f));
	FrameEdgeBottomBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Edge_Bottom.png"))), FVector2D(1600.0f, 64.0f));
	FrameEdgeLeftBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Edge_Left.png"))), FVector2D(64.0f, 760.0f));
	FrameEdgeRightBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Edge_Right.png"))), FVector2D(64.0f, 760.0f));
	ControlPanelBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*UIFile(TEXT("Frame_Control_Panel.png"))), FVector2D(280.0f, 96.0f));

	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas)
		// Background
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f))
		[
			SNew(SImage)
			.Image(BackgroundBrush.Get())
		]
		// Smoke overlay 1 (animated drift + opacity)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f))
		.Offset_Lambda([this]() -> FMargin {
			FVector2D Offset = GetSmokeOffset(0, SmokeTimer);
			return FMargin(150.0f + Offset.X, 690.0f + Offset.Y, 520.0f, 140.0f);
		})
		[
			SNew(SImage)
			.Image_Lambda([this]() -> const FSlateBrush* { return SmokeBrush.Get(); })
			.ColorAndOpacity_Lambda([this]() { return GetSmokeColor(0); })
		]
		// Smoke overlay 2 (animated drift + opacity, offset)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f))
		.Offset_Lambda([this]() -> FMargin {
			FVector2D Offset = GetSmokeOffset(1, SmokeTimer);
			return FMargin(640.0f + Offset.X, 700.0f + Offset.Y, 500.0f, 130.0f);
		})
		[
			SNew(SImage)
			.Image_Lambda([this]() -> const FSlateBrush* { return SmokeBrush.Get(); })
			.ColorAndOpacity_Lambda([this]() { return GetSmokeColor(1); })
		]
		// Title text
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f))
		.Offset(FMargin(96.0f, 116.0f, 720.0f, 230.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("TitleLine1", "OUR LAST"))
				.ColorAndOpacity(FLinearColor::White)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 64))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("TitleLine2", "CHANCE"))
				.ColorAndOpacity(FLinearColor::White)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 64))
			]
		]
		// Menu buttons
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f))
		.Offset(FMargin(96.0f, 382.0f, 470.0f, 420.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildMenuButton(LOCTEXT("Continue", "CONTINUE"), true, 0) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildMenuButton(LOCTEXT("NewCampaign", "NEW CAMPAIGN"), false, 1) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildMenuButton(LOCTEXT("LoadGame", "LOAD GAME"), false, 2) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildMenuButton(LOCTEXT("Settings", "SETTINGS"), false, 3) ]
			+ SVerticalBox::Slot().AutoHeight()
			[ BuildMenuButton(LOCTEXT("Exit", "EXIT"), false, 4) ]
		]
		// Frame corners
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f))
		.Offset(FMargin(150.0f, 690.0f, 520.0f, 140.0f))
		[ SNew(SImage).Image(FrameCornerTLBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 0.0f))
		.Alignment(FVector2D(1.0f, 0.0f))
		.Offset(FMargin(0.0f, 0.0f, 160.0f, 160.0f))
		[ SNew(SImage).Image(FrameCornerTRBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 1.0f))
		.Alignment(FVector2D(1.0f, 1.0f))
		.Offset(FMargin(0.0f, 0.0f, 160.0f, 160.0f))
		[ SNew(SImage).Image(FrameCornerBRBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 1.0f))
		.Alignment(FVector2D(0.0f, 1.0f))
		.Offset(FMargin(0.0f, 0.0f, 160.0f, 160.0f))
		[ SNew(SImage).Image(FrameCornerBLBrush.Get()) ]
		// Frame edges
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 0.0f))
		.Offset(FMargin(150.0f, 0.0f, -150.0f, 64.0f))
		[ SNew(SImage).Image(FrameEdgeTopBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 1.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, -64.0f, -150.0f, 64.0f))
		[ SNew(SImage).Image(FrameEdgeBottomBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 0.0f, 1.0f))
		.Offset(FMargin(0.0f, 150.0f, 64.0f, -150.0f))
		[ SNew(SImage).Image(FrameEdgeLeftBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(-64.0f, 150.0f, 64.0f, -150.0f))
		[ SNew(SImage).Image(FrameEdgeRightBrush.Get()) ]
		// Control panel
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(1.0f, 1.0f))
		.Alignment(FVector2D(1.0f, 1.0f))
		.Offset(FMargin(-56.0f, -62.0f, 280.0f, 96.0f))
		[ SNew(SImage).Image(ControlPanelBrush.Get()) ]
		// Settings panel
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f))
		[ BuildSettingsPanel() ]
		// Orange flash overlay (top Z-order) — procedural per WP-13 spec
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f))
		[
			SNew(SImage)
			.Image_Lambda([this]() -> const FSlateBrush* { return bFlashAssetLoaded ? FlashBrush.Get() : nullptr; })
			.ColorAndOpacity(FLinearColor(0.95f, 0.42f, 0.03f, 0.0f))
		];

	return SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		[
			SNew(SBox)
			.WidthOverride(DesignWidth)
			.HeightOverride(DesignHeight)
			[ Canvas ]
		];
}

void UOLCWelcomeScreenWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	BackgroundBrush.Reset();
	ButtonActiveBrush.Reset();
	ButtonInactiveBrush.Reset();
	SmokeBrush.Reset();
	FlashBrush.Reset();
	FrameCornerTLBrush.Reset();
	FrameCornerTRBrush.Reset();
	FrameCornerBRBrush.Reset();
	FrameCornerBLBrush.Reset();
	FrameEdgeTopBrush.Reset();
	FrameEdgeBottomBrush.Reset();
	FrameEdgeLeftBrush.Reset();
	FrameEdgeRightBrush.Reset();
	ControlPanelBrush.Reset();
	SmokeImages.Empty();
	FlashOverlay.Reset();
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::BuildMenuButton(const FText& Label, bool bPrimary, int32 ActionId)
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, ActionId]()
		{
			switch (ActionId)
			{
			case 0: return HandleContinueClicked();
			case 1: return HandleNewCampaignClicked();
			case 2: return HandleLoadGameClicked();
			case 3: return HandleSettingsClicked();
			case 4: return HandleExitClicked();
			default: return FReply::Handled();
			}
		})
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(bPrimary ? ButtonActiveBrush.Get() : ButtonInactiveBrush.Get())
			]
			+ SOverlay::Slot().Padding(38.0f, 10.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(Label)
				.ColorAndOpacity(bPrimary ? FLinearColor::White : FLinearColor(0.78f, 0.82f, 0.88f, 1.0f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 31))
			]
		];
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::BuildSettingsPanel()
{
	InitializeSettingsOptions();

	return SNew(SBorder)
		.Visibility_Lambda([this]() { return bSettingsVisible ? EVisibility::Visible : EVisibility::Collapsed; })
		.BorderImage(FCoreStyle::Get().GetBrush("NoBrush"))
		.Padding(FMargin(0.0f))
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				.WidthOverride(820.0f)
				.HeightOverride(610.0f)
				[
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(PanelBlack())
					.Padding(FMargin(34.0f))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.VAlign(VAlign_Top)
						.HAlign(HAlign_Fill)
						[
							SNew(SBox)
							.HeightOverride(3.0f)
							[
								SNew(SBorder)
								.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
								.BorderBackgroundColor(MenuOrange())
							]
						]
						+ SOverlay::Slot()
						.Padding(FMargin(0.0f, 18.0f, 0.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("SettingsTitle", "SETTINGS"))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 44))
							]
							+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 10.0f, 0.0f, 26.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("SettingsSubtitle", "DISPLAY CONFIGURATION"))
								.ColorAndOpacity(MenuOrange())
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
							]
							+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 14.0f)
							[
								BuildSettingsRow(
									LOCTEXT("ResolutionLabel", "RESOLUTION"),
									SNew(SComboBox<TSharedPtr<FString>>)
									.OptionsSource(&ResolutionOptions)
									.InitiallySelectedItem(SelectedResolution)
									.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Option) { return BuildSettingOption(Option); })
									.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo) { HandleResolutionChanged(NewSelection, SelectInfo); })
									[
										SNew(STextBlock)
										.Text_Lambda([this]() { return GetSelectedResolutionText(); })
										.ColorAndOpacity(MenuTextColor())
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
									])
							]
							+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 14.0f)
							[
								BuildSettingsRow(
									LOCTEXT("QualityLabel", "QUALITY"),
									SNew(SComboBox<TSharedPtr<FString>>)
									.OptionsSource(&QualityOptions)
									.InitiallySelectedItem(SelectedQuality)
									.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Option) { return BuildSettingOption(Option); })
									.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo) { HandleQualityChanged(NewSelection, SelectInfo); })
									[
										SNew(STextBlock)
										.Text_Lambda([this]() { return GetSelectedQualityText(); })
										.ColorAndOpacity(MenuTextColor())
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
									])
							]
							+ SVerticalBox::Slot().AutoHeight()
							[
								BuildSettingsRow(
									LOCTEXT("ShadowLabel", "SHADOW VOLUME"),
									SNew(SComboBox<TSharedPtr<FString>>)
									.OptionsSource(&ShadowOptions)
									.InitiallySelectedItem(SelectedShadow)
									.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Option) { return BuildSettingOption(Option); })
									.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo) { HandleShadowChanged(NewSelection, SelectInfo); })
									[
										SNew(STextBlock)
										.Text_Lambda([this]() { return GetSelectedShadowText(); })
										.ColorAndOpacity(MenuTextColor())
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
									])
							]
							+ SVerticalBox::Slot().FillHeight(1.0f)
							[ SNew(SSpacer) ]
							+ SVerticalBox::Slot().AutoHeight()
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot().AutoWidth()
								[ BuildMenuActionButton(LOCTEXT("ApplySettings", "APPLY"), true, FOnClicked::CreateUObject(this, &UOLCWelcomeScreenWidget::HandleApplySettingsClicked)) ]
								+ SHorizontalBox::Slot().AutoWidth().Padding(14.0f, 0.0f, 0.0f, 0.0f)
								[ BuildMenuActionButton(LOCTEXT("ResetSettings", "RESET"), false, FOnClicked::CreateUObject(this, &UOLCWelcomeScreenWidget::HandleResetSettingsClicked)) ]
								+ SHorizontalBox::Slot().FillWidth(1.0f)
								[ SNew(SSpacer) ]
								+ SHorizontalBox::Slot().AutoWidth()
								[ BuildMenuActionButton(LOCTEXT("Back", "BACK"), false, FOnClicked::CreateUObject(this, &UOLCWelcomeScreenWidget::HandleCloseSettingsClicked)) ]
							]
						]
					]
				]
			]
		];
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::BuildSettingsRow(const FText& Label, const TSharedRef<SWidget>& Control)
{
	return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(PanelSteel())
		.Padding(FMargin(18.0f, 12.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(Label)
				.ColorAndOpacity(MenuOrange())
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(300.0f)
				.HeightOverride(44.0f)
				[ Control ]
			]
		];
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::BuildSettingOption(TSharedPtr<FString> Option) const
{
	return SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(PanelSteel())
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(STextBlock)
			.Text(Option.IsValid() ? FText::FromString(*Option) : FText::GetEmpty())
			.ColorAndOpacity(MenuTextColor())
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
		];
}

TSharedRef<SWidget> UOLCWelcomeScreenWidget::BuildMenuActionButton(const FText& Label, bool bPrimary, const FOnClicked& OnClicked) const
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked(OnClicked)
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.BorderBackgroundColor(bPrimary ? MenuOrange() : PanelSteel())
			.Padding(FMargin(24.0f, 10.0f))
			[
				SNew(STextBlock)
				.Text(Label)
				.ColorAndOpacity(FLinearColor::White)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
			]
		];
}

void UOLCWelcomeScreenWidget::InitializeSettingsOptions()
{
	if (ResolutionOptions.Num() == 0)
	{
		const FVector2D Resolutions[] = {
			FVector2D(1280.0f, 720.0f),
			FVector2D(1600.0f, 900.0f),
			FVector2D(1920.0f, 1080.0f),
			FVector2D(2560.0f, 1440.0f),
			FVector2D(3840.0f, 2160.0f),
		};

		for (const FVector2D& Resolution : Resolutions)
		{
			ResolutionOptions.Add(MakeShared<FString>(FString::Printf(TEXT("%dx%d"), FMath::RoundToInt(Resolution.X), FMath::RoundToInt(Resolution.Y))));
		}
	}

	if (QualityOptions.Num() == 0)
	{
		QualityOptions.Add(MakeShared<FString>(TEXT("LOW")));
		QualityOptions.Add(MakeShared<FString>(TEXT("MEDIUM")));
		QualityOptions.Add(MakeShared<FString>(TEXT("HIGH")));
		QualityOptions.Add(MakeShared<FString>(TEXT("EPIC")));
		QualityOptions.Add(MakeShared<FString>(TEXT("CINEMATIC")));
	}

	if (ShadowOptions.Num() == 0)
	{
		ShadowOptions.Add(MakeShared<FString>(TEXT("LOW")));
		ShadowOptions.Add(MakeShared<FString>(TEXT("MEDIUM")));
		ShadowOptions.Add(MakeShared<FString>(TEXT("HIGH")));
		ShadowOptions.Add(MakeShared<FString>(TEXT("EPIC")));
		ShadowOptions.Add(MakeShared<FString>(TEXT("CINEMATIC")));
	}

	SyncSettingsFromGameUserSettings();
}

void UOLCWelcomeScreenWidget::SyncSettingsFromGameUserSettings()
{
	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (!UserSettings)
	{
		SelectedResolution = ResolutionOptions.IsValidIndex(2) ? ResolutionOptions[2] : nullptr;
		SelectedQuality = QualityOptions.IsValidIndex(2) ? QualityOptions[2] : nullptr;
		SelectedShadow = ShadowOptions.IsValidIndex(2) ? ShadowOptions[2] : nullptr;
		return;
	}

	const FIntPoint CurrentResolution = UserSettings->GetScreenResolution();
	const FString ResolutionText = FString::Printf(TEXT("%dx%d"), CurrentResolution.X, CurrentResolution.Y);
	SelectedResolution = ResolutionOptions.IsValidIndex(2) ? ResolutionOptions[2] : nullptr;
	for (const TSharedPtr<FString>& Option : ResolutionOptions)
	{
		if (Option.IsValid() && *Option == ResolutionText)
		{
			SelectedResolution = Option;
			break;
		}
	}

	const int32 OverallQuality = FMath::Clamp(UserSettings->GetOverallScalabilityLevel(), 0, QualityOptions.Num() - 1);
	SelectedQuality = QualityOptions.IsValidIndex(OverallQuality) ? QualityOptions[OverallQuality] : nullptr;

	const int32 ShadowQuality = FMath::Clamp(UserSettings->GetShadowQuality(), 0, ShadowOptions.Num() - 1);
	SelectedShadow = ShadowOptions.IsValidIndex(ShadowQuality) ? ShadowOptions[ShadowQuality] : nullptr;
}

FText UOLCWelcomeScreenWidget::GetSelectedResolutionText() const
{
	return SelectedResolution.IsValid() ? FText::FromString(*SelectedResolution) : LOCTEXT("ResolutionFallback", "1920x1080");
}

FText UOLCWelcomeScreenWidget::GetSelectedQualityText() const
{
	return SelectedQuality.IsValid() ? FText::FromString(*SelectedQuality) : LOCTEXT("QualityFallback", "HIGH");
}

FText UOLCWelcomeScreenWidget::GetSelectedShadowText() const
{
	return SelectedShadow.IsValid() ? FText::FromString(*SelectedShadow) : LOCTEXT("ShadowFallback", "HIGH");
}

void UOLCWelcomeScreenWidget::HandleResolutionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		SelectedResolution = NewSelection;
	}
}

void UOLCWelcomeScreenWidget::HandleQualityChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		SelectedQuality = NewSelection;
	}
}

void UOLCWelcomeScreenWidget::HandleShadowChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		SelectedShadow = NewSelection;
	}
}

FReply UOLCWelcomeScreenWidget::HandleContinueClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Welcome menu: Continue clicked"));
	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleNewCampaignClicked()
{
	UE_LOG(LogTemp, Display, TEXT("[OLC] Welcome menu: New Campaign clicked — starting crash sequence"));

	// Fire the delegate so the game mode can start the crash animation.
	OnNewCampaign.Broadcast();

	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleLoadGameClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Welcome menu: Load Game clicked"));
	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleSettingsClicked()
{
	SyncSettingsFromGameUserSettings();
	bSettingsVisible = true;
	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleExitClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleCloseSettingsClicked()
{
	bSettingsVisible = false;
	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleApplySettingsClicked()
{
	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (!UserSettings)
	{
		return FReply::Handled();
	}

	if (SelectedResolution.IsValid())
	{
		FString WidthText;
		FString HeightText;
		if (SelectedResolution->Split(TEXT("x"), &WidthText, &HeightText))
		{
			UserSettings->SetScreenResolution(FIntPoint(FCString::Atoi(*WidthText), FCString::Atoi(*HeightText)));
			UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		}
	}

	const int32 QualityIndex = QualityOptions.IndexOfByPredicate([this](const TSharedPtr<FString>& Option)
	{
		return Option == SelectedQuality;
	});
	if (QualityIndex != INDEX_NONE)
	{
		UserSettings->SetOverallScalabilityLevel(QualityIndex);
	}

	const int32 ShadowIndex = ShadowOptions.IndexOfByPredicate([this](const TSharedPtr<FString>& Option)
	{
		return Option == SelectedShadow;
	});
	if (ShadowIndex != INDEX_NONE)
	{
		UserSettings->SetShadowQuality(ShadowIndex);
	}

	UserSettings->ApplySettings(false);
	UserSettings->SaveSettings();

	UE_LOG(LogTemp, Display, TEXT("Welcome settings applied: Resolution=%s Quality=%d Shadow=%d"),
		SelectedResolution.IsValid() ? **SelectedResolution : TEXT("Unset"),
		QualityIndex,
		ShadowIndex);

	return FReply::Handled();
}

FReply UOLCWelcomeScreenWidget::HandleResetSettingsClicked()
{
	UGameUserSettings* UserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (UserSettings)
	{
		UserSettings->SetToDefaults();
		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}

	SyncSettingsFromGameUserSettings();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
