#include "OLCBuildingBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Logging/LogMacros.h"

#define LOCTEXT_NAMESPACE "OLCBuildingBase"

AOLCBuildingBase::AOLCBuildingBase()
{
	// Scene root is the default root component.
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Building mesh — visible anywhere, not editable per-instance.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Footprint box for placement overlap detection.
	Footprint = CreateDefaultSubobject<UBoxComponent>(TEXT("Footprint"));
	Footprint->SetupAttachment(RootComponent);
	Footprint->SetBoxExtent(FVector(BuildingData.GridSize.X * GridCellSize * 0.5f,
		BuildingData.GridSize.Y * GridCellSize * 0.5f, 10.0f));
	Footprint->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Footprint->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Default grid cell size matches crash site prototype (180 units per tile).
	GridCellSize = 180.0f;

	PrimaryActorTick.bCanEverTick = false;
}

void AOLCBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building '%s' spawned at %s"),
		*BuildingData.DisplayName.ToString(),
		*GetActorLocation().ToString());

	// Snap to grid on spawn.
	SnapToGrid();
}

void AOLCBuildingBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building '%s' ended play (reason=%d)"),
		*BuildingData.DisplayName.ToString(),
		static_cast<int32>(EndPlayReason));
}

void AOLCBuildingBase::SetBuildingData(const FOLCBuildingConfig& InData)
{
	BuildingData = InData;

	// Update footprint size to match new grid dimensions.
	if (Footprint)
	{
		Footprint->SetBoxExtent(FVector(BuildingData.GridSize.X * GridCellSize * 0.5f,
			BuildingData.GridSize.Y * GridCellSize * 0.5f, 10.0f));
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building data updated: %s (%dx%d)"),
		*BuildingData.DisplayName.ToString(),
		FMath::RoundToInt(BuildingData.GridSize.X),
		FMath::RoundToInt(BuildingData.GridSize.Y));
}

void AOLCBuildingBase::SnapToGrid()
{
	if (!GetWorld()) return;

	// Snap to nearest grid cell based on footprint center.
	const FVector HalfExtent = Footprint->GetScaledBoxExtent();
	const FVector Center = GetActorLocation();

	const float GridX = FMath::RoundToFloat(Center.X / GridCellSize) * GridCellSize;
	const float GridY = FMath::RoundToFloat(Center.Y / GridCellSize) * GridCellSize;
	const float GridZ = Center.Z; // Keep Z (height) as-is.

	SetActorLocation(FVector(GridX, GridY, GridZ));

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building snapped to grid: %s"), *GetActorLocation().ToString());
}

void AOLCBuildingBase::RotateBuild()
{
	BuildRotationDegrees = (BuildRotationDegrees + 90) % 360;
	SetActorRotation(FRotator(0.0f, static_cast<float>(BuildRotationDegrees), 0.0f));

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building '%s' rotated to %d degrees"),
		*BuildingData.DisplayName.ToString(),
		BuildRotationDegrees);
}

void AOLCBuildingBase::SetPowered(bool bNewPowered)
{
	if (bIsPowered == bNewPowered) return;

	bIsPowered = bNewPowered;

	// Toggle mesh visibility based on power state.
	if (Mesh)
	{
		Mesh->SetVisibility(bIsPowered, true);
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Building '%s' powered: %s"),
		*BuildingData.DisplayName.ToString(),
		bIsPowered ? TEXT("ON") : TEXT("OFF"));
}

#undef LOCTEXT_NAMESPACE
