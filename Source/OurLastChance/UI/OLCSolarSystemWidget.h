#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "OLCSolarSystemWidget.generated.h"

class UOLCUIDataSubsystem;

/** A planet in the solar system. */
USTRUCT()
struct FOLCPlanetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText PlanetName;

	/** Orbital angle in degrees (0-360). */
	UPROPERTY()
	float OrbitAngle = 0.0f;

	/** Distance from sun (used for orbital radius). */
	UPROPERTY()
	float OrbitRadius = 200.0f;

	/** Planet radius on screen. */
	UPROPERTY()
	float Radius = 24.0f;

	/** Computed screen position for the current orbital canvas pass. */
	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	/** Biome type (empty until scanned). */
	UPROPERTY()
	FText BiomeType;

	/** Resources found on planet (empty until scanned). */
	UPROPERTY()
	FText Resources;

	/** TIR rating of the planet. */
	UPROPERTY()
	int32 TIR = 1;

	/** Whether this planet has been scanned. */
	UPROPERTY()
	bool bScanned = false;

	/** Whether the player currently occupies this planet. */
	UPROPERTY()
	bool bIsCurrentPlanet = false;

	/** Fuel cost to travel here from current planet. */
	UPROPERTY()
	int32 FuelCost = 100;

	/** Energy cost for basic scan (10 energy). */
	UPROPERTY()
	int32 ScanCost = 10;

	FOLCPlanetInfo() {}
};

/** A station or waypoint in the solar system. */
USTRUCT()
struct FOLCStationInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FText StationName;

	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	UPROPERTY()
	bool bVisited = false;

	FOLCStationInfo() {}
};

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

	/** Initialize sample solar system data from Briefing specs. */
	void InitializeSolarSystemData();

	/** Perform a scan on the selected planet (deducts energy, reveals info). */
	void ScanSelectedPlanet();

	/** Navigate to the selected planet (deducts fuel). */
	void NavigateToPlanet();

	TArray<FOLCPlanetInfo> Planets;
	TArray<FOLCStationInfo> Stations;

	int32 SelectedPlanetIndex = -1;

	FVector2d CanvasSize = FVector2d(1920.0, 1080.0);
	FVector2d SunPosition = FVector2d::ZeroVector;

	TUniquePtr<FSlateDynamicImageBrush> PlanetBackgroundBrush;
	TUniquePtr<FSlateDynamicImageBrush> OrbitLineBrush;

	UPROPERTY()
	TObjectPtr<UOLCUIDataSubsystem> ResourceSubsystem;

	bool bIsScanning = false;
	float ScanProgress = 0.0f;
	float ScanDuration = 300.0f; // 5 minutes in seconds (scaled for prototype: 10s)
};
