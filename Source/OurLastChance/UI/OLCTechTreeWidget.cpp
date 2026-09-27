#include "OLCTechTreeWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Rendering/SlateRenderer.h"
#include "Core/OLCResearchSubsystem.h"
#include "Player/OLCMenuPlayerController.h"

#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCTechTreeWidget"

// ---------------------------------------------------------------------------
// Screen-specific asset paths (WP-13 Step 4)
// ---------------------------------------------------------------------------
namespace TechTreeAssetPath
{
	FString Base()
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Research Tech Tree/Assets"));
	}

	FString CategoryIcon(int32 Index)
	{
		return FString::Printf(TEXT("%s/Sliced/RES-CAT-%02d_*.png"),
			*TechTreeAssetPath::Base(), Index + 1);
	}

	FString StateIcon(int32 Index)
	{
		return FString::Printf(TEXT("%s/Sliced/RES-STA-%02d_*.png"),
			*TechTreeAssetPath::Base(), Index + 1);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

UOLCTechTreeWidget::UOLCTechTreeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCTechTreeWidget::RebuildWidget()
{
	// Get the research subsystem.
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		ResearchSubsystem = GI->GetSubsystem<UOLCResearchSubsystem>();
	}

	// Build nodes and edges from registered techs.
	Nodes.Reset();
	Edges.Reset();
	CategoryIconBrushes.Reset();

	if (ResearchSubsystem)
	{
		const auto& AllTechs = ResearchSubsystem->GetAllTechs();

		// Create a node for each tech.
		for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
		{
			if (!Tech) continue;

			FOLCTreeNode Node;
			Node.Tech = Tech.Get();
			Node.IncomingPrereqs = Tech->Prerequisites.Num();
			Nodes.Add(Node);
		}

		// Create edges from prerequisites.
		for (const auto& Node : Nodes)
		{
			if (!Node.Tech) continue;
			for (const TObjectPtr<UOLCTechData>& Prereq : Node.Tech->Prerequisites)
			{
				if (!Prereq) continue;
				FOLCTechEdge Edge;
				Edge.FromTech = Prereq.Get();
				Edge.ToTech = Node.Tech;
				Edges.Add(Edge);
			}
		}

		// Compute layout positions.
		CanvasSize = FVector2D(1920.0f, 1080.0f - TopBarHeight);
		RecomputeLayout();
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.005f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(0.0f))
		[
			SNew(SOverlay)
			// Top bar
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[ BuildTopBar() ]
			// Tech canvas (nodes + edges)
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			.Padding(0.0f, TopBarHeight, 0.0f, 0.0f)
			[ BuildTechCanvas() ]
			// Detail panel (right side)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(0.0f, TopBarHeight + 60.0f, 30.0f, 60.0f)
			[ BuildDetailPanel() ]
		];
}

void UOLCTechTreeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CanvasSize = MyGeometry.GetLocalSize();
}

FLinearColor UOLCTechTreeWidget::GetNodeStateColor(UOLCTechData* Tech) const
{
	if (!Tech || !ResearchSubsystem.Get()) return OLCStyleColors::TextDim; // Gray = locked

	if (ResearchSubsystem->IsTechCompleted(Tech))
		return OLCStyleColors::ValidGreen; // Green = completed

	if (Tech == ResearchSubsystem->GetCurrentResearch())
		return OLCStyleColors::TacticalBlue; // Blue = researching

	// Check if available (prerequisites met, not yet started).
	TArray<UOLCTechData*> Available = ResearchSubsystem->GetAvailableTechs();
	if (Available.Contains(Tech))
		return OLCStyleColors::PrimaryOrange; // Orange = available

	return OLCStyleColors::TextDim; // Gray = locked
}

// ---------------------------------------------------------------------------
// Category filter helper
// ---------------------------------------------------------------------------
FString UOLCTechTreeWidget::GetCategoryIconPath(EOLCTechCategory Category) const
{
	int32 Index = static_cast<int32>(Category);
	if (Index < 0 || Index >= 8) return TEXT("");

	FString BasePath = TechTreeAssetPath::Base();
	FString FileName;
	switch (Category)
	{
		case EOLCTechCategory::Weapons:    FileName = TEXT("RES-CAT-01_Weapons.png"); break;
		case EOLCTechCategory::Armor:      FileName = TEXT("RES-CAT-02_Armor.png"); break;
		case EOLCTechCategory::Drives:     FileName = TEXT("RES-CAT-03_Drives.png"); break;
		case EOLCTechCategory::Energy:     FileName = TEXT("RES-CAT-04_Energy.png"); break;
		case EOLCTechCategory::Vision:     FileName = TEXT("RES-CAT-05_VisionScanning.png"); break;
		case EOLCTechCategory::Storage:    FileName = TEXT("RES-CAT-06_Storage.png"); break;
		case EOLCTechCategory::Buildings:  FileName = TEXT("RES-CAT-07_Buildings.png"); break;
		case EOLCTechCategory::Units:      FileName = TEXT("RES-CAT-08_Units.png"); break;
		default: return TEXT("");
	}

	return FPaths::ConvertRelativePathToFull(BasePath / FileName);
}

TArray<UOLCTechData*> UOLCTechTreeWidget::GetFilteredTechs() const
{
	TArray<UOLCTechData*> Result;
	if (!ResearchSubsystem.Get()) return Result;

	const auto& AllTechs = ResearchSubsystem->GetAllTechs();
	for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
	{
		if (!Tech) continue;
		if (ActiveCategoryFilter == -1)
		{
			Result.Add(Tech.Get());
		}
		else if (static_cast<int32>(Tech->Category) == ActiveCategoryFilter)
		{
			Result.Add(Tech.Get());
		}
	}
	return Result;
}

// ---------------------------------------------------------------------------
// Tech Canvas — edges + nodes
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCTechTreeWidget::BuildTechCanvas()
{
	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas);

	// Get filtered techs for this layout pass.
	TArray<UOLCTechData*> FilteredTechs = GetFilteredTechs();

	// Build edge list from filtered nodes only.
	TArray<FOLCTechEdge> VisibleEdges;
	for (const FOLCTechEdge& Edge : Edges)
	{
		bool bFromVisible = false, bToVisible = false;
		for (UOLCTechData* T : FilteredTechs)
		{
			if (T == Edge.FromTech) bFromVisible = true;
			if (T == Edge.ToTech)   bToVisible = true;
		}
		if (bFromVisible && bToVisible)
			VisibleEdges.Add(Edge);
	}

	// Draw edges first (behind nodes).
	for (const FOLCTechEdge& Edge : VisibleEdges)
	{
		const FOLCTreeNode* FromNode = nullptr;
		const FOLCTreeNode* ToNode   = nullptr;

		for (const auto& Node : Nodes)
		{
			if (Node.Tech == Edge.FromTech) FromNode = &Node;
			if (Node.Tech == Edge.ToTech)   ToNode   = &Node;
		}

		if (!FromNode || !ToNode) continue;

		const FVector2D Start = FromNode->Position;
		const FVector2D End   = ToNode->Position;

		// Determine edge color.
		bool bFromCompleted = ResearchSubsystem && ResearchSubsystem->IsTechCompleted(Edge.FromTech);
		bool bToAvailable   = ResearchSubsystem && (ResearchSubsystem->GetAvailableTechs().Contains(Edge.ToTech) ||
		                                           ResearchSubsystem->IsTechCompleted(Edge.ToTech) ||
		                                           Edge.ToTech == ResearchSubsystem->GetCurrentResearch());

		FLinearColor EdgeColor;
		if (bFromCompleted && bToAvailable)
			EdgeColor = OLCStyleColors::ValidGreen; // Green edge = unlocked path
		else if (bFromCompleted)
			EdgeColor = FLinearColor(0.2f, 0.5f, 0.3f, 0.4f); // Dim green
		else
			EdgeColor = FLinearColor(0.15f, 0.15f, 0.18f, 0.5f); // Gray locked

		float DX = End.X - Start.X;
		float DY = End.Y - Start.Y;
		float Distance = FMath::Sqrt(DX * DX + DY * DY);

		Canvas->AddSlot()
			.Offset(FMargin((Start).X, (Start).Y, 0.0f, 0.0f)).AutoSize(true)
			[
				SNew(SBox)
				.WidthOverride(1.5f)
				.HeightOverride(Distance)
				[
					SNew(SBorder)
					.BorderBackgroundColor(EdgeColor)
					.Padding(FMargin(0.0f))
				]
			];
	}

	// Draw nodes on top of edges.
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		FOLCTreeNode& Node = Nodes[i];
		Canvas->AddSlot()
			.Offset(FMargin((Node.Position - FVector2D(Node.Radius, Node.Radius)).X, (Node.Position - FVector2D(Node.Radius, Node.Radius)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildNode(Node) ];
	}

	return Canvas;
}

// ---------------------------------------------------------------------------
// Tech node — circular with state icon overlay
// Uses RES-STA-* sliced images as circular node backgrounds, with a colored
// inner glow to indicate availability.  Intersection nodes (multiple prereqs)
// are larger and have a subtle ring highlight.
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCTechTreeWidget::BuildNode(FOLCTreeNode& Node)
{
	if (!Node.Tech) return SNew(SSpacer);

	FLinearColor StateColor = GetNodeStateColor(Node.Tech);
	bool bIsCompleted = ResearchSubsystem && ResearchSubsystem->IsTechCompleted(Node.Tech);
	bool bIsResearching = ResearchSubsystem && Node.Tech == ResearchSubsystem->GetCurrentResearch();
	bool bIsAvailable = !bIsCompleted && !bIsResearching &&
		(ResearchSubsystem ? ResearchSubsystem->GetAvailableTechs().Contains(Node.Tech) : false);

	// Node radius: larger for intersection nodes (multiple prerequisites).
	float Radius = 30.0f;
	if (Node.IncomingPrereqs > 1) Radius = 36.0f; // Intersection node highlight

	const float Diameter = Radius * 2.0f;

	// Determine state icon index: Locked=0, Available=1, Researching=2, Completed=3.
	int32 StateIconIdx = 0; // default locked
	if (bIsCompleted)       StateIconIdx = 3;
	else if (bIsResearching) StateIconIdx = 2;
	else if (bIsAvailable)  StateIconIdx = 1;

	// Build state icon brush path.
	FString StateIconPath = TechTreeAssetPath::StateIcon(StateIconIdx);
	const FSlateBrush* StateBrush = nullptr;
	if (FPaths::FileExists(StateIconPath))
	{
		CategoryIconBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*StateIconPath), FVector2D(Diameter, Diameter)));
		StateBrush = CategoryIconBrushes.Last().Get();
	}

	// Inner glow color: brighten the state color for the node fill.
	FLinearColor InnerGlow = StateColor;
	if (bIsAvailable) InnerGlow = FLinearColor(StateColor.R * 1.2f, StateColor.G * 1.2f, StateColor.B * 1.2f, 0.35f);
	else if (bIsResearching) InnerGlow = FLinearColor(StateColor.R * 1.1f, StateColor.G * 1.1f, StateColor.B * 1.1f, 0.25f);

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Node]()
		{
			for (int32 i = 0; i < Nodes.Num(); i++)
			{
				if (Nodes[i].Tech == Node.Tech)
				{
					SelectedNodeIndex = i;
					break;
				}
			}
			InvalidateLayoutAndVolatility();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.01f, 0.02f, 0.03f, 0.95f))
			.Padding(FMargin(0.0f))
			[
				SNew(SOverlay)
				// State icon background (sliced circle image).
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(Diameter)
					.HeightOverride(Diameter)
					[
						SNew(SImage)
						.Image(StateBrush)
						.ColorAndOpacity(FLinearColor::White)
					]
				]
				// Inner glow overlay.
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(Diameter * 0.7f)
					.HeightOverride(Diameter * 0.7f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(InnerGlow)
						.Padding(FMargin(0.0f))
					]
				]
				// Intersection ring highlight (for nodes with multiple prereqs).
				+ SOverlay::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(Diameter + 4.0f)
					.HeightOverride(Diameter + 4.0f)
					[
						Node.IncomingPrereqs > 1
							? SNew(SBorder)
								.BorderBackgroundColor(FLinearColor(StateColor.R, StateColor.G, StateColor.B, 0.5f))
								.Padding(FMargin(0.0f))
							: SNew(SBorder)
								.BorderBackgroundColor(FLinearColor::Transparent)
								.Padding(FMargin(0.0f))
					]
				]
			]
		];
}

// ---------------------------------------------------------------------------
// Top bar — title, available count, active research, category filters
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCTechTreeWidget::BuildTopBar()
{
	TArray<UOLCTechData*> Available = ResearchSubsystem ? ResearchSubsystem->GetAvailableTechs() : TArray<UOLCTechData*>();

	TSharedRef<SHorizontalBox> TopBar = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("TechTreeTitle", "TECH TREE"))
			.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20))
		]
		+ SHorizontalBox::Slot().AutoWidth().Padding(24.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text(FText::Format(
				FText::FromString(TEXT("{0} AVAILABLE")),
				FText::AsNumber(Available.Num())))
			.ColorAndOpacity(OLCStyleColors::TacticalBlue)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		];

	if (ResearchSubsystem && ResearchSubsystem->GetCurrentResearch())
	{
		UOLCTechData* Active = ResearchSubsystem->GetCurrentResearch();
		float Progress = ResearchSubsystem->GetResearchProgress();
		TopBar->AddSlot()
			.AutoWidth()
			.Padding(24.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(
					FText::FromString(TEXT("RESEARCHING: {0} ({1}%)")),
					Active->DisplayName,
					FText::AsNumber(FMath::RoundToInt(Progress * 100.0f))))
				.ColorAndOpacity(OLCStyleColors::TacticalBlue)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			];
	}

	// Category filter tabs.
	if (ResearchSubsystem)
	{
		const auto& AllTechs = ResearchSubsystem->GetAllTechs();

		// Collect unique categories present in registered techs.
		TArray<EOLCTechCategory> UsedCategories;
		for (const TObjectPtr<UOLCTechData>& Tech : AllTechs)
		{
			if (!Tech) continue;
			bool bFound = false;
			for (EOLCTechCategory Cat : UsedCategories)
			{
				if (Cat == Tech->Category) { bFound = true; break; }
			}
			if (!bFound) UsedCategories.Add(Tech->Category);
		}

		TSharedRef<SHorizontalBox> FilterRow = SNew(SHorizontalBox);

		// "ALL" filter button.
		const bool bAllActive = (ActiveCategoryFilter == -1);
		FilterRow->AddSlot().AutoWidth()
			.Padding(2.0f, 0.0f)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.ContentPadding(FMargin(0.0f))
				.OnClicked_Lambda([this]() -> FReply {
					ActiveCategoryFilter = -1;
					InvalidateLayoutAndVolatility();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(bAllActive ? OLCStyleColors::PrimaryOrange : FLinearColor(0.025f, 0.04f, 0.05f, 0.92f))
					.Padding(FMargin(10.0f, 6.0f))
					[
						SNew(STextBlock)
						.Text(LOCTEXT("FilterAll", "ALL"))
						.ColorAndOpacity(bAllActive ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					]
				]
			];

		// Per-category filter buttons.
		for (int32 i = 0; i < UsedCategories.Num(); i++)
		{
			EOLCTechCategory Cat = UsedCategories[i];
			const bool bCatActive = (ActiveCategoryFilter == static_cast<int32>(Cat));

			// Load category icon brush.
			FString IconPath = GetCategoryIconPath(Cat);
			const FSlateBrush* IconBrush = nullptr;
			if (FPaths::FileExists(IconPath))
			{
				CategoryIconBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*IconPath), FVector2D(18.0f, 18.0f)));
				IconBrush = CategoryIconBrushes.Last().Get();
			}

			FText CatLabel;
			switch (Cat)
			{
				case EOLCTechCategory::Weapons:    CatLabel = LOCTEXT("Filter_Weapons", "WEAPONS"); break;
				case EOLCTechCategory::Armor:      CatLabel = LOCTEXT("Filter_Armor", "ARMOR"); break;
				case EOLCTechCategory::Drives:     CatLabel = LOCTEXT("Filter_Drives", "DRIVES"); break;
				case EOLCTechCategory::Energy:     CatLabel = LOCTEXT("Filter_Energy", "ENERGY"); break;
				case EOLCTechCategory::Vision:     CatLabel = LOCTEXT("Filter_Vision", "VISION"); break;
				case EOLCTechCategory::Storage:    CatLabel = LOCTEXT("Filter_Storage", "STORAGE"); break;
				case EOLCTechCategory::Buildings:  CatLabel = LOCTEXT("Filter_Buildings", "BUILDINGS"); break;
				case EOLCTechCategory::Units:      CatLabel = LOCTEXT("Filter_Units", "UNITS"); break;
				default: CatLabel = FText::FromString(TEXT("?")); break;
			}

			const int32 CatIndex = i;
			FilterRow->AddSlot().AutoWidth()
				.Padding(2.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.ContentPadding(FMargin(0.0f))
					.OnClicked_Lambda([this, CatIndex]() -> FReply {
						ActiveCategoryFilter = CatIndex;
						InvalidateLayoutAndVolatility();
						return FReply::Handled();
					})
					[
						SNew(SBorder)
						.BorderBackgroundColor(bCatActive ? OLCStyleColors::PrimaryOrange : FLinearColor(0.025f, 0.04f, 0.05f, 0.92f))
						.Padding(FMargin(10.0f, 6.0f))
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0.0f, 0.0f, 4.0f, 0.0f)
							[
								SNew(SBox)
								.WidthOverride(18.0f)
								.HeightOverride(18.0f)
								[
									SNew(SImage)
									.Image(IconBrush)
									.ColorAndOpacity(bCatActive ? FLinearColor::White : OLCStyleColors::TextDim)
								]
							]
							+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
							[ SNew(STextBlock).Text(CatLabel).ColorAndOpacity(bCatActive ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
						]
					]
				];
		}

		TopBar->AddSlot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(24.0f, 0.0f, 16.0f, 0.0f)
			[ FilterRow ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f, 8.0f))
		[ TopBar ];
}

// ---------------------------------------------------------------------------
// Detail panel — build menu frame styling with research-specific data
// ---------------------------------------------------------------------------
TSharedRef<SWidget> UOLCTechTreeWidget::BuildDetailPanel()
{
	if (SelectedNodeIndex < 0 || SelectedNodeIndex >= Nodes.Num())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("DetailSelect", "SELECT A TECH NODE"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			];
	}

	const FOLCTreeNode& Node = Nodes[SelectedNodeIndex];
	if (!Node.Tech)
	{
		return SNew(SSpacer);
	}

	UOLCTechData* Tech = Node.Tech;
	FLinearColor StateColor = GetNodeStateColor(Tech);
	bool bIsCompleted = ResearchSubsystem && ResearchSubsystem->IsTechCompleted(Tech);
	bool bIsResearching = ResearchSubsystem && Tech == ResearchSubsystem->GetCurrentResearch();

	TArray<UOLCTechData*> Available = ResearchSubsystem ? ResearchSubsystem->GetAvailableTechs() : TArray<UOLCTechData*>();
	bool bCanBeResearched = !bIsCompleted && !bIsResearching && Available.Contains(Tech);

	FText StateText;
	if (bIsCompleted)     StateText = LOCTEXT("StateComplete", "COMPLETED");
	else if (bIsResearching) StateText = LOCTEXT("StateResearching", "RESEARCHING");
	else if (bCanBeResearched) StateText = LOCTEXT("StateAvailable", "AVAILABLE");
	else                   StateText = LOCTEXT("StateLocked", "LOCKED");

	FText CostText;
	if (Tech->MaterialCost.Num() > 0)
	{
		CostText = Tech->GetCostSummary();
	}
	else
	{
		CostText = LOCTEXT("Cost_Free", "FREE");
	}

	if (Tech->Prerequisites.Num() > 0)
	{
		FString PrereqNames;
		for (const TObjectPtr<UOLCTechData>& P : Tech->Prerequisites)
		{
			if (!P) continue;
			PrereqNames += P->DisplayName.ToString() + TEXT(", ");
		}
		// Remove trailing comma.
		if (PrereqNames.Len() > 2) PrereqNames = PrereqNames.Left(PrereqNames.Len() - 2);
		CostText = FText::FromString(*PrereqNames);
	}

	FText RingText;
	switch (Tech->RingTier)
	{
		case ERingTier::Core:   RingText = LOCTEXT("Ring_Core", "CORE"); break;
		case ERingTier::Ring1:  RingText = LOCTEXT("Ring_Ring1", "RING 1"); break;
		case ERingTier::Ring2:  RingText = LOCTEXT("Ring_Ring2", "RING 2"); break;
		case ERingTier::Ring3:  RingText = LOCTEXT("Ring_Ring3", "RING 3"); break;
		case ERingTier::Outer:  RingText = LOCTEXT("Ring_Outer", "OUTER"); break;
		default:                RingText = FText::FromString(TEXT("?")); break;
	}

	FText CategoryText;
	switch (Tech->Category)
	{
		case EOLCTechCategory::Weapons:    CategoryText = LOCTEXT("Cat_Weapons", "WEAPONS"); break;
		case EOLCTechCategory::Armor:      CategoryText = LOCTEXT("Cat_Armor", "ARMOR"); break;
		case EOLCTechCategory::Drives:     CategoryText = LOCTEXT("Cat_Drives", "DRIVES"); break;
		case EOLCTechCategory::Energy:     CategoryText = LOCTEXT("Cat_Energy", "ENERGY"); break;
		case EOLCTechCategory::Vision:     CategoryText = LOCTEXT("Cat_Vision", "VISION"); break;
		case EOLCTechCategory::Storage:    CategoryText = LOCTEXT("Cat_Storage", "STORAGE"); break;
		case EOLCTechCategory::Buildings:  CategoryText = LOCTEXT("Cat_Buildings", "BUILDINGS"); break;
		case EOLCTechCategory::Units:      CategoryText = LOCTEXT("Cat_Units", "UNITS"); break;
		default:                           CategoryText = FText::FromString(TEXT("?")); break;
	}

	FText ResearchButtonLabel;
	if (bIsCompleted)      ResearchButtonLabel = LOCTEXT("Btn_Completed", "COMPLETED");
	else if (bIsResearching) ResearchButtonLabel = LOCTEXT("Btn_Researching", "RESEARCHING...");
	else if (bCanBeResearched) ResearchButtonLabel = LOCTEXT("Btn_Research", "RESEARCH");
	else                   ResearchButtonLabel = LOCTEXT("Btn_Locked", "LOCKED");

	bool bCanClick = bCanBeResearched;

	// Category icon for the detail panel.
	FString CatIconPath = GetCategoryIconPath(Tech->Category);
	const FSlateBrush* DetailCatIcon = nullptr;
	if (FPaths::FileExists(CatIconPath))
	{
		CategoryIconBrushes.Add(MakeUnique<FSlateDynamicImageBrush>(FName(*CatIconPath), FVector2D(32.0f, 32.0f)));
		DetailCatIcon = CategoryIconBrushes.Last().Get();
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[
			SNew(SOverlay)
			// Top accent bar
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[
				SNew(SColorBlock).Color(StateColor).Size(FVector2D(1.0f, 3.0f))
			]
			// Content
			+ SOverlay::Slot()
			.Padding(FMargin(0.0f, 12.0f, 0.0f, 0.0f))
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					// Tech name + category icon
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 2.0f, 8.0f, 0.0f)
						[
							SNew(SBox)
							.WidthOverride(32.0f)
							.HeightOverride(32.0f)
							[
								SNew(SImage).Image(DetailCatIcon)
							]
						]
						+ SHorizontalBox::Slot().AutoWidth()
						[
							SNew(STextBlock)
							.Text(Tech->DisplayName)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
						]
					]
					// State badge
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 4.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(StateColor)
						.Padding(FMargin(8.0f, 3.0f))
						[
							SNew(STextBlock)
							.Text(StateText)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
					]
					// Ring + Category row
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f)
						[ SNew(STextBlock).Text(FText::FromString(TEXT("RING"))).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
						[ SNew(STextBlock).Text(RingText).ColorAndOpacity(StateColor).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 4.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f)
						[ SNew(STextBlock).Text(FText::FromString(TEXT("CATEGORY"))).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
						[ SNew(STextBlock).Text(CategoryText).ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					]
					// Prerequisites / Cost
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("PREREQUISITES:")))
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 8.0f)
					[
						SNew(STextBlock)
						.Text(CostText)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.AutoWrapText(true)
					]
					// Effect description
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("EFFECT:")))
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 8.0f)
					[
						SNew(STextBlock)
						.Text(Tech->EffectDescription.IsEmpty() ? FText::FromString(TEXT("-")) : Tech->EffectDescription)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.AutoWrapText(true)
					]
					// Research time
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 4.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().FillWidth(1.0f)
						[ SNew(STextBlock).Text(FText::FromString(TEXT("TIME"))).ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
						+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
						[
							SNew(STextBlock)
							.Text(FText::Format(
								FText::FromString(TEXT("{0}m {1}s")),
								FText::AsNumber(FMath::FloorToInt(Tech->ResearchTimeSeconds / 60.0f)),
								FText::AsNumber(FMath::RoundToInt(Tech->ResearchTimeSeconds - FMath::FloorToInt(Tech->ResearchTimeSeconds / 60.0f) * 60))))
							.ColorAndOpacity(OLCStyleColors::TextDim)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						]
					]
					// Research button — styled like build menu frame
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 0.0f)
					[
						SNew(SButton)
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.OnClicked_Lambda([this]() -> FReply {
							if (ResearchSubsystem.Get() && SelectedNodeIndex >= 0 && SelectedNodeIndex < Nodes.Num())
							{
								ResearchSubsystem->StartResearch(Nodes[SelectedNodeIndex].Tech);
								InvalidateLayoutAndVolatility();
							}
							return FReply::Handled();
						})
						.IsEnabled(bCanClick)
						[
							SNew(SBorder)
							.BorderBackgroundColor(bCanClick ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
							.Padding(FMargin(16.0f, 8.0f))
							[
								SNew(STextBlock)
								.Text(ResearchButtonLabel)
								.ColorAndOpacity(bCanClick ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
							]
						]
					]
				]
			]
		];
}

void UOLCTechTreeWidget::RecomputeLayout()
{
	if (Nodes.IsEmpty()) return;

	// Group nodes by ring tier.
	TMap<int32, TArray<FOLCTreeNode*>> RingGroups; // ring index → nodes
	for (auto& Node : Nodes)
	{
		int32 RingIdx = Node.Tech ? Node.Tech->GetRingTierIndex() : 0;
		if (!RingGroups.Contains(RingIdx)) RingGroups.Add(RingIdx);
		RingGroups[RingIdx].Add(&Node);
	}

	// Layout each ring horizontally, rings stacked bottom-up.
	float CanvasH = CanvasSize.Y;
	float CanvasW = CanvasSize.X;
	float StartY = TopBarHeight + 40.0f; // Bottom of canvas area

	for (int32 RingIdx = 0; RingIdx <= 4; RingIdx++)
	{
		if (!RingGroups.Contains(RingIdx)) continue;

		TArray<FOLCTreeNode*>& RingNodes = RingGroups[RingIdx];
		float Y = StartY + RingIdx * RingSpacing; // Core at bottom, Outer at top

		float TotalWidth = (RingNodes.Num() - 1) * NodeSpacing;
		float StartX = (CanvasW - TotalWidth) / 2.0f;

		for (int32 i = 0; i < RingNodes.Num(); i++)
		{
			RingNodes[i]->Position = FVector2D(StartX + i * NodeSpacing, Y);
		}
	}
}

void UOLCTechTreeWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	NodeBackgroundBrush.Reset();
	CategoryIconBrushes.Reset();
}

#undef LOCTEXT_NAMESPACE


