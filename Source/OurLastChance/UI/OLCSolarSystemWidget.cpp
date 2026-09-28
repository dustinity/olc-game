#include "OLCSolarSystemWidget.h"
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
#include "Core/OLCScanTierData.h"
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
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/Galaxy System UI/Assets") / FileName);
	}

	FString NavMarker(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/Galaxy System UI/Assets") / FileName);
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
	{
		ResourceSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();
		NavigationSubsystem = GI->GetSubsystem<UOLCNavigationSubsystem>();
	}

	if (NavigationSubsystem)
	{
		NavigationSubsystem->OnScanCompleted.RemoveDynamic(this, &UOLCSolarSystemWidget::HandleScanCompleted);
		NavigationSubsystem->OnScanCompleted.AddDynamic(this, &UOLCSolarSystemWidget::HandleScanCompleted);
	}

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

	if (NavigationSubsystem)
		NavigationSubsystem->TickScans(InDeltaTime);
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
			.Offset(FMargin((FVector2D::ZeroVector).X, (FVector2D::ZeroVector).Y, 0.0f, 0.0f)).AutoSize(true)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.08f, 0.1f, 0.12f, 0.35f))
				.Padding(FMargin(0.0f))
			];
	}

	// Draw sun at center.
	Canvas->AddSlot()
		.Offset(FMargin((SunPosition - FVector2D(40.0f, 40.0f)).X, (SunPosition - FVector2D(40.0f, 40.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
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
		FVector2D PlanetPos(SunPosition.X + FMath::Cos(RadAngle) * RingRadius,
		                     SunPosition.Y + FMath::Sin(RadAngle) * RingRadius);

		Planet.Position = PlanetPos;
		Canvas->AddSlot()
			.Offset(FMargin((PlanetPos - FVector2D(Planet.Radius, Planet.Radius)).X, (PlanetPos - FVector2D(Planet.Radius, Planet.Radius)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildPlanetNode(Planet) ];
	}

	// Draw stations.
	for (const auto& Station : Stations)
	{
		Canvas->AddSlot()
			.Offset(FMargin((Station.Position - FVector2D(10.0f, 10.0f)).X, (Station.Position - FVector2D(10.0f, 10.0f)).Y, 0.0f, 0.0f)).AutoSize(true)
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
	bool bCanScan = !Planet.bIsCurrentPlanet && NavigationSubsystem && !bIsScanning;

	FText NavBtnLabel = LOCTEXT("Btn_Navigate", "NAVIGATE");
	if (Planet.bIsCurrentPlanet) NavBtnLabel = LOCTEXT("Btn_Current", "CURRENT");
	else if (!bCanNavigate)      NavBtnLabel = LOCTEXT("Btn_NoFuel", "NO FUEL");

	FText ResourcesText = Planet.Resources.IsEmpty() ? FText::FromString(TEXT("?")) : Planet.Resources;
	FText BiomeText = Planet.BiomeType.IsEmpty() ? FText::FromString(TEXT("?")) : Planet.BiomeType;

	FText FeaturesText = FText::FromString(TEXT("?"));
	if (Planet.bScanned)
	{
		if (Planet.DiscoveredFeatures.Num() > 0)
		{
			TArray<FString> FeatureStrings;
			for (const FText& Feature : Planet.DiscoveredFeatures)
				FeatureStrings.Add(Feature.ToString());
			FeaturesText = FText::FromString(FString::Join(FeatureStrings, TEXT(", ")));
		}
		else
		{
			FeaturesText = LOCTEXT("Features_None", "None found");
		}
	}

	TArray<TSharedRef<SWidget>> TierButtons;
	if (NavigationSubsystem)
	{
		if (UOLCScanTierData* TierData = NavigationSubsystem->GetScanTierData())
		{
			for (const FOLCScanTierConfig& TierConfig : TierData->ScanTiers)
			{
				const EOLCScanTier ThisTier = TierConfig.ScanTier;
				const FText BtnLabel = FText::Format(
					FText::FromString(TEXT("{0} ({1} EN)")),
					TierConfig.DisplayName, FText::AsNumber(TierConfig.EnergyCost));

				TierButtons.Add(
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.OnClicked_Lambda([this, ThisTier]() -> FReply { ScanSelectedPlanet(ThisTier); return FReply::Handled(); })
					.IsEnabled(bCanScan)
					[ SNew(SBorder)
						.BorderBackgroundColor(bCanScan ? OLCStyleColors::TacticalBlue : OLCStyleColors::GunmetalBlack)
						.Padding(FMargin(10.0f, 6.0f))
						[ SNew(STextBlock).Text(BtnLabel)
							.ColorAndOpacity(bCanScan ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ] ]
				);
			}
		}
	}
	if (TierButtons.Num() == 0)
	{
		TierButtons.Add(
			SNew(STextBlock).Text(bIsScanning ? LOCTEXT("Btn_Scanning", "SCANNING...") : LOCTEXT("Btn_NoTierData", "NO SCAN TIER DATA"))
				.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)));
	}

	TSharedRef<SHorizontalBox> TierButtonBox = SNew(SHorizontalBox);
	for (const TSharedRef<SWidget>& Btn : TierButtons)
	{
		TierButtonBox->AddSlot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 6.0f)
			[ Btn ];
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
					// Discovered features
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(FText::FromString(TEXT("FEATURES:")))
						.ColorAndOpacity(OLCStyleColors::TextDim).Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(FeaturesText).AutoWrapText(true)
						.ColorAndOpacity(Planet.bScanned ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
					// Scan status
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
					[ SNew(STextBlock).Text(bIsScanning ? LOCTEXT("Btn_Scanning", "SCANNING...") : LOCTEXT("Scan_TierPrompt", "SCAN AT TIER:"))
						.ColorAndOpacity(OLCStyleColors::TacticalBlue).Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					// Scan-tier buttons (sourced from DA_ScanTiers)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 6.0f)
					[ TierButtonBox ]
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

	if (NavigationSubsystem)
	{
		Planets = NavigationSubsystem->GetOrGeneratePlanetsForSystem(NavigationSubsystem->GetCurrentSystemID());
		for (int32 i = 0; i < Planets.Num(); i++)
			Planets[i].Radius = 24.0f + i * 2.0f;
	}

	FOLCStationInfo Station1;
	Station1.StationName = FText::FromString(TEXT("Abandoned Outpost"));
	Station1.Position = SunPosition + FVector2D(300.0f, -150.0f);
	Stations.Add(Station1);

	FOLCStationInfo Station2;
	Station2.StationName = FText::FromString(TEXT("Jump Point Alpha"));
	Station2.Position = SunPosition + FVector2D(-400.0f, 200.0f);
	Stations.Add(Station2);
}

void UOLCSolarSystemWidget::ScanSelectedPlanet(EOLCScanTier Tier)
{
	if (SelectedPlanetIndex < 0 || SelectedPlanetIndex >= Planets.Num()) return;
	if (!NavigationSubsystem) return;
	if (bIsScanning) return;

	const bool bStarted = NavigationSubsystem->StartScan(
		NavigationSubsystem->GetCurrentSystemID(), SelectedPlanetIndex, Tier);

	if (!bStarted)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Could not start scan of planet %s at tier %d (insufficient energy or already active)"),
			*Planets[SelectedPlanetIndex].PlanetName.ToString(), (int32)Tier);
		return;
	}

	bIsScanning = true;
	UE_LOG(LogOLC, Display, TEXT("[OLC] Started scanning planet: %s (tier %d)"),
		*Planets[SelectedPlanetIndex].PlanetName.ToString(), (int32)Tier);

	InvalidateLayoutAndVolatility();
}

void UOLCSolarSystemWidget::HandleScanCompleted(int32 SystemIndex, int32 PlanetIndex, EOLCScanTier Tier)
{
	if (!NavigationSubsystem || SystemIndex != NavigationSubsystem->GetCurrentSystemID()) return;

	bIsScanning = false;

	// Refresh from the subsystem's cache — ApplyScanReveal already updated the cached planet in place.
	Planets = NavigationSubsystem->GetOrGeneratePlanetsForSystem(SystemIndex);
	for (int32 i = 0; i < Planets.Num(); i++)
		Planets[i].Radius = 24.0f + i * 2.0f;

	if (PlanetIndex >= 0 && PlanetIndex < Planets.Num())
	{
		UE_LOG(LogOLC, Display, TEXT("[OLC] Planet scan completed: %s — Biome: %s, Resources: %s"),
			*Planets[PlanetIndex].PlanetName.ToString(),
			*Planets[PlanetIndex].BiomeType.ToString(),
			*Planets[PlanetIndex].Resources.ToString());
	}

	InvalidateLayoutAndVolatility();
}

void UOLCSolarSystemWidget::NavigateToPlanet()
{
	if (SelectedPlanetIndex < 0 || SelectedPlanetIndex >= Planets.Num()) return;
	if (!ResourceSubsystem) return;

	// Target.FuelCost is sourced from NavigationSubsystem->GetOrGeneratePlanetsForSystem's
	// deterministic generation (see InitializeSolarSystemData/HandleScanCompleted), not
	// widget-local arithmetic.
	const FOLCPlanetInfo& Target = Planets[SelectedPlanetIndex];
	if (Target.bIsCurrentPlanet) return;

	// Check drive status — offline drive cannot travel.
	if (ResourceSubsystem->GetDriveStatus() == EOLCModuleState::Offline)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Cannot navigate: drive is OFFLINE"));
		return;
	}

	// Check drive max range — planet fuel cost must be within reach.
	const int32 MaxRange = ResourceSubsystem->GetMaxReachableFuelCost();
	if (Target.FuelCost > MaxRange)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Cannot navigate: %s is out of range (need %d fuel, max reach: %d)"),
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
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Insufficient fuel to navigate: need %d, have %d"),
			Target.FuelCost, FuelAvailable);
		return;
	}

	ResourceSubsystem->AddResource(EOLCResourceType::Fuel, -Target.FuelCost);

	for (auto& Planet : Planets)
		Planet.bIsCurrentPlanet = false;
	Planets[SelectedPlanetIndex].bIsCurrentPlanet = true;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Navigated to planet: %s (fuel cost: %d)"),
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


