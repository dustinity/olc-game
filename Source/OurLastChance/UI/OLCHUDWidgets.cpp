#include "OLCHUDWidgets.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#include "Player/OLCMenuPlayerController.h"
#include "UI/OLCSharedWidgets.h" // OLCStyleColors
#include "Core/OLCUIDataSubsystem.h"
#include "Core/OLCResearchSubsystem.h"
#include "World/OLCPlanetTerrainActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OLCToastWidget.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "OLCHUDWidgets"

// ---------------------------------------------------------------------------
// Shared layout helpers
// ---------------------------------------------------------------------------
namespace HUDLayout
{
	constexpr float DesignW = 1920.0f;
	constexpr float DesignH = 1080.0f;

	// Panel backgrounds
	constexpr FLinearColor PanelBG       = OLCStyleColors::DarkSteel;
	constexpr FLinearColor DimOverlay    = FLinearColor(0.0f, 0.0f, 0.0f, 0.04f);

	FLinearColor GetPressureColor(EOLCStoragePressure P)
	{
		switch (P) {
			case EOLCStoragePressure::Approaching: return OLCStyleColors::WarningYellow;
			case EOLCStoragePressure::Full:
			case EOLCStoragePressure::Overflow:  return OLCStyleColors::DangerRed;
			default:                              return OLCStyleColors::TextWhite;
		}
	}

	/** Main HUD asset path — relative to project dir via ../../UE5/Assets/UI/Main HUD Elements/Assets/Sliced */
	FString MainHUDAssetPath(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Main HUD Elements/Assets/Sliced") / FileName);
	}

	/** Shared asset path — relative to project dir via ../../UE5/Assets/UI/Shared Assets/Sliced */
	FString SharedAssetPath(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Shared Assets/Sliced") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

// ===================================================================
// WBP_HUD_Root — Main RTS HUD
// ===================================================================
UOLCMainRTSHUDWidget::UOLCMainRTSHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCMainRTSHUDWidget::RebuildWidget()
{
	TSharedRef<SOverlay> Root = SNew(SOverlay)
	// Dim background overlay (so HUD is visible over gameplay)
	+ SOverlay::Slot()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.08f))
	]
	// ---- TOP: Resource Strip ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Fill)
	.Padding(18.0f, 14.0f, 18.0f, 0.0f)
	[ BuildResourceStrip() ]
	// ---- UPPER-LEFT: Mission Progress ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	.Padding(18.0f, 92.0f, 0.0f, 0.0f)
	[
		SNew(SBox)
		.WidthOverride(330.0f)
		[ BuildMissionProgress() ]
	]
	// ---- UPPER-LEFT (below mission): Research Progress ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	.Padding(18.0f, 342.0f, 0.0f, 0.0f)
	[
		SNew(SBox)
		.WidthOverride(330.0f)
		[ BuildResearchProgress() ]
	]
	// ---- UPPER-LEFT (below research): Biome/Hazard badges ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	.Padding(18.0f, 452.0f, 0.0f, 0.0f)
	[ BuildBiomeHazardsStrip() ]
	// ---- LOWER-LEFT: Selected Entity Panel ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Bottom)
	.HAlign(HAlign_Left)
	.Padding(18.0f, 0.0f, 0.0f, 18.0f)
	[
		SNew(SBox)
		.WidthOverride(310.0f)
		[ BuildSelectedEntityPanel() ]
	]
	// ---- LOWER-CENTER: Quick Build Tray ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Bottom)
	.HAlign(HAlign_Center)
	.Padding(0.0f, 0.0f, 0.0f, 18.0f)
	[ BuildQuickBuildTray() ]
	// ---- LOWER-RIGHT: Minimap ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Bottom)
	.HAlign(HAlign_Right)
	.Padding(0.0f, 0.0f, 18.0f, 18.0f)
	[ BuildMinimap() ]
	// ---- LOWER-RIGHT (above minimap): Ship Status Indicator ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Bottom)
	.HAlign(HAlign_Right)
	.Padding(0.0f, 140.0f, 18.0f, 200.0f)
	[ BuildShipStatusIndicator() ]
	// ---- UPPER-RIGHT: Time/Speed Control ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Right)
	.Padding(0.0f, 92.0f, 18.0f, 0.0f)
	[ BuildTimeSpeedControl() ]
	// ---- UPPER-RIGHT (below time control): Toast Notifications ----
	+ SOverlay::Slot()
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Right)
	.Padding(0.0f, 210.0f, 18.0f, 0.0f)
	[ BuildToastStack() ];

	return Root;
}

// ---------------------------------------------------------------------------
// Resource Strip (top bar) — now uses shared resource frame images
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildResourceStrip()
{
	TArray<FOLCResourceCounterViewData> Resources;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Resources = Data->GetResourceCounters();
		}
	}

	TSharedRef<SHorizontalBox> Strip = SNew(SHorizontalBox);

	for (const auto& Res : Resources)
	{
		const FOLCResourceCounterViewData LocalCopy = Res; // Copy to avoid dangling ref in lambda

		// WP-107 Step 1: Capacity bar color coding per spec
		// <30% = red (#EF4444), 30-70% = yellow (#F59E0B), >70% = green (#22C55E)
		const float Ratio = LocalCopy.Capacity > 0.0f
			? (LocalCopy.Value / LocalCopy.Capacity)
			: 0.0f;
		FLinearColor BarColor;
		if (Ratio < 0.30f)
			BarColor = FLinearColor(0.937f, 0.267f, 0.267f, 1.0f); // #EF4444 red
		else if (Ratio <= 0.70f)
			BarColor = FLinearColor(0.961f, 0.620f, 0.153f, 1.0f); // #F59E0B yellow
		else
			BarColor = FLinearColor(0.133f, 0.773f, 0.369f, 1.0f); // #22C55E green

		Strip->AddSlot()
			.FillWidth(1.0f)
			.Padding(2.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.02f, 0.035f, 0.04f, 0.94f))
				.Padding(FMargin(10.0f, 6.0f))
				[
					SNew(SVerticalBox)
					// Resource name label
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(LocalCopy.DisplayName)
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					]
					// Value + capacity bar + delta rate
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
					[
						SNew(SHorizontalBox)
						// Current / Capacity value
						+ SHorizontalBox::Slot().FillWidth(1.0f)
						[
							SNew(STextBlock)
							.Text(FText::Format(
								FText::FromString(TEXT("{0} / {1}")),
								FText::AsNumber(FMath::RoundToInt(LocalCopy.Value)),
								FText::AsNumber(FMath::RoundToInt(LocalCopy.Capacity))))
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
						]
						// Delta rate: "+X/s" in green, "-X/s" in red
						+ SHorizontalBox::Slot().AutoWidth().Padding(4.0f, 0.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(FMath::IsNearlyZero(LocalCopy.Delta)
								? FText::FromString(TEXT(""))
								: FText::Format(FText::FromString(TEXT("{0}{1}/s")),
									FText::FromString(LocalCopy.Delta > 0.0f ? TEXT("+") : TEXT("-")),
									FText::AsNumber(FMath::Abs(LocalCopy.Delta))))
							.ColorAndOpacity(LocalCopy.Delta >= 0.0f ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						]
					]
					// Capacity bar (WP-107 Step 1)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.06f, 0.08f, 0.10f, 1.0f))
						.Padding(FMargin(1.0f))
						[
							SNew(SBox)
							.HeightOverride(3.0f)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								[
									SNew(SBorder)
									.BorderBackgroundColor(BarColor)
									.Padding(FMargin(0.0f))
									[
										SNew(SBox)
										.WidthOverride(FMath::Clamp(Ratio * 120.0f, 0.0f, 120.0f))
										.HeightOverride(3.0f)
									]
								]
							]
						]
					]
				]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.005f, 0.012f, 0.016f, 0.96f))
		.Padding(FMargin(10.0f, 6.0f))
		[ Strip ];
}

// ---------------------------------------------------------------------------
// Mission Progress (upper-left) — state-aware styling: active=blue, complete=green check, idle=gray
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildMissionProgress()
{
	TArray<FOLCMissionObjectiveViewData> Objectives;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Objectives = Data->GetMissionObjectives();
		}
	}

	TSharedRef<SVerticalBox> Panel = SNew(SVerticalBox);

	// Title
	Panel->AddSlot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("MissionTitle", "MISSION OBJECTIVES"))
			.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		];

	// Separator
	Panel->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 4.0f, 0.0f, 6.0f)
		[
			SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 1.0f))
		];

	for (const auto& Obj : Objectives)
	{
		const FOLCMissionObjectiveViewData LocalObj = Obj; // Copy to avoid dangling ref in lambda

		// WP-107 Step 3: State-aware styling
		FLinearColor StateColor, BorderAccentColor, ProgressColor;
		TSharedPtr<SWidget> StateIcon;

		switch (LocalObj.State)
		{
			case EOLCProgressState::Active:
				StateColor = OLCStyleColors::TextWhite;
				BorderAccentColor = OLCStyleColors::TacticalBlue; // blue accent border for active
				ProgressColor = OLCStyleColors::TacticalBlue;
				break;
			case EOLCProgressState::Complete:
				StateColor = OLCStyleColors::ValidGreen;
				BorderAccentColor = OLCStyleColors::ValidGreen; // green for completed
				ProgressColor = OLCStyleColors::ValidGreen;
				break;
			case EOLCProgressState::Failed:
				StateColor = OLCStyleColors::DangerRed;
				BorderAccentColor = OLCStyleColors::DangerRed;
				ProgressColor = OLCStyleColors::DangerRed;
				break;
			default: // Idle
				StateColor = OLCStyleColors::TextDim; // grayed out for idle
				BorderAccentColor = OLCStyleColors::BorderGray;
				ProgressColor = OLCStyleColors::BorderGray;
				break;
		}

		float ProgressRatio = FMath::Clamp(
			LocalObj.Progress / FMath::Max(LocalObj.TargetProgress, 1.0f), 0.0f, 1.0f);
		float ProgressWidth = ProgressRatio * 280.0f;

		// Build state icon (checkmark for complete, dot for active, dash for idle)
		auto MakeStateIcon = [this, LocalObj]() -> TSharedRef<SWidget> {
			switch (LocalObj.State)
			{
				case EOLCProgressState::Complete:
					// Green checkmark — green square with inner dark square
					return SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::ValidGreen)
						.Padding(FMargin(2.0f))
						[
							SNew(SBox)
							.WidthOverride(10.0f)
							.HeightOverride(10.0f)
						];
				case EOLCProgressState::Active:
					// Blue dot — small filled circle
					return SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::TacticalBlue)
						.Padding(FMargin(2.0f))
						[
							SNew(SBox)
							.WidthOverride(6.0f)
							.HeightOverride(6.0f)
						];
				case EOLCProgressState::Failed:
					// Red X indicator
					return SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::DangerRed)
						.Padding(FMargin(2.0f))
						[
							SNew(SBox)
							.WidthOverride(10.0f)
							.HeightOverride(2.0f)
						];
				default: // Idle — gray dash
					return SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::BorderGray)
						.Padding(FMargin(3.0f, 2.0f))
						[
							SNew(SBox)
							.WidthOverride(8.0f)
							.HeightOverride(2.0f)
						];
			}
		};

		Panel->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 6.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.02f, 0.035f, 0.04f, 0.94f))
				.Padding(FMargin(8.0f, 6.0f))
				[
					SNew(SVerticalBox)
					// Top accent line (color changes with state)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SColorBlock).Color(BorderAccentColor).Size(FVector2D(1.0f, 2.0f))
					]
					// Objective name + progress count
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
					[
						SNew(SHorizontalBox)
						// State icon
						+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 0.0f)
						[ MakeStateIcon() ]
						// Objective name
						+ SHorizontalBox::Slot().FillWidth(1.0f)
						[
							SNew(STextBlock)
							.Text(LocalObj.ObjectiveName)
							.ColorAndOpacity(StateColor)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
						// Progress count
						+ SHorizontalBox::Slot().AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::Format(
								FText::FromString(TEXT("{0}/{1}")),
								FText::AsNumber(FMath::RoundToInt(LocalObj.Progress)),
								FText::AsNumber(FMath::RoundToInt(LocalObj.TargetProgress))))
							.ColorAndOpacity(StateColor)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						]
					]
					// Progress bar
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
						.Padding(FMargin(1.0f))
						[
							SNew(SBox)
							.HeightOverride(4.0f)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								[
									SNew(SBorder)
									.BorderBackgroundColor(ProgressColor)
									.Padding(FMargin(0.0f))
									[
										SNew(SBox)
										.WidthOverride(ProgressWidth)
										.HeightOverride(4.0f)
									]
								]
							]
						]
					]
				]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::DarkSteel)
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SOverlay)
			// Top accent
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 2.0f))
			]
			// Content
			+ SOverlay::Slot()
			.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
			[ Panel ]
		];
}

// ---------------------------------------------------------------------------
// Research Progress (upper-left, below mission objectives)
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildResearchProgress()
{
	UOLCResearchSubsystem* Research = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		Research = GI->GetSubsystem<UOLCResearchSubsystem>();
	}

	if (!Research || !Research->GetCurrentResearch())
	{
		// No active research — show compact "NO RESEARCH" panel.
		return SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::DarkSteel)
			.Padding(FMargin(12.0f, 8.0f))
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Fill)
				[
					SNew(SColorBlock).Color(OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 2.0f))
				]
				+ SOverlay::Slot()
				.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ResearchIdle", "NO ACTIVE RESEARCH"))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
			];
	}

	UOLCTechData* Active = Research->GetCurrentResearch();
	float Progress = Research->GetResearchProgress();
	float Elapsed = Research->GetCurrentProgressSeconds();
	float Total = Research->GetTotalResearchTime();

	int32 ElapsedSec = FMath::RoundToInt(Elapsed);
	int32 RemainingSec = FMath::RoundToInt(Total - Elapsed);
	int32 RemMin = RemainingSec / 60;
	int32 RemSec = RemainingSec % 60;

	const float BarWidth = Progress * 290.0f;
	FLinearColor StateColor = OLCStyleColors::TacticalBlue;

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::DarkSteel)
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(StateColor).Size(FVector2D(1.0f, 2.0f))
			]
			+ SOverlay::Slot()
			.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
			[
				SNew(SVerticalBox)
				// Title
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ResearchTitle", "RESEARCHING"))
					.ColorAndOpacity(StateColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
				// Tech name
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(Active->DisplayName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
				// Progress bar
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
					.Padding(FMargin(1.0f))
					[
						SNew(SBox)
						.HeightOverride(4.0f)
						[
							SNew(SBorder)
							.BorderBackgroundColor(StateColor)
							.Padding(FMargin(0.0f))
							[
								SNew(SBox)
								.WidthOverride(BarWidth)
								.HeightOverride(4.0f)
							]
						]
					]
				]
				// Time info
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::Format(
						FText::FromString(TEXT("{0} — {1}:{2} remaining")),
						FText::AsNumber(FMath::RoundToInt(Progress * 100.0f)),
						FText::AsNumber(RemMin),
						FText::FromString(FString::Printf(TEXT("%02d"), RemSec))))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Selected Entity Panel (lower-left)
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildSelectedEntityPanel()
{
	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::DarkSteel)
		.Padding(FMargin(14.0f, 10.0f))
		[
			SNew(SOverlay)
			// Top accent
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(OLCStyleColors::TacticalBlue).Size(FVector2D(1.0f, 2.0f))
			]
			// Content
			+ SOverlay::Slot()
			.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("SelectedTitle", "SELECTED ENTITY"))
					.ColorAndOpacity(OLCStyleColors::TacticalBlue)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Sel_Name", "NAME")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(LOCTEXT("Sel_NameVal", "Command Center")).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11)) ]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Sel_Status", "STATUS")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(LOCTEXT("Sel_StatusVal", "OPERATIONAL")).ColorAndOpacity(OLCStyleColors::ValidGreen).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11)) ]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Sel_Power", "POWER")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("+50/s"))).ColorAndOpacity(OLCStyleColors::ValidGreen).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11)) ]
				]
				// Health bar placeholder
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
					.Padding(FMargin(2.0f))
					[
						SNew(SBox)
						.HeightOverride(4.0f)
						[
							SNew(SBorder)
							.BorderBackgroundColor(OLCStyleColors::ValidGreen)
							.Padding(FMargin(0.0f))
							[
								SNew(SBox)
								.WidthOverride(160.0f)
								.HeightOverride(4.0f)
							]
						]
					]
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Quick Build Tray (lower-center)
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildQuickBuildTray()
{
	TArray<FText> SlotLabels = {
		LOCTEXT("QB1", "QUICK POWER"),
		LOCTEXT("QB2", "QUICK DEFENSE"),
		LOCTEXT("QB3", "QUICK STORAGE"),
		LOCTEXT("QB4", "QUICK SCAN"),
	};

	TSharedRef<SHorizontalBox> Tray = SNew(SHorizontalBox);

	for (int32 i = 0; i < SlotLabels.Num(); i++)
	{
		const FText Label = SlotLabels[i];
		Tray->AddSlot()
			.AutoWidth()
			.Padding(3.0f, 0.0f)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.ContentPadding(FMargin(0.0f))
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::CharcoalGray)
					.Padding(FMargin(12.0f, 8.0f))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.VAlign(VAlign_Top)
						.HAlign(HAlign_Fill)
						[
							SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 2.0f))
						]
						+ SOverlay::Slot()
						.Padding(FMargin(0.0f, 6.0f, 0.0f, 0.0f))
						[
							SNew(STextBlock)
							.Text(Label)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
					]
				]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
		.Padding(FMargin(8.0f, 6.0f))
		[ Tray ];
}

// ---------------------------------------------------------------------------
// Minimap (lower-right) — terrain grid + colored markers for buildings, resources, dungeons
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildMinimap()
{
	const float MapWidth = 190.0f;
	const float MapHeight = 130.0f;

	TArray<FOLCMinimapMarkerViewData> Markers;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Markers = Data->GetMinimapMarkers();
		}
	}

	// WP-107 Step 2: Get terrain tile data for minimap rendering
	TArray<FOLCTerrainTile> TerrainTiles;
	int32 MapWidthTiles = 64, MapHeightTiles = 64; // default fallback
	AOLCPlanetTerrainActor* Terrain = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		Terrain = Cast<AOLCPlanetTerrainActor>(UGameplayStatics::GetActorOfClass(GI, AOLCPlanetTerrainActor::StaticClass()));
		if (Terrain)
		{
			TerrainTiles = Terrain->GetTiles();
			FIntPoint MapDims = Terrain->GetMapDimensions();
			MapWidthTiles = MapDims.X;
			MapHeightTiles = MapDims.Y;
		}
	}

	// Build a tile color map: each tile gets a base color based on its role
	TMap<FIntPoint, FLinearColor> TileColors;
	for (const auto& Tile : TerrainTiles)
	{
		FLinearColor TileColor;
		switch (Tile.Role)
		{
			case EOLCTerrainTileRole::Buildable:
				TileColor = FLinearColor(0.15f, 0.17f, 0.20f, 1.0f); // dark gray — buildable tiles
				break;
			case EOLCTerrainTileRole::Restricted:
				TileColor = FLinearColor(0.80f, 0.70f, 0.10f, 0.6f); // yellow outline for restricted zones
				break;
			case EOLCTerrainTileRole::Blocked:
				TileColor = FLinearColor(0.10f, 0.10f, 0.12f, 1.0f); // very dark — blocked
				break;
			case EOLCTerrainTileRole::Water:
				TileColor = FLinearColor(0.05f, 0.30f, 0.60f, 0.7f); // blue — water
				break;
			case EOLCTerrainTileRole::Resource:
				TileColor = FLinearColor(0.0f, 0.85f, 0.90f, 0.8f); // cyan — resource tiles
				break;
			case EOLCTerrainTileRole::Dungeon:
				TileColor = FLinearColor(0.55f, 0.10f, 0.90f, 0.8f); // purple — dungeon tiles
				break;
			case EOLCTerrainTileRole::Landing:
				TileColor = FLinearColor(0.95f, 0.95f, 0.95f, 1.0f); // white — landing zone
				break;
			default:
				TileColor = FLinearColor(0.12f, 0.14f, 0.16f, 1.0f); // default dark gray
				break;
		}
		TileColors.Add(Tile.Coord, TileColor);
	}

	// Calculate tile size in minimap pixels
	const float TilePixelW = MapWidth / static_cast<float>(MapWidthTiles);
	const float TilePixelH = MapHeight / static_cast<float>(MapHeightTiles);
	const float MinTileSize = 1.5f; // minimum visible tile size for readability

	TSharedRef<SOverlay> MinimapContent = SNew(SOverlay);

	// Layer 1: Terrain tile grid (only render if tiles are large enough to see)
	if (TilePixelW >= MinTileSize && TilePixelH >= MinTileSize && !TerrainTiles.IsEmpty())
	{
		for (const auto& TileEntry : TileColors)
		{
			const FIntPoint& Coord = TileEntry.Key;
			const FLinearColor& Color = TileEntry.Value;

			const float ScreenX = Coord.X * TilePixelW;
			const float ScreenY = Coord.Y * TilePixelH;

			MinimapContent->AddSlot()
				.Padding(FMargin(ScreenX, ScreenY, 0.0f, 0.0f))
				[
					SNew(SBox)
					.WidthOverride(FMath::CeilToInt(TilePixelW))
					.HeightOverride(FMath::CeilToInt(TilePixelH))
					[
						SNew(SBorder)
						.BorderBackgroundColor(Color)
						.Padding(FMargin(0.0f))
					]
				];
		}
	}

	// Layer 2: Building markers (green squares from registered buildings)
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			// WP-107 Step 2: Get registered buildings from subsystem and render as green squares
			TArray<FRegisteredBuildingEntry> Buildings;
			// Buildings are tracked via RegisterBuilding — we get their world positions
			// and project them to minimap coordinates
			for (TActorIterator<AActor> It(GetWorld()); It; ++It)
			{
				AActor* Actor = *It;
				if (Actor->GetClass()->GetName().Contains(TEXT("Building")) ||
					Actor->GetClass()->GetName().Contains(TEXT("Infrastructure")) ||
					Actor->GetClass()->GetName().Contains(TEXT("Power")) ||
					Actor->GetClass()->GetName().Contains(TEXT("Extraction")))
				{
					FVector WorldLoc = Actor->GetActorLocation();
					if (Terrain && MapWidthTiles > 0 && MapHeightTiles > 0)
					{
						// Convert world position to tile coordinate
						FIntPoint TileCoord;
						TileCoord.X = FMath::Clamp(WorldLoc.X / Terrain->GetTileWorldPosition(FIntPoint(1, 0)).X, 0, MapWidthTiles - 1);
						TileCoord.Y = FMath::Clamp(WorldLoc.Z / Terrain->GetTileWorldPosition(FIntPoint(0, 1)).Z, 0, MapHeightTiles - 1);

						const float MarkerX = (static_cast<float>(TileCoord.X) / static_cast<float>(MapWidthTiles)) * MapWidth;
						const float MarkerY = (static_cast<float>(TileCoord.Y) / static_cast<float>(MapHeightTiles)) * MapHeight;

						MinimapContent->AddSlot()
							.Padding(FMargin(MarkerX - 3.0f, MarkerY - 3.0f, 0.0f, 0.0f))
							[
								SNew(SBox)
								.WidthOverride(6.0f)
								.HeightOverride(6.0f)
								[
									SNew(SBorder)
									.BorderBackgroundColor(OLCStyleColors::ValidGreen) // green squares for buildings
									.Padding(FMargin(0.5f))
									[
										SNew(SBorder)
										.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
										.Padding(FMargin(0.0f))
										[
											SNew(SBox)
											.WidthOverride(3.0f)
											.HeightOverride(3.0f)
										]
									]
								]
							];
					}
				}
			}
		}
	}

	// Layer 3: Subsystem marker dots (resources, dungeons, enemies, objectives)
	for (const auto& Marker : Markers)
	{
		if (!Marker.bVisible || !Marker.bDiscovered) continue;

		const FVector2D DotPos(Marker.Position.X * MapWidth, Marker.Position.Y * MapHeight);
		const FLinearColor MarkerColor = [&]() {
			switch (Marker.ColorRole) {
				case EOLCColorRole::Primary:   return OLCStyleColors::PrimaryOrange;
				case EOLCColorRole::Secondary: return OLCStyleColors::TacticalBlue;
				case EOLCColorRole::Success:   return OLCStyleColors::ValidGreen;
				case EOLCColorRole::Danger:    return OLCStyleColors::DangerRed;
				case EOLCColorRole::Warning:   return OLCStyleColors::WarningYellow;
				default:                       return OLCStyleColors::TextWhite;
			}
		}();

		const FText Tooltip = Marker.TooltipText.IsEmpty()
			? UEnum::GetDisplayValueAsText(Marker.MarkerType)
			: Marker.TooltipText;

		const FVector2D DotSize(6.0f, 6.0f);
		MinimapContent->AddSlot()
			.Padding(FMargin(DotPos.X - (DotSize.X / 2.0f), DotPos.Y - (DotSize.Y / 2.0f), 0.0f, 0.0f))
			[
				SNew(SBorder)
				.BorderBackgroundColor(MarkerColor)
				.Padding(FMargin(0.0f))
				[
					SNew(SBox)
					.WidthOverride(DotSize.X)
					.HeightOverride(DotSize.Y)
				]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.025f, 0.04f, 0.05f, 0.88f))
		.Padding(FMargin(8.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock).Text(LOCTEXT("MinimapLabel", "MINIMAP")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.08f, 0.10f, 0.12f, 1.0f))
				.Padding(FMargin(0.0f))
				[
					SNew(SBox)
					.WidthOverride(MapWidth)
					.HeightOverride(MapHeight)
					[ MinimapContent ]
				]
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)
				// Legend items
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::ValidGreen)
						.Padding(FMargin(2.0f))
						[ SNew(SBox).WidthOverride(6.0f).HeightOverride(6.0f) ]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock).Text(LOCTEXT("LegBuilding", "Bld")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.0f, 0.85f, 0.90f, 1.0f))
						.Padding(FMargin(2.0f))
						[ SNew(SBox).WidthOverride(6.0f).HeightOverride(6.0f) ]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock).Text(LOCTEXT("LegResource", "Res")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 8.0f, 0.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.55f, 0.10f, 0.90f, 1.0f))
						.Padding(FMargin(2.0f))
						[ SNew(SBox).WidthOverride(6.0f).HeightOverride(6.0f) ]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock).Text(LOCTEXT("LegDungeon", "Dgn")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
					]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::DangerRed)
						.Padding(FMargin(2.0f))
						[ SNew(SBox).WidthOverride(6.0f).HeightOverride(6.0f) ]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock).Text(LOCTEXT("LegEnemy", "Enm")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 8))
					]
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Time/Speed Control (upper-right)
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Time/Speed Control (upper-right) — direct set, visual indicator, tick interval
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildTimeSpeedControl()
{
	UOLCUIDataSubsystem* DataSubsystem = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		DataSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();
	}

	const EOLCSimulationSpeed CurrentSpeed = DataSubsystem ? DataSubsystem->GetCurrentSimulationSpeed() : EOLCSimulationSpeed::Paused;

	// WP-107 Step 4: Get production tick interval for display
	float TickInterval = 5.0f; // default 1x
	switch (CurrentSpeed)
	{
		case EOLCSimulationSpeed::Paused: TickInterval = 0.0f; break;
		case EOLCSimulationSpeed::Normal: TickInterval = 5.0f; break;
		case EOLCSimulationSpeed::Fast:   TickInterval = 2.5f; break;
	}

	auto MakeSpeedButton = [this, DataSubsystem](const FText& Label, EOLCSimulationSpeed Speed) {
		const bool bActive = (DataSubsystem && DataSubsystem->GetCurrentSimulationSpeed() == Speed);
		return SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.ContentPadding(FMargin(0.0f))
			.OnClicked_Lambda([DataSubsystem, Speed]() {
				// WP-107 Step 4: Direct set instead of cycle
				if (DataSubsystem)
				{
					DataSubsystem->SetCurrentSimulationSpeed(Speed);

					// Adjust production tick interval per spec
					float NewInterval = 5.0f;
					switch (Speed)
					{
						case EOLCSimulationSpeed::Paused: NewInterval = 0.0f; break; // no ticks
						case EOLCSimulationSpeed::Normal: NewInterval = 5.0f; break; // 1x = 5s
						case EOLCSimulationSpeed::Fast:   NewInterval = 2.5f; break; // 2x = 2.5s
					}
					DataSubsystem->SetProductionTickInterval(NewInterval);

					// Stop or restart tick timer as needed
					if (Speed == EOLCSimulationSpeed::Paused)
					{
						DataSubsystem->StopProductionTick();
					}
					else if (DataSubsystem->GetProductionTickInterval() > 0.0f)
					{
						DataSubsystem->StartProductionTick();
					}
				}
				return FReply::Handled();
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(bActive ? FLinearColor(0.910f, 0.522f, 0.165f, 0.30f) : FLinearColor(0.08f, 0.10f, 0.12f, 0.90f)) // orange tint active, dark gray inactive
				.Padding(FMargin(12.0f, 8.0f))
				[
					SNew(STextBlock)
					.Text(Label)
					.ColorAndOpacity(bActive ? FLinearColor(0.910f, 0.522f, 0.165f, 1.0f) : OLCStyleColors::TextDim) // #E8852A orange active
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", bActive ? 16 : 13))
				]
			];
	};

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.025f, 0.04f, 0.05f, 0.90f))
		.Padding(FMargin(14.0f, 10.0f))
		[
			SNew(SVerticalBox)
			// Visual indicator above controls — larger text showing current speed
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 8.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(
					FText::FromString(TEXT("SPEED: {0}")),
					FText::FromString(CurrentSpeed == EOLCSimulationSpeed::Paused ? TEXT("PAUSED") :
						CurrentSpeed == EOLCSimulationSpeed::Normal ? TEXT("1x (5s/tick)") :
						TEXT("2x (2.5s/tick)"))))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
			]
			// Tick interval info
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(
					FText::FromString(TEXT("{0}s per tick")),
					FText::AsNumber(FMath::RoundToInt(TickInterval * 10.0f) / 10.0f)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			]
			// Three buttons: Pause | 1x | 2x
			+ SVerticalBox::Slot().FillHeight(1.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().FillWidth(1.0f)
				[ MakeSpeedButton(LOCTEXT("SimPaused", "PAUSED"), EOLCSimulationSpeed::Paused) ]
				+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(8.0f, 0.0f, 8.0f, 0.0f)
				[ MakeSpeedButton(LOCTEXT("Sim1x", "1x"), EOLCSimulationSpeed::Normal) ]
				+ SHorizontalBox::Slot().FillWidth(1.0f)
				[ MakeSpeedButton(LOCTEXT("Sim2x", "2x"), EOLCSimulationSpeed::Fast) ]
			]
		];
}

// ---------------------------------------------------------------------------
// Ship Status Indicator (lower-right, above minimap)
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildShipStatusIndicator()
{
	UOLCUIDataSubsystem* DataSubsystem = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		DataSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();
	}

	// Drive status icon and label
	EOLCModuleState DriveStatus = EOLCModuleState::Offline;
	FText DriveLabel = LOCTEXT("Ship_DriveUnknown", "DRIVE ?");
	FLinearColor DriveColor = OLCStyleColors::DangerRed;

	if (DataSubsystem)
	{
		DriveStatus = DataSubsystem->GetDriveStatus();
		switch (DriveStatus)
		{
			case EOLCModuleState::Installed:
				DriveLabel = LOCTEXT("Ship_DriveOK", "DRIVE OK");
				DriveColor = OLCStyleColors::ValidGreen;
				break;
			case EOLCModuleState::Damaged:
				DriveLabel = LOCTEXT("Ship_DriveDamaged", "DRIVE DAMAGED");
				DriveColor = OLCStyleColors::WarningYellow;
				break;
			case EOLCModuleState::Offline:
				DriveLabel = LOCTEXT("Ship_DriveDown", "DRIVE DOWN");
				DriveColor = OLCStyleColors::DangerRed;
				break;
		}
	}

	// Shield status icon and label
	EOLCModuleState ShieldStatus = EOLCModuleState::Offline;
	FText ShieldLabel = LOCTEXT("Ship_ShieldUnknown", "SHIELD ?");
	FLinearColor ShieldColor = OLCStyleColors::DangerRed;

	if (DataSubsystem)
	{
		ShieldStatus = DataSubsystem->GetShieldStatus();
		switch (ShieldStatus)
		{
			case EOLCModuleState::Installed:
				ShieldLabel = LOCTEXT("Ship_ShieldOK", "SHIELD OK");
				ShieldColor = OLCStyleColors::ValidGreen;
				break;
			case EOLCModuleState::Damaged:
				ShieldLabel = LOCTEXT("Ship_ShieldDamaged", "SHIELD DAMAGED");
				ShieldColor = OLCStyleColors::WarningYellow;
				break;
			case EOLCModuleState::Offline:
				ShieldLabel = LOCTEXT("Ship_ShieldDown", "SHIELD DOWN");
				ShieldColor = OLCStyleColors::DangerRed;
				break;
		}
	}

	// Storage bonus display
	int32 StorageBonus = 0;
	if (DataSubsystem)
	{
		StorageBonus = DataSubsystem->GetStorageBonusPerResource();
	}

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(12.0f, 8.0f))
		.OnClicked_Lambda([this]() -> FReply
			{
				OnShipStatusClicked.Broadcast();
				return FReply::Handled();
			})
		[
			SNew(SVerticalBox)
			// Drive status row
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(DriveColor)
					.Padding(FMargin(4.0f))
					[
						SNew(SBox)
						.WidthOverride(10.0f)
						.HeightOverride(10.0f)
					]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
					.Text(DriveLabel)
					.ColorAndOpacity(DriveColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
			]
			// Shield status row
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(ShieldColor)
					.Padding(FMargin(4.0f))
					[
						SNew(SBox)
						.WidthOverride(10.0f)
						.HeightOverride(10.0f)
					]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
					.Text(ShieldLabel)
					.ColorAndOpacity(ShieldColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
			]
			// Storage bonus row
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(StorageBonus > 0 ? OLCStyleColors::ValidGreen : OLCStyleColors::TextDim)
					.Padding(FMargin(4.0f))
					[
						SNew(SBox)
						.WidthOverride(10.0f)
						.HeightOverride(10.0f)
					]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::Format(
						FText::FromString(TEXT("STORAGE +{0}")),
						FText::AsNumber(StorageBonus)))
					.ColorAndOpacity(StorageBonus > 0 ? OLCStyleColors::ValidGreen : OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Biome/Hazards Strip (upper-left, below mission) — uses shared badge images
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildBiomeHazardsStrip()
{
	TArray<FOLCBadgeViewData> Badges;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Badges = Data->GetCurrentPlanetBadges();

			// Append ship state badges (shield/drive) from subsystem.
			const EOLCModuleState ShieldStatus = Data->GetShieldStatus();
			const float ShieldIntegrity = Data->GetShieldIntegrity();
			FText ShieldLabel;
			EOLCColorRole ShieldColor = EOLCColorRole::Default;

			if (ShieldStatus == EOLCModuleState::Installed)
			{
				ShieldLabel = LOCTEXT("Badge_ShieldOK", "SHIELD OK");
				ShieldColor = EOLCColorRole::Success;
			}
			else if (ShieldStatus == EOLCModuleState::Damaged)
			{
				ShieldLabel = FText::Format(
					FText::FromString(TEXT("SHIELD {0}%")),
					FText::AsNumber(FMath::RoundToInt(ShieldIntegrity * 100.0f)));
				ShieldColor = EOLCColorRole::Warning;
			}
			else // Offline
			{
				ShieldLabel = LOCTEXT("Badge_ShieldDown", "SHIELD DOWN");
				ShieldColor = EOLCColorRole::Danger;
			}

			Badges.Emplace(EOLCBadgeType::Status, ShieldLabel, FText(), ShieldColor);

			const EOLCModuleState DriveStatus = Data->GetDriveStatus();
			FText DriveBadgeLabel;
			EOLCColorRole DriveBadgeColor = EOLCColorRole::Default;

			switch (DriveStatus)
			{
				case EOLCModuleState::Installed:
					DriveBadgeLabel = LOCTEXT("Badge_DriveOK", "DRIVE OK");
					DriveBadgeColor = EOLCColorRole::Success;
					break;
				case EOLCModuleState::Damaged:
					DriveBadgeLabel = LOCTEXT("Badge_DriveDamaged", "DRIVE DAMAGED");
					DriveBadgeColor = EOLCColorRole::Warning;
					break;
				case EOLCModuleState::Offline:
					DriveBadgeLabel = LOCTEXT("Badge_DriveDown", "DRIVE DOWN");
					DriveBadgeColor = EOLCColorRole::Danger;
					break;
			}

			Badges.Emplace(EOLCBadgeType::Status, DriveBadgeLabel, FText(), DriveBadgeColor);
		}
	}

	TSharedRef<SHorizontalBox> BadgeRow = SNew(SHorizontalBox);

	for (const auto& Badge : Badges)
	{
		const FOLCBadgeViewData LocalBadge = Badge;
		FLinearColor BadgeColor;
		switch (LocalBadge.ColorRole) {
			case EOLCColorRole::Primary:   BadgeColor = OLCStyleColors::PrimaryOrange; break;
			case EOLCColorRole::Secondary: BadgeColor = OLCStyleColors::TacticalBlue; break;
			case EOLCColorRole::Success:   BadgeColor = OLCStyleColors::ValidGreen; break;
			case EOLCColorRole::Danger:    BadgeColor = OLCStyleColors::DangerRed; break;
			case EOLCColorRole::Warning:   BadgeColor = OLCStyleColors::WarningYellow; break;
			default:                       BadgeColor = OLCStyleColors::TextDim; break;
		}

		BadgeRow->AddSlot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 4.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::DarkSteel)
				.Padding(FMargin(8.0f, 3.0f))
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Left)
					[
						SNew(SColorBlock).Color(BadgeColor).Size(FVector2D(2.0f, 14.0f))
					]
					+ SOverlay::Slot()
					.Padding(5.0f, 0.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LocalBadge.Label)
						.ColorAndOpacity(BadgeColor)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
				]
			];
	}

	return BadgeRow;
}

// ===================================================================
// WBP_ConstructionOverlay — Construction Mode
// ===================================================================
UOLCConstructionOverlayWidget::UOLCConstructionOverlayWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCConstructionOverlayWidget::RefreshConstructionData()
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			ConstructionCategories = Data->GetConstructionCategories();
			BuildCards = Data->GetBuildCards();
		}
	}
}

TSharedRef<SWidget> UOLCConstructionOverlayWidget::RebuildWidget()
{
	RefreshConstructionData();
	BuildCardBrushes.Reset();

	const EOLCConstructionCategory CurrentCat = ConstructionCategories.IsValidIndex(SelectedCategoryIndex)
		? ConstructionCategories[SelectedCategoryIndex]
		: EOLCConstructionCategory::Power;

	TSharedRef<SScrollBox> BuildCardList = SNew(SScrollBox);
	int32 CatCardIdx = 0;
	for (const auto& Card : BuildCards)
	{
		if (Card.Category == CurrentCat)
		{
			const bool bSelected = (CatCardIdx == SelectedCardIndex);
			BuildCardList->AddSlot()
				.Padding(0.0f, 0.0f, 0.0f, 10.0f)
				[ BuildCardFor(Card, bSelected) ];
			CatCardIdx++;
		}
	}

	return SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.22f))
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			.Padding(12.0f, 8.0f, 12.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
				.Padding(FMargin(16.0f, 10.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 34.0f, 0.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("ENERGY   742   +15 / h"))).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 34.0f, 0.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("FUEL   380   -5 / h"))).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 34.0f, 0.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("CONSTR. MAT.   210   +8 / h"))).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 34.0f, 0.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("MINERALS   450   +3 / h"))).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(LOCTEXT("ConPlanet", "ARID WASTES   STORM ACTIVITY   SURVIVAL 78%")).ColorAndOpacity(OLCStyleColors::PrimaryOrange).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
				]
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			.Padding(14.0f, 72.0f, 0.0f, 0.0f)
			[
				SNew(SBox)
				.WidthOverride(420.0f)
				.HeightOverride(880.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
					.Padding(FMargin(16.0f))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.VAlign(VAlign_Top)
						.HAlign(HAlign_Fill)
						[
							SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 3.0f))
						]
						+ SOverlay::Slot()
						.Padding(FMargin(0.0f, 12.0f, 0.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("ConstructionTitle", "CONSTRUCTION"))
								.ColorAndOpacity(OLCStyleColors::TextWhite)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 26))
							]
							+ SVerticalBox::Slot().AutoHeight()
							[ BuildCategoryList() ]
							+ SVerticalBox::Slot().FillHeight(1.0f).Padding(0.0f, 10.0f, 0.0f, 0.0f)
							[ BuildCardList ]
						]
					]
				]
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Right)
			.Padding(0.0f, 88.0f, 22.0f, 0.0f)
			[ BuildBuildDetailPanel() ]
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 60.0f, 0.0f, 0.0f)
			[ BuildPlacementPreview() ]
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 0.0f, 0.0f, 36.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.ContentPadding(FMargin(0.0f))
					.OnClicked_Lambda([this]() { return OnRotateClicked(); })
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
						.Padding(FMargin(34.0f, 14.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("R   ROTATE")))
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
						]
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(26.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.ContentPadding(FMargin(0.0f))
					.OnClicked_Lambda([this]() { return OnConfirmClicked(); })
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
						.Padding(FMargin(34.0f, 14.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("LMB   BUILD")))
							.ColorAndOpacity(OLCStyleColors::ValidGreen)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
						]
					]
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(26.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.ContentPadding(FMargin(0.0f))
					.OnClicked_Lambda([this]() { return OnCancelClicked(); })
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
						.Padding(FMargin(34.0f, 14.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("ESC   ABORT")))
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
						]
					]
				]
			];
}

TSharedRef<SWidget> UOLCConstructionOverlayWidget::BuildCategoryList()
{
	// Category label lookup — mirrors EOLCConstructionCategory enum order.
	static const TArray<FText> AllCategoryLabels = {
		LOCTEXT("Cat_Power", "POWER"),
		LOCTEXT("Cat_Extraction", "EXTRACTION"),
		LOCTEXT("Cat_Infrastructure", "INFRASTRUCTURE"),
		LOCTEXT("Cat_Storage", "STORAGE"),
		LOCTEXT("Cat_Production", "PRODUCTION"),
		LOCTEXT("Cat_Defense", "DEFENSE"),
		LOCTEXT("Cat_Support", "SUPPORT"),
		LOCTEXT("Cat_HighTier", "HIGH Tier"),
		LOCTEXT("Cat_Special", "SPECIAL"),
	};

	TSharedRef<SVerticalBox> CatList = SNew(SVerticalBox);

	for (int32 i = 0; i < ConstructionCategories.Num(); i++)
	{
		const EOLCConstructionCategory Cat = ConstructionCategories[i];
		const int32 GlobalIndex = static_cast<int32>(Cat);
		const FText Label = (GlobalIndex >= 0 && GlobalIndex < AllCategoryLabels.Num())
			? AllCategoryLabels[GlobalIndex]
			: FText::FromString(TEXT("UNKNOWN"));
		const int32 Index = i;
		bool IsActive = (i == SelectedCategoryIndex);

		// Use BLD-CAT-* category icon images from sliced assets
		const TCHAR* CategoryAssetName =
			i == 0 ? TEXT("Power") : i == 1 ? TEXT("Extraction") : i == 2 ? TEXT("Infrastructure") :
			i == 3 ? TEXT("Storage") : i == 4 ? TEXT("Production") : i == 5 ? TEXT("Defense") :
			i == 6 ? TEXT("Support") : TEXT("Unknown");
		FString IconFileName = FString::Printf(TEXT("BLD-CAT-%02d_%sCategory.png"), i + 1, CategoryAssetName);

		FString IconPath = HUDLayout::MainHUDAssetPath(IconFileName);
		BuildCardBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*IconPath), FVector2D(30.0f, 30.0f)));
		const FSlateBrush* IconBrush = BuildCardBrushes.Last().Get();

		CatList->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 5.0f)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.ContentPadding(FMargin(0.0f))
				.OnClicked_Lambda([this, Index]() { OnCategorySelected(Index); return FReply::Handled(); })
				[
					SNew(SBorder)
					.BorderBackgroundColor(IsActive ? FLinearColor(0.12f, 0.095f, 0.035f, 0.96f) : FLinearColor(0.025f, 0.04f, 0.05f, 0.92f))
					.Padding(FMargin(10.0f, 8.0f))
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Left)
						[
							SNew(SColorBlock).Color(IsActive ? OLCStyleColors::PrimaryOrange : FLinearColor::Transparent).Size(FVector2D(2.0f, 14.0f))
						]
						+ SOverlay::Slot()
						.Padding(8.0f, 0.0f, 0.0f, 0.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
							[
								SNew(SImage)
								.Image(IconBrush)
								.ColorAndOpacity(IsActive ? FLinearColor::White : FLinearColor(0.58f, 0.66f, 0.68f, 0.78f))
							]
							+ SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center).Padding(10.0f, 0.0f, 0.0f, 0.0f)
							[
								SNew(STextBlock)
								.Text(Label)
								.ColorAndOpacity(IsActive ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextDim)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
							]
						]
					]
				]
			];
	}

	return CatList;
}

TSharedRef<SWidget> UOLCConstructionOverlayWidget::BuildCardFor(const FOLCBuildCardViewData& Card, bool bSelected)
{
	const FOLCBuildCardViewData LocalCard = Card;

	// Use BLD-CRD-* card frame images from sliced assets
	FString FrameFileName = bSelected ? TEXT("BLD-CRD-02_BuildCardSelectedFrame.png") : TEXT("BLD-CRD-01_BuildCardNormalFrame.png");
	FString FramePath = HUDLayout::MainHUDAssetPath(FrameFileName);
	BuildCardBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*FramePath), FVector2D(356.0f, 132.0f)));
	const FSlateBrush* FrameBrush = BuildCardBrushes.Last().Get();

	FText CostText = FText::FromString(TEXT("Free"));
	if (LocalCard.BuildCost.Num() > 0)
	{
		CostText = FText::Format(
			FText::FromString(TEXT("Cost: {0} CM")),
			FText::AsNumber(FMath::RoundToInt(LocalCard.BuildCost[0].CurrentValue)));
	}

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, LocalCard]()
		{
			const EOLCConstructionCategory CurrentCat = ConstructionCategories.IsValidIndex(SelectedCategoryIndex)
				? ConstructionCategories[SelectedCategoryIndex]
				: EOLCConstructionCategory::Power;
			int32 IndexInCategory = 0;
			for (const FOLCBuildCardViewData& ExistingCard : BuildCards)
			{
				if (ExistingCard.Category == CurrentCat)
				{
					if (ExistingCard.BuildingName.EqualTo(LocalCard.BuildingName))
					{
						OnCardSelected(IndexInCategory);
						return FReply::Handled();
					}
					IndexInCategory++;
				}
			}
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(bSelected ? FLinearColor(0.09f, 0.105f, 0.12f, 0.96f) : FLinearColor(0.025f, 0.04f, 0.05f, 0.90f))
			.Padding(FMargin(6.0f))
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SImage)
					.Image(FrameBrush)
					.ColorAndOpacity(FLinearColor::White)
				]
				+ SOverlay::Slot()
				.VAlign(VAlign_Top)
				.HAlign(HAlign_Fill)
				[
					SNew(SColorBlock).Color(bSelected ? OLCStyleColors::PrimaryOrange : OLCStyleColors::BorderGray).Size(FVector2D(1.0f, 2.0f))
				]
				+ SOverlay::Slot()
				.Padding(FMargin(10.0f, 10.0f, 10.0f, 8.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth()
					[
						SNew(SBox)
						.WidthOverride(154.0f)
						.HeightOverride(112.0f)
						[ BuildCardThumbnail(LocalCard) ]
					]
					+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(12.0f, 0.0f, 0.0f, 0.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight()
						[
							SNew(STextBlock)
							.Text(LocalCard.BuildingName)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(FText::Format(FText::FromString(TEXT("{0}x{1} GRID")), FText::AsNumber(FMath::RoundToInt(LocalCard.GridSize.X)), FText::AsNumber(FMath::RoundToInt(LocalCard.GridSize.Y))))
							.ColorAndOpacity(OLCStyleColors::TextDim)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(FText::Format(FText::FromString(TEXT("TIR {0}")), FText::AsNumber(LocalCard.TIRRequirement)))
							.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(CostText)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
						]
					]
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Build card thumbnail — uses relative PB- lookup from sliced assets (Step 2 fix)
// Falls back to colored SBox with building name text overlay if PNG not found.
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCConstructionOverlayWidget::BuildCardThumbnail(const FOLCBuildCardViewData& Card)
{
	// WP-13 Step 2: Use relative path lookup from shared assets instead of hardcoded absolute paths.
	// The thumbnail naming convention follows the building category prefix pattern.
	FString ThumbnailPath = TEXT("");

	const FString BuildingNameStr = Card.BuildingName.ToString();

	// Map building names to PB- thumbnail file patterns (relative to project dir)
	if (BuildingNameStr.Contains(TEXT("SOLAR PANEL")) || BuildingNameStr.Contains(TEXT("SOLAR ARRAY")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-PG-01-SolarArray.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("FUSION")) || BuildingNameStr.Contains(TEXT("POWER PLANT")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-PG-06-PowerPlant.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("MINE")) || BuildingNameStr.Contains(TEXT("DRILL")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-EX-01-MetalOreMine.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("REFINERY")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-PF-03-DistillationTowers.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("COMMAND")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SB-02-Main.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("DEPOT")) || BuildingNameStr.Contains(TEXT("LOCKER")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-ST-01-Main.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("FABRICATOR")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-PF-04-Main.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("TURRET")) || BuildingNameStr.Contains(TEXT("DEFENSE")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-DS-01-Main.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("MEDICAL")) || BuildingNameStr.Contains(TEXT("MEDBAY")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-FS-04-Main.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("BARRACKS")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SB-03-Barracks.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("HABITATION")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SB-04-Habitation.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("WALL")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-INF-01-Wall.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("GATE")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-INF-02-Gate.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("VOID LAB")) || BuildingNameStr.Contains(TEXT("CRYSTAL")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-HT-01-VoidLab.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("ASSEMBLY")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-HT-02-AssemblyPlant.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("DUNGEON SCANNER")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SP-01-DungeonScanner.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("CONVERTER")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SP-02-ResourceConverter.png"));
	}
	else if (BuildingNameStr.Contains(TEXT("ARTIFACT")) || BuildingNameStr.Contains(TEXT("DECODER")))
	{
		ThumbnailPath = HUDLayout::SharedAssetPath(TEXT("PB-SP-03-AlienDecoder.png"));
	}

	// Try to load the thumbnail; fall back to colored SBox with text overlay if not found.
	if (!ThumbnailPath.IsEmpty() && HUDLayout::AssetExists(ThumbnailPath))
	{
		BuildCardBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*ThumbnailPath), FVector2D(154.0f, 112.0f)));
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.25f))
			.Padding(FMargin(2.0f))
			[
				SNew(SImage)
				.Image(BuildCardBrushes.Last().Get())
			];
	}

	// Fallback: colored SBox with building name text overlay (WP-13 spec)
	FLinearColor FallbackColor;
	switch (static_cast<int32>(Card.Category))
	{
		case static_cast<int32>(EOLCConstructionCategory::Power):          FallbackColor = OLCStyleColors::PrimaryOrange; break;
		case static_cast<int32>(EOLCConstructionCategory::Extraction):     FallbackColor = OLCStyleColors::TacticalBlue; break;
		case static_cast<int32>(EOLCConstructionCategory::Infrastructure): FallbackColor = OLCStyleColors::ValidGreen; break;
		case static_cast<int32>(EOLCConstructionCategory::Storage):        FallbackColor = OLCStyleColors::WarningYellow; break;
		case static_cast<int32>(EOLCConstructionCategory::Production):     FallbackColor = FLinearColor(0.5f, 0.3f, 0.8f, 1.0f); break;
		case static_cast<int32>(EOLCConstructionCategory::Defense):        FallbackColor = OLCStyleColors::DangerRed; break;
		default:                                                           FallbackColor = OLCStyleColors::TextDim; break;
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(FallbackColor.R, FallbackColor.G, FallbackColor.B, 0.15f))
		.Padding(FMargin(4.0f))
		[
			SNew(SBox)
			.WidthOverride(154.0f)
			.HeightOverride(112.0f)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(Card.BuildingName)
					.ColorAndOpacity(FallbackColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					.Justification(ETextJustify::Center)
					.AutoWrapText(true)
				]
			]
		];
}

TSharedRef<SWidget> UOLCConstructionOverlayWidget::BuildPlacementPreview()
{
	const bool bHasSelection = SelectedCardIndex >= 0;

	// WP-103 Step 5: Green glow (#00FF88) for valid, red glow (#FF4444) for invalid.
	const FLinearColor ValidGlowColor(0.0f, 1.0f, 0.53f, 0.6f);   // #00FF88 with alpha
	const FLinearColor InvalidGlowColor(1.0f, 0.27f, 0.27f, 0.6f); // #FF4444 with alpha

	const bool bValid = bHasSelection && bIsPlacementValid;
	const FText StatusText = !bHasSelection
		? LOCTEXT("PlacementNone", "SELECT A BUILDING")
		: (bValid ? LOCTEXT("PlacementValid", "VALID PLACEMENT") : LOCTEXT("PlacementInvalid", "INVALID PLACEMENT"));
	const FLinearColor StatusColor = bValid ? FLinearColor(0.0f, 1.0f, 0.53f, 1.0f) : FLinearColor(1.0f, 0.27f, 0.27f, 1.0f);
	const FLinearColor BorderGlow = bValid ? ValidGlowColor : InvalidGlowColor;
	const FLinearColor InnerGlow = FLinearColor(StatusColor.R, StatusColor.G, StatusColor.B, 0.08f);

	return SNew(SBorder)
		.BorderBackgroundColor(BorderGlow)
		.Padding(FMargin(14.0f, 10.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(StatusText)
				.ColorAndOpacity(StatusColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f).HAlign(HAlign_Center)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(StatusColor.R, StatusColor.G, StatusColor.B, 0.45f))
				.Padding(FMargin(0.0f))
				[
					SNew(SBox)
					.WidthOverride(180.0f)
					.HeightOverride(84.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(InnerGlow)
					]
				]
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f).HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::Format(FText::FromString(TEXT("Rotation: {0}deg")), FText::AsNumber(BuildRotationDegrees)))
				.ColorAndOpacity(OLCStyleColors::TacticalBlue)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
		];
}

TSharedRef<SWidget> UOLCConstructionOverlayWidget::BuildBuildDetailPanel()
{
	// Use first card in selected category as default detail.
	const FOLCBuildCardViewData* DetailCard = nullptr;
	if (SelectedCardIndex >= 0)
	{
		const EOLCConstructionCategory CurrentCat = ConstructionCategories[SelectedCategoryIndex];
		int32 Idx = 0;
		for (const auto& Card : BuildCards)
		{
			if (Card.Category == CurrentCat)
			{
				if (Idx == SelectedCardIndex) { DetailCard = &Card; break; }
				Idx++;
			}
		}
	}

	// Fallback: first card in category.
	if (!DetailCard)
	{
		const EOLCConstructionCategory CurrentCat = ConstructionCategories[SelectedCategoryIndex];
		for (const auto& Card : BuildCards)
		{
			if (Card.Category == CurrentCat) { DetailCard = &Card; break; }
		}
	}

	FText BuildingName = DetailCard ? DetailCard->BuildingName : LOCTEXT("Det_NoSelection", "— NO SELECTION —");
	FText Description  = DetailCard ? DetailCard->Description : FText();
	int32 TIRReq       = DetailCard ? DetailCard->TIRRequirement : 0;
	FVector2D GridSz   = DetailCard ? DetailCard->GridSize : FVector2D(0.0f, 0.0f);

	FText CostText = FText::FromString(TEXT("-"));
	if (DetailCard && DetailCard->BuildCost.Num() > 0)
	{
		CostText = FText::Format(FText::FromString(TEXT("{0} CM")), FText::AsNumber(FMath::RoundToInt(DetailCard->BuildCost[0].CurrentValue)));
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 14.0f))
		[
			SNew(SOverlay)
			// Top accent
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(OLCStyleColors::PrimaryOrange).Size(FVector2D(1.0f, 2.0f))
			]
			// Content
			+ SOverlay::Slot()
			.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
					.Text(BuildingName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 14.0f).HAlign(HAlign_Center)
				[
					DetailCard ? BuildCardThumbnail(*DetailCard) : SNew(SSpacer)
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Det_Building", "BUILDING")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(BuildingName).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Det_TIR", "TIR REQ")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(FText::AsNumber(TIRReq)).ColorAndOpacity(OLCStyleColors::WarningYellow).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Det_Grid", "GRID")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("{0}x{1}")), FText::AsNumber(GridSz.X), FText::AsNumber(GridSz.Y))).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Det_Cost", "COST")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[ SNew(STextBlock).Text(CostText).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
				]
				// Description
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(Description)
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.AutoWrapText(true)
				]
				// WP-104: Expected Output section (production preview for unplaced buildings)
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor(0.015f, 0.025f, 0.03f, 0.95f))
					.Padding(FMargin(8.0f, 6.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("Det_Output", "EXPECTED OUTPUT"))
							.ColorAndOpacity(OLCStyleColors::ValidGreen)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(DetailCard && DetailCard->ExpectedOutputPerTick.Num() > 0
								? FText::FromString(TEXT("See production panel"))
								: FText::FromString(TEXT("—")))
							.ColorAndOpacity(DetailCard && DetailCard->ExpectedOutputPerTick.Num() > 0
								? OLCStyleColors::ValidGreen
								: OLCStyleColors::TextDim)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						]
					]
				]
				// WP-104: Power status indicator
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 4.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("Det_Power", "POWER")).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
					[
						SNew(STextBlock)
						.Text(DetailCard
							? (DetailCard->PowerConsumption < 0.0f
								? FText::Format(FText::FromString(TEXT("+{0} PROD")), FText::AsNumber(FMath::RoundToInt(FMath::Abs(DetailCard->PowerConsumption))))
								: (DetailCard->PowerConsumption > 0.0f
									? FText::Format(FText::FromString(TEXT("{0} CONSUME")), FText::AsNumber(FMath::RoundToInt(DetailCard->PowerConsumption)))
									: FText::FromString(TEXT("NEUTRAL"))))
							: FText::FromString(TEXT("-")))
						.ColorAndOpacity(DetailCard
							? (DetailCard->PowerConsumption < 0.0f
								? OLCStyleColors::ValidGreen
								: (DetailCard->PowerConsumption > 0.0f ? OLCStyleColors::WarningYellow : OLCStyleColors::TextDim))
							: OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					]
				]
			]
		];
}

void UOLCConstructionOverlayWidget::OnCategorySelected(int32 CategoryIndex)
{
	SelectedCategoryIndex = CategoryIndex;
	SelectedCardIndex = 0;
	RefreshConstructionScreen();
}

void UOLCConstructionOverlayWidget::OnCardSelected(int32 CardIndex)
{
	SelectedCardIndex = CardIndex;
	RefreshConstructionScreen();
}

FReply UOLCConstructionOverlayWidget::OnRotateClicked()
{
	BuildRotationDegrees = (BuildRotationDegrees + 90) % 360;
	UE_LOG(LogTemp, Display, TEXT("[OLC] Construction: Rotation = %d deg"), BuildRotationDegrees);
	RefreshConstructionScreen();
	return FReply::Handled();
}

FReply UOLCConstructionOverlayWidget::OnCancelClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PC))
		{
			MenuPC->CloseActiveScreen();
		}
	}
	return FReply::Handled();
}

FReply UOLCConstructionOverlayWidget::OnConfirmClicked()
{
	if (SelectedCardIndex < 0) return FReply::Unhandled();

	// WP-103 Step 6: Confirm placement — deduct resources, spawn building
	UpdatePlacementValidity();
	if (!bIsPlacementValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot build — invalid placement"));
		return FReply::Handled();
	}

	// Get selected card data.
	const EOLCConstructionCategory CurrentCat = ConstructionCategories.IsValidIndex(SelectedCategoryIndex)
		? ConstructionCategories[SelectedCategoryIndex]
		: EOLCConstructionCategory::Power;

	int32 Idx = 0;
	const FOLCBuildCardViewData* SelectedCard = nullptr;
	for (const auto& Card : BuildCards)
	{
		if (Card.Category == CurrentCat)
		{
			if (Idx == SelectedCardIndex) { SelectedCard = &Card; break; }
			Idx++;
		}
	}

	if (!SelectedCard) return FReply::Handled();

	// Check affordability and consume resources.
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			TArray<FOLCResourceAmount> Cost;
			for (const auto& C : SelectedCard->BuildCost)
			{
				Cost.Emplace(C.ResourceType, C.CurrentValue, C.Capacity);
			}

			if (!Data->ConsumeResourcesForBuild(Cost))
			{
				UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot afford build: %s"), *SelectedCard->BuildingName.ToString());
				return FReply::Handled();
			}
		}
	}

	// Place the building on the terrain actor; PlaceBuilding owns occupancy.
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCPlanetTerrainActor::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		if (AOLCPlanetTerrainActor* Terrain = Cast<AOLCPlanetTerrainActor>(Actor))
		{
			FVector WorldPos;
			if (Terrain->PlaceBuilding(PlacementGridOrigin, SelectedCard->GridSize, WorldPos, BuildRotationDegrees))
			{
				UE_LOG(LogTemp, Log, TEXT("[OLC] Building placed at %s"), *WorldPos.ToString());
			}
			break;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Confirmed build: %s at grid %s"),
		*SelectedCard->BuildingName.ToString(),
		*PlacementGridOrigin.ToString());

	return FReply::Handled();
}

void UOLCConstructionOverlayWidget::UpdatePlacementValidity()
{
	bIsPlacementValid = false;

	if (SelectedCardIndex < 0) return;

	// Get selected card data.
	const EOLCConstructionCategory CurrentCat = ConstructionCategories.IsValidIndex(SelectedCategoryIndex)
		? ConstructionCategories[SelectedCategoryIndex]
		: EOLCConstructionCategory::Power;

	int32 Idx = 0;
	const FOLCBuildCardViewData* SelectedCard = nullptr;
	for (const auto& Card : BuildCards)
	{
		if (Card.Category == CurrentCat)
		{
			if (Idx == SelectedCardIndex) { SelectedCard = &Card; break; }
			Idx++;
		}
	}

	if (!SelectedCard) return;

	// Raycast from mouse to get world position.
	if (APlayerController* PC = GetOwningPlayer())
	{
		FVector Start, Direction;
		if (PC->DeprojectMousePositionToWorld(Start, Direction))
		{
			const FVector End = Start + Direction * 100000.0f;
			FHitResult Hit;
			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(OLCConstructionPlacement), true);
			QueryParams.AddIgnoredActor(PC->GetPawn());

			if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
			{
				// Convert world position to grid coordinate.
				const float GridCellSize = 180.0f;
				const int32 GridX = FMath::RoundToInt(Hit.Location.X / GridCellSize);
				const int32 GridY = FMath::RoundToInt(Hit.Location.Y / GridCellSize);
				PlacementGridOrigin = FIntPoint(GridX, GridY);

				// Check against terrain.
				TArray<AActor*> AllActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCPlanetTerrainActor::StaticClass(), AllActors);
				for (AActor* Actor : AllActors)
				{
					if (AOLCPlanetTerrainActor* Terrain = Cast<AOLCPlanetTerrainActor>(Actor))
					{
						bIsPlacementValid = Terrain->CanPlaceBuilding(PlacementGridOrigin, SelectedCard->GridSize, BuildRotationDegrees);
						break;
					}
				}
			}
		}
	}
}

void UOLCConstructionOverlayWidget::ConfirmBuild()
{
	OnConfirmClicked();
}

void UOLCConstructionOverlayWidget::RotateBuild()
{
	BuildRotationDegrees = (BuildRotationDegrees + 90) % 360;
	RefreshConstructionScreen();
}

void UOLCConstructionOverlayWidget::CancelBuild()
{
	OnCancelClicked();
}

void UOLCConstructionOverlayWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	BuildCardBrushes.Reset();
}

void UOLCConstructionOverlayWidget::RefreshConstructionScreen()
{
	InvalidateLayoutAndVolatility();
	if (MyWidget.IsValid())
	{
		MyWidget = RebuildWidget();
	}
	RemoveFromParent();
	SetAnchorsInViewport(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	SetAlignmentInViewport(FVector2D::ZeroVector);
	SetDesiredSizeInViewport(FVector2D::ZeroVector);
	AddToViewport(200);
}

// ---------------------------------------------------------------------------
// WP-107 Step 7: Toast notification stack (top-right, below time control)
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCMainRTSHUDWidget::BuildToastStack()
{
	// WP-107 Step 7: Create toast stack widget instance
	UOLCToastStackWidget* ToastStack = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		ToastStack = CreateWidget<UOLCToastStackWidget>(GI, UOLCToastStackWidget::StaticClass());
	}

	TSharedRef<SScrollBox> ToastList = SNew(SScrollBox);

	if (ToastStack)
	{
		// Queue some demo toasts for testing
		ToastStack->QueueToast(FOLCToastData(
			LOCTEXT("ToastWelcome", "WELCOME COMMANDER"),
			LOCTEXT("ToastWelcomeMsg", "Base operations initialized. All systems nominal."),
			EOLCColorRole::Success));
	}

	return SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Right)
		.Padding(0.0f, 210.0f, 18.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(320.0f)
			.HeightOverride(200.0f)
			[ ToastList ]
		];
}

void UOLCMainRTSHUDWidget::QueueToastNotification(const FText& InTitle, const FText& InMessage, EOLCColorRole InColor)
{
	// WP-107 Step 7: Queue a toast from gameplay events
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCToastStackWidget* NewToastStack = CreateWidget<UOLCToastStackWidget>(GI, UOLCToastStackWidget::StaticClass()))
		{
			NewToastStack->QueueToast(FOLCToastData(InTitle, InMessage, InColor));
			NewToastStack->AddToViewport(100);
			UE_LOG(LogTemp, Log, TEXT("[OLC] Toast stack created and queued: %s"), *InTitle.ToString());
		}
	}
}

#undef LOCTEXT_NAMESPACE
