#include "OLCSolarSystemWidget.h"

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

#define LOCTEXT_NAMESPACE "OLCSolarSystemWidget"

// ---------------------------------------------------------------------------
// Screen-specific asset paths (WP-13 Step 4)
// ---------------------------------------------------------------------------
namespace SolarAssetPath
{
	FString PlanetMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Solar System UI/Assets") / FileName);
	}

	FString NavMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Solar System UI/Assets") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

UOLCSolarSystemWidget::UOLCSolarSystemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCSolarSystemWidget::RebuildWidget()
{
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
		ResourceSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();

	InitializeSolarSystemData();

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
			// Orbital canvas
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[ BuildOrbitalCanvas() ]
			// Detail panel (right side)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(0.0f, 80.0f, 30.0f, 80.0f)
			[ BuildDetailPanel() ]
		];
}

void UOLCSolarSystemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CanvasSize = MyGeometry.GetLocalSize();
	SunPosition = CanvasSize / 2.0f;

	if (bIsScanning && ResourceSubsystem)
	{
		ScanProgress += InDeltaTime;
		if (ScanProgress >= ScanDuration)
		{
			bIsScanning = false;
			ScanProgress = 0.0f;
			if (SelectedPlanetIndex >= 0 && SelectedPlanetIndex < Planets.Num())
			{
				Planets[SelectedPlanetIndex].bScanned = true;
				const FText Biomes[] = {
					FText::FromString(TEXT("Desert")), FText::FromString(TEXT("Dusty")),
					FText::FromString(TEXT("Rocky")), FText::FromString(TEXT("Water")),
					FText::FromString(TEXT("Swamp")), FText::FromString(TEXT("Jungle")),
					FText::FromString(TEXT("LightSnow")), FText::FromString(TEXT("Ice"))
				};
				const int32 BiomeIdx = (SelectedPlanetIndex * 3 + 7) % UE_ARRAY_COUNT(Biomes);
				Planets[SelectedPlanetIndex].BiomeType = Biomes[BiomeIdx];

				FString ResStr;
				if (SelectedPlanetIndex % 2 == 0)
					ResStr = TEXT("Minerals, Construction Material");
				else if (SelectedPlanetIndex % 3 == 0)
					ResStr = TEXT("Fuel, Energy Cells");
				else
					ResStr = TEXT("Minerals, Hull Parts, Survival");
				Planets[SelectedPlanetIndex].Resources = FText::FromString(ResStr);

				UE_LOG(LogTemp, Display, TEXT("[OLC] Planet scanned: %s — Biome: %s, Resources: %s"),
					*Planets[SelectedPlanetIndex].PlanetName.ToString(),
					*Planets[SelectedPlanetIndex].BiomeType.ToString(),
					*Planets[SelectedPlanetIndex].Resources.ToString());

				InvalidateLayoutAndVolatility();
			}
		}
	}
}

TSharedRef<SWidget> UOLCSolarSystemWidget::BuildOrbitalCanvas()
{
	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas);

	// Draw orbital rings.
	for (int32 i = 0; i < Planets.Num(); i++)
	{
		const auto& Planet = Planets[i];
		float RingRadius = Planet.OrbitRadius * FMath::Min(CanvasSize.X / 1920.0f, CanvasSize.Y / 1080.0f) * 0.35f;

		Canvas->AddSlot()
			.Offset(FMargin((FVector2d::ZeroVector).X, (FVector2d::ZeroVector).Y, 0.0f, 0.0f)).AutoSize(true)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.08f, 0.1f, 0.12f, 0.35f))
				.Padding(FMargin(0.0f))
			];
	}

	// Draw sun at center.
	Canvas->AddSlot()
		.Offset(FMargin((SunPosition - FVector2d(40.0f, 40.0f)).X, (SunPosition - FVector2d(40.0f, 40.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
		[
			SNew(SBox)
			.WidthOverride(80.0f)
			.HeightOverride(80.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(1.0f, 0.75f, 0.2f, 0.9f))
				.Padding(FMargin(0.0f))
				[ SNew(SBox).WidthOverride(80.0f).HeightOverride(80.0f) ]
			]
		];

	// Draw planets on their orbits.
	for (int32 i = 0; i < Planets.Num(); i++)
	{
		auto& Planet = Planets[i];
		float RingRadius = Planet.OrbitRadius * FMath::Min(CanvasSize.X / 1920.0f, CanvasSize.Y / 1080.0f) * 0.35f;
		float RadAngle = Planet.OrbitAngle * PI / 180.0f;
		FVector2d PlanetPos(SunPosition.X + FMath::Cos(RadAngle) * RingRadius,
		                     SunPosition.Y + FMath::Sin(RadAngle) * RingRadius);

		Planet.Position = PlanetPos;
		Canvas->AddSlot()
			.Offset(FMargin((PlanetPos - FVector2d(Planet.Radius, Planet.Radius)).X, (PlanetPos - FVector2d(Planet.Radius, Planet.Radius)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildPlanetNode(Planet) ];
	}

	// Draw stations.
	for (const auto& Station : Stations)
	{
		Canvas->AddSlot()
			.Offset(FMargin((Station.Position - FVector2d(10.0f, 10.0f)).X, (Station.Position - FVector2d(10.0f, 10.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildStationMarker(Station) ];
	}

	return Canvas;
}

TSharedRef<SWidget> UOLCSolarSystemWidget::BuildPlanetNode(FOLCPlanetInfo& Planet)
{
	const float Diameter = Planet.Radius * 2.0f;

	FLinearColor StateColor;
	if (Planet.bIsCurrentPlanet)
		StateColor = OLCStyleColors::ValidGreen;
	else if (Planet.bScanned)
		StateColor = OLCStyleColors::TacticalBlue;
	else
		StateColor = FLinearColor(0.3f, 0.3f, 0.35f, 0.9f);

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Planet]() -> FReply {
			for (int32 i = 0; i < Planets.Num(); i++)
			{
				if (Planets[i].PlanetName.EqualTo(Planet.PlanetName))
				{
					SelectedPlanetIndex = i;
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

TSharedRef<SWidget> UOLCSolarSystemWidget::BuildStationMarker(const FOLCStationInfo& Station)
{
	FLinearColor Color = Station.bVisited
		? OLCStyleColors::TextDim
		: OLCStyleColors::PrimaryOrange;

	return SNew(SBorder)
		.BorderBackgroundColor(Color)
		.Padding(FMargin(0.0f))
		[ SNew(SBox).WidthOverride(20.0f).HeightOverride(20.0f) ];
}

TSharedRef<SWidget> UOLCSolarSystemWidget::BuildTopBar()
{
	TArray<FOLCResourceCounterViewData> Resources;
	if (ResourceSubsystem)
		Resources = ResourceSubsystem->GetResourceCounters();

	FText FuelText = FText::FromString(TEXT("0 / 500"));
	FText EnergyText = FText::FromString(TEXT("0"));
	for (const auto& Res : Resources)
	{
		if (Res.ResourceType == EOLCResourceType::Fuel)
			FuelText = FText::Format(FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber(FMath::RoundToInt(Res.Value)),
				FText::AsNumber(FMath::RoundToInt(Res.Capacity)));
		if (Res.ResourceType == EOLCResourceType::Energy)
			EnergyText = FText::Format(FText::FromString(TEXT("{0}")),
				FText::AsNumber(FMath::RoundToInt(Res.Value)));
	}

	const FText DriveLabel = GetDriveStatusText();

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(LOCTEXT("SolarSystemTitle", "SOLAR SYSTEM"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange).Font(FCoreStyle::GetDefaultFontStyle("Bold", 20)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::FromString(TEXT("FUEL:")))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(FuelText)
				.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(20.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::FromString(TEXT("ENERGY:")))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(EnergyText)
				.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(20.0f, 0.0f, 30.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::FromString(TEXT("DRIVE:")))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(DriveLabel)
				.ColorAndOpacity(OLCStyleColors::WarningYellow).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]() -> FReply {
					if (APlayerController* PC = GetOwningPlayer())
						if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PC))
							MenuPC->OpenGalaxyMap();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::PrimaryOrange)
					.Padding(FMargin(14.0f, 8.0f))
					[ SNew(STextBlock).Text(LOCTEXT("BtnGalaxyMap", "ZOOM OUT → GALAXY"))
						.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				]
			]
		];
}

FText UOLCSolarSystemWidget::GetDriveStatusText() const
{
	if (!ResourceSubsystem)
		return LOCTEXT("DriveUnknown", "UNKNOWN");

	const EOLCModuleState Status = ResourceSubsystem->GetDriveStatus();
	const int32 Tier = ResourceSubsystem->GetDriveTier();
	const int32 MaxRange = ResourceSubsystem->GetMaxReachableFuelCost();

	FText StatusLabel;
	switch (Status)
	{
		case EOLCModuleState::Installed:  StatusLabel = LOCTEXT("DriveOperational", "OPERATIONAL"); break;
		case EOLCModuleState::Damaged:    StatusLabel = LOCTEXT("DriveDamaged", "DAMAGED"); break;
		case EOLCModuleState::Offline:    StatusLabel = LOCTEXT("DriveOffline", "OFFLINE"); break;
	}

	return FText::Format(
		FText::FromString(TEXT("{0} T{1} (max {2} fuel)")),
		StatusLabel,
		FText::AsNumber(Tier),
		FText::AsNumber(MaxRange));
}

TArray<FOLCBadgeViewData> UOLCSolarSystemWidget::GetShipStateBadges() const
{
	TArray<FOLCBadgeViewData> Badges;

	if (!ResourceSubsystem)
		return Badges;

	// Shield badge.
	const EOLCModuleState ShieldStatus = ResourceSubsystem->GetShieldStatus();
	const float ShieldIntegrity = ResourceSubsystem->GetShieldIntegrity();
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

	// Drive badge.
	const EOLCModuleState DriveStatus = ResourceSubsystem->GetDriveStatus();
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

	return Badges;
}

TSharedRef<SWidget> UOLCSolarSystemWidget::BuildDetailPanel()
{
	if (SelectedPlanetIndex < 0 || SelectedPlanetIndex >= Planets.Num())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("DetailSelect", "SELECT A PLANET"))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	const FOLCPlanetInfo& Planet = Planets[SelectedPlanetIndex];

	FText StateText;
	if (Planet.bIsCurrentPlanet)      StateText = LOCTEXT("State_Current", "CURRENT PLANET");
	else if (Planet.bScanned)         StateText = LOCTEXT("State_Scanned", "SCANNED");
	else                              StateText = LOCTEXT("State_Unscanned", "UNSCANNED");

	FLinearColor StateColor;
	if (Planet.bIsCurrentPlanet)      StateColor = OLCStyleColors::ValidGreen;
	else if (Planet.bScanned)         StateColor = OLCStyleColors::TacticalBlue;
	else                              StateColor = OLCStyleColors::TextDim;

	bool bCanNavigate = !Planet.bIsCurrentPlanet && ResourceSubsystem;
	bool bCanScan = !Planet.bScanned && !Planet.bIsCurrentPlanet && ResourceSubsystem;

	FText ScanBtnLabel = LOCTEXT("Btn_Scan", "SCAN");
	if (bIsScanning)  ScanBtnLabel = LOCTEXT("Btn_Scanning", "SCANNING...");
	else if (!bCanScan) ScanBtnLabel = LOCTEXT("Btn_NoEnergy", "NO ENERGY");

	FText NavBtnLabel = LOCTEXT("Btn_Navigate", "NAVIGATE");
	if (Planet.bIsCurrentPlanet) NavBtnLabel = LOCTEXT("Btn_Current", "CURRENT");
	else if (!bCanNavigate)      NavBtnLabel = LOCTEXT("Btn_NoFuel", "NO FUEL");

	FText ResourcesText = Planet.Resources.IsEmpty() ? FText::FromString(TEXT("?")) : Planet.Resources;
	FText BiomeText = Planet.BiomeType.IsEmpty() ? FText::FromString(TEXT("?")) : Planet.BiomeType;

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
					// Planet name
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(Planet.PlanetName)
						.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ]
					// State badge
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 6.0f, 0.0f, 4.0f)
					[ SNew(SBorder).BorderBackgroundColor(StateColor).Padding(FMargin(8.0f, 3.0f))
						[ SNew(STextBlock).Text(StateText)
							.ColorAndOpacity(OLCStyleColors::TextWhite).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ] ]
					// TIR
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("TIR: {0}")), FText::AsNumber(Planet.TIR)))
						.ColorAndOpacity(OLCStyleColors::WarningYellow).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
					// Biome
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("BIOME:")))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(BiomeText)
						.ColorAndOpacity(Planet.bScanned ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
					// Resources
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("RESOURCES:")))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(ResourcesText)
						.ColorAndOpacity(Planet.bScanned ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
					// Fuel cost
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("FUEL COST: {0}")), FText::AsNumber(Planet.FuelCost)))
						.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
					// Scan cost
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("SCAN COST: {0} ENERGY")), FText::AsNumber(Planet.ScanCost)))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
					// Scan progress bar if scanning
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::Format(FText::FromString(TEXT("SCAN: {0}%")),
						FText::AsNumber(FMath::RoundToInt(ScanProgress / ScanDuration * 100.0f))))
						.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SBorder).BorderBackgroundColor(OLCStyleColors::GunmetalBlack).Padding(FMargin(1.0f))
						[ SNew(SBox).HeightOverride(4.0f)
							[ SNew(SBorder).BorderBackgroundColor(OLCStyleColors::TacticalBlue).Padding(FMargin(0.0f))
								[ SNew(SBox).WidthOverride(200.0f * ScanProgress / FMath::Max(ScanDuration, 1.0f)).HeightOverride(4.0f) ] ] ]
					]
					// Scan button
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 6.0f)
					[
						SNew(SButton)
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.OnClicked_Lambda([this]() -> FReply { ScanSelectedPlanet(); return FReply::Handled(); })
						.IsEnabled(bCanScan && !bIsScanning)
						[ SNew(SBorder)
							.BorderBackgroundColor((bCanScan && !bIsScanning) ? OLCStyleColors::TacticalBlue : OLCStyleColors::GunmetalBlack)
							.Padding(FMargin(16.0f, 8.0f))
							[ SNew(STextBlock).Text(ScanBtnLabel)
								.ColorAndOpacity((bCanScan && !bIsScanning) ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ] ]
					]
					// Navigate button
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SButton)
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.OnClicked_Lambda([this]() -> FReply { NavigateToPlanet(); return FReply::Handled(); })
						.IsEnabled(bCanNavigate)
						[ SNew(SBorder)
							.BorderBackgroundColor(bCanNavigate ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
							.Padding(FMargin(16.0f, 8.0f))
							[ SNew(STextBlock).Text(NavBtnLabel)
								.ColorAndOpacity(bCanNavigate ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ] ]
					]
				]
			]
		];
}

void UOLCSolarSystemWidget::InitializeSolarSystemData()
{
	Planets.Reset();
	Stations.Reset();

	TArray<FText> PlanetNames = {
		FText::FromString(TEXT("Aethel")), FText::FromString(TEXT("Vornax")),
		FText::FromString(TEXT("Kaelis")), FText::FromString(TEXT("Theron")),
		FText::FromString(TEXT("Nyx")), FText::FromString(TEXT("Helios"))
	};

	TArray<float> OrbitAngles = { 0.0f, 60.0f, 135.0f, 200.0f, 270.0f, 330.0f };
	TArray<float> OrbitRadii = { 180.0f, 240.0f, 300.0f, 360.0f, 420.0f, 500.0f };
	TArray<int32> TIRs = { 1, 1, 2, 2, 3, 3 };
	TArray<int32> FuelCosts = { 100, 150, 200, 280, 350, 500 };

	for (int32 i = 0; i < PlanetNames.Num(); i++)
	{
		FOLCPlanetInfo Planet;
		Planet.PlanetName = PlanetNames[i];
		Planet.OrbitAngle = OrbitAngles[i];
		Planet.OrbitRadius = OrbitRadii[i];
		Planet.Radius = 24.0f + i * 2.0f;
		Planet.TIR = TIRs[i];
		Planet.FuelCost = FuelCosts[i];
		Planet.ScanCost = 10;
		Planet.bIsCurrentPlanet = (i == 0); // First planet is current location
		Planets.Add(Planet);
	}

	FOLCStationInfo Station1;
	Station1.StationName = FText::FromString(TEXT("Abandoned Outpost"));
	Station1.Position = SunPosition + FVector2d(300.0f, -150.0f);
	Stations.Add(Station1);

	FOLCStationInfo Station2;
	Station2.StationName = FText::FromString(TEXT("Jump Point Alpha"));
	Station2.Position = SunPosition + FVector2d(-400.0f, 200.0f);
	Stations.Add(Station2);
}

void UOLCSolarSystemWidget::ScanSelectedPlanet()
{
	if (SelectedPlanetIndex < 0 || SelectedPlanetIndex >= Planets.Num()) return;
	if (!ResourceSubsystem) return;
	if (Planets[SelectedPlanetIndex].bScanned) return;

	TArray<FOLCResourceCounterViewData> Resources = ResourceSubsystem->GetResourceCounters();
	int32 EnergyAvailable = 0;
	for (const auto& Res : Resources)
	{
		if (Res.ResourceType == EOLCResourceType::Energy)
			EnergyAvailable = FMath::RoundToInt(Res.Value);
	}

	if (EnergyAvailable < Planets[SelectedPlanetIndex].ScanCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Insufficient energy to scan planet: need %d, have %d"),
			Planets[SelectedPlanetIndex].ScanCost, EnergyAvailable);
		return;
	}

	ResourceSubsystem->AddResource(EOLCResourceType::Energy, -Planets[SelectedPlanetIndex].ScanCost);

	bIsScanning = true;
	ScanProgress = 0.0f;
	ScanDuration = 10.0f; // Prototype speed (scaled from 5 min)

	UE_LOG(LogTemp, Display, TEXT("[OLC] Started scanning planet: %s (%d energy)"),
		*Planets[SelectedPlanetIndex].PlanetName.ToString(),
		Planets[SelectedPlanetIndex].ScanCost);

	InvalidateLayoutAndVolatility();
}

void UOLCSolarSystemWidget::NavigateToPlanet()
{
	if (SelectedPlanetIndex < 0 || SelectedPlanetIndex >= Planets.Num()) return;
	if (!ResourceSubsystem) return;

	const FOLCPlanetInfo& Target = Planets[SelectedPlanetIndex];
	if (Target.bIsCurrentPlanet) return;

	// Check drive status — offline drive cannot travel.
	if (ResourceSubsystem->GetDriveStatus() == EOLCModuleState::Offline)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot navigate: drive is OFFLINE"));
		return;
	}

	// Check drive max range — planet fuel cost must be within reach.
	const int32 MaxRange = ResourceSubsystem->GetMaxReachableFuelCost();
	if (Target.FuelCost > MaxRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Cannot navigate: %s is out of range (need %d fuel, max reach: %d)"),
			*Target.PlanetName.ToString(), Target.FuelCost, MaxRange);
		return;
	}

	TArray<FOLCResourceCounterViewData> Resources = ResourceSubsystem->GetResourceCounters();
	int32 FuelAvailable = 0;
	for (const auto& Res : Resources)
	{
		if (Res.ResourceType == EOLCResourceType::Fuel)
			FuelAvailable = FMath::RoundToInt(Res.Value);
	}

	if (FuelAvailable < Target.FuelCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Insufficient fuel to navigate: need %d, have %d"),
			Target.FuelCost, FuelAvailable);
		return;
	}

	ResourceSubsystem->AddResource(EOLCResourceType::Fuel, -Target.FuelCost);

	for (auto& Planet : Planets)
		Planet.bIsCurrentPlanet = false;
	Planets[SelectedPlanetIndex].bIsCurrentPlanet = true;

	UE_LOG(LogTemp, Display, TEXT("[OLC] Navigated to planet: %s (fuel cost: %d)"),
		*Target.PlanetName.ToString(), Target.FuelCost);

	InvalidateLayoutAndVolatility();
}

void UOLCSolarSystemWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	PlanetBackgroundBrush.Reset();
	OrbitLineBrush.Reset();
}

#undef LOCTEXT_NAMESPACE


