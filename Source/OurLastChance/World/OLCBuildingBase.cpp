#include "OLCBuildingBase.h"
#include "OurLastChance.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Logging/LogMacros.h"
#include "NiagaraComponent.h"
#include "VFX/OLCVFXSubsystem.h"

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

	ProductionTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ProductionText"));
	ProductionTextComponent->SetupAttachment(RootComponent);
	ProductionTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	ProductionTextComponent->SetHorizontalAlignment(EHTA_Center);
	ProductionTextComponent->SetWorldSize(32.0f);
	ProductionTextComponent->SetVisibility(false);

	// Default grid cell size matches crash site prototype (180 units per tile).
	GridCellSize = 180.0f;

	PrimaryActorTick.bCanEverTick = false;
}

void AOLCBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' spawned at %s"),
		*BuildingData.DisplayName.ToString(),
		*GetActorLocation().ToString());

	// Snap to grid on spawn.
	SnapToGrid();
}

void AOLCBuildingBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' ended play (reason=%d)"),
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

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building data updated: %s (%dx%d)"),
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

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building snapped to grid: %s"), *GetActorLocation().ToString());
}

void AOLCBuildingBase::RotateBuild()
{
	BuildRotationDegrees = (BuildRotationDegrees + 90) % 360;
	SetActorRotation(FRotator(0.0f, static_cast<float>(BuildRotationDegrees), 0.0f));

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' rotated to %d degrees"),
		*BuildingData.DisplayName.ToString(),
		BuildRotationDegrees);
}

void AOLCBuildingBase::ShowProduction(const FText& ProductionText, bool bDeficit)
{
	if (!ProductionTextComponent) return;
	ProductionTextComponent->SetText(ProductionText);
	ProductionTextComponent->SetTextRenderColor(bDeficit ? FColor(239, 68, 68) : FColor(34, 197, 94));
	ProductionTextComponent->SetVisibility(!ProductionText.IsEmpty());
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

	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' powered: %s"),
		*BuildingData.DisplayName.ToString(),
		bIsPowered ? TEXT("ON") : TEXT("OFF"));
}

// ---------------------------------------------------------------------------
// VFX integration hooks (WP-126 step-7; append-only)
// ---------------------------------------------------------------------------

void AOLCBuildingBase::PlayDestructionVFX()
{
	if (UOLCVFXSubsystem* VFX = GetGameInstance()->GetSubsystem<UOLCVFXSubsystem>())
	{
		VFX->PlayBuildingDestruction(GetActorLocation());
	}
	else
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] PlayDestructionVFX: VFX subsystem unavailable — no-op."));
	}
}

void AOLCBuildingBase::StartConstructionGlow()
{
	if (ConstructionGlow)
	{
		return; // Already active.
	}

	UOLCVFXSubsystem* VFX = GetGameInstance()->GetSubsystem<UOLCVFXSubsystem>();
	if (!VFX || !SceneRoot)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] StartConstructionGlow: VFX subsystem or SceneRoot unavailable — no-op."));
		return;
	}

	ConstructionGlow = VFX->PlayBuildingConstructionGlow(SceneRoot);

	// Diagnostic: make the attach observable in PIE logs (WP-126 step-7 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' construction glow %s"),
		*BuildingData.DisplayName.ToString(),
		ConstructionGlow ? TEXT("attached") : TEXT("FAILED to attach"));
}

void AOLCBuildingBase::StopConstructionGlow()
{
	if (!ConstructionGlow)
	{
		return; // Nothing to stop.
	}

	ConstructionGlow->DestroyComponent();
	ConstructionGlow = nullptr;

	// Diagnostic: make the detach observable in PIE logs (WP-126 step-7 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC] Building '%s' construction glow detached"),
		*BuildingData.DisplayName.ToString());
}

#undef LOCTEXT_NAMESPACE
