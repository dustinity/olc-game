#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCTechData.h"
#include "Core/OLCResourceTypes.h"
#include "OLCTechTreeWidget.generated.h"

class SBorder;
class SWidget;
class UOLCResearchSubsystem;

/** A single node position computed during layout pass. */
USTRUCT()
struct FOLCTreeNode
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UOLCTechData> Tech = nullptr;

	/** Computed screen position (center of circle). */
	FVector2D Position = FVector2D::ZeroVector;

	/** Radius of the node circle. */
	float Radius = 30.0f;

	/** How many incoming prerequisite edges this node has. */
	int32 IncomingPrereqs = 0;

	FOLCTreeNode() {}
};

/** An edge connecting two nodes (prerequisite → dependent). */
USTRUCT()
struct FOLCTechEdge
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UOLCTechData> FromTech = nullptr;

	UPROPERTY()
	TObjectPtr<UOLCTechData> ToTech = nullptr;

	FOLCTechEdge() {}
};

/**
 * S05 Tech Tree UI screen — Anno 1800-style convergence tree.
 * Bottom-up layout: Core ring at bottom, Outer ring at top.
 * Clickable circles connected by curved lines, color-coded by state.
 * Category filter tabs in top bar narrow visible nodes by tech category.
 */
UCLASS()
class OURLASTCHANCE_API UOLCTechTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCTechTreeWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Get the visual state color for a tech node. */
	FLinearColor GetNodeStateColor(UOLCTechData* Tech) const;

	/** Build the background canvas with edges and nodes. */
	TSharedRef<SWidget> BuildTechCanvas();

	/** Build a single tech node as an SButton (clickable). */
	TSharedRef<SWidget> BuildNode(FOLCTreeNode& Node);

	/** Build the top bar with title, available count, active research, and category filters. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the right-side detail panel for selected tech. */
	TSharedRef<SWidget> BuildDetailPanel();

	/** Recompute node positions (bottom-up layout). */
	void RecomputeLayout();

private:
	/** Get category icon path for a given category enum. */
	FString GetCategoryIconPath(EOLCTechCategory Category) const;

	/** Get the filtered tech list based on current category filter. */
	TArray<UOLCTechData*> GetFilteredTechs() const;

	/** The research subsystem providing data. */
	UPROPERTY()
	TObjectPtr<UOLCResearchSubsystem> ResearchSubsystem;

	/** Computed nodes for the current layout pass. */
	TArray<FOLCTreeNode> Nodes;

	/** Edges between nodes (prerequisite → dependent). */
	TArray<FOLCTechEdge> Edges;

	/** Index of currently selected node (-1 = none). */
	int32 SelectedNodeIndex = -1;

	/** Currently active category filter index (-1 = all categories). */
	int32 ActiveCategoryFilter = -1;

	/** Cached brushes for category icon images. */
	TArray<TUniquePtr<FSlateDynamicImageBrush>> CategoryIconBrushes;

	/** Cached brush for the node background image. */
	TUniquePtr<FSlateDynamicImageBrush> NodeBackgroundBrush;

	/** Screen dimensions from last layout pass. */
	FVector2D CanvasSize = FVector2D(1920.0f, 1080.0f);

	/** Layout constants. */
	float RingSpacing = 160.0f;   // Vertical spacing between rings
	float NodeSpacing = 140.0f;   // Horizontal spacing within a ring
	float TopBarHeight = 70.0f;
};
