#include "OLCSharedWidgets.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Misc/Paths.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include "Player/OLCMenuPlayerController.h" // EOLCUIScreen, AOLCMenuPlayerController

#define LOCTEXT_NAMESPACE "OLCSharedWidgets"

// ---------------------------------------------------------------------------
// Design constants + shared asset path helpers (WP-13 Step 3)
// ---------------------------------------------------------------------------
namespace OLCDesign
{
	constexpr float BtnWidth = 200.0f;
	constexpr float BtnHeight = 40.0f;
	constexpr float FramePadding = 12.0f;
	constexpr float BadgeMinWidth = 60.0f;
	constexpr float BadgeHeight = 22.0f;
	constexpr float ProgressBarHeight = 8.0f;
	constexpr float IconButtonSize = 32.0f;
	constexpr float ResourceCounterWidth = 140.0f;
	constexpr float ResourceCounterHeight = 56.0f;

	/** Shared asset path — relative to project dir via ../../UE5/Assets/UI/Shared Assets/Sliced */
	FString SharedAssetPath(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Shared Assets/Sliced") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }

	FLinearColor GetPressureColor(EOLCStoragePressure Pressure)
	{
		switch (Pressure)
		{
			case EOLCStoragePressure::Approaching: return OLCStyleColors::WarningYellow;
			case EOLCStoragePressure::Full:
			case EOLCStoragePressure::Overflow:  return OLCStyleColors::DangerRed;
			default:                              return OLCStyleColors::TextWhite;
		}
	}

	FLinearColor GetProgressColor(EOLCColorRole Role)
	{
		switch (Role)
		{
			case EOLCColorRole::Primary:   return OLCStyleColors::PrimaryOrange;
			case EOLCColorRole::Secondary: return OLCStyleColors::TacticalBlue;
			case EOLCColorRole::Success:   return OLCStyleColors::ValidGreen;
			case EOLCColorRole::Danger:    return OLCStyleColors::DangerRed;
			case EOLCColorRole::Warning:   return OLCStyleColors::WarningYellow;
			default:                       return OLCStyleColors::TextDim;
		}
	}

	FLinearColor GetBadgeColor(EOLCColorRole Role)
	{
		return GetProgressColor(Role);
	}
}

// ===================================================================
// WBP_UI_Frame
// ===================================================================
UOLCFrameWidget::UOLCFrameWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCFrameWidget::RebuildWidget()
{
	return BuildFrame();
}

TSharedRef<SWidget> UOLCFrameWidget::BuildFrame()
{
	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
		.Padding(OLCDesign::FramePadding)
		[
			SNew(SOverlay)
			// Top border line
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 2.0f))
			]
			// Bottom border line
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 2.0f))
			]
			// Left border line
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Left)
			[
				SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(2.0f, 1.0f))
			]
			// Right border line
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Right)
			[
				SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(2.0f, 1.0f))
			]
			// Content area (slot for child widgets)
			+ SOverlay::Slot()
			[
				SNew(SVerticalBox)
			]
		];
}

// ===================================================================
// WBP_UI_Button
// ===================================================================
UOLCButtonWidget::UOLCButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCButtonWidget::RebuildWidget()
{
	FLinearColor TextCol = bDisabled ? OLCStyleColors::TextDim : OLCStyleColors::TextWhite;
	FLinearColor BorderCol;

	if (bDisabled)
		BorderCol = OLCStyleColors::CharcoalGray;
	else if (bDanger)
		BorderCol = OLCStyleColors::DangerRed;
	else if (bPrimary)
		BorderCol = OLCStyleColors::PrimaryOrange;
	else
		BorderCol = OLCStyleColors::TacticalBlue;

	// WP-13 Step 3: Load button image from shared atlas slices (SHR-BTN-*)
	FString BtnImageFile;
	if (bDisabled)
		BtnImageFile = TEXT("SHR-BTN-04_DisabledGrayButton.png");
	else if (bDanger)
		BtnImageFile = TEXT("SHR-BTN-03_DangerRedButton.png");
	else if (bPrimary)
		BtnImageFile = TEXT("SHR-BTN-01_PrimaryOrangeButton.png");
	else
		BtnImageFile = TEXT("SHR-BTN-02_SecondaryBlueButton.png");

	FString BtnImagePath = OLCDesign::SharedAssetPath(BtnImageFile);
	TSharedPtr<FSlateDynamicImageBrush> BtnBrush;
	if (OLCDesign::AssetExists(BtnImagePath))
	{
		BtnBrush = MakeShared<FSlateDynamicImageBrush>(FName(*BtnImagePath), FVector2D(200.0f, 40.0f));
	}

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.IsEnabled(!bDisabled)
		.OnClicked_Lambda([this]() { return OnClicked(); })
		[
			SNew(SOverlay)
			// Button image background (from atlas slice if available)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(BtnBrush.IsValid() ? BtnBrush.Get() : nullptr)
			]
			// Fallback: colored border with accent
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.Visibility(BtnBrush.IsValid() ? EVisibility::Collapsed : EVisibility::Visible)
				.BorderBackgroundColor(OLCStyleColors::DarkSteel)
				.Padding(FMargin(16.0f, 8.0f))
				[
					SNew(SOverlay)
					// Accent border (top line)
					+ SOverlay::Slot()
					.VAlign(VAlign_Top)
					.HAlign(HAlign_Fill)
					[
						SNew(SColorBlock).Color(BorderCol).Size(FVector2D(1.0f, 2.0f))
					]
					// Text
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return ButtonText; })
						.ColorAndOpacity(TextCol)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
						.Justification(ETextJustify::Center)
					]
				]
			]
			// Text overlay (always on top for readability with image buttons)
			+ SOverlay::Slot()
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return ButtonText; })
				.ColorAndOpacity(TextCol)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
				.Justification(ETextJustify::Center)
			]
		];
}

FReply UOLCButtonWidget::OnClicked()
{
	OnButtonClick();
	return FReply::Handled();
}

// ===================================================================
// WBP_UI_IconButton
// ===================================================================
UOLCIconButtonWidget::UOLCIconButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCIconButtonWidget::RebuildWidget()
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this]() { return OnClicked(); })
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
			.Padding(FMargin(8.0f, 6.0f))
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return IconLabel; })
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
				.Justification(ETextJustify::Center)
			]
		];
}

FReply UOLCIconButtonWidget::OnClicked()
{
	OnIconClick();
	return FReply::Handled();
}

// ===================================================================
// WBP_UI_TabButton
// ===================================================================
UOLCTabButtonWidget::UOLCTabButtonWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCTabButtonWidget::RebuildWidget()
{
	FLinearColor TextCol = bActive ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextDim;
	FLinearColor BgCol   = bActive ? OLCStyleColors::CharcoalGray : OLCStyleColors::DarkSteel;

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this]() { return OnClicked(); })
		[
			SNew(SBorder)
			.BorderBackgroundColor(BgCol)
			.Padding(FMargin(14.0f, 8.0f))
			[
				SNew(SOverlay)
				// Active indicator (bottom orange line)
				+ SOverlay::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Fill)
				[
					SNew(SColorBlock)
					.Color_Lambda([this]() { return bActive ? OLCStyleColors::PrimaryOrange : FLinearColor::Transparent; })
					.Size(FVector2D(1.0f, 2.0f))
				]
				// Label
				+ SOverlay::Slot()
				[
					SNew(STextBlock)
					.Text_Lambda([this]() { return TabLabel; })
					.ColorAndOpacity(TextCol)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
				]
			]
		];
}

FReply UOLCTabButtonWidget::OnClicked()
{
	bActive = true;
	OnTabClick();
	return FReply::Handled();
}

// ===================================================================
// WBP_UI_ResourceCounter
// ===================================================================
UOLCResourceCounterWidget::UOLCResourceCounterWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCResourceCounterWidget::SetViewData(const FOLCResourceCounterViewData& Data)
{
	CurrentData = Data;
}

TSharedRef<SWidget> UOLCResourceCounterWidget::RebuildWidget()
{
	FLinearColor ValueColor = OLCDesign::GetPressureColor(CurrentData.PressureState);

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
		.Padding(FMargin(10.0f, 8.0f))
		[
			SNew(SVerticalBox)
			// Label (resource name)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return CurrentData.DisplayName; })
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
			]
			// Value / Capacity
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() {
					return FText::Format(
						FText::FromString(TEXT("{0} / {1}")),
						FText::AsNumber(FMath::RoundToInt(CurrentData.Value)),
						FText::AsNumber(FMath::RoundToInt(CurrentData.Capacity)));
				})
				.ColorAndOpacity_Lambda([this, ValueColor]() { return ValueColor; })
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
			]
			// Delta indicator
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() {
					if (FMath::IsNearlyZero(CurrentData.Delta))
						return FText::FromString(TEXT(""));
					return FText::Format(
						FText::FromString(TEXT("{0}{1}/s")),
						FText::FromString(CurrentData.Delta > 0.0f ? TEXT("+") : TEXT("")),
						FText::AsNumber(CurrentData.Delta));
				})
				.ColorAndOpacity_Lambda([this]() {
					if (CurrentData.Delta >= 0.0f) return OLCStyleColors::ValidGreen;
					return OLCStyleColors::DangerRed;
				})
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			]
			// Mini progress bar (capacity fill)
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
				.Padding(FMargin(0.0f))
				[
					SNew(SBox)
					.HeightOverride(3.0f)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SNew(SBorder)
							.BorderBackgroundColor_Lambda([this, ValueColor]() { return ValueColor; })
							.Padding(FMargin(0.0f))
						]
					]
				]
			]
		];
}

// ===================================================================
// WBP_UI_ResourceStrip
// ===================================================================
UOLCResourceStripWidget::UOLCResourceStripWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCResourceStripWidget::SetResources(const TArray<FOLCResourceCounterViewData>& Resources)
{
	// In Slate we rebuild the layout. For a C++-only widget this is handled in RebuildWidget.
	// This method is here for Blueprint interoperability.
}

TSharedRef<SWidget> UOLCResourceStripWidget::RebuildWidget()
{
	TArray<FOLCResourceCounterViewData> Resources;
	Resources.Emplace(EOLCResourceType::Energy, 742.0f, 1000.0f, 15.3f);
	Resources.Emplace(EOLCResourceType::Fuel, 380.0f, 500.0f, -5.1f);
	Resources.Emplace(EOLCResourceType::ConstructionMaterial, 210.0f, 800.0f, 8.7f);
	Resources.Emplace(EOLCResourceType::Minerals, 450.0f, 600.0f, 3.2f);
	Resources.Emplace(EOLCResourceType::HullParts, 95.0f, 300.0f, 1.5f);
	Resources.Emplace(EOLCResourceType::Survival, 87.0f, 100.0f, -2.0f);

	TSharedRef<SHorizontalBox> Strip = SNew(SHorizontalBox);

	// Resources label
	Strip->AddSlot()
		.AutoWidth()
		.Padding(0.0f, 0.0f, 12.0f, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ResourcesLabel", "RESOURCES"))
			.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		];

	// Separator line
	Strip->AddSlot()
		.AutoWidth()
		.Padding(0.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(2.0f, 24.0f))
		];

	for (const auto& Res : Resources)
	{
		FLinearColor ValueColor = OLCDesign::GetPressureColor(Res.PressureState);
		const FOLCResourceCounterViewData LocalCopy = Res; // capture by copy for lambda

		Strip->AddSlot()
			.AutoWidth()
			.Padding(4.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
				.Padding(FMargin(10.0f, 6.0f))
				[
					SNew(SVerticalBox)
					// Name
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(LocalCopy.DisplayName)
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
					// Value / Capacity
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 1.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(FText::Format(
							FText::FromString(TEXT("{0} / {1}")),
							FText::AsNumber(FMath::RoundToInt(LocalCopy.Value)),
							FText::AsNumber(FMath::RoundToInt(LocalCopy.Capacity))))
						.ColorAndOpacity(ValueColor)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
					]
					// Delta
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 1.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(FMath::IsNearlyZero(LocalCopy.Delta)
							? FText::FromString(TEXT(""))
							: FText::Format(FText::FromString(TEXT("{0}{1}/s")),
								FText::FromString(LocalCopy.Delta > 0.0f ? TEXT("+") : TEXT("")),
								FText::AsNumber(LocalCopy.Delta)))
						.ColorAndOpacity(LocalCopy.Delta >= 0.0f ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					]
					// Mini bar
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 3.0f, 0.0f, 0.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
						.Padding(FMargin(0.0f))
						[
							SNew(SBox)
							.WidthOverride(80.0f)
							.HeightOverride(3.0f)
							[
								SNew(SBorder)
								.BorderBackgroundColor(ValueColor)
								.Padding(FMargin(0.0f))
								[
									SNew(SBox)
									.WidthOverride_Lambda([&LocalCopy]() {
										return FMath::Clamp(LocalCopy.Value / LocalCopy.Capacity, 0.0f, 1.0f) * 80.0f;
									})
									.HeightOverride(3.0f)
								]
							]
						]
					]
				]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
		.Padding(FMargin(10.0f, 6.0f))
		[
			Strip
		];
}

// ===================================================================
// WBP_UI_Badge
// ===================================================================
UOLCBadgeWidget::UOLCBadgeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCBadgeWidget::SetViewData(const FOLCBadgeViewData& Data)
{
	CurrentData = Data;
}

TSharedRef<SWidget> UOLCBadgeWidget::RebuildWidget()
{
	FLinearColor BadgeColor = OLCDesign::GetBadgeColor(CurrentData.ColorRole);

	// WP-13 Step 3: Load badge image from shared atlas slices (SHR-BDG-*)
	FString BadgeImageFile;
	switch (CurrentData.BadgeType)
	{
		case EOLCBadgeType::TIR:      BadgeImageFile = TEXT("SHR-BDG-01_TIRBadge.png"); break;
		case EOLCBadgeType::Biome:    BadgeImageFile = TEXT("SHR-BDG-02_BiomeBadge.png"); break;
		default:                      BadgeImageFile = TEXT(""); break;
	}

	TSharedPtr<FSlateDynamicImageBrush> BadgeBrush;
	if (!BadgeImageFile.IsEmpty())
	{
		FString BadgeImagePath = OLCDesign::SharedAssetPath(BadgeImageFile);
		if (OLCDesign::AssetExists(BadgeImagePath))
		{
			BadgeBrush = MakeShared<FSlateDynamicImageBrush>(FName(*BadgeImagePath), FVector2D(32.0f, 22.0f));
		}
	}

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::DarkSteel)
		.Padding(FMargin(8.0f, 4.0f))
		[
			SNew(SOverlay)
			// Badge image (from atlas slice if available)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			[
				SNew(SImage)
				.Visibility(BadgeBrush.IsValid() ? EVisibility::Visible : EVisibility::Collapsed)
				.Image(BadgeBrush.Get())
			]
			// Left accent bar (fallback when no badge image)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Left)
			[
				SNew(SColorBlock)
				.Visibility(BadgeBrush.IsValid() ? EVisibility::Collapsed : EVisibility::Visible)
				.Color_Lambda([this]() { return OLCDesign::GetBadgeColor(CurrentData.ColorRole); })
				.Size(FVector2D(3.0f, 1.0f))
			]
			// Label text
			+ SOverlay::Slot()
			.Padding(BadgeBrush.IsValid() ? FMargin(6.0f, 0.0f, 0.0f, 0.0f) : FMargin(6.0f, 0.0f, 0.0f, 0.0f))
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return CurrentData.Label; })
				.ColorAndOpacity_Lambda([this]() { return OLCDesign::GetBadgeColor(CurrentData.ColorRole); })
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
			]
		];
}

// ===================================================================
// WBP_UI_ProgressBar
// ===================================================================
UOLCProgressBarWidget::UOLCProgressBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCProgressBarWidget::SetViewData(const FOLCProgressViewData& Data)
{
	CurrentData = Data;
}

TSharedRef<SWidget> UOLCProgressBarWidget::RebuildWidget()
{
	FLinearColor BarColor = OLCDesign::GetProgressColor(CurrentData.ColorRole);

	// WP-13 Step 3: Load progress bar images from shared atlas slices (SHR-PRG-*)
	FString EmptyBarPath = OLCDesign::SharedAssetPath(TEXT("SHR-PRG-01_ProgressBarEmpty.png"));
	TSharedPtr<FSlateDynamicImageBrush> EmptyBarBrush;
	if (OLCDesign::AssetExists(EmptyBarPath))
	{
		EmptyBarBrush = MakeShared<FSlateDynamicImageBrush>(FName(*EmptyBarPath), FVector2D(304.0f, 12.0f));
	}

	FString FillBarPath = OLCDesign::SharedAssetPath(TEXT("SHR-PRG-02_ProgressBarFillSegment.png"));
	TSharedPtr<FSlateDynamicImageBrush> FillBarBrush;
	if (OLCDesign::AssetExists(FillBarPath))
	{
		FillBarBrush = MakeShared<FSlateDynamicImageBrush>(FName(*FillBarPath), FVector2D(8.0f, 12.0f));
	}

	return SNew(SVerticalBox)
	// Label row
	+ SVerticalBox::Slot().AutoHeight()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() { return CurrentData.Label; })
			.ColorAndOpacity(OLCStyleColors::TextWhite)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
		]
		+ SHorizontalBox::Slot().AutoWidth().Padding(8.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() {
				return FText::Format(
					FText::FromString(TEXT("{0}%")),
					FText::AsNumber(FMath::RoundToInt(CurrentData.CurrentValue / FMath::Max(CurrentData.MaxValue, 1.0f) * 100.0f)));
			})
			.ColorAndOpacity_Lambda([this]() { return OLCDesign::GetProgressColor(CurrentData.ColorRole); })
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
		]
	]
	// Bar track — uses atlas images if available, falls back to colored SBorder
	+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
	[
		SNew(SOverlay)
		// Background track (empty bar image or fallback color)
		+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(EmptyBarBrush.IsValid() ? EmptyBarBrush.Get() : nullptr)
		]
		// Fallback colored track (when no atlas image)
		+ SOverlay::Slot()
		[
			SNew(SBorder)
			.Visibility(EmptyBarBrush.IsValid() ? EVisibility::Collapsed : EVisibility::Visible)
			.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(2.0f))
			[
				SNew(SBox)
				.HeightOverride(OLCDesign::ProgressBarHeight)
				[
					SNew(SBorder)
					.BorderBackgroundColor_Lambda([this]() { return OLCDesign::GetProgressColor(CurrentData.ColorRole); })
					.Padding(FMargin(0.0f))
					[
						SNew(SBox)
						.WidthOverride_Lambda([this]() {
							return FMath::Clamp(CurrentData.CurrentValue / FMath::Max(CurrentData.MaxValue, 1.0f), 0.0f, 1.0f) * 300.0f;
						})
						.HeightOverride(OLCDesign::ProgressBarHeight)
					]
				]
			]
		]
		// Fill overlay (colored fill on top of empty bar image)
		+ SOverlay::Slot()
		[
			SNew(SBorder)
			.Visibility(EmptyBarBrush.IsValid() ? EVisibility::Visible : EVisibility::Collapsed)
			.BorderBackgroundColor_Lambda([this]() { return OLCDesign::GetProgressColor(CurrentData.ColorRole); })
			.Padding(FMargin(0.0f))
			[
				SNew(SBox)
				.WidthOverride_Lambda([this]() {
					return FMath::Clamp(CurrentData.CurrentValue / FMath::Max(CurrentData.MaxValue, 1.0f), 0.0f, 1.0f) * 300.0f;
				})
				.HeightOverride(OLCDesign::ProgressBarHeight)
			]
		]
	];
}

// ===================================================================
// WBP_UI_DetailPanel
// ===================================================================
UOLCDetailPanelWidget::UOLCDetailPanelWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCDetailPanelWidget::RebuildWidget()
{
	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::DarkSteel)
		.Padding(FMargin(16.0f))
		[
			SNew(SVerticalBox)
			// Title
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("DetailPanelTitle", "DETAILS"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
			]
			// Separator
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 8.0f)
			[
				SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 1.0f))
			]
			// Detail rows (fake data for testing)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 6.0f)
				[ BuildDetailRow(LOCTEXT("Det_Name", "NAME"), LOCTEXT("Det_NameVal", "Command Center")) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 6.0f)
				[ BuildDetailRow(LOCTEXT("Det_Category", "CATEGORY"), LOCTEXT("Det_CatVal", "Infrastructure")) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 6.0f)
				[ BuildDetailRow(LOCTEXT("Det_TIR", "TIR REQ"), FText::FromString(TEXT("2")), OLCStyleColors::PrimaryOrange) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 6.0f)
				[ BuildDetailRow(LOCTEXT("Det_Grid", "GRID SIZE"), FText::FromString(TEXT("4x4"))) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 6.0f)
				[ BuildDetailRow(LOCTEXT("Det_Power", "POWER"), FText::FromString(TEXT("-50/s")), OLCStyleColors::DangerRed) ]
				+ SVerticalBox::Slot().AutoHeight()
				[ BuildDetailRow(LOCTEXT("Det_Status", "STATUS"), LOCTEXT("Det_StatusVal", "OPERATIONAL"), OLCStyleColors::ValidGreen) ]
			]
			// Description
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Det_Desc", "Central command hub for base operations. Unlocks additional buildings and increases command range."))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
				.AutoWrapText(true)
			]
		];
}

TSharedRef<SWidget> UOLCDetailPanelWidget::BuildDetailRow(const FText& Label, const FText& Value, const FLinearColor& Color)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Text(Label)
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		]
		+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
		[
			SNew(STextBlock)
			.Text(Value)
			.ColorAndOpacity(Color)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
		];
}

// ===================================================================
// WBP_UI_Tooltip
// ===================================================================
UOLCTooltipWidget::UOLCTooltipWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCTooltipWidget::SetTooltipContent(const FText& Title, const FText& Body)
{
	TooltipTitle = Title;
	TooltipBody = Body;
}

TSharedRef<SWidget> UOLCTooltipWidget::RebuildWidget()
{
	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return TooltipTitle; })
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return TooltipBody; })
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
				.AutoWrapText(true)
			]
		];
}

// ===================================================================
// WBP_UI_ModalOverlay
// ===================================================================
UOLCModalOverlayWidget::UOLCModalOverlayWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCModalOverlayWidget::RebuildWidget()
{
	return SNew(SOverlay)
	// Dim background
	+ SOverlay::Slot()
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.Cursor(EMouseCursor::Default)
		.OnClicked_Lambda([this]() { return OnCloseClicked(); })
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::OverlayBG)
		]
	]
	// Modal panel
	+ SOverlay::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBox)
		.WidthOverride(500.0f)
		.HeightOverride(350.0f)
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::DarkSteel)
			.Padding(FMargin(0.0f))
			[
				SNew(SOverlay)
				// Top accent line
				+ SOverlay::Slot()
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Fill)
				[
					SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 3.0f))
				]
				// Content
				+ SOverlay::Slot()
				.Padding(FMargin(24.0f))
				[
					SNew(SVerticalBox)
					// Title
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text_Lambda([this]() { return ModalTitle; })
						.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
					]
					// Body placeholder
					+ SVerticalBox::Slot().FillHeight(1.0f).Padding(0.0f, 16.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModalBody", "Modal content placeholder."))
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
					]
					// Close button
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 16.0f, 0.0f, 0.0f).HAlign(HAlign_Right)
					[
						SNew(SButton)
						.Text(LOCTEXT("Close", "CLOSE"))
						.OnClicked_Lambda([this]() { return OnCloseClicked(); })
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.ContentPadding(FMargin(16.0f, 8.0f))
						[
							SNew(SBorder)
							.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
							.Padding(FMargin(16.0f, 8.0f))
							[
								SNew(STextBlock)
								.Text(LOCTEXT("CloseBtn", "CLOSE"))
								.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
							]
						]
					]
				]
			]
		]
	];
}

FReply UOLCModalOverlayWidget::OnCloseClicked()
{
	OnClose();
	return FReply::Handled();
}

// ===================================================================
// WBP_UI_TestSwitcher — Developer screen selector
// ===================================================================
UOLCTestSwitcherWidget::UOLCTestSwitcherWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCTestSwitcherWidget::RebuildWidget()
{
	TSharedRef<SVerticalBox> Content = SNew(SVerticalBox);

	// Title header
	Content->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("SwitcherTitle", "UI TEST SWITCHER"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("SwitcherSubtitle", "[DEV ONLY]"))
				.ColorAndOpacity(OLCStyleColors::DangerRed)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
			]
		];

	// Separator
	Content->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 8.0f, 0.0f, 8.0f)
		[
			SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 2.0f))
		];

	// Instructions
	Content->AddSlot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("SwitcherInstructions", "Press F1-F10 to open screens. Press Esc to return here."))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
		];

	// Screen buttons grid (2 columns)
	TSharedRef<SHorizontalBox> TopRow = SNew(SHorizontalBox);
	TSharedRef<SHorizontalBox> BottomRow = SNew(SHorizontalBox);

	// F1-F5 in top row
	for (int32 i = 0; i < 5; i++)
	{
		const FText Labels[5] = {
			LOCTEXT("Screen_F1", "Main RTS HUD"),
			LOCTEXT("Screen_F2", "Construction Mode"),
			LOCTEXT("Screen_F3", "Colony Resource Network"),
			LOCTEXT("Screen_F4", "Solar System"),
			LOCTEXT("Screen_F5", "Galaxy Map"),
		};
		const FText Hotkeys[5] = {
			FText::FromString(TEXT("[F1]")),
			FText::FromString(TEXT("[F2]")),
			FText::FromString(TEXT("[F3]")),
			FText::FromString(TEXT("[F4]")),
			FText::FromString(TEXT("[F5]")),
		};

		TopRow->AddSlot()
			.FillWidth(1.0f)
			.Padding(6.0f, 0.0f)
			[
				BuildScreenButton(Labels[i], Hotkeys[i], i + 1)
			];
	}

	// F6-F10 in bottom row
	for (int32 i = 0; i < 5; i++)
	{
		const FText Labels[5] = {
			LOCTEXT("Screen_F6", "Tactical Dungeon"),
			LOCTEXT("Screen_F7", "Research"),
			LOCTEXT("Screen_F8", "Dropship Repair"),
			LOCTEXT("Screen_F9", "Mothership Builder"),
			LOCTEXT("Screen_F10", "Equipment"),
		};
		const FText Hotkeys[5] = {
			FText::FromString(TEXT("[F6]")),
			FText::FromString(TEXT("[F7]")),
			FText::FromString(TEXT("[F8]")),
			FText::FromString(TEXT("[F9]")),
			FText::FromString(TEXT("[F10]")),
		};

		BottomRow->AddSlot()
			.FillWidth(1.0f)
			.Padding(6.0f, 0.0f)
			[
				BuildScreenButton(Labels[i], Hotkeys[i], i + 6)
			];
	}

	Content->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 12.0f, 0.0f, 8.0f)
		[ TopRow ];

	Content->AddSlot()
		.AutoHeight()
		[ BottomRow ];

	return SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		[
			SNew(SOverlay)
			// Background
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.06f, 0.07f, 0.09f, 0.95f))
			]
			// Centered content box
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				.WidthOverride(700.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::DarkSteel)
					.Padding(FMargin(24.0f))
					[
						SNew(SOverlay)
						// Top accent
						+ SOverlay::Slot()
						.VAlign(VAlign_Top)
						.HAlign(HAlign_Fill)
						[
							SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 3.0f))
						]
						// Content
						+ SOverlay::Slot()
						.Padding(FMargin(0.0f, 16.0f, 0.0f, 0.0f))
						[ Content ]
					]
				]
			]
		];
}

TSharedRef<SWidget> UOLCTestSwitcherWidget::BuildScreenButton(const FText& Label, const FText& Hotkey, int32 Index)
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Index]() { return OnScreenSelect(Index); })
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
			.Padding(FMargin(12.0f, 10.0f))
			[
				SNew(SOverlay)
				// Left accent bar
				+ SOverlay::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Left)
				[
					SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(3.0f, 1.0f))
				]
				// Button content
				+ SOverlay::Slot()
				.Padding(10.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(Hotkey)
						.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(Label)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
					]
				]
			]
		];
}

FReply UOLCTestSwitcherWidget::OnScreenSelect(int32 ScreenIndex)
{
	UE_LOG(LogTemp, Display, TEXT("[OLC] TestSwitcher: Screen %d selected"), ScreenIndex);

	// Forward to the player controller so it opens the right screen.
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PC))
		{
			// ScreenIndex 1-10 maps to F1-F10 enum values.
			EOLCUIScreen TargetScreen = EOLCUIScreen::None;
			switch (ScreenIndex)
			{
				case 1: TargetScreen = EOLCUIScreen::MainRTSHUD; break;
				case 2: TargetScreen = EOLCUIScreen::ConstructionMode; break;
				case 3: TargetScreen = EOLCUIScreen::ColonyResourceNetwork; break;
				case 4: TargetScreen = EOLCUIScreen::SolarSystem; break;
				case 5: TargetScreen = EOLCUIScreen::GalaxyMap; break;
				case 6: TargetScreen = EOLCUIScreen::TacticalDungeon; break;
				case 7: TargetScreen = EOLCUIScreen::Research; break;
				case 8: TargetScreen = EOLCUIScreen::DropshipRepair; break;
				case 9: TargetScreen = EOLCUIScreen::MothershipBuilder; break;
				case 10: TargetScreen = EOLCUIScreen::Equipment; break;
				default: return FReply::Unhandled();
			}
			MenuPC->OpenUIScreen(TargetScreen);
		}
	}

	return FReply::Handled();
}

void UOLCTestSwitcherWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

// ===================================================================
// WBP_UI_Keymap — Player-facing shortcut reference overlay
// ===================================================================
UOLCKeymapWidget::UOLCKeymapWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCKeymapWidget::RebuildWidget()
{
	TSharedRef<SVerticalBox> Rows = SNew(SVerticalBox);
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_B", "B"), LOCTEXT("Keymap_Build", "Build"), LOCTEXT("Keymap_BuildDetail", "Open construction mode")) ];
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_R", "R"), LOCTEXT("Keymap_Research", "Research"), LOCTEXT("Keymap_ResearchDetail", "Open research tree")) ];
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_M", "M"), LOCTEXT("Keymap_Mothership", "Mothership Builder"), LOCTEXT("Keymap_MothershipDetail", "Place and upgrade ship modules")) ];
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_F1", "F1"), LOCTEXT("Keymap_Keymap", "Keymap"), LOCTEXT("Keymap_KeymapDetail", "Show this shortcut menu")) ];
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_Esc", "Esc"), LOCTEXT("Keymap_Close", "Close"), LOCTEXT("Keymap_CloseDetail", "Return to the UI switcher")) ];
	Rows->AddSlot().AutoHeight()[ BuildKeyRow(LOCTEXT("Keymap_Wheel", "Mouse Wheel"), LOCTEXT("Keymap_Zoom", "Zoom"), LOCTEXT("Keymap_ZoomDetail", "Adjust tactical camera distance")) ];

	return SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::OverlayBG)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SBox)
				.WidthOverride(560.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::DarkSteel)
					.Padding(FMargin(24.0f))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.VAlign(VAlign_Top)
						.HAlign(HAlign_Fill)
						[
							SNew(SColorBlock).Color(OLCStyleColors::TacticalBlue).Size(FVector2D(1.0f, 3.0f))
						]
						+ SOverlay::Slot()
						.Padding(FMargin(0.0f, 18.0f, 0.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("KeymapTitle", "KEYMAP"))
								.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 24))
							]
							+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 14.0f)
							[
								SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 1.0f))
							]
							+ SVerticalBox::Slot().AutoHeight()
							[
								Rows
							]
						]
					]
				]
			]
		];
}

TSharedRef<SWidget> UOLCKeymapWidget::BuildKeyRow(const FText& KeyLabel, const FText& ActionLabel, const FText& DetailLabel)
{
	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(104.0f)
				[
					SNew(STextBlock)
					.Text(KeyLabel)
					.ColorAndOpacity(OLCStyleColors::TacticalBlue)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
				]
			]
			+ SHorizontalBox::Slot().FillWidth(0.9f)
			[
				SNew(STextBlock)
				.Text(ActionLabel)
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
			]
			+ SHorizontalBox::Slot().FillWidth(1.25f)
			[
				SNew(STextBlock)
				.Text(DetailLabel)
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
			]
		];
}

#undef LOCTEXT_NAMESPACE
