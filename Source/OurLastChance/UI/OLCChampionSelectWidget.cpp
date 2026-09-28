#include "UI/OLCChampionSelectWidget.h"
#include "Player/OLCMenuPlayerController.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "OLCChampionSelect"

namespace
{
	// Frame border art lives under the Welcome Screen's asset folder and is
	// reused here as-is (same PNGs, same 1920x1080 design canvas), same as
	// WBP_FactionSelect.
	FString FrameAssetFile(const TCHAR* FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/WelcomeScreen") / FileName);
	}

	// Champion Selection screen art -- see UE5/Assets/UI/ChampionSelection/ASSET_MANIFEST.md.
	FString ChampionAssetFile(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/ChampionSelection") / FileName);
	}

	// Segment_Stat_*.png -- no Champion-specific stat segment set was
	// generated, so this reuses the Faction Selection art (same visual
	// style already established for stat rows).
	FString FactionSegmentAssetFile(const TCHAR* FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/FactionSelection") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }

	FText GetFactionDisplayName(const FString& InFactionId)
	{
		if (InFactionId == TEXT("NeonPunk")) return LOCTEXT("Fac_NeonPunk", "NEON PUNK");
		if (InFactionId == TEXT("DarkRealistic")) return LOCTEXT("Fac_DarkRealistic", "DARK REALISTIC");
		if (InFactionId == TEXT("CartoonSciFi")) return LOCTEXT("Fac_CartoonSciFi", "CARTOON SCIFI");
		if (InFactionId == TEXT("BrightRealistic")) return LOCTEXT("Fac_BrightRealistic", "BRIGHT REALISTIC");
		return FText::FromString(InFactionId.ToUpper());
	}

	struct FSeed
	{
		const TCHAR* Id;
		const TCHAR* F;
		const TCHAR* N;
		const TCHAR* R;
		EOLCChampionRole Role;
		int32 C, E, M, S;
		const TCHAR* P;
	};

	const FSeed Seeds[] = {
		{ TEXT("CH-NP-01"), TEXT("NeonPunk"), TEXT("Voltage"), TEXT("Kai Nakamura"), EOLCChampionRole::Combatant, 5, 2, 4, 3, TEXT("Overcharge: improved energy-weapon output.") },
		{ TEXT("CH-NP-02"), TEXT("NeonPunk"), TEXT("Glitch"), TEXT("Alex Chen"), EOLCChampionRole::Scout, 3, 4, 5, 2, TEXT("Ghost Protocol: faster scouting and detection.") },
		{ TEXT("CH-NP-03"), TEXT("NeonPunk"), TEXT("Wrench"), TEXT("Sam Okafor"), EOLCChampionRole::Engineer, 2, 5, 3, 4, TEXT("Field Fabricator: faster repairs and construction.") },
		{ TEXT("CH-DR-01"), TEXT("DarkRealistic"), TEXT("Ironclad"), TEXT("Major Rachel Torres"), EOLCChampionRole::Combatant, 5, 3, 2, 5, TEXT("Hold the Line: nearby allies gain defense.") },
		{ TEXT("CH-DR-02"), TEXT("DarkRealistic"), TEXT("Sniper"), TEXT("Viktor Petrov"), EOLCChampionRole::Scout, 5, 2, 4, 3, TEXT("Marked Target: precision attacks expose enemies.") },
		{ TEXT("CH-DR-03"), TEXT("DarkRealistic"), TEXT("Demolitions"), TEXT("Corporal Jake Morrison"), EOLCChampionRole::Specialist, 5, 4, 2, 3, TEXT("Breach Expert: bonus structure damage.") },
		{ TEXT("CH-CS-01"), TEXT("CartoonSciFi"), TEXT("Blaze"), TEXT("Rico Delgado"), EOLCChampionRole::Combatant, 5, 2, 4, 3, TEXT("Hot Streak: consecutive hits increase damage.") },
		{ TEXT("CH-CS-02"), TEXT("CartoonSciFi"), TEXT("Zoom"), TEXT("Pip Tanaka"), EOLCChampionRole::Scout, 3, 2, 5, 4, TEXT("Kinetic Rush: exceptional movement speed.") },
		{ TEXT("CH-CS-03"), TEXT("CartoonSciFi"), TEXT("Boomstick"), TEXT("Daisy Mayhem"), EOLCChampionRole::Specialist, 5, 3, 3, 3, TEXT("Bigger Boom: increased explosive radius.") },
		{ TEXT("CH-BR-01"), TEXT("BrightRealistic"), TEXT("Medic"), TEXT("Dr. Amara Osei"), EOLCChampionRole::Support, 2, 4, 3, 5, TEXT("Triage: passive squad health regeneration.") },
		{ TEXT("CH-BR-02"), TEXT("BrightRealistic"), TEXT("Gardener"), TEXT("Elias Greenfield"), EOLCChampionRole::Support, 2, 4, 3, 5, TEXT("Cultivator: improved survival production.") },
		{ TEXT("CH-BR-03"), TEXT("BrightRealistic"), TEXT("Architect"), TEXT("Morgan Lee"), EOLCChampionRole::Engineer, 2, 5, 3, 4, TEXT("Efficient Design: reduced building costs.") },
	};
}

void UOLCChampionSelectWidget::InitializeForFaction(const FString& F)
{
	FactionId = F;
	Selected = INDEX_NONE;
	BuildCatalog();
	EnsureDefaultSelection();
}

void UOLCChampionSelectWidget::BuildCatalog()
{
	Champions.Reset();
	for (const FSeed& X : Seeds)
	{
		if (FactionId.IsEmpty() || FactionId == X.F)
		{
			UOLCChampionData* D = NewObject<UOLCChampionData>(this);
			D->ChampionId = X.Id;
			D->FactionId = X.F;
			D->DisplayName = FText::FromString(X.N);
			D->RealName = FText::FromString(X.R);
			D->Role = X.Role;
			D->CombatRating = X.C;
			D->EngineeringRating = X.E;
			D->MobilityRating = X.M;
			D->SurvivalRating = X.S;
			D->PassiveDescription = FText::FromString(X.P);
			D->BenefitSummary = D->PassiveDescription;
			Champions.Add(D);
		}
	}
}

void UOLCChampionSelectWidget::EnsureDefaultSelection()
{
	if (!Champions.IsEmpty() && !Champions.IsValidIndex(Selected))
	{
		Selected = 0;
	}
}

void UOLCChampionSelectWidget::LoadChampionArt()
{
	BackgroundBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Background_ChampionSelection_3840x2160.png"))), FVector2D(1920.0f, 1080.0f));
	ButtonChampionActiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_Champion_Active.png"))), FVector2D(640.0f, 160.0f));
	ButtonChampionInactiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_Champion_Inactive.png"))), FVector2D(640.0f, 160.0f));
	PanelDossierBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Panel_Champion_Dossier.png"))), FVector2D(896.0f, 672.0f));
	NavButtonActiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_BeginCrash.png"))), FVector2D(470.0f, 64.0f));
	NavButtonBackBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_Back.png"))), FVector2D(470.0f, 64.0f));

	SegmentStatGreenBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionSegmentAssetFile(TEXT("Segment_Stat_Green.png"))), FVector2D(44.0f, 14.0f));
	SegmentStatOrangeBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionSegmentAssetFile(TEXT("Segment_Stat_Orange.png"))), FVector2D(44.0f, 14.0f));
	SegmentStatBlueBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionSegmentAssetFile(TEXT("Segment_Stat_Blue.png"))), FVector2D(44.0f, 14.0f));
	SegmentStatRedBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionSegmentAssetFile(TEXT("Segment_Stat_Red.png"))), FVector2D(44.0f, 14.0f));

	// Champion_Ability_Icon_Atlas_4x1.png packs four 128px icon cells left-to-right.
	AbilityIconBrushes.Empty();
	const FString AtlasPath = ChampionAssetFile(TEXT("Champion_Ability_Icon_Atlas_4x1.png"));
	if (AssetExists(AtlasPath))
	{
		for (int32 Cell = 0; Cell < 4; ++Cell)
		{
			TUniquePtr<FSlateDynamicImageBrush> IconBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*AtlasPath), FVector2D(128.0f, 128.0f));
			IconBrush->SetUVRegion(FBox2D(FVector2D(Cell * 0.25f, 0.0f), FVector2D((Cell + 1) * 0.25f, 1.0f)));
			AbilityIconBrushes.Add(MoveTemp(IconBrush));
		}
	}

	// Per-champion card portrait and dossier full-body art.
	PortraitBrushes.Empty();
	FullBodyBrushes.Empty();
	for (const TObjectPtr<UOLCChampionData>& Champion : Champions)
	{
		if (!Champion)
		{
			PortraitBrushes.Add(nullptr);
			FullBodyBrushes.Add(nullptr);
			continue;
		}

		const FString PortraitPath = ChampionAssetFile(FString::Printf(TEXT("Portrait_%s.png"), *Champion->ChampionId));
		PortraitBrushes.Add(AssetExists(PortraitPath)
			? MakeUnique<FSlateDynamicImageBrush>(FName(*PortraitPath), FVector2D(512.0f, 160.0f))
			: nullptr);

		const FString FullBodyPath = ChampionAssetFile(FString::Printf(TEXT("FullBody_%s.png"), *Champion->ChampionId));
		FullBodyBrushes.Add(AssetExists(FullBodyPath)
			? MakeUnique<FSlateDynamicImageBrush>(FName(*FullBodyPath), FVector2D(512.0f, 672.0f))
			: nullptr);
	}
}

TSharedRef<SWidget> UOLCChampionSelectWidget::RebuildWidget()
{
	if (Champions.IsEmpty())
	{
		BuildCatalog();
	}

	EnsureDefaultSelection();
	LoadChampionArt();

	// Frame border brushes -- reused from the Welcome Screen's art, same
	// pattern as WBP_FactionSelect.
	FrameCornerTLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_TL.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerTRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_TR.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerBRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_BR.png"))), FVector2D(160.0f, 160.0f));
	FrameCornerBLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_BL.png"))), FVector2D(160.0f, 160.0f));
	FrameEdgeTopBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Top.png"))), FVector2D(1600.0f, 64.0f));
	FrameEdgeBottomBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Bottom.png"))), FVector2D(1600.0f, 64.0f));
	FrameEdgeLeftBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Left.png"))), FVector2D(64.0f, 760.0f));
	FrameEdgeRightBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Right.png"))), FVector2D(64.0f, 760.0f));

	TSharedRef<SVerticalBox> CardList = SNew(SVerticalBox);
	ChampionListBox = CardList;
	for (int32 Index = 0; Index < Champions.Num(); ++Index)
	{
		CardList->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 16.0f)
		[
			BuildChampionCard(Index)
		];
	}

	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas)
		// Full-bleed background art.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f))
		[
			SNew(SImage)
			.Image(BackgroundBrush.Get())
		]
		// Title.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 150.0f, 1170.0f, 860.0f))
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Title", "SELECT CHAMPION"))
			.ColorAndOpacity(FLinearColor::White)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 46))
		]
		// Champion card list.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 260.0f, 1210.0f, 150.0f))
		[ CardList ]
		// Faction-selected indicator strip.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 728.0f, 1210.0f, 292.0f))
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.10f, 0.06f, 0.02f, 0.85f))
			.Padding(FMargin(16.0f, 8.0f))
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("FactionSelected", "{0} SELECTED"), GetFactionDisplayName(FactionId)))
				.ColorAndOpacity(FLinearColor(0.98f, 0.60f, 0.20f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
			]
		]
		// Back button.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 800.0f, 1550.0f, 224.0f))
		[ BuildNavButton(LOCTEXT("Back", "BACK"), NavButtonBackBrush.Get(), FOnClicked::CreateUObject(this, &UOLCChampionSelectWidget::Back), true) ]
		// Dossier panel (background art + swappable selected-champion content).
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(820.0f, 185.0f, 204.0f, 223.0f))
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(PanelDossierBrush.Get())
			]
			+ SOverlay::Slot()
			[
				SAssignNew(DossierHost, SBox)
			]
		]
		// Frame edges -- identical geometry to OLCWelcomeScreenWidget, placed
		// before the corners so the corners paint on top where they overlap.
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 0.0f, -150.0f, 1016.0f))
		[ SNew(SImage).Image(FrameEdgeTopBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(150.0f, 1016.0f, -150.0f, 0.0f))
		[ SNew(SImage).Image(FrameEdgeBottomBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f, 150.0f, 1856.0f, -150.0f))
		[ SNew(SImage).Image(FrameEdgeLeftBrush.Get()) ]
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(1856.0f, 150.0f, 0.0f, -150.0f))
		[ SNew(SImage).Image(FrameEdgeRightBrush.Get()) ]
		// Frame corners (painted after the edges above, so they sit on top
		// where they overlap).
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
		.Offset(FMargin(0.0f, 0.0f, 1760.0f, 920.0f))
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
		[ SNew(SImage).Image(FrameCornerBLBrush.Get()) ];

	RefreshDossier();

	// A bare SBorder here reports its own content-driven desired size to the
	// ancestor viewport slot instead of filling it -- same fix as
	// OLCWelcomeScreenWidget / WBP_FactionSelect. The SBox forces a stable
	// 1920x1080 design size that the ancestor's DPI scaler then correctly
	// scales to the real viewport.
	return SNew(SBox)
		.WidthOverride(1920.0f)
		.HeightOverride(1080.0f)
		[ Canvas ];
}

void UOLCChampionSelectWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	FrameCornerTLBrush.Reset();
	FrameCornerTRBrush.Reset();
	FrameCornerBRBrush.Reset();
	FrameCornerBLBrush.Reset();
	FrameEdgeTopBrush.Reset();
	FrameEdgeBottomBrush.Reset();
	FrameEdgeLeftBrush.Reset();
	FrameEdgeRightBrush.Reset();
	BackgroundBrush.Reset();
	ButtonChampionActiveBrush.Reset();
	ButtonChampionInactiveBrush.Reset();
	PanelDossierBrush.Reset();
	SegmentStatGreenBrush.Reset();
	SegmentStatOrangeBrush.Reset();
	SegmentStatBlueBrush.Reset();
	SegmentStatRedBrush.Reset();
	NavButtonActiveBrush.Reset();
	NavButtonBackBrush.Reset();
	AbilityIconBrushes.Empty();
	PortraitBrushes.Empty();
	FullBodyBrushes.Empty();
}

TSharedRef<SWidget> UOLCChampionSelectWidget::BuildChampionCard(int32 Index)
{
	UOLCChampionData* Champion = Champions.IsValidIndex(Index) ? Champions[Index].Get() : nullptr;
	const FSlateBrush* PortraitBrush = PortraitBrushes.IsValidIndex(Index) ? PortraitBrushes[Index].Get() : nullptr;

	return SNew(SBox)
		.WidthOverride(560.0f)
		.HeightOverride(140.0f)
		[
			SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.ContentPadding(FMargin(0.0f))
			.OnClicked(FOnClicked::CreateUObject(this, &UOLCChampionSelectWidget::Select, Index))
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SImage)
					.Image_Lambda([this, Index]() -> const FSlateBrush* {
						return Index == Selected ? ButtonChampionActiveBrush.Get() : ButtonChampionInactiveBrush.Get();
					})
				]
				// Portrait_*.png is a wide 512x160 strip with the character
				// posed against transparent padding on the left, not a
				// square bust shot -- boxing it at the card's own aspect
				// (140 tall x 448 wide, matching the source's 3.2:1 ratio)
				// keeps the character full-size instead of shrinking it to
				// fit a much narrower box.
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SBox)
					.WidthOverride(448.0f)
					.HeightOverride(140.0f)
					[
						SNew(SScaleBox)
						.Stretch(EStretch::ScaleToFit)
						[ SNew(SImage).Image(PortraitBrush) ]
					]
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(48.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(Champion ? Champion->DisplayName : FText::GetEmpty())
					.ColorAndOpacity_Lambda([this, Index]() {
						return Index == Selected ? FLinearColor(0.98f, 0.60f, 0.20f) : FLinearColor::White;
					})
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
				]
			]
		];
}

TSharedRef<SWidget> UOLCChampionSelectWidget::BuildStatRow(const FText& Label, int32 Rating, const FSlateBrush* SegmentBrush, const FSlateBrush* IconBrush) const
{
	// Segment_Stat_*.png is a single reusable tile meant to be repeated N
	// times (same reasoning and technique as UOLCFactionSelectWidget::
	// BuildStatRow -- see its comment for why this replaced an earlier
	// clip-based-fill attempt). Sized smaller than the Faction Selection
	// version so the row fits inside the dossier panel's narrower right
	// column without the segments running past its angled border.
	constexpr int32 TotalSegments = 10;
	constexpr int32 MaxRating = 5;
	const int32 FilledSegments = FMath::Clamp(Rating, 0, MaxRating) * (TotalSegments / MaxRating);
	const float SegmentWidth = 22.0f;
	const float SegmentHeight = 10.0f;
	const float SegmentSpacing = 2.0f;

	TSharedRef<SHorizontalBox> SegmentsBox = SNew(SHorizontalBox);
	for (int32 SegmentIndex = 0; SegmentIndex < TotalSegments; ++SegmentIndex)
	{
		SegmentsBox->AddSlot()
		.AutoWidth()
		.Padding(0.0f, 0.0f, SegmentIndex == TotalSegments - 1 ? 0.0f : SegmentSpacing, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(SegmentWidth)
			.HeightOverride(SegmentHeight)
			[
				SegmentIndex < FilledSegments
					? StaticCastSharedRef<SWidget>(SNew(SImage).Image(SegmentBrush))
					: StaticCastSharedRef<SWidget>(SNew(SBorder).BorderBackgroundColor(FLinearColor(0.05f, 0.06f, 0.07f, 1.0f)).Padding(FMargin(0.0f)))
			]
		];
	}

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 6.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(16.0f)
			.HeightOverride(16.0f)
			[ SNew(SImage).Image(IconBrush) ]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(122.0f)
			[
				SNew(STextBlock)
				.Text(Label)
				.ColorAndOpacity(FLinearColor(0.80f, 0.83f, 0.87f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[ SegmentsBox ];
}

TSharedRef<SWidget> UOLCChampionSelectWidget::BuildNavButton(const FText& Label, const FSlateBrush* Brush, const FOnClicked& OnClicked, TAttribute<bool> IsEnabledAttr, FLinearColor TextColor) const
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.IsEnabled(IsEnabledAttr)
		.OnClicked(OnClicked)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(Brush)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(Label)
				.ColorAndOpacity(TextColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
				.Justification(ETextJustify::Center)
			]
		];
}

void UOLCChampionSelectWidget::RefreshDossier()
{
	// Swaps DossierHost's entire content for a brand new subtree rather
	// than mutating an existing panel's children -- see
	// UOLCFactionSelectWidget::RefreshStatRows for why a live-bound
	// WidthOverride and even a clear-and-re-add on a persistent panel both
	// failed to visually update per-child sizes reliably. A full content
	// swap sidesteps it entirely, so the whole selected-champion block
	// (portrait, name, role, abilities, stats, confirm) is rebuilt here as
	// one unit rather than only the stat bars.
	if (!DossierHost.IsValid())
	{
		return;
	}

	if (!Champions.IsValidIndex(Selected))
	{
		DossierHost->SetContent(
			SNew(SBox)
			.Padding(40.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NoSelection", "Select a champion to review their profile."))
				.ColorAndOpacity(FLinearColor(0.76f, 0.78f, 0.82f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]
		);
		return;
	}

	UOLCChampionData* Champion = Champions[Selected].Get();
	const FSlateBrush* FullBodyBrush = FullBodyBrushes.IsValidIndex(Selected) ? FullBodyBrushes[Selected].Get() : nullptr;
	const FSlateBrush* RoleIconBrush = AbilityIconBrushes.IsValidIndex(0) ? AbilityIconBrushes[0].Get() : nullptr;

	const FText RoleText = StaticEnum<EOLCChampionRole>()->GetDisplayNameTextByValue(static_cast<int64>(Champion->Role));

	TSharedRef<SHorizontalBox> AbilityRow = SNew(SHorizontalBox);
	for (int32 IconIndex = 0; IconIndex < AbilityIconBrushes.Num(); ++IconIndex)
	{
		AbilityRow->AddSlot()
		.AutoWidth()
		.Padding(0.0f, 0.0f, IconIndex == AbilityIconBrushes.Num() - 1 ? 0.0f : 10.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(48.0f)
			.HeightOverride(48.0f)
			[ SNew(SImage).Image(AbilityIconBrushes[IconIndex].Get()) ]
		];
	}

	DossierHost->SetContent(
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(24.0f)
		[
			SNew(SBox)
			.WidthOverride(380.0f)
			.HeightOverride(624.0f)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[ SNew(SImage).Image(FullBodyBrush) ]
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(0.0f, 60.0f, 56.0f, 24.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(STextBlock)
				.Text(Champion->DisplayName)
				.ColorAndOpacity(FLinearColor(0.98f, 0.60f, 0.20f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 34))
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
				[
					SNew(SBox).WidthOverride(22.0f).HeightOverride(22.0f)
					[ SNew(SImage).Image(RoleIconBrush) ]
				]
				+ SHorizontalBox::Slot().AutoWidth().Padding(8.0f, 0.0f, 0.0f, 0.0f).VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(RoleText)
					.ColorAndOpacity(FLinearColor(0.80f, 0.83f, 0.87f))
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 15))
				]
			]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 20.0f, 0.0f, 8.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Abilities", "ABILITIES"))
				.ColorAndOpacity(FLinearColor(0.60f, 0.63f, 0.67f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
			+ SVerticalBox::Slot().AutoHeight()
			[ AbilityRow ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 22.0f, 0.0f, 10.0f)
			[ BuildStatRow(LOCTEXT("Combat", "COMBAT"), Champion->CombatRating, SegmentStatGreenBrush.Get(), AbilityIconBrushes.IsValidIndex(0) ? AbilityIconBrushes[0].Get() : nullptr) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildStatRow(LOCTEXT("Engineering", "ENGINEERING"), Champion->EngineeringRating, SegmentStatRedBrush.Get(), AbilityIconBrushes.IsValidIndex(1) ? AbilityIconBrushes[1].Get() : nullptr) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildStatRow(LOCTEXT("Mobility", "MOBILITY"), Champion->MobilityRating, SegmentStatBlueBrush.Get(), AbilityIconBrushes.IsValidIndex(2) ? AbilityIconBrushes[2].Get() : nullptr) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[ BuildStatRow(LOCTEXT("Survival", "SURVIVAL"), Champion->SurvivalRating, SegmentStatOrangeBrush.Get(), AbilityIconBrushes.IsValidIndex(3) ? AbilityIconBrushes[3].Get() : nullptr) ]
			+ SVerticalBox::Slot().FillHeight(1.0f)
			[ SNullWidget::NullWidget ]
			// Inset well clear of the dossier panel art's diagonal chamfered
			// corner -- same fix as UOLCFactionSelectWidget's Confirm button
			// (a flush right/bottom placement let the chevron's arrow tip
			// poke through the corner cut).
			+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Right).Padding(0.0f, 0.0f, 36.0f, 24.0f)
			[
				SNew(SBox)
				.WidthOverride(320.0f)
				.HeightOverride(56.0f)
				[
					BuildNavButton(LOCTEXT("BeginCrash", "BEGIN CRASH"), NavButtonActiveBrush.Get(),
						FOnClicked::CreateUObject(this, &UOLCChampionSelectWidget::Confirm), true,
						FLinearColor(0.93f, 0.43f, 0.09f, 1.0f))
				]
			]
		]
	);
}

FReply UOLCChampionSelectWidget::Select(int32 Index)
{
	if (Champions.IsValidIndex(Index))
	{
		Selected = Index;
		InvalidateLayoutAndVolatility();
		RefreshDossier();
	}
	return FReply::Handled();
}

FReply UOLCChampionSelectWidget::Confirm()
{
	if (Champions.IsValidIndex(Selected))
	{
		if (AOLCMenuPlayerController* P = Cast<AOLCMenuPlayerController>(GetOwningPlayer()))
		{
			P->ConfirmChampionSelection(Champions[Selected]->ChampionId);
		}
	}
	return FReply::Handled();
}

FReply UOLCChampionSelectWidget::Back()
{
	if (AOLCMenuPlayerController* P = Cast<AOLCMenuPlayerController>(GetOwningPlayer()))
	{
		P->OpenFactionSelect();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
