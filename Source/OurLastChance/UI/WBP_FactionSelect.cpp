#include "WBP_FactionSelect.h"
#include "OurLastChance.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "Logging/LogMacros.h"
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

#define LOCTEXT_NAMESPACE "FactionSelectWidget"

namespace
{
	// Frame border art lives under the Welcome Screen's asset folder and is
	// reused here as-is (same PNGs, same 1920x1080 design canvas) rather than
	// duplicated, per the request to reuse the Welcome Screen frame.
	FString FrameAssetFile(const TCHAR* FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/WelcomeScreen") / FileName);
	}

	// Faction Selection screen art -- see UE5/Assets/UI/FactionSelection/ASSET_MANIFEST.md.
	FString FactionAssetFile(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/FactionSelection") / FileName);
	}

	// Button_BeginCrash.png -- generated for Champion Selection, reused here
	// too for the Confirm button (no equivalent was made for Faction
	// Selection specifically).
	FString ChampionAssetFile(const TCHAR* FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/ChampionSelection") / FileName);
	}

	bool FactionSelectAssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

void UOLCFactionSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (FactionData.IsEmpty())
    {
        PopulateDefaultFactions();
    }

    EnsureDefaultSelection();
}

void UOLCFactionSelectWidget::InitFactions(const TArray<FOLCFactionDisplayData>& InFactions)
{
    FactionData = InFactions;
    SelectedFactionIndex = -1;
    EnsureDefaultSelection();

    InvalidateLayoutAndVolatility();
}

void UOLCFactionSelectWidget::OnFactionSelected(int32 Index)
{
    if (Index >= 0 && Index < FactionData.Num())
    {
        SelectedFactionIndex = Index;
        InvalidateLayoutAndVolatility();
        RefreshStatRows();
    }
    else
    {
        UE_LOG(LogOLC, Warning, TEXT("Invalid faction index selected: %d"), Index);
    }
}

void UOLCFactionSelectWidget::OnConfirmPressed()
{
    if (SelectedFactionIndex >= 0 && SelectedFactionIndex < FactionData.Num())
    {
        const FString& SelectedId = FactionData[SelectedFactionIndex].FactionId;
        OnFactionConfirmed.Broadcast(SelectedId);
    }
    else
    {
        UE_LOG(LogOLC, Warning, TEXT("Confirm pressed with no valid faction selected."));
    }
}

void UOLCFactionSelectWidget::HandleBackPressed()
{
    OnBackPressed.Broadcast();
}

void UOLCFactionSelectWidget::RefreshStatRows()
{
    // Swaps StatsHost's entire content for a brand new SVerticalBox rather
    // than mutating an existing panel's children in place. Both were tried:
    // a live SBox::WidthOverride_Lambda (never re-arranged after the first
    // Prepass, regardless of Invalidate() reason) and clearing/re-adding
    // slots on a persistent SVerticalBox (new child widgets each time, with
    // correct baked-in data confirmed via logging -- text rows updated
    // correctly, but each bar's rendered width stayed pinned to whatever
    // its ROW POSITION originally measured, not the new child's own
    // WidthOverride, implying the panel was reusing a per-slot-index cached
    // arrangement instead of re-measuring the replacement child). Replacing
    // the whole content of a host SBox forces a genuinely fresh subtree.
    if (!StatsHost.IsValid())
    {
        return;
    }

    if (!FactionData.IsValidIndex(SelectedFactionIndex))
    {
        StatsHost->SetContent(SNullWidget::NullWidget);
        return;
    }

    const FOLCFactionDisplayData& Faction = FactionData[SelectedFactionIndex];

    StatsHost->SetContent(
        SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 12.0f)
        [
            BuildStatRow(LOCTEXT("Mining", "MINING"), Faction.MiningRating, SegmentStatGreenBrush.Get())
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 12.0f)
        [
            BuildStatRow(LOCTEXT("Defense", "DEFENSE"), Faction.DefenseRating, SegmentStatRedBrush.Get())
        ]
        + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 12.0f)
        [
            BuildStatRow(LOCTEXT("Mobility", "MOBILITY"), Faction.MobilityRating, SegmentStatBlueBrush.Get())
        ]
        + SVerticalBox::Slot().AutoHeight()
        [
            BuildStatRow(LOCTEXT("Survival", "SURVIVAL"), Faction.SurvivalRating, SegmentStatOrangeBrush.Get())
        ]
    );
}

void UOLCFactionSelectWidget::LoadFactionArt()
{
    BackgroundBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Background_FactionSelection_3840x2160.png"))), FVector2D(1920.0f, 1080.0f));
    ButtonFactionActiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Button_Faction_Active.png"))), FVector2D(512.0f, 96.0f));
    ButtonFactionInactiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Button_Faction_Inactive.png"))), FVector2D(512.0f, 96.0f));
    PanelDossierBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Panel_Faction_Dossier.png"))), FVector2D(896.0f, 640.0f));
    SegmentStatGreenBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Segment_Stat_Green.png"))), FVector2D(44.0f, 14.0f));
    SegmentStatOrangeBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Segment_Stat_Orange.png"))), FVector2D(44.0f, 14.0f));
    SegmentStatBlueBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Segment_Stat_Blue.png"))), FVector2D(44.0f, 14.0f));
    SegmentStatRedBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FactionAssetFile(TEXT("Segment_Stat_Red.png"))), FVector2D(44.0f, 14.0f));
    NavButtonActiveBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_BeginCrash.png"))), FVector2D(470.0f, 64.0f));
    NavButtonBackBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*ChampionAssetFile(TEXT("Button_Back.png"))), FVector2D(470.0f, 64.0f));

    // Faction_Icon_Atlas_4x1.png packs four 128px icon cells left-to-right,
    // one per faction slot in FactionData order. A separate dynamic brush
    // per cell (same source texture, different UV sub-region) lets each
    // faction button show its own icon without slicing the PNG on disk.
    FactionIconBrushes.Empty();
    const FString AtlasPath = FactionAssetFile(TEXT("Faction_Icon_Atlas_4x1.png"));
    if (FactionSelectAssetExists(AtlasPath))
    {
        for (int32 Cell = 0; Cell < 4; ++Cell)
        {
            TUniquePtr<FSlateDynamicImageBrush> IconBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*AtlasPath), FVector2D(128.0f, 128.0f));
            IconBrush->SetUVRegion(FBox2D(FVector2D(Cell * 0.25f, 0.0f), FVector2D((Cell + 1) * 0.25f, 1.0f)));
            FactionIconBrushes.Add(MoveTemp(IconBrush));
        }
    }

    // Per-faction hero art. Only some factions may have art generated yet --
    // entries for factions without a matching file are left null and hidden.
    PreviewBrushes.Empty();
    for (const FOLCFactionDisplayData& Faction : FactionData)
    {
        const FString PreviewPath = FactionAssetFile(FString::Printf(TEXT("Preview_Faction_%s.png"), *Faction.FactionId));
        if (FactionSelectAssetExists(PreviewPath))
        {
            PreviewBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*PreviewPath), FVector2D(896.0f, 384.0f)));
        }
        else
        {
            PreviewBrushes.Add(nullptr);
        }
    }
}

TSharedRef<SWidget> UOLCFactionSelectWidget::RebuildWidget()
{
    if (FactionData.IsEmpty())
    {
        PopulateDefaultFactions();
    }

    EnsureDefaultSelection();
    LoadFactionArt();

    // Frame border brushes -- reused from the Welcome Screen's art (same
    // PNGs, same 1920x1080 design canvas), loaded fresh each RebuildWidget
    // like OLCWelcomeScreenWidget does.
    FrameCornerTLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_TL.png"))), FVector2D(160.0f, 160.0f));
    FrameCornerTRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_TR.png"))), FVector2D(160.0f, 160.0f));
    FrameCornerBRBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_BR.png"))), FVector2D(160.0f, 160.0f));
    FrameCornerBLBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Corner_BL.png"))), FVector2D(160.0f, 160.0f));
    FrameEdgeTopBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Top.png"))), FVector2D(1600.0f, 64.0f));
    FrameEdgeBottomBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Bottom.png"))), FVector2D(1600.0f, 64.0f));
    FrameEdgeLeftBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Left.png"))), FVector2D(64.0f, 760.0f));
    FrameEdgeRightBrush = MakeUnique<FSlateDynamicImageBrush>(FName(*FrameAssetFile(TEXT("Frame_Edge_Right.png"))), FVector2D(64.0f, 760.0f));

    TSharedRef<SVerticalBox> ButtonList = SNew(SVerticalBox);
    FactionListBox = ButtonList;

    for (int32 Index = 0; Index < FactionData.Num(); ++Index)
    {
        ButtonList->AddSlot()
        .AutoHeight()
        .Padding(0.0f, 0.0f, 0.0f, 16.0f)
        [
            BuildFactionButton(Index)
        ];
    }

    TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas)
        // Full-bleed background art, painted behind everything else.
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
        .Offset(FMargin(150.0f, 150.0f, 1170.0f, 760.0f))
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("TitleLine1", "SELECT"))
                .ColorAndOpacity(FLinearColor::White)
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 52))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("TitleLine2", "FACTION"))
                .ColorAndOpacity(FLinearColor::White)
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 52))
            ]
        ]
        // Faction button list.
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(150.0f, 340.0f, 1210.0f, 150.0f))
        [ ButtonList ]
        // Dossier panel (preview art, stat rows, confirm button).
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(820.0f, 185.0f, 204.0f, 255.0f))
        [ BuildDossierPanel() ]
        // Back button. Sized to match Champion Selection's smaller BACK
        // button rather than the nav art's full native size.
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(150.0f, 850.0f, 1550.0f, 174.0f))
        [ BuildNavButton(LOCTEXT("Back", "BACK"), NavButtonBackBrush.Get(), FOnClicked::CreateUObject(this, &UOLCFactionSelectWidget::Back), true) ]
        // Frame edges -- identical geometry to OLCWelcomeScreenWidget (same
        // 1920x1080 design canvas), placed before the corners so the corners
        // paint on top where they overlap.
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

    // A bare SBorder here reports its own content-driven desired size to the
    // ancestor viewport slot instead of filling it -- same bug and same fix
    // as OLCWelcomeScreenWidget: a plain UUserWidget added via AddToViewport
    // does not reliably override desired size with the stretch-anchor slot
    // it's given, so without a fixed design-space size, this screen renders
    // confined to a small top-left box instead of filling the screen. The
    // SBox forces a stable 1920x1080 design size that the ancestor's DPI
    // scaler then correctly scales to the real viewport.
    return SNew(SBox)
        .WidthOverride(1920.0f)
        .HeightOverride(1080.0f)
        [ Canvas ];
}

void UOLCFactionSelectWidget::ReleaseSlateResources(bool bReleaseChildren)
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
    ButtonFactionActiveBrush.Reset();
    ButtonFactionInactiveBrush.Reset();
    PanelDossierBrush.Reset();
    SegmentStatGreenBrush.Reset();
    SegmentStatOrangeBrush.Reset();
    SegmentStatBlueBrush.Reset();
    SegmentStatRedBrush.Reset();
    NavButtonActiveBrush.Reset();
    NavButtonBackBrush.Reset();
    FactionIconBrushes.Empty();
    PreviewBrushes.Empty();
}

void UOLCFactionSelectWidget::PopulateDefaultFactions()
{
    FactionData.Reset();

    auto AddFaction = [this](const TCHAR* Id, const TCHAR* Name, int32 Mining, int32 Defense, int32 Mobility, int32 Survival)
    {
        FOLCFactionDisplayData& Data = FactionData.AddDefaulted_GetRef();
        Data.FactionId = Id;
        Data.DisplayName = Name;
        Data.MiningRating = Mining;
        Data.DefenseRating = Defense;
        Data.MobilityRating = Mobility;
        Data.SurvivalRating = Survival;
    };

    // Ratings per UE5/Assets/UI/FactionSelection/IMPLEMENTATION_GUIDE.md.
    AddFaction(TEXT("NeonPunk"), TEXT("Neon Punk"), 5, 2, 2, 2);
    AddFaction(TEXT("DarkRealistic"), TEXT("Dark Realistic"), 2, 5, 3, 3);
    AddFaction(TEXT("CartoonSciFi"), TEXT("Cartoon SciFi"), 2, 3, 5, 3);
    AddFaction(TEXT("BrightRealistic"), TEXT("Bright Realistic"), 3, 3, 3, 5);
}

void UOLCFactionSelectWidget::EnsureDefaultSelection()
{
    if (!FactionData.IsEmpty() && !FactionData.IsValidIndex(SelectedFactionIndex))
    {
        SelectedFactionIndex = 0;
    }
}

TSharedRef<SWidget> UOLCFactionSelectWidget::BuildFactionButton(int32 Index)
{
    const FOLCFactionDisplayData& Faction = FactionData[Index];
    const FSlateBrush* IconBrush = FactionIconBrushes.IsValidIndex(Index) ? FactionIconBrushes[Index].Get() : nullptr;

    // Bound attributes (not static bSelected-computed-once values): this
    // widget's RebuildWidget() only runs once when the Slate tree is first
    // created, and InvalidateLayoutAndVolatility() (called from
    // OnFactionSelected) does not force it to run again -- so anything
    // baked in as a plain value here would never reflect selection changes.
    // TAttribute lambdas are re-evaluated every frame instead, matching the
    // pattern already used for ConfirmButton's IsEnabled below.
    return SNew(SBox)
        .WidthOverride(512.0f)
        .HeightOverride(96.0f)
        [
            SNew(SButton)
            .ButtonStyle(FCoreStyle::Get(), "NoBorder")
            .ContentPadding(FMargin(0.0f))
            .OnClicked(FOnClicked::CreateUObject(this, &UOLCFactionSelectWidget::SelectFaction, Index))
            [
                SNew(SOverlay)
                + SOverlay::Slot()
                [
                    SNew(SImage)
                    .Image_Lambda([this, Index]() -> const FSlateBrush* {
                        return Index == SelectedFactionIndex ? ButtonFactionActiveBrush.Get() : ButtonFactionInactiveBrush.Get();
                    })
                ]
                + SOverlay::Slot()
                .Padding(46.0f, 0.0f, 0.0f, 0.0f)
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Center)
                [
                    SNew(SBox)
                    .WidthOverride(36.0f)
                    .HeightOverride(36.0f)
                    [
                        SNew(SImage)
                        .Image(IconBrush)
                        .ColorAndOpacity_Lambda([this, Index]() {
                            return Index == SelectedFactionIndex ? FLinearColor(0.95f, 0.55f, 0.18f) : FLinearColor(0.72f, 0.76f, 0.80f);
                        })
                    ]
                ]
                + SOverlay::Slot()
                .Padding(132.0f, 0.0f, 0.0f, 0.0f)
                .HAlign(HAlign_Left)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(Faction.DisplayName.ToUpper()))
                    .ColorAndOpacity_Lambda([this, Index]() {
                        return Index == SelectedFactionIndex ? FLinearColor(0.98f, 0.60f, 0.20f) : FLinearColor::White;
                    })
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 19))
                ]
            ]
        ];
}

TSharedRef<SWidget> UOLCFactionSelectWidget::BuildStatRow(const FText& Label, int32 Rating, const FSlateBrush* SegmentBrush) const
{
    // Segment_Stat_*.png (see ASSET_MANIFEST.md) is a single reusable tile
    // meant to be repeated N times, not a resizable fill bar -- Bar_Stat_*.
    // png was tried first, clipped to a rating-proportional width, but it
    // turned out to be a fixed illustration (a static "mostly full" bar
    // baked into the art itself), not a blank bar to crop, so every row
    // rendered identically regardless of rating no matter how the width
    // was computed. Ten segments matches the mockup's granularity; the max
    // rating is 5, so each rating point fills two segments.
    constexpr int32 TotalSegments = 10;
    constexpr int32 MaxRating = 5;
    const int32 FilledSegments = FMath::Clamp(Rating, 0, MaxRating) * (TotalSegments / MaxRating);
    const float SegmentWidth = 44.0f;
    const float SegmentHeight = 14.0f;
    const float SegmentSpacing = 3.0f;

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
        [
            SNew(SBox)
            .WidthOverride(120.0f)
            [
                SNew(STextBlock)
                .Text(Label)
                .ColorAndOpacity(FLinearColor(0.80f, 0.83f, 0.87f))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 13))
            ]
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [ SegmentsBox ];
}

TSharedRef<SWidget> UOLCFactionSelectWidget::BuildDossierPanel()
{
    // Bound attributes throughout (see the note above BuildFactionButton):
    // this content is generated once by RebuildWidget() and never rebuilt,
    // so it must react to SelectedFactionIndex changes on its own via
    // per-frame-evaluated lambdas rather than being computed once here.
    // The stat rows are the one exception -- see RefreshStatRows().
    TSharedRef<SConstraintCanvas> DossierCanvas = SNew(SConstraintCanvas)
        // Panel background/frame art (base layer -- fills/tints the whole panel).
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(0.0f))
        [
            SNew(SImage)
            .Image(PanelDossierBrush.Get())
        ]
        // "No selection" prompt.
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(40.0f, 40.0f, 40.0f, 40.0f))
        [
            SNew(STextBlock)
            .Text(LOCTEXT("NoSelection", "Select a faction to review its profile."))
            .ColorAndOpacity(FLinearColor(0.76f, 0.78f, 0.82f))
            .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
            .Visibility_Lambda([this]() {
                return FactionData.IsValidIndex(SelectedFactionIndex) ? EVisibility::Collapsed : EVisibility::Visible;
            })
        ]
        // Preview art -- inset well clear of the panel's border/chamfer
        // artwork (drawn just before this) so the border stays visible in
        // front of the image instead of being painted over by it.
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(56.0f, 56.0f, 56.0f, 300.0f))
        [
            // ScaleToFill (cover) crops centered by default -- these renders
            // put the characters' heads near the top of the source image,
            // so centering the crop cut them off. VAlign_Top anchors the
            // scaled image to the top of the box instead, so any cropped
            // overflow comes off the bottom.
            SNew(SScaleBox)
            .Stretch(EStretch::ScaleToFill)
            .VAlign(VAlign_Top)
            .Visibility_Lambda([this]() {
                return FactionData.IsValidIndex(SelectedFactionIndex) && PreviewBrushes.IsValidIndex(SelectedFactionIndex) && PreviewBrushes[SelectedFactionIndex].IsValid()
                    ? EVisibility::Visible : EVisibility::Collapsed;
            })
            [
                SNew(SImage)
                .Image_Lambda([this]() -> const FSlateBrush* {
                    return PreviewBrushes.IsValidIndex(SelectedFactionIndex) ? PreviewBrushes[SelectedFactionIndex].Get() : nullptr;
                })
            ]
        ]
        // Stat rows.
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f))
        .Offset(FMargin(40.0f, 408.0f, 40.0f, 56.0f))
        [
            SAssignNew(StatsHost, SBox)
            .Visibility_Lambda([this]() {
                return FactionData.IsValidIndex(SelectedFactionIndex) ? EVisibility::Visible : EVisibility::Collapsed;
            })
        ]
        // Confirm button. Inset well clear of the dossier panel art's
        // diagonal chamfered corner (a plain 40px margin still let the
        // chevron's arrow tip poke through the corner cut).
        + SConstraintCanvas::Slot()
        .Anchors(FAnchors(1.0f, 1.0f))
        .Alignment(FVector2D(1.0f, 1.0f))
        .Offset(FMargin(-80.0f, -60.0f, 300.0f, 56.0f))
        [
            BuildNavButton(LOCTEXT("Confirm", "CONFIRM"), NavButtonActiveBrush.Get(),
                FOnClicked::CreateUObject(this, &UOLCFactionSelectWidget::ConfirmSelection),
                TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateUObject(this, &UOLCFactionSelectWidget::CanConfirmSelection)),
                FLinearColor(0.93f, 0.43f, 0.09f, 1.0f))
        ];

    RefreshStatRows();

    return SNew(SBox)
        .WidthOverride(896.0f)
        .HeightOverride(640.0f)
        [ DossierCanvas ];
}

TSharedRef<SWidget> UOLCFactionSelectWidget::BuildNavButton(const FText& Label, const FSlateBrush* Brush, const FOnClicked& OnClicked, TAttribute<bool> IsEnabledAttr, FLinearColor TextColor) const
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

FReply UOLCFactionSelectWidget::SelectFaction(int32 Index)
{
    OnFactionSelected(Index);
    return FReply::Handled();
}

FReply UOLCFactionSelectWidget::ConfirmSelection()
{
    OnConfirmPressed();
    return FReply::Handled();
}

FReply UOLCFactionSelectWidget::Back()
{
    HandleBackPressed();
    return FReply::Handled();
}

bool UOLCFactionSelectWidget::CanConfirmSelection() const
{
    return FactionData.IsValidIndex(SelectedFactionIndex);
}

#undef LOCTEXT_NAMESPACE
