#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCGalaxyMapWidget.generated.h"

class UOLCUIDataSubsystem;
class UOLCNavigationSubsystem;

/** A solar system cluster on the galaxy map. */
USTRUCT()
struct FOLCGalaxySystem
{
	GENERATED_BODY()

	UPROPERTY()
	FText SystemName;

	/** ClusterID from UOLCNavigationSubsystem::GetOrGenerateGalaxyClusters, for TryPayFuelAndTravel. */
	UPROPERTY()
	int32 ClusterID = 0;

	/** Position on the galaxy map (0-1 normalized). */
	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	/** TIR rating of the system (determines difficulty). */
	UPROPERTY()
	int32 TIR = 1;

	/** Distance from galaxy center. */
	UPROPERTY()
	float DistanceFromCenter = 0.5f;

	/** Whether the player has visited this system. */
	UPROPERTY()
	bool bVisited = false;

	/** Whether the system is reachable (drive tier allows). */
	UPROPERTY()
	bool bReachable = false;

	FOLCGalaxySystem() {}
};

/** A warp route between two systems. */
USTRUCT()
struct FOLCWarpRoute
{
	GENERATED_BODY()

	FOLCGalaxySystem* FromSystem = nullptr;

	FOLCGalaxySystem* ToSystem = nullptr;

	/** Fuel cost for this jump. */
	UPROPERTY()
	int32 FuelCost = 500;

	FOLCWarpRoute() {}
};

/**
 * S12 Galaxy Map — zoomed-out view with solar system clusters,
 * warp routes, and center galaxy marked as objective.
 */
UCLASS()
class OURLASTCHANCE_API UOLCGalaxyMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCGalaxyMapWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** Build the galaxy canvas with systems and routes. */
	TSharedRef<SWidget> BuildGalaxyCanvas();

	/** Build a system node as an SButton (clickable). */
	TSharedRef<SWidget> BuildSystemNode(FOLCGalaxySystem& System);

	/** Build the top bar with galaxy info. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the right-side detail panel for selected system. */
	TSharedRef<SWidget> BuildDetailPanel();

	/** Initialize galaxy data via NavigationSubsystem->GetOrGenerateGalaxyClusters/GetWarpRoutes. */
	void InitializeGalaxyData();

	/** Pay fuel and warp to the selected system via NavigationSubsystem->TryPayFuelAndTravel. */
	void WarpToSelectedSystem();

	TArray<FOLCGalaxySystem> Systems;
	TArray<FOLCWarpRoute> Routes;

	int32 SelectedSystemIndex = -1;

	FVector2D CanvasSize = FVector2D(1920.0, 1080.0);
	FVector2D GalaxyCenter = FVector2D::ZeroVector;

	UPROPERTY()
	TObjectPtr<UOLCUIDataSubsystem> ResourceSubsystem;

	UPROPERTY()
	TObjectPtr<UOLCNavigationSubsystem> NavigationSubsystem;
};
