#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCChampionData.h"
#include "OLCChampionSelectWidget.generated.h"

class SVerticalBox;
class SBox;
struct FSlateDynamicImageBrush;
struct FSlateBrush;

UCLASS()
class OURLASTCHANCE_API UOLCChampionSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeForFaction(const FString& InFactionId);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void BuildCatalog();
	void EnsureDefaultSelection();
	void LoadChampionArt();
	void RefreshDossier();
	TSharedRef<SWidget> BuildChampionCard(int32 Index);
	TSharedRef<SWidget> BuildStatRow(const FText& Label, int32 Rating, const FSlateBrush* SegmentBrush, const FSlateBrush* IconBrush) const;
	TSharedRef<SWidget> BuildNavButton(const FText& Label, const FSlateBrush* Brush, const FOnClicked& OnClicked, TAttribute<bool> IsEnabledAttr, FLinearColor TextColor = FLinearColor::White) const;
	FReply Select(int32 Index);
	FReply Confirm();
	FReply Back();

	UPROPERTY(Transient)
	TArray<TObjectPtr<UOLCChampionData>> Champions;

	FString FactionId;
	int32 Selected = INDEX_NONE;

	TSharedPtr<class SVerticalBox> ChampionListBox;
	TSharedPtr<class SBox> DossierHost;

	/** Frame border brushes — reused from the Welcome Screen's Assets/UI/WelcomeScreen art. */
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerTLBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerTRBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerBRBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameCornerBLBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeTopBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeBottomBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeLeftBrush;
	TUniquePtr<FSlateDynamicImageBrush> FrameEdgeRightBrush;

	/** Champion Selection art — Assets/UI/ChampionSelection (see ASSET_MANIFEST.md). */
	TUniquePtr<FSlateDynamicImageBrush> BackgroundBrush;
	TUniquePtr<FSlateDynamicImageBrush> ButtonChampionActiveBrush;
	TUniquePtr<FSlateDynamicImageBrush> ButtonChampionInactiveBrush;
	TUniquePtr<FSlateDynamicImageBrush> PanelDossierBrush;
	/** Four 128px cells sliced from Champion_Ability_Icon_Atlas_4x1.png. */
	TArray<TUniquePtr<FSlateDynamicImageBrush>> AbilityIconBrushes;
	/** Per-champion card portrait (Portrait_<ChampionId>.png), same order as Champions. */
	TArray<TUniquePtr<FSlateDynamicImageBrush>> PortraitBrushes;
	/** Per-champion dossier full-body art (FullBody_<ChampionId>.png), same order as Champions. */
	TArray<TUniquePtr<FSlateDynamicImageBrush>> FullBodyBrushes;

	/** Stat segment tiles — reused from the Faction Selection art (same style, no Champion-specific set was provided). */
	TUniquePtr<FSlateDynamicImageBrush> SegmentStatGreenBrush;
	TUniquePtr<FSlateDynamicImageBrush> SegmentStatOrangeBrush;
	TUniquePtr<FSlateDynamicImageBrush> SegmentStatBlueBrush;
	TUniquePtr<FSlateDynamicImageBrush> SegmentStatRedBrush;

    /** Chevron nav buttons -- both this screen's own art (Button_BeginCrash.png / Button_Back.png). */
	TUniquePtr<FSlateDynamicImageBrush> NavButtonActiveBrush;
	TUniquePtr<FSlateDynamicImageBrush> NavButtonBackBrush;
};
