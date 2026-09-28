#include "OLCPlanetTerrainActor.h"
#include "OurLastChance.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Core/OLCPlanetTerrainProfile.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "World/OLCPlanetTerrainGenerator.h"
#include "Engine/World.h"
#include "VFX/OLCVFXSubsystem.h"

namespace
{
	static const FName PlanetColorParameterName(TEXT("Color"));
	static const FName PlanetRoughnessParameterName(TEXT("Roughness"));
}

AOLCPlanetTerrainActor::AOLCPlanetTerrainActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	SurfaceMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SurfaceMesh"));
	SurfaceMesh->SetupAttachment(SceneRoot);

	DebugBuildableTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugBuildableTiles"));
	DebugRestrictedTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugRestrictedTiles"));
	DebugBlockedTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugBlockedTiles"));
	DebugWaterTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugWaterTiles"));
	DebugResourceTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugResourceTiles"));
	DebugDungeonTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugDungeonTiles"));
	DebugLandingTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugLandingTiles"));
	DebugOutsideTiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DebugOutsideTiles"));
	BlockerProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockerProps"));
	ResourceProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ResourceProps"));
	DungeonProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DungeonProps"));
	PlantProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PlantProps"));

	// WP-108 Step 4: Billboard sprite components for PNG-based props
	BillboardPlantProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BillboardPlantProps"));
	BillboardResourceProps = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BillboardResourceProps"));

	for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get(), BlockerProps.Get(), ResourceProps.Get(), DungeonProps.Get(), PlantProps.Get() })
	{
		Component->SetupAttachment(SceneRoot);
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Setup billboard sprite components
	for (UInstancedStaticMeshComponent* Component : { BillboardPlantProps.Get(), BillboardResourceProps.Get() })
	{
		Component->SetupAttachment(SceneRoot);
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (PlaneMesh.Succeeded())
	{
		for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get() })
		{
			Component->SetStaticMesh(PlaneMesh.Object);
		}
	}
	if (CubeMesh.Succeeded())
	{
		BlockerProps->SetStaticMesh(CubeMesh.Object);
		DungeonProps->SetStaticMesh(CubeMesh.Object);
		PlantProps->SetStaticMesh(CubeMesh.Object);
	}
	if (SphereMesh.Succeeded())
	{
		ResourceProps->SetStaticMesh(SphereMesh.Object);
	}

	// Assign a default material so ConfigureMaterial can create dynamic instances with Color/Roughness
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMat(TEXT("/Game/Terrain/MAT_TerrainBase.MAT_TerrainBase"));
	if (DefaultMat.Succeeded())
	{
		for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get(), BlockerProps.Get(), ResourceProps.Get(), DungeonProps.Get(), PlantProps.Get() })
		{
			Component->SetMaterial(0, DefaultMat.Object);
		}

		// Also assign to the procedural mesh surface
		SurfaceMesh->SetMaterial(0, DefaultMat.Object);
	}
}

void AOLCPlanetTerrainActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateAndRender();
}

void AOLCPlanetTerrainActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentBiome = GenerationSettings.Biome;
	TargetBiome = GenerationSettings.Biome;
	GenerateAndRender();
}

void AOLCPlanetTerrainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBiomeCrossfade(DeltaTime);
}

void AOLCPlanetTerrainActor::UpdateBiomeCrossfade(float DeltaTime)
{
	if (!bCrossfading)
		return;

	CrossfadeProgress += DeltaTime / BiomeCrossfadeDuration;

	if (CrossfadeProgress >= 1.0f)
	{
		CrossfadeProgress = 1.0f;
		bCrossfading = false;
		CurrentBiome = TargetBiome;
		GenerationSettings.Biome = TargetBiome;
		GenerateAndRender();
		return;
	}

	// Apply interpolated colors during crossfade
	FLinearColor InterpolatedSurface = FMath::Lerp(
		GetBiomeSurfaceColor(CurrentBiome),
		GetBiomeSurfaceColor(TargetBiome),
		CrossfadeProgress);

	FLinearColor InterpolatedBlocker = FMath::Lerp(
		GetBiomeBlockerColor(CurrentBiome),
		GetBiomeBlockerColor(TargetBiome),
		CrossfadeProgress);

	// Update surface mesh vertex colors for crossfade effect
	UpdateSurfaceMeshCrossfade(InterpolatedSurface);
	UpdatePropColorsCrossfade(InterpolatedBlocker);
}

void AOLCPlanetTerrainActor::UpdateSurfaceMeshCrossfade(const FLinearColor& Color)
{
	if (!SurfaceMesh)
		return;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	const int32 TotalTiles = GenerationSettings.MapWidth * GenerationSettings.MapHeight;
	Vertices.Reserve(TotalTiles);
	UVs.Reserve(TotalTiles);

	for (const FOLCTerrainTile& Tile : Tiles)
	{
		Vertices.Add(TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height));
		UVs.Add(FVector2D(static_cast<float>(Tile.Coord.X) / GenerationSettings.MapWidth, static_cast<float>(Tile.Coord.Y) / GenerationSettings.MapHeight));
		Normals.Add(FVector::UpVector);
		VertexColors.Add(Color);
		Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
	}

	for (int32 Y = 0; Y < GenerationSettings.MapHeight - 1; ++Y)
	{
		for (int32 X = 0; X < GenerationSettings.MapWidth - 1; ++X)
		{
			const int32 A = Y * GenerationSettings.MapWidth + X;
			const int32 B = A + 1;
			const int32 C = A + GenerationSettings.MapWidth;
			const int32 D = C + 1;
			// Winding must face +Z (up) for the single-sided MAT_TerrainBase to be
			// visible from the top-down gameplay camera — the previous order
			// (A,C,B / B,C,D) faced -Z and was back-face culled, leaving the whole
			// ground invisible while convex prop meshes (which always show some
			// front face) still rendered.
			Triangles.Append({ A, B, C, B, D, C });
		}
	}

	SurfaceMesh->UpdateMeshSection_LinearColor(0, Vertices, Normals, UVs, VertexColors, Tangents);
}

void AOLCPlanetTerrainActor::UpdatePropColorsCrossfade(const FLinearColor& BlockerColor)
{
	if (BlockerProps && BlockerProps->GetMaterial(0))
	{
		UMaterialInstanceDynamic* DynamicMat = Cast<UMaterialInstanceDynamic>(BlockerProps->GetMaterial(0));
		if (!DynamicMat)
		{
			DynamicMat = UMaterialInstanceDynamic::Create(BlockerProps->GetMaterial(0), this);
			BlockerProps->SetMaterial(0, DynamicMat);
		}
		DynamicMat->SetVectorParameterValue(PlanetColorParameterName, BlockerColor);
	}

	if (PlantProps && PlantProps->GetMaterial(0))
	{
		UMaterialInstanceDynamic* DynamicMat = Cast<UMaterialInstanceDynamic>(PlantProps->GetMaterial(0));
		if (!DynamicMat)
		{
			DynamicMat = UMaterialInstanceDynamic::Create(PlantProps->GetMaterial(0), this);
			PlantProps->SetMaterial(0, DynamicMat);
		}
		FLinearColor PlantColor = FMath::Lerp(
			FLinearColor(0.12f, 0.80f, 0.20f),
			FLinearColor(0.30f, 0.60f, 0.25f),
			CrossfadeProgress);
		DynamicMat->SetVectorParameterValue(PlanetColorParameterName, PlantColor);
	}
}

void AOLCPlanetTerrainActor::SetBiome(EOLCBiomeType NewBiome)
{
	if (NewBiome == CurrentBiome && !bCrossfading)
		return;

	TargetBiome = NewBiome;
	bCrossfading = true;
	CrossfadeProgress = 0.0f;

	// WP-126 step 9: swap the looping biome ambient VFX to match the new biome (append-only).
	StartBiomeAmbientVFX(NewBiome);
}

void AOLCPlanetTerrainActor::SetBiomeByIndex(int32 BiomeIndex)
{
	const int32 ClampedIndex = FMath::Clamp(BiomeIndex, 0, 7);
	SetBiome(static_cast<EOLCBiomeType>(ClampedIndex));
}

void AOLCPlanetTerrainActor::ToggleRenderMode()
{
	RenderMode = RenderMode == EOLCTerrainRenderMode::SurfaceMesh ? EOLCTerrainRenderMode::DebugTiles : EOLCTerrainRenderMode::SurfaceMesh;
	GenerateAndRender();
}

void AOLCPlanetTerrainActor::Regenerate(int32 SeedOffset)
{
	GenerationSettings.Seed += SeedOffset;
	GenerateAndRender();
}

void AOLCPlanetTerrainActor::MarkTilesOccupied(const TArray<FIntPoint>& TileCoords)
{
	if (TileCoords.Num() == 0)
	{
		return;
	}

	for (FOLCTerrainTile& Tile : Tiles)
	{
		if (TileCoords.Contains(Tile.Coord))
		{
			Tile.bOccupied = true;
		}
	}
}

// ---------------------------------------------------------------------------
// Building placement helpers (WP-103 Step 3)
// ---------------------------------------------------------------------------
FVector AOLCPlanetTerrainActor::GetTileWorldPosition(FIntPoint Coord) const
{
	const FOLCTerrainTile* Tile = nullptr;
	for (const auto& T : Tiles)
	{
		if (T.Coord == Coord)
		{
			Tile = &T;
			break;
		}
	}
	if (!Tile)
	{
		return TileToWorld(Coord.X, Coord.Y, 0.0f);
	}
	return TileToWorld(Coord.X, Coord.Y, Tile->Height);
}

bool AOLCPlanetTerrainActor::CanPlaceBuilding(FIntPoint Origin, FVector2D Footprint, int32 RotationDegrees) const
{
	const int32 Width = FMath::RoundToInt(Footprint.X);
	const int32 Height = FMath::RoundToInt(Footprint.Y);

	// Swap dimensions if rotated 90 or 270 degrees.
	const int32 FinalWidth = (RotationDegrees % 180 != 0) ? Height : Width;
	const int32 FinalHeight = (RotationDegrees % 180 != 0) ? Width : Height;

	for (int32 Y = 0; Y < FinalHeight; ++Y)
	{
		for (int32 X = 0; X < FinalWidth; ++X)
		{
			const FIntPoint Coord = FIntPoint(Origin.X + X, Origin.Y + Y);

			// Check bounds.
			if (Coord.X < 0 || Coord.Y < 0 ||
				Coord.X >= GenerationSettings.MapWidth ||
				Coord.Y >= GenerationSettings.MapHeight)
			{
				return false;
			}

			// Find tile and check role + occupancy.
			for (const auto& Tile : Tiles)
			{
				if (Tile.Coord == Coord)
				{
					if (!Tile.IsBuildable() || Tile.bOccupied)
					{
						return false;
					}
					break;
				}
			}
		}
	}

	return true;
}

bool AOLCPlanetTerrainActor::PlaceBuilding(FIntPoint Origin, FVector2D Footprint, FVector& OutWorldPosition, int32 RotationDegrees)
{
	if (!CanPlaceBuilding(Origin, Footprint, RotationDegrees))
	{
		return false;
	}

	const int32 Width = FMath::RoundToInt(Footprint.X);
	const int32 Height = FMath::RoundToInt(Footprint.Y);

	// Swap dimensions if rotated 90 or 270 degrees.
	const int32 FinalWidth = (RotationDegrees % 180 != 0) ? Height : Width;
	const int32 FinalHeight = (RotationDegrees % 180 != 0) ? Width : Height;

	TArray<FIntPoint> TileCoords;
	TileCoords.Reserve(FinalWidth * FinalHeight);

	for (int32 Y = 0; Y < FinalHeight; ++Y)
	{
		for (int32 X = 0; X < FinalWidth; ++X)
		{
			const FIntPoint Coord = FIntPoint(Origin.X + X, Origin.Y + Y);
			TileCoords.Add(Coord);

			// Mark tile as occupied.
			for (auto& Tile : Tiles)
			{
				if (Tile.Coord == Coord)
				{
					Tile.bOccupied = true;
					break;
				}
			}
		}
	}

	// Calculate center tile world position.
	const FIntPoint CenterCoord = FIntPoint(
		Origin.X + FinalWidth / 2,
		Origin.Y + FinalHeight / 2);
	OutWorldPosition = GetTileWorldPosition(CenterCoord);

	return true;
}

void AOLCPlanetTerrainActor::GenerateAndRender()
{
	UOLCPlanetTerrainGenerator::GenerateTerrain(GenerationSettings, FindActiveProfile(), Tiles);
	ClearInstances();
	SurfaceMesh->ClearAllMeshSections();

	if (RenderMode == EOLCTerrainRenderMode::SurfaceMesh)
	{
		RenderSurfaceMesh();
		for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get() })
		{
			Component->SetVisibility(false);
		}
		SurfaceMesh->SetVisibility(true);
	}
	else
	{
		RenderDebugTiles();
		for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get() })
		{
			Component->SetVisibility(true);
		}
		SurfaceMesh->SetVisibility(false);
	}

	RenderProps();
	ApplyBiomeMaterials();
}

void AOLCPlanetTerrainActor::RenderSurfaceMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	Vertices.Reserve(GenerationSettings.MapWidth * GenerationSettings.MapHeight);
	UVs.Reserve(Vertices.Num());

	for (const FOLCTerrainTile& Tile : Tiles)
	{
		Vertices.Add(TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height));
		UVs.Add(FVector2D(static_cast<float>(Tile.Coord.X) / GenerationSettings.MapWidth, static_cast<float>(Tile.Coord.Y) / GenerationSettings.MapHeight));
		Normals.Add(FVector::UpVector);
		VertexColors.Add(GetBiomeSurfaceColor(GenerationSettings.Biome).ToFColor(true));
		Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
	}

	for (int32 Y = 0; Y < GenerationSettings.MapHeight - 1; ++Y)
	{
		for (int32 X = 0; X < GenerationSettings.MapWidth - 1; ++X)
		{
			const int32 A = Y * GenerationSettings.MapWidth + X;
			const int32 B = A + 1;
			const int32 C = A + GenerationSettings.MapWidth;
			const int32 D = C + 1;
			// Winding must face +Z (up) for the single-sided MAT_TerrainBase to be
			// visible from the top-down gameplay camera — the previous order
			// (A,C,B / B,C,D) faced -Z and was back-face culled, leaving the whole
			// ground invisible while convex prop meshes (which always show some
			// front face) still rendered.
			Triangles.Append({ A, B, C, B, D, C });
		}
	}

	SurfaceMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void AOLCPlanetTerrainActor::RenderDebugTiles()
{
	for (const FOLCTerrainTile& Tile : Tiles)
	{
		UInstancedStaticMeshComponent* Target = DebugBuildableTiles.Get();
		switch (Tile.Role)
		{
			case EOLCTerrainTileRole::Buildable: Target = DebugBuildableTiles.Get(); break;
			case EOLCTerrainTileRole::Restricted: Target = DebugRestrictedTiles.Get(); break;
			case EOLCTerrainTileRole::Blocked: Target = DebugBlockedTiles.Get(); break;
			case EOLCTerrainTileRole::Water: Target = DebugWaterTiles.Get(); break;
			case EOLCTerrainTileRole::Resource: Target = DebugResourceTiles.Get(); break;
			case EOLCTerrainTileRole::Dungeon: Target = DebugDungeonTiles.Get(); break;
			case EOLCTerrainTileRole::Landing: Target = DebugLandingTiles.Get(); break;
			case EOLCTerrainTileRole::Road: Target = DebugRestrictedTiles.Get(); break;
			case EOLCTerrainTileRole::Outside: Target = DebugOutsideTiles.Get(); break;
		}
		Target->AddInstance(FTransform(FRotator::ZeroRotator, TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height + 0.01f), FVector(GenerationSettings.TileSize / 100.0f, GenerationSettings.TileSize / 100.0f, 1.0f)));
	}
}

void AOLCPlanetTerrainActor::RenderProps()
{
	for (const FOLCTerrainTile& Tile : Tiles)
	{
		if (Tile.Role == EOLCTerrainTileRole::Blocked && Tile.DecorationDensity > 0.08f)
		{
			const float Scale = FMath::Lerp(0.45f, 1.85f, Tile.DecorationDensity);
			BlockerProps->AddInstance(FTransform(FRotator(0.0f, Tile.DecorationDensity * 360.0f, 0.0f), TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height + 0.15f), FVector(Scale, Scale * 0.78f, Scale * 0.65f)));
		}
		else if (Tile.Role == EOLCTerrainTileRole::Resource)
		{
			const float Scale = FMath::Lerp(0.45f, 1.1f, Tile.ResourceRichness);
			ResourceProps->AddInstance(FTransform(FRotator::ZeroRotator, TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height + 0.18f), FVector(Scale, Scale, Scale * 0.45f)));
		}
		else if (Tile.Role == EOLCTerrainTileRole::Dungeon)
		{
			DungeonProps->AddInstance(FTransform(FRotator(0.0f, 45.0f, 0.0f), TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height + 0.35f), FVector(2.2f, 1.4f, 0.7f)));
		}
		else if (Tile.Role == EOLCTerrainTileRole::Buildable && Tile.DecorationDensity > 0.22f)
		{
			PlantProps->AddInstance(FTransform(FRotator::ZeroRotator, TileToWorld(Tile.Coord.X, Tile.Coord.Y, Tile.Height + 0.12f), FVector(0.3f, 0.3f, 0.5f)));
		}
	}
}

void AOLCPlanetTerrainActor::ClearInstances()
{
	for (UInstancedStaticMeshComponent* Component : { DebugBuildableTiles.Get(), DebugRestrictedTiles.Get(), DebugBlockedTiles.Get(), DebugWaterTiles.Get(), DebugResourceTiles.Get(), DebugDungeonTiles.Get(), DebugLandingTiles.Get(), DebugOutsideTiles.Get(), BlockerProps.Get(), ResourceProps.Get(), DungeonProps.Get(), PlantProps.Get() })
	{
		if (Component)
		{
			Component->ClearInstances();
		}
	}
}

void AOLCPlanetTerrainActor::ApplyBiomeMaterials()
{
	const UOLCPlanetTerrainProfile* Profile = FindActiveProfile();
	if (Profile && Profile->TerrainMaterial)
	{
		SurfaceMesh->SetMaterial(0, Profile->TerrainMaterial);
	}

	ConfigureMaterial(DebugBuildableTiles, GetDebugColor(EOLCTerrainTileRole::Buildable));
	ConfigureMaterial(DebugRestrictedTiles, GetDebugColor(EOLCTerrainTileRole::Restricted));
	ConfigureMaterial(DebugBlockedTiles, GetDebugColor(EOLCTerrainTileRole::Blocked));
	ConfigureMaterial(DebugWaterTiles, GetDebugColor(EOLCTerrainTileRole::Water));
	ConfigureMaterial(DebugResourceTiles, GetDebugColor(EOLCTerrainTileRole::Resource));
	ConfigureMaterial(DebugDungeonTiles, GetDebugColor(EOLCTerrainTileRole::Dungeon));
	ConfigureMaterial(DebugLandingTiles, GetDebugColor(EOLCTerrainTileRole::Landing));
	ConfigureMaterial(DebugOutsideTiles, GetDebugColor(EOLCTerrainTileRole::Outside));
	ConfigureMaterial(BlockerProps, Profile ? Profile->BlockerColor : GetBiomeBlockerColor(GenerationSettings.Biome));
	ConfigureMaterial(ResourceProps, FLinearColor(0.08f, 0.65f, 0.78f));
	ConfigureMaterial(DungeonProps, FLinearColor(0.42f, 0.16f, 0.78f));
	ConfigureMaterial(PlantProps, FLinearColor(0.12f, 0.80f, 0.20f), 0.4f);

	// WP-108 Step 5: Apply biome-specific atmosphere
	ApplyAtmosphere(Profile);
}

void AOLCPlanetTerrainActor::ApplyAtmosphere(const UOLCPlanetTerrainProfile* Profile)
{
	if (!GetWorld())
		return;

	// Get world settings for fog and lighting
	AWorldSettings* WS = GetWorld()->GetWorldSettings();
	if (!WS)
		return;

	FLinearColor FogCol = FLinearColor(0.75f, 0.65f, 0.50f); // Default warm desert haze
	float FogDens = 0.002f;
	FLinearColor DirLightCol = FLinearColor(1.0f, 0.90f, 0.75f); // Orange sunrise

	if (Profile)
	{
		FogCol = Profile->FogColor;
		FogDens = Profile->FogDensity;
		DirLightCol = Profile->DirectionalLightColor;
	}
	else
	{
		// Fallback biome-specific defaults
		switch (GenerationSettings.Biome)
		{
			case EOLCBiomeType::Desert: FogCol = FLinearColor(0.85f, 0.75f, 0.60f); FogDens = 0.002f; DirLightCol = FLinearColor(1.0f, 0.85f, 0.60f); break;
			case EOLCBiomeType::Dusty: FogCol = FLinearColor(0.70f, 0.65f, 0.55f); FogDens = 0.004f; DirLightCol = FLinearColor(0.90f, 0.85f, 0.75f); break;
			case EOLCBiomeType::Rocky: FogCol = FLinearColor(0.60f, 0.60f, 0.62f); FogDens = 0.003f; DirLightCol = FLinearColor(0.75f, 0.75f, 0.78f); break;
			case EOLCBiomeType::Water: FogCol = FLinearColor(0.40f, 0.55f, 0.75f); FogDens = 0.006f; DirLightCol = FLinearColor(0.70f, 0.80f, 0.95f); break;
			case EOLCBiomeType::Swamp: FogCol = FLinearColor(0.45f, 0.55f, 0.35f); FogDens = 0.008f; DirLightCol = FLinearColor(0.75f, 0.80f, 0.60f); break;
			case EOLCBiomeType::Jungle: FogCol = FLinearColor(0.40f, 0.60f, 0.35f); FogDens = 0.008f; DirLightCol = FLinearColor(0.70f, 0.90f, 0.65f); break;
			case EOLCBiomeType::LightSnow: FogCol = FLinearColor(0.80f, 0.82f, 0.85f); FogDens = 0.010f; DirLightCol = FLinearColor(0.85f, 0.88f, 0.92f); break;
			case EOLCBiomeType::Ice: FogCol = FLinearColor(0.75f, 0.85f, 0.92f); FogDens = 0.015f; DirLightCol = FLinearColor(0.70f, 0.85f, 0.95f); break;
		}
	}

	// Apply fog settings to world
	for (TActorIterator<AExponentialHeightFog> It(GetWorld()); It; ++It)
	{
		if (UExponentialHeightFogComponent* FogComponent = (*It)->GetComponent())
		{
			FogComponent->SetFogDensity(FogDens);
			FogComponent->SetFogInscatteringColor(FogCol);
		}
		break;
	}

	// Update directional light color if available
	for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
	{
		if (UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>((*It)->GetLightComponent()))
		{
			LightComponent->SetLightColor(DirLightCol);
		}
		break;
	}
}

void AOLCPlanetTerrainActor::ConfigureMaterial(UInstancedStaticMeshComponent* Component, const FLinearColor& Color, float Roughness)
{
	if (!Component || !Component->GetMaterial(0))
	{
		return;
	}

	// Create a dynamic instance from the current material (or reuse it if this component
	// already has one from a prior call — GenerateAndRender runs more than once per actor
	// lifetime, and MIDs cannot parent other MIDs).
	UMaterialInterface* SourceMat = Component->GetMaterial(0);
	if (!SourceMat)
	{
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(SourceMat);
	if (!DynamicMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(SourceMat, this);
		Component->SetMaterial(0, DynamicMaterial);
	}

	DynamicMaterial->SetVectorParameterValue(PlanetColorParameterName, Color);
	DynamicMaterial->SetScalarParameterValue(PlanetRoughnessParameterName, Roughness);
}

FVector AOLCPlanetTerrainActor::TileToWorld(int32 X, int32 Y, float Height01) const
{
	return FVector((X - (GenerationSettings.MapWidth - 1) * 0.5f) * GenerationSettings.TileSize, (Y - (GenerationSettings.MapHeight - 1) * 0.5f) * GenerationSettings.TileSize, Height01 * GenerationSettings.HeightScale);
}

FLinearColor AOLCPlanetTerrainActor::GetDebugColor(EOLCTerrainTileRole InRole) const
{
	switch (InRole)
	{
		case EOLCTerrainTileRole::Buildable: return FLinearColor(0.0f, 0.902f, 0.18f);   // #00E62E
		case EOLCTerrainTileRole::Restricted: return FLinearColor(1.0f, 0.82f, 0.08f);    // #FFD114
		case EOLCTerrainTileRole::Blocked: return FLinearColor(0.902f, 0.055f, 0.031f);   // #E60E08
		case EOLCTerrainTileRole::Water: return FLinearColor(0.051f, 0.322f, 0.949f);     // #0D52F2
		case EOLCTerrainTileRole::Resource: return FLinearColor(0.0f, 0.851f, 0.902f);    // #00D9E6
		case EOLCTerrainTileRole::Dungeon: return FLinearColor(0.549f, 0.102f, 0.902f);   // #8C1AE6
		case EOLCTerrainTileRole::Landing: return FLinearColor::White;                    // White
		case EOLCTerrainTileRole::Road: return FLinearColor(1.0f, 0.451f, 0.078f);        // Orange (#FF7314)
		case EOLCTerrainTileRole::Outside: return FLinearColor::Black;                    // Invisible
	}
	return FLinearColor::Black;
}

FLinearColor AOLCPlanetTerrainActor::GetBiomeSurfaceColor(EOLCBiomeType Biome) const
{
	switch (Biome)
	{
		case EOLCBiomeType::Desert: return FLinearColor(0.72f, 0.55f, 0.26f);
		case EOLCBiomeType::Dusty: return FLinearColor(0.46f, 0.36f, 0.25f);
		case EOLCBiomeType::Rocky: return FLinearColor(0.30f, 0.29f, 0.27f);
		case EOLCBiomeType::Water: return FLinearColor(0.08f, 0.24f, 0.45f);
		case EOLCBiomeType::Swamp: return FLinearColor(0.20f, 0.30f, 0.16f);
		case EOLCBiomeType::Jungle: return FLinearColor(0.12f, 0.36f, 0.16f);
		case EOLCBiomeType::LightSnow: return FLinearColor(0.72f, 0.76f, 0.74f);
		case EOLCBiomeType::Ice: return FLinearColor(0.58f, 0.76f, 0.88f);
	}
	return FLinearColor(0.25f, 0.25f, 0.25f);
}

FLinearColor AOLCPlanetTerrainActor::GetBiomeBlockerColor(EOLCBiomeType Biome) const
{
	switch (Biome)
	{
		case EOLCBiomeType::Desert: return FLinearColor(0.55f, 0.42f, 0.22f);
		case EOLCBiomeType::Dusty: return FLinearColor(0.28f, 0.24f, 0.20f);
		case EOLCBiomeType::Rocky: return FLinearColor(0.12f, 0.12f, 0.12f);
		case EOLCBiomeType::Water: return FLinearColor(0.08f, 0.16f, 0.22f);
		case EOLCBiomeType::Swamp: return FLinearColor(0.11f, 0.18f, 0.08f);
		case EOLCBiomeType::Jungle: return FLinearColor(0.05f, 0.20f, 0.08f);
		case EOLCBiomeType::LightSnow: return FLinearColor(0.56f, 0.59f, 0.58f);
		case EOLCBiomeType::Ice: return FLinearColor(0.36f, 0.56f, 0.68f);
	}
	return FLinearColor(0.12f, 0.12f, 0.12f);
}

const UOLCPlanetTerrainProfile* AOLCPlanetTerrainActor::FindActiveProfile() const
{
	for (const UOLCPlanetTerrainProfile* Profile : BiomeProfiles)
	{
		if (Profile && Profile->Biome == GenerationSettings.Biome)
		{
			return Profile;
		}
	}
	return nullptr;
}

// ---------------------------------------------------------------------------
// Biome ambient VFX hook (WP-126 step-9; append-only)
// ---------------------------------------------------------------------------
void AOLCPlanetTerrainActor::StartBiomeAmbientVFX(EOLCBiomeType NewBiome)
{
	// Stop the currently active ambient first — exactly one ambient at a time.
	if (AActor* Old = BiomeAmbientActor.Get())
	{
		Old->Destroy();
		BiomeAmbientActor = nullptr;

		// Diagnostic: make the stop observable in PIE logs (WP-126 step-9 verification).
		UE_LOG(LogOLC, Log, TEXT("[OLC] Biome ambient VFX: previous ambient stopped"));
	}

	UWorld* World = GetWorld();
	const UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	if (!GI)
	{
		return;
	}

	if (UOLCVFXSubsystem* VFX = GI->GetSubsystem<UOLCVFXSubsystem>())
	{
		BiomeAmbientActor = VFX->SpawnBiomeAmbient(NewBiome, GetActorLocation());

		// Diagnostic: make the start observable in PIE logs (WP-126 step-9 verification).
		UE_LOG(LogOLC, Log, TEXT("[OLC] Biome ambient VFX: biome %d ambient %s"),
			static_cast<int32>(NewBiome),
			BiomeAmbientActor.IsValid() ? TEXT("started") : TEXT("FAILED to start"));
	}
}
