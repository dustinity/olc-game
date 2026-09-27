#include "OLCGalaxyMapWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Player/OLCMenuPlayerController.h"

#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCGalaxyMapWidget"

// ---------------------------------------------------------------------------
// Screen-specific asset paths (WP-13 Step 4)
// ---------------------------------------------------------------------------
namespace GalaxyAssetPath
{
	FString PlanetMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Galaxy System UI/Assets") / FileName);
	}

	FString NavMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Galaxy System UI/Assets") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

UOLCGalaxyMapWidget::UOLCGalaxyMapWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCGalaxyMapWidget::RebuildWidget()
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
		ResourceSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();

	InitializeGalaxyData();

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
			// Galaxy canvas
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[ BuildGalaxyCanvas() ]
			// Detail panel (right side)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(0.0f, 80.0f, 30.0f, 80.0f)
			[ BuildDetailPanel() ]
		];
}

void UOLCGalaxyMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CanvasSize = MyGeometry.GetLocalSize();
	GalaxyCenter = CanvasSize / 2.0f;
}

TSharedRef<SWidget> UOLCGalaxyMapWidget::BuildGalaxyCanvas()
{
	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas);

	// Draw warp routes first (behind systems).
	for (const auto& Route : Routes)
	{
		if (!Route.FromSystem || !Route.ToSystem) continue;

		FVector2d FromPos = Route.FromSystem->Position * CanvasSize;
		FVector2d ToPos   = Route.ToSystem->Position   * CanvasSize;

		float DX = ToPos.X - FromPos.X;
		float DY = ToPos.Y - FromPos.Y;
		float Distance = FMath::Sqrt(DX * DX + DY * DY);

		bool bFromVisited  = Route.FromSystem->bVisited;
		bool bToReachable  = Route.ToSystem->bReachable;

		FLinearColor RouteColor;
		if (bFromVisited && bToReachable)
			RouteColor = FLinearColor(0.2f, 0.5f, 0.3f, 0.6f); // Green reachable route
		else if (bFromVisited)
			RouteColor = FLinearColor(0.15f, 0.15f, 0.18f, 0.4f); // Dim gray explored but not reachable
		else
			RouteColor = FLinearColor(0.1f, 0.1f, 0.12f, 0.3f); // Gray unexplored

		Canvas->AddSlot()
			.Offset(FMargin((FromPos).X, (FromPos).Y, 0.0f, 0.0f)).AutoSize(true)
			[
				SNew(SBox)
				.WidthOverride(1.5f)
				.HeightOverride(Distance)
			];
	}

	// Draw galaxy center (objective marker).
	Canvas->AddSlot()
		.Offset(FMargin((GalaxyCenter - FVector2d(30.0f, 30.0f)).X, (GalaxyCenter - FVector2d(30.0f, 30.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
		[
			SNew(SBox)
			.WidthOverride(60.0f)
			.HeightOverride(60.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(1.0f, 0.3f, 0.1f, 0.8f))
				.Padding(FMargin(0.0f))
				[ SNew(SBox).WidthOverride(60.0f).HeightOverride(60.0f) ]
			]
		];

	// Draw system nodes.
	for (int32 i = 0; i < Systems.Num(); i++)
	{
		auto& System = Systems[i];
		FVector2d Pos = System.Position * CanvasSize;
		float Radius = System.bVisited ? 18.0f : (System.bReachable ? 14.0f : 12.0f);

		System.Position = Pos; // Store screen position for detail panel
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2d(Radius, Radius)).X, (Pos - FVector2d(Radius, Radius)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildSystemNode(System) ];
	}

	return Canvas;
}

TSharedRef<SWidget> UOLCGalaxyMapWidget::BuildSystemNode(FOLCGalaxySystem& System)
{
	const float Diameter = System.bVisited ? 36.0f : (System.bReachable ? 28.0f : 24.0f);

	FLinearColor StateColor;
	if (System.bVisited)
		StateColor = OLCStyleColors::ValidGreen; // Green = visited
	else if (System.bReachable)
		StateColor = OLCStyleColors::PrimaryOrange; // Orange = reachable
	else
		StateColor = FLinearColor(0.3f, 0.3f, 0.35f, 0.9f); // Gray = locked

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, System]() -> FReply {
			for (int32 i = 0; i < Systems.Num(); i++)
			{
				if (Systems[i].SystemName.EqualTo(System.SystemName))
				{
					SelectedSystemIndex = i;
					break;
				}
			}
			InvalidateLayoutAndVolatility();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(StateColor)
			.Padding(FMargin(0.0f))
			[ SNew(SBox).WidthOverride(Diameter).HeightOverride(Diameter) ]
		];
}

TSharedRef<SWidget> UOLCGalaxyMapWidget::BuildTopBar()
{
	TArray<FOLCResourceCounterViewData> Resources;
	if (ResourceSubsystem)
		Resources = ResourceSubsystem->GetResourceCounters();

	FText FuelText = FText::FromString(TEXT("0 / 500"));
	for (const auto& Res : Resources)
	{
		if (Res.ResourceType == EOLCResourceType::Fuel)
			FuelText = FText::Format(FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber(FMath::RoundToInt(Res.Value)),
				FText::AsNumber(FMath::RoundToInt(Res.Capacity)));
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(LOCTEXT("GalaxyMapTitle", "GALAXY MAP"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange).Font(FCoreStyle::GetDefaultFontStyle("Bold", 20)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::FromString(TEXT("FUEL:")))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(FuelText)
				.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]() -> FReply {
					if (APlayerController* PC = GetOwningPlayer())
						if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PC))
							MenuPC->OpenSolarSystem();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::PrimaryOrange)
					.Padding(FMargin(14.0f, 8.0f))
					[ SNew(STextBlock).Text(LOCTEXT("BtnSolarSystem", "← ZOOM IN"))
						.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				]
			]
		];
}

TSharedRef<SWidget> UOLCGalaxyMapWidget::BuildDetailPanel()
{
	if (SelectedSystemIndex < 0 || SelectedSystemIndex >= Systems.Num())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("DetailSelect", "SELECT A SYSTEM"))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	const FOLCGalaxySystem& System = Systems[SelectedSystemIndex];

	FText StateText;
	if (System.bVisited)      StateText = LOCTEXT("State_Visited", "VISITED");
	else if (System.bReachable) StateText = LOCTEXT("State_Reachable", "REACHABLE");
	else                      StateText = LOCTEXT("State_Locked", "LOCKED");

	FLinearColor StateColor;
	if (System.bVisited)      StateColor = OLCStyleColors::ValidGreen;
	else if (System.bReachable) StateColor = OLCStyleColors::PrimaryOrange;
	else                      StateColor = OLCStyleColors::TextDim;

	// Find connected routes.
	FString ConnectedSystems;
	for (const auto& Route : Routes)
	{
		if (Route.FromSystem == &System && Route.ToSystem)
		{
			if (!ConnectedSystems.IsEmpty()) ConnectedSystems += TEXT(", ");
			ConnectedSystems += Route.ToSystem->SystemName.ToString();
		}
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[
			SNew(SOverlay)
			+ SOverlay::Slot().VAlign(VAlign_Top).HAlign(HAlign_Fill)
			[ SNew(SColorBlock).Color(StateColor).Size(FVector2D(1.0f, 3.0f)) ]
			+ SOverlay::Slot().Padding(FMargin(0.0f, 12.0f, 0.0f, 0.0f))
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SVerticalBox)
					// System name
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(System.SystemName)
						.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ]
					// State badge
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 4.0f)
					[ SNew(SBorder).BorderBackgroundColor(StateColor).Padding(FMargin(8.0f, 3.0f))
						[ SNew(STextBlock).Text(StateText)
							.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ] ]
					// TIR
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("TIR: {0}")), FText::AsNumber(System.TIR)))
						.ColorAndOpacity(OLCStyleColors::WarningYellow).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
					// Distance from center
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("DISTANCE: {0:P1} system units")), System.DistanceFromCenter))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
					// Connected systems
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("CONNECTED:")))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(FText::FromString(*ConnectedSystems))
						.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						.AutoWrapText(true) ]
				]
			]
		];
}

void UOLCGalaxyMapWidget::InitializeGalaxyData()
{
	Systems.Reset();
	Routes.Reset();

	// Generate galaxy systems in a ring pattern around center.
	TArray<FText> SystemNames = {
		FText::FromString(TEXT("Home System")),     // Current system (center-ish)
		FText::FromString(TEXT("Kepler Reach")),     // Ring 1
		FText::FromString(TEXT("Vega Frontier")),    // Ring 1
		FText::FromString(TEXT("Orion Belt")),       // Ring 2
		FText::FromString(TEXT("Cygnus Deep")),      // Ring 2
		FText::FromString(TEXT("Lyra Outpost")),     // Ring 3
		FText::FromString(TEXT("Draco Core")),       // Ring 3
		FText::FromString(TEXT("Andromeda Gate")),   // Ring 4
		FText::FromString(TEXT("Galactic Center"))   // Objective (center)
	};

	TArray<float> Distances = { 0.15f, 0.25f, 0.30f, 0.40f, 0.45f, 0.55f, 0.60f, 0.70f, 0.85f };
	TArray<int32> TIRs = { 1, 1, 2, 2, 3, 3, 4, 4, 5 };

	// Position systems in a spiral/ring pattern.
	float AngleStep = 2.0f * PI / (SystemNames.Num() - 1);
	for (int32 i = 0; i < SystemNames.Num(); i++)
	{
		FOLCGalaxySystem System;
		System.SystemName = SystemNames[i];
		System.DistanceFromCenter = Distances[i];
		System.TIR = TIRs[i];

		if (i == 0)
		{
			// Home system near center.
			System.Position = FVector2d(0.15f, 0.15f);
		}
		else
		{
			// Spiral pattern.
			float Angle = (i - 1) * AngleStep;
			System.Position.X = 0.5f + FMath::Cos(Angle) * System.DistanceFromCenter * 0.8f;
			System.Position.Y = 0.5f + FMath::Sin(Angle) * System.DistanceFromCenter * 0.8f;
		}

		// Home system is visited and reachable.
		if (i == 0)
		{
			System.bVisited = true;
			System.bReachable = true;
		}
		else if (i <= 2)
		{
			// Ring 1 systems are reachable from home.
			System.bReachable = true;
		}

		Systems.Add(System);
	}

	// Create warp routes between adjacent systems.
	for (int32 i = 0; i < Systems.Num() - 1; i++)
	{
		FOLCWarpRoute Route;
		Route.FromSystem = &Systems[i];
		Route.ToSystem   = &Systems[i + 1];
		Route.FuelCost = 500 * (i + 1); // Scales with distance

		Routes.Add(Route);

		// Also add reverse route.
		FOLCWarpRoute ReverseRoute;
		ReverseRoute.FromSystem = &Systems[i + 1];
		ReverseRoute.ToSystem   = &Systems[i];
		ReverseRoute.FuelCost = 500 * (i + 1);

		Routes.Add(ReverseRoute);
	}

	// Add some cross-routes for variety.
	if (Systems.Num() >= 4)
	{
		FOLCWarpRoute CrossRoute;
		CrossRoute.FromSystem = &Systems[0];
		CrossRoute.ToSystem   = &Systems[3];
		CrossRoute.FuelCost = 800;
		Routes.Add(CrossRoute);
	}
}

void UOLCGalaxyMapWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

#undef LOCTEXT_NAMESPACE


