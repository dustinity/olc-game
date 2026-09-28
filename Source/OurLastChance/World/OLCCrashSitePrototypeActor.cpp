#include "OLCCrashSitePrototypeActor.h"
#include "OurLastChance.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Core/OLCTutorialSubsystem.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "World/OLCUnitBase.h"

// WP-129 Step 2: tutorial inspect/deposit hooks (interact sphere + proximity scan) live in this file.

namespace
{
	static const FName CrashSiteColorParameterName(TEXT("Color"));
	static const FName CrashSiteRoughnessParameterName(TEXT("Roughness"));
}

AOLCCrashSitePrototypeActor::AOLCCrashSitePrototypeActor()
{
	// WP-129 Step 2: tick is enabled for the throttled tutorial proximity scan.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	SceneRoot->SetMobility(EComponentMobility::Static);

	DustTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DustTiles"));
	BuildableTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BuildableTiles"));
	BlockedTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockedTiles"));
	CliffRocks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CliffRocks"));
	SmallRocks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SmallRocks"));
	MineralNodes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MineralNodes"));
	FuelNodes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FuelNodes"));
	WreckParts = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WreckParts"));

	TArray<UInstancedStaticMeshComponent*> Components = {
		DustTiles.Get(), BuildableTiles.Get(), BlockedTiles.Get(), CliffRocks.Get(), SmallRocks.Get(), MineralNodes.Get(), FuelNodes.Get(), WreckParts.Get()
	};

	for (UInstancedStaticMeshComponent* Component : Components)
	{
		Component->SetupAttachment(SceneRoot);
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetMobility(EComponentMobility::Static);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (PlaneMesh.Succeeded())
	{
		DustTiles->SetStaticMesh(PlaneMesh.Object);
		BuildableTiles->SetStaticMesh(PlaneMesh.Object);
		BlockedTiles->SetStaticMesh(PlaneMesh.Object);
	}

	if (CubeMesh.Succeeded())
	{
		CliffRocks->SetStaticMesh(CubeMesh.Object);
		WreckParts->SetStaticMesh(CubeMesh.Object);
	}

	if (SphereMesh.Succeeded())
	{
		SmallRocks->SetStaticMesh(SphereMesh.Object);
		MineralNodes->SetStaticMesh(SphereMesh.Object);
		FuelNodes->SetStaticMesh(SphereMesh.Object);
	}

	ConfigureMaterial(DustTiles, FLinearColor(0.09f, 0.095f, 0.09f));
	ConfigureMaterial(BuildableTiles, FLinearColor(0.075f, 0.11f, 0.105f));
	ConfigureMaterial(BlockedTiles, FLinearColor(0.035f, 0.038f, 0.038f));
	ConfigureMaterial(CliffRocks, FLinearColor(0.13f, 0.13f, 0.125f));
	ConfigureMaterial(SmallRocks, FLinearColor(0.19f, 0.19f, 0.18f));
	ConfigureMaterial(MineralNodes, FLinearColor(0.34f, 0.62f, 0.72f));
	ConfigureMaterial(FuelNodes, FLinearColor(0.85f, 0.38f, 0.08f));
	ConfigureMaterial(WreckParts, FLinearColor(0.06f, 0.065f, 0.07f));

	// -----------------------------------------------------------------------
	// WP-129 Step 2 — tutorial interact sphere at the wreck + input binding
	// -----------------------------------------------------------------------

	// Wreck center matches AddCrashWreck() (tile 21,16). The sphere is the
	// "within proximity" gate for the inspect interaction.
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(SceneRoot);
	InteractSphere->SetRelativeLocation(TileToWorld(21, 16, 90.0f));
	InteractSphere->SetSphereRadius(InteractRadius);
	// Movable so overlap events fire even though the scene root is static.
	InteractSphere->SetMobility(EComponentMobility::Movable);
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractSphere->SetGenerateOverlapEvents(true);
	InteractSphere->SetCollisionObjectType(ECC_WorldDynamic);
	InteractSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AOLCCrashSitePrototypeActor::OnInteractSphereOverlapBegin);

	TutorialInput = CreateDefaultSubobject<UInputComponent>(TEXT("TutorialInput"));
	if (TutorialInput)
	{
		// Spec-mandated interact action binding. Note: a plain AActor's
		// InputComponent is not fed by the PlayerController in UE 5.8 — the
		// functional "interact within proximity" paths are (a) the overlap
		// sphere / tick scan below and (b) the player-controller E-key routing
		// into InteractWithCrashSite() (see Knowledge/runs/WP-129/OPEN.md).
		TutorialInput->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AOLCCrashSitePrototypeActor::OnInteractCrashSite);
	}
}

void AOLCCrashSitePrototypeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateCrashSite();
}

void AOLCCrashSitePrototypeActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateCrashSite();

	// WP-129 Step 3: capture presence already inside the interact sphere (e.g.
	// the player pawn standing at PlayerStart since PIE start). Their initial
	// overlap must not auto-complete InspectCrashSite — the intended tutorial
	// start is an explicit E-key press near the wreck. Without this gate,
	// TransitionToGameplay's spawn registered the sphere around a pawn that was
	// already inside and completed objective 1 instantly (before the Step-3
	// highlight could ever be shown for it). See OPEN.md step-3 note 11.
	if (InteractSphere)
	{
		TArray<AActor*> AlreadyOverlapping;
		InteractSphere->GetOverlappingActors(AlreadyOverlapping);
		for (AActor* Actor : AlreadyOverlapping)
		{
			if (Actor)
			{
				PreExistingOverlapActors.Add(Actor);
			}
		}
	}

	// The initial overlap sweep is deferred to the first tick after
	// registration, so also swallow events for a short settling window —
	// otherwise the pawn that was already inside at spawn time fires its
	// BeginOverlap one frame after this capture and slips past it.
	if (UWorld* World = GetWorld())
	{
		InteractGraceEndTime = World->GetTimeSeconds() + 1.0f;
	}
}

// ---------------------------------------------------------------------------
// WP-129 Step 2 — tutorial objective hooks (guarded, idempotent)
// ---------------------------------------------------------------------------

void AOLCCrashSitePrototypeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UOLCTutorialSubsystem* Tutorial = UGameplayStatics::GetGameInstance(this)
		? UGameplayStatics::GetGameInstance(this)->GetSubsystem<UOLCTutorialSubsystem>()
		: nullptr;
	if (!Tutorial || Tutorial->IsObjectiveComplete(EOLCTutorialObjective::DiscoverDeposit))
	{
		return; // Nothing left to scan for — keep the tick path free.
	}

	TutorialScanAccumulator += DeltaTime;
	if (TutorialScanAccumulator < 0.5f)
	{
		return;
	}
	TutorialScanAccumulator = 0.0f;

	ScanTutorialProximity();
}

void AOLCCrashSitePrototypeActor::OnInteractSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OverlappedComponent || !OtherActor || OtherActor == this)
	{
		return;
	}

	// WP-129 Step 3: presence that was already inside the sphere when BeginPlay
	// ran does not count as "entering" the wreck — otherwise
	// TransitionToGameplay's spawn would auto-complete InspectCrashSite the
	// instant the tutorial-only actor is created, before the Step-3 highlight
	// could be shown for objective 1. Two guards: the pre-existing-overlap set
	// (synchronous sweeps) and a short settling window covering UE's deferred
	// initial overlap sweep (see InteractGraceEndTime).
	if (const UWorld* World = GetWorld())
	{
		if (World->GetTimeSeconds() < InteractGraceEndTime)
		{
			return;
		}
	}
	for (const TWeakObjectPtr<AActor>& Pre : PreExistingOverlapActors)
	{
		if (Pre.Get() == OtherActor)
		{
			return;
		}
	}

	// Only player-controlled presence counts as "interacting with the wreck".
	const bool bIsPlayerPresence = Cast<AOLCUnitBase>(OtherActor) != nullptr || Cast<APawn>(OtherActor) != nullptr;
	if (!bIsPlayerPresence)
	{
		return;
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Crash site: %s entered the wreck interact sphere"), *OtherActor->GetName());
	InteractWithCrashSite();
}

void AOLCCrashSitePrototypeActor::OnInteractCrashSite()
{
	InteractWithCrashSite();
}

bool AOLCCrashSitePrototypeActor::IsWithinInteractProximity(const FVector& WorldLocation) const
{
	if (!InteractSphere)
	{
		return false;
	}

	const FVector SphereCenter = InteractSphere->GetComponentLocation();
	const float DistSq = FMath::Square(WorldLocation.X - SphereCenter.X) + FMath::Square(WorldLocation.Y - SphereCenter.Y);
	return DistSq <= FMath::Square(InteractRadius);
}

bool AOLCCrashSitePrototypeActor::GetNearestMineralNodeLocation(const FVector& From, FVector& OutLocation) const
{
	if (MineralNodePositions.Num() == 0)
	{
		return false;
	}

	float BestDistSq = TNumericLimits<float>::Max();
	const FVector* Best = nullptr;
	for (const FVector& Node : MineralNodePositions)
	{
		const float DistSq = FVector::DistSquared(Node, From);
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			Best = &Node;
		}
	}

	OutLocation = *Best;
	return true;
}

void AOLCCrashSitePrototypeActor::SetTutorialOnly(bool bInTutorialOnly)
{
	if (bTutorialOnly == bInTutorialOnly)
	{
		return;
	}

	bTutorialOnly = bInTutorialOnly;

	// Re-run generation so visual instances are added or cleared accordingly.
	GenerateCrashSite();
}

void AOLCCrashSitePrototypeActor::InteractWithCrashSite()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UOLCTutorialSubsystem* Tutorial = GameInstance ? GameInstance->GetSubsystem<UOLCTutorialSubsystem>() : nullptr;
	if (!Tutorial)
	{
		return;
	}

	// Guard: re-performing the interaction after completion is a no-op.
	if (Tutorial->IsObjectiveComplete(EOLCTutorialObjective::InspectCrashSite))
	{
		return;
	}

	Tutorial->CompleteObjective(EOLCTutorialObjective::InspectCrashSite);

	// Grant the dropship-storage reward exactly once (the guard above ensures it).
	UOLCUIDataSubsystem* UI = GameInstance ? GameInstance->GetSubsystem<UOLCUIDataSubsystem>() : nullptr;
	if (UI)
	{
		const UOLCTutorialData* Data = Tutorial->GetTutorialData();
		const TArray<FOLCResourceAmount> Reward = (Data && Data->StartingInventory.Num() > 0)
			? Data->StartingInventory
			: UOLCTutorialData::GetDefaultStartingInventory();

		for (const FOLCResourceAmount& Amount : Reward)
		{
			UI->AddResource(Amount.ResourceType, Amount.CurrentValue);
		}

		UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: dropship storage reward granted (%d resource types)"), Reward.Num());
	}
}

void AOLCCrashSitePrototypeActor::ScanTutorialProximity()
{
	if (MineralNodePositions.Num() == 0 || !GetWorld())
	{
		return;
	}

	UOLCTutorialSubsystem* Tutorial = UGameplayStatics::GetGameInstance(this)
		? UGameplayStatics::GetGameInstance(this)->GetSubsystem<UOLCTutorialSubsystem>()
		: nullptr;
	if (!Tutorial || Tutorial->IsObjectiveComplete(EOLCTutorialObjective::DiscoverDeposit))
	{
		return;
	}

	for (TActorIterator<AOLCUnitBase> It(GetWorld()); It; ++It)
	{
		const FVector UnitLocation = It->GetActorLocation();
		for (const FVector& NodeLocation : MineralNodePositions)
		{
			if (FVector::DistSquared(UnitLocation, NodeLocation) <= FMath::Square(DepositProximityRadius))
			{
				Tutorial->CompleteObjective(EOLCTutorialObjective::DiscoverDeposit);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: mineral deposit discovered by unit in proximity"));
				return;
			}
		}
	}

	// WP-129 Step 2 (repair): RTS presence. The real gameplay flow has no
	// player pawn/unit, so the camera view target counts as "entering
	// proximity" of a mineral node. Horizontal distance only (top-down camera).
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		const FVector ViewLocation = PC->GetFocalLocation();
		for (const FVector& NodeLocation : MineralNodePositions)
		{
			const float DistSq = FMath::Square(ViewLocation.X - NodeLocation.X) + FMath::Square(ViewLocation.Y - NodeLocation.Y);
			if (DistSq <= FMath::Square(DepositProximityRadius))
			{
				Tutorial->CompleteObjective(EOLCTutorialObjective::DiscoverDeposit);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: mineral deposit discovered by player view proximity"));
				return;
			}
		}
	}
}

bool AOLCCrashSitePrototypeActor::WorldToTile(const FVector& WorldLocation, FIntPoint& OutTile) const
{
	const FVector LocalLocation = GetActorTransform().InverseTransformPosition(WorldLocation);
	const float X = (LocalLocation.X / TileSize) + (MapWidth - 1) * 0.5f;
	const float Y = (LocalLocation.Y / TileSize) + (MapHeight - 1) * 0.5f;
	OutTile = FIntPoint(FMath::RoundToInt(X), FMath::RoundToInt(Y));
	return OutTile.X >= 0 && OutTile.X < MapWidth && OutTile.Y >= 0 && OutTile.Y < MapHeight;
}

bool AOLCCrashSitePrototypeActor::CanPlaceFootprint(FIntPoint OriginTile, FIntPoint FootprintSize) const
{
	if (FootprintSize.X <= 0 || FootprintSize.Y <= 0)
	{
		return false;
	}

	for (int32 Y = OriginTile.Y; Y < OriginTile.Y + FootprintSize.Y; ++Y)
	{
		for (int32 X = OriginTile.X; X < OriginTile.X + FootprintSize.X; ++X)
		{
			const FOLCPlanetTile* Tile = FindTile(FIntPoint(X, Y));
			if (!Tile || !Tile->bBuildable || Tile->bOccupied)
			{
				return false;
			}
		}
	}

	return true;
}

bool AOLCCrashSitePrototypeActor::TryReserveFootprint(FIntPoint OriginTile, FIntPoint FootprintSize)
{
	if (!CanPlaceFootprint(OriginTile, FootprintSize))
	{
		return false;
	}

	for (int32 Y = OriginTile.Y; Y < OriginTile.Y + FootprintSize.Y; ++Y)
	{
		for (int32 X = OriginTile.X; X < OriginTile.X + FootprintSize.X; ++X)
		{
			if (FOLCPlanetTile* Tile = FindTileMutable(FIntPoint(X, Y)))
			{
				Tile->bOccupied = true;
			}
		}
	}

	return true;
}

void AOLCCrashSitePrototypeActor::GenerateCrashSite()
{
	ClearGeneratedInstances();
	Tiles.Reset(MapWidth * MapHeight);
	MineralNodePositions.Reset();

	for (int32 Y = 0; Y < MapHeight; ++Y)
	{
		for (int32 X = 0; X < MapWidth; ++X)
		{
			FOLCPlanetTile Tile;
			Tile.Coord = FIntPoint(X, Y);

			const bool bInBasin = IsInsideBuildBasin(X, Y);
			const float EdgeX = FMath::Min(X, MapWidth - 1 - X);
			const float EdgeY = FMath::Min(Y, MapHeight - 1 - Y);
			const float EdgeDistance = FMath::Min(EdgeX, EdgeY);
			const float RockNoise = TileNoise(X, Y, 7);

			if (EdgeDistance < 3.0f || !bInBasin)
			{
				Tile.Type = EOLCPlanetTileType::Blocked;
				Tile.bBuildable = false;
			}
			else if (RockNoise > 0.88f)
			{
				Tile.Type = EOLCPlanetTileType::Rock;
				Tile.bBuildable = false;
			}
			else
			{
				Tile.Type = EOLCPlanetTileType::Buildable;
				Tile.bBuildable = true;
			}

			Tiles.Add(Tile);

			// WP-129 Step 2: tutorial-only mode records the tile layout but adds
			// no visual instances (no second planet on top of the real terrain).
			if (!bTutorialOnly)
			{
				AddTileInstance(Tile);

				if (Tile.Type == EOLCPlanetTileType::Blocked)
				{
					AddBorderRocks(X, Y, EdgeDistance);
				}
				else if (RockNoise > 0.75f)
				{
					const FVector Position = TileToWorld(X, Y, 12.0f);
					const float Scale = FMath::Lerp(0.12f, 0.38f, TileNoise(X, Y, 17));
					SmallRocks->AddInstance(FTransform(FRotator(0.0f, TileNoise(X, Y, 29) * 360.0f, 0.0f), Position, FVector(Scale, Scale * 0.75f, Scale * 0.28f)));
				}
			}
		}
	}

	AddResourceNode(9, 12, EOLCPlanetTileType::Minerals);
	AddResourceNode(31, 10, EOLCPlanetTileType::Minerals);
	AddResourceNode(13, 24, EOLCPlanetTileType::Fuel);
	AddResourceNode(33, 23, EOLCPlanetTileType::Fuel);

	if (!bTutorialOnly)
	{
		AddCrashWreck();
	}
}

void AOLCCrashSitePrototypeActor::ClearGeneratedInstances()
{
	for (UInstancedStaticMeshComponent* Component : { DustTiles.Get(), BuildableTiles.Get(), BlockedTiles.Get(), CliffRocks.Get(), SmallRocks.Get(), MineralNodes.Get(), FuelNodes.Get(), WreckParts.Get() })
	{
		if (Component)
		{
			Component->ClearInstances();
		}
	}
}

void AOLCCrashSitePrototypeActor::AddTileInstance(const FOLCPlanetTile& Tile)
{
	UInstancedStaticMeshComponent* Target = DustTiles;
	if (Tile.Type == EOLCPlanetTileType::Buildable)
	{
		Target = BuildableTiles;
	}
	else if (Tile.Type == EOLCPlanetTileType::Blocked || Tile.Type == EOLCPlanetTileType::Rock)
	{
		Target = BlockedTiles;
	}

	const float Height = Tile.Type == EOLCPlanetTileType::Buildable ? 0.0f : FMath::Lerp(2.0f, 18.0f, TileNoise(Tile.Coord.X, Tile.Coord.Y, 41));
	Target->AddInstance(FTransform(FRotator::ZeroRotator, TileToWorld(Tile.Coord.X, Tile.Coord.Y, Height), FVector(TileSize / 100.0f, TileSize / 100.0f, 1.0f)));
}

void AOLCCrashSitePrototypeActor::AddBorderRocks(int32 X, int32 Y, float EdgeDistance)
{
	const float Density = EdgeDistance < 3.0f ? 0.92f : 0.34f;
	if (TileNoise(X, Y, 61) > Density)
	{
		return;
	}

	const float Height = FMath::Lerp(0.65f, 2.45f, TileNoise(X, Y, 67));
	const float Footprint = FMath::Lerp(0.85f, 1.85f, TileNoise(X, Y, 71));
	const FVector Position = TileToWorld(X, Y, Height * 38.0f);
	const FRotator Rotation(0.0f, TileNoise(X, Y, 73) * 360.0f, 0.0f);
	CliffRocks->AddInstance(FTransform(Rotation, Position, FVector(Footprint, Footprint * FMath::Lerp(0.65f, 1.35f, TileNoise(X, Y, 79)), Height)));
}

void AOLCCrashSitePrototypeActor::AddResourceNode(int32 X, int32 Y, EOLCPlanetTileType Type)
{
	UInstancedStaticMeshComponent* Target = Type == EOLCPlanetTileType::Fuel ? FuelNodes : MineralNodes;

	// WP-129 Step 2: remember mineral-node cluster centers for the deposit
	// proximity scan (tiles themselves are not retyped to Minerals).
	if (Type == EOLCPlanetTileType::Minerals)
	{
		MineralNodePositions.Add(TileToWorld(X, Y, 18.0f));
	}

	if (bTutorialOnly)
	{
		return; // Positions recorded; no visual instances in tutorial-only mode.
	}

	for (int32 Index = 0; Index < 7; ++Index)
	{
		const float Angle = (static_cast<float>(Index) / 7.0f) * 360.0f;
		const float Radius = Index == 0 ? 0.0f : FMath::Lerp(38.0f, 84.0f, TileNoise(X + Index, Y, 83));
		const FVector Offset(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius, FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 28.0f);
		const float Scale = FMath::Lerp(0.32f, 0.62f, TileNoise(X, Y + Index, 89));
		Target->AddInstance(FTransform(FRotator(0.0f, Angle, 0.0f), TileToWorld(X, Y, 18.0f) + Offset, FVector(Scale, Scale, Scale * 0.42f)));
	}
}

void AOLCCrashSitePrototypeActor::AddCrashWreck()
{
	const FVector Center = TileToWorld(21, 16, 90.0f);
	const FRotator HullRotation(0.0f, -21.0f, -7.0f);
	WreckParts->AddInstance(FTransform(HullRotation, Center, FVector(8.2f, 2.15f, 0.74f)));
	WreckParts->AddInstance(FTransform(FRotator(0.0f, -22.0f, 2.0f), Center + FVector(-520.0f, -130.0f, 28.0f), FVector(3.2f, 1.55f, 0.48f)));
	WreckParts->AddInstance(FTransform(FRotator(0.0f, -12.0f, 0.0f), Center + FVector(410.0f, 180.0f, -6.0f), FVector(2.85f, 1.3f, 0.38f)));
	WreckParts->AddInstance(FTransform(FRotator(0.0f, -55.0f, 12.0f), Center + FVector(-180.0f, 390.0f, 24.0f), FVector(4.4f, 0.5f, 0.2f)));
}

void AOLCCrashSitePrototypeActor::ConfigureMaterial(UInstancedStaticMeshComponent* Component, const FLinearColor& Color, float Roughness)
{
	if (!Component)
	{
		return;
	}

	UMaterialInterface* SourceMaterial = Component->GetMaterial(0);
	if (!SourceMaterial)
	{
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(SourceMaterial, this);
	DynamicMaterial->SetVectorParameterValue(CrashSiteColorParameterName, Color);
	DynamicMaterial->SetScalarParameterValue(CrashSiteRoughnessParameterName, Roughness);
	Component->SetMaterial(0, DynamicMaterial);
}

FVector AOLCCrashSitePrototypeActor::TileToWorld(int32 X, int32 Y, float Z) const
{
	return FVector((X - (MapWidth - 1) * 0.5f) * TileSize, (Y - (MapHeight - 1) * 0.5f) * TileSize, Z);
}

bool AOLCCrashSitePrototypeActor::IsInsideBuildBasin(int32 X, int32 Y) const
{
	const float CenterX = (MapWidth - 1) * 0.5f;
	const float CenterY = (MapHeight - 1) * 0.5f;
	const float NormalizedX = (X - CenterX) / (MapWidth * 0.43f);
	const float NormalizedY = (Y - CenterY) / (MapHeight * 0.36f);
	const float Distortion = (TileNoise(X, Y, 101) - 0.5f) * 0.18f;
	return (NormalizedX * NormalizedX + NormalizedY * NormalizedY + Distortion) < 1.0f;
}

float AOLCCrashSitePrototypeActor::TileNoise(int32 X, int32 Y, int32 Salt) const
{
	FRandomStream Stream(Seed + X * 928371 + Y * 689287 + Salt * 31337);
	return Stream.GetFraction();
}

FOLCPlanetTile* AOLCCrashSitePrototypeActor::FindTileMutable(FIntPoint Coord)
{
	if (Coord.X < 0 || Coord.X >= MapWidth || Coord.Y < 0 || Coord.Y >= MapHeight)
	{
		return nullptr;
	}

	return Tiles.IsValidIndex(Coord.Y * MapWidth + Coord.X) ? &Tiles[Coord.Y * MapWidth + Coord.X] : nullptr;
}

const FOLCPlanetTile* AOLCCrashSitePrototypeActor::FindTile(FIntPoint Coord) const
{
	if (Coord.X < 0 || Coord.X >= MapWidth || Coord.Y < 0 || Coord.Y >= MapHeight)
	{
		return nullptr;
	}

	return Tiles.IsValidIndex(Coord.Y * MapWidth + Coord.X) ? &Tiles[Coord.Y * MapWidth + Coord.X] : nullptr;
}
