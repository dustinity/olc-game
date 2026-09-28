#include "OLCGalaxyMapWidget.h"
#include "OurLastChance.h"

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
#include "Core/OLCNavigationSubsystem.h"
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
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/Galaxy System UI/Assets") / FileName);
	}

	FString NavMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/Galaxy System UI/Assets") / FileName);
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
	{
		ResourceSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();
		NavigationSubsystem = GI->GetSubsystem<UOLCNavigationSubsystem>();
	}

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

		FVector2D FromPos = Route.FromSystem->Position * CanvasSize;
		FVector2D ToPos   = Route.ToSystem->Position   * CanvasSize;

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
		.Offset(FMargin((GalaxyCenter - FVector2D(30.0f, 30.0f)).X, (GalaxyCenter - FVector2D(30.0f, 30.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
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
		FVector2D Pos = System.Position * CanvasSize;
		float Radius = System.bVisited ? 18.0f : (System.bReachable ? 14.0f : 12.0f);

		System.Position = Pos; // Store screen position for detail panel
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2D(Radius, Radius)).X, (Pos - FVector2D(Radius, Radius)).Y, 0.0f, 0.0f)).AutoSize(true)
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

	bool bCanWarp = !System.bVisited && System.bReachable && NavigationSubsystem;
	FText WarpBtnLabel = LOCTEXT("Btn_Warp", "WARP");
	if (System.bVisited)        WarpBtnLabel = LOCTEXT("Btn_Current", "CURRENT");
	else if (!System.bReachable) WarpBtnLabel = LOCTEXT("Btn_Unreachable", "UNREACHABLE");

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
					// Warp button
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 0.0f)
					[
						SNew(SButton)
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.OnClicked_Lambda([this]() -> FReply { WarpToSelectedSystem(); return FReply::Handled(); })
						.IsEnabled(bCanWarp)
						[ SNew(SBorder)
							.BorderBackgroundColor(bCanWarp ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
							.Padding(FMargin(16.0f, 8.0f))
							[ SNew(STextBlock).Text(WarpBtnLabel)
								.ColorAndOpacity(bCanWarp ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ] ]
					]
				]
			]
		];
}

void UOLCGalaxyMapWidget::InitializeGalaxyData()
{
	Systems.Reset();
	Routes.Reset();

	if (!NavigationSubsystem) return;

	const TArray<FOLCGalaxyCluster> Clusters = NavigationSubsystem->GetOrGenerateGalaxyClusters();
	const int32 CurrentID = NavigationSubsystem->GetCurrentSystemID();

	// Clusters are positioned in canvas-space spiral coordinates (see
	// UOLCNavigationSubsystem::GetOrGenerateGalaxyClusters); normalize to the
	// 0-1 range BuildGalaxyCanvas expects (it multiplies by CanvasSize itself).
	FVector2D MinPos(TNumericLimits<double>::Max(), TNumericLimits<double>::Max());
	FVector2D MaxPos(TNumericLimits<double>::Lowest(), TNumericLimits<double>::Lowest());
	for (const FOLCGalaxyCluster& Cluster : Clusters)
	{
		MinPos.X = FMath::Min(MinPos.X, (double)Cluster.Position.X);
		MinPos.Y = FMath::Min(MinPos.Y, (double)Cluster.Position.Y);
		MaxPos.X = FMath::Max(MaxPos.X, (double)Cluster.Position.X);
		MaxPos.Y = FMath::Max(MaxPos.Y, (double)Cluster.Position.Y);
	}
	const double SpanX = FMath::Max(MaxPos.X - MinPos.X, 1.0);
	const double SpanY = FMath::Max(MaxPos.Y - MinPos.Y, 1.0);

	for (const FOLCGalaxyCluster& Cluster : Clusters)
	{
		FOLCGalaxySystem System;
		System.SystemName = Cluster.ClusterName;
		System.ClusterID = Cluster.ClusterID;
		System.TIR = FMath::Clamp(1 + Cluster.ClusterID / 2, 1, 5);
		System.Position.X = (Cluster.Position.X - MinPos.X) / SpanX;
		System.Position.Y = (Cluster.Position.Y - MinPos.Y) / SpanY;
		System.DistanceFromCenter = FVector2D::Distance(System.Position, FVector2D(0.5, 0.5));
		System.bVisited = (Cluster.ClusterID == CurrentID);
		Systems.Add(System);
	}

	// Warp routes: match by ClusterName against the systems just built, and
	// mark reachable when the player can currently afford the fuel cost.
	int32 CurrentFuel = 0;
	if (ResourceSubsystem)
	{
		for (const auto& Res : ResourceSubsystem->GetResourceCounters())
			if (Res.ResourceType == EOLCResourceType::Fuel)
				CurrentFuel = FMath::RoundToInt(Res.Value);
	}

	for (const FOLCSolarWarpRoute& NavRoute : NavigationSubsystem->GetWarpRoutes())
	{
		FOLCGalaxySystem* FromSys = Systems.FindByPredicate([&](const FOLCGalaxySystem& S) { return S.SystemName.EqualTo(NavRoute.FromSystem); });
		FOLCGalaxySystem* ToSys   = Systems.FindByPredicate([&](const FOLCGalaxySystem& S) { return S.SystemName.EqualTo(NavRoute.ToSystem); });
		if (!FromSys || !ToSys) continue;

		if (FromSys->bVisited && NavRoute.FuelCost <= CurrentFuel)
			ToSys->bReachable = true;

		FOLCWarpRoute Route;
		Route.FromSystem = FromSys;
		Route.ToSystem = ToSys;
		Route.FuelCost = NavRoute.FuelCost;
		Routes.Add(Route);
	}
}

void UOLCGalaxyMapWidget::WarpToSelectedSystem()
{
	if (!NavigationSubsystem) return;
	if (SelectedSystemIndex < 0 || SelectedSystemIndex >= Systems.Num()) return;

	const FOLCGalaxySystem& Target = Systems[SelectedSystemIndex];
	if (Target.bVisited) return;
	if (!Target.bReachable) return;

	if (!NavigationSubsystem->TryPayFuelAndTravel(Target.ClusterID))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Warp to %s failed (insufficient fuel)"), *Target.SystemName.ToString());
		return;
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Warped to system: %s (cluster %d)"), *Target.SystemName.ToString(), Target.ClusterID);
	InitializeGalaxyData();
	InvalidateLayoutAndVolatility();
}

void UOLCGalaxyMapWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

#undef LOCTEXT_NAMESPACE


