#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_FactionSelect.generated.h"

struct FSlateDynamicImageBrush;
struct FSlateBrush;

/**
 * Structure representing a single faction's display data.
 */
USTRUCT(BlueprintType)
struct FOLCFactionDisplayData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FString FactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 MiningRating = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 DefenseRating = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 MobilityRating = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 SurvivalRating = 0;
};

/**
 * Delegate for when a faction is confirmed.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactionConfirmedSignature, const FString&, FactionId);

/**
 * Delegate for navigation events (Back).
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackPressedSignature);

/**
 * UOLCFactionSelectWidget
 * 
 * Root widget for the New Campaign Faction Selection screen.
 * Handles loading faction data assets and managing child widgets (buttons, dossier, confirm bar).
 */
UCLASS()
class OURLASTCHANCE_API UOLCFactionSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Event fired when the player confirms a faction selection. */
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnFactionConfirmedSignature OnFactionConfirmed;

    /** Event fired when the player presses back. */
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBackPressedSignature OnBackPressed;

protected:
    virtual void NativeConstruct() override;
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

public:
    /**
     * Initialize the widget with faction data.
     * @param InFactions Array of faction display data to populate the UI.
     */
    UFUNCTION(BlueprintCallable, Category = "Faction Select")
    void InitFactions(const TArray<FOLCFactionDisplayData>& InFactions);

    /**
     * Handle selection of a specific faction index.
     * @param Index The index of the selected faction in the loaded list.
     */
    UFUNCTION(BlueprintCallable, Category = "Faction Select")
    void OnFactionSelected(int32 Index);

    /**
     * Handle confirmation of the currently selected faction.
     */
    UFUNCTION(BlueprintCallable, Category = "Faction Select")
    void OnConfirmPressed();

    /**
     * Handle back button press.
     */
    UFUNCTION(BlueprintCallable, Category = "Faction Select")
    void HandleBackPressed();

private:
    void PopulateDefaultFactions();
    void EnsureDefaultSelection();
    void LoadFactionArt();
    void RefreshStatRows();
    TSharedRef<SWidget> BuildFactionButton(int32 Index);
    TSharedRef<SWidget> BuildDossierPanel();
    TSharedRef<SWidget> BuildStatRow(const FText& Label, int32 Rating, const FSlateBrush* BarBrush) const;
    TSharedRef<SWidget> BuildConfirmBar();
    TSharedRef<SWidget> BuildNavButton(const FText& Label, const FSlateBrush* Brush, const FOnClicked& OnClicked, TAttribute<bool> IsEnabledAttr, FLinearColor TextColor = FLinearColor::White) const;
    FReply SelectFaction(int32 Index);
    FReply ConfirmSelection();
    FReply Back();
    bool CanConfirmSelection() const;

    /** Cached list of faction data loaded into this widget. */
    TArray<FOLCFactionDisplayData> FactionData;

    /** Index of the currently selected faction. -1 if none selected. */
    int32 SelectedFactionIndex = -1;

    TSharedPtr<class SVerticalBox> FactionListBox;
    TSharedPtr<class SBorder> DossierBox;
    TSharedPtr<class SButton> ConfirmButton;
    /** Host whose entire content is swapped out on selection change (see RefreshStatRows). */
    TSharedPtr<class SBox> StatsHost;

    /** Frame border brushes — reused from the Welcome Screen's Assets/UI/WelcomeScreen art. */
    TUniquePtr<FSlateDynamicImageBrush> FrameCornerTLBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameCornerTRBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameCornerBRBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameCornerBLBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameEdgeTopBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameEdgeBottomBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameEdgeLeftBrush;
    TUniquePtr<FSlateDynamicImageBrush> FrameEdgeRightBrush;

    /** Faction Selection art — Assets/UI/FactionSelection (see ASSET_MANIFEST.md). */
    TUniquePtr<FSlateDynamicImageBrush> BackgroundBrush;
    TUniquePtr<FSlateDynamicImageBrush> ButtonFactionActiveBrush;
    TUniquePtr<FSlateDynamicImageBrush> ButtonFactionInactiveBrush;
    TUniquePtr<FSlateDynamicImageBrush> PanelDossierBrush;
    /** Single reusable segment tiles, tiled N times per stat row (see RefreshStatRows). */
    TUniquePtr<FSlateDynamicImageBrush> SegmentStatGreenBrush;
    TUniquePtr<FSlateDynamicImageBrush> SegmentStatOrangeBrush;
    TUniquePtr<FSlateDynamicImageBrush> SegmentStatBlueBrush;
    TUniquePtr<FSlateDynamicImageBrush> SegmentStatRedBrush;
    /** Four 128px cells sliced from Faction_Icon_Atlas_4x1.png, one per faction slot. */
    TArray<TUniquePtr<FSlateDynamicImageBrush>> FactionIconBrushes;
    /** Per-faction square/hero art (Preview_Faction_<FactionId>.png). Entry is null if not yet provided. */
    TArray<TUniquePtr<FSlateDynamicImageBrush>> PreviewBrushes;

    /** Chevron nav buttons -- both reused from Champion Selection's art (Confirm/Back). */
    TUniquePtr<FSlateDynamicImageBrush> NavButtonActiveBrush;
    TUniquePtr<FSlateDynamicImageBrush> NavButtonBackBrush;
};
