#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCNavigationTypes.h"
#include "OLCSolarSystemWidget.generated.h"

class UOLCUIDataSubsystem;
class UOLCNavigationSubsystem;

/**
 * S11 Solar System View — orbital layout with clickable planets,
 * station markers, fuel costs, and scan mechanics.
 */
UCLASS()
class OURLASTCHANCE_API UOLCSolarSystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCSolarSystemWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** Build the orbital canvas with sun, planets, and stations. */
	TSharedRef<SWidget> BuildOrbitalCanvas();

	/** Build a planet node as an SButton (clickable). */
	TSharedRef<SWidget> BuildPlanetNode(FOLCPlanetInfo& Planet);

	/** Build a station marker as an SImage. */
	TSharedRef<SWidget> BuildStationMarker(const FOLCStationInfo& Station);

	/** Build the top bar with system info, fuel display, and drive status. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the right-side detail panel for selected planet. */
	TSharedRef<SWidget> BuildDetailPanel();

	/** Get the drive status label text based on current subsystem state. */
	FText GetDriveStatusText() const;

	/** Get the shield status badge data for HUD display. */
	TArray<FOLCBadgeViewData> GetShipStateBadges() const;

	/** Initialize solar system data via NavigationSubsystem->GetOrGeneratePlanetsForSystem. */
	void InitializeSolarSystemData();

	/** Start a scan of the selected planet at the given tier via NavigationSubsystem->StartScan. */
	void ScanSelectedPlanet(EOLCScanTier Tier);

	/** Navigate to the selected planet via NavigationSubsystem->TryPayFuelAndTravel. */
	void NavigateToPlanet();

	/** Bound to NavigationSubsystem->OnScanCompleted; refreshes the revealed planet data. */
	UFUNCTION()
	void HandleScanCompleted(int32 SystemIndex, int32 PlanetIndex, EOLCScanTier Tier);

	TArray<FOLCPlanetInfo> Planets;
	TArray<FOLCStationInfo> Stations;

	int32 SelectedPlanetIndex = -1;

	FVector2D CanvasSize = FVector2D(1920.0, 1080.0);
	FVector2D SunPosition = FVector2D::ZeroVector;

	TUniquePtr<FSlateDynamicImageBrush> PlanetBackgroundBrush;
	TUniquePtr<FSlateDynamicImageBrush> OrbitLineBrush;

	UPROPERTY()
	TObjectPtr<UOLCUIDataSubsystem> ResourceSubsystem;

	UPROPERTY()
	TObjectPtr<UOLCNavigationSubsystem> NavigationSubsystem;

	/** Locally-tracked UI state for "is a scan currently outstanding" — the subsystem itself doesn't expose per-scan progress. */
	bool bIsScanning = false;
};
