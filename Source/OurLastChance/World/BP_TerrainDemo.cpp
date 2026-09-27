// Copyright Chris, All rights reserved.

#include "BP_TerrainDemo.h"

#include "Components/SceneComponent.h"
#include "World/OLCPlanetTerrainActor.h"
#include "Core/OLCPlanetTerrainProfile.h"

ABP_TerrainDemo::ABP_TerrainDemo()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	TerrainActor = CreateDefaultSubobject<AOLCPlanetTerrainActor>(TEXT("TerrainActor"));

	// Default generation settings for demo — 128x128 Desert map
	GenerationSettings.MapWidth = 128;
	GenerationSettings.MapHeight = 128;
	GenerationSettings.TileSize = 180.0f;
	GenerationSettings.HeightScale = 180.0f;
	GenerationSettings.Seed = 184736;
	GenerationSettings.Biome = EOLCBiomeType::Desert;
}

void ABP_TerrainDemo::BeginPlay()
{
	Super::BeginPlay();
	ConfigureTerrainActor();
}

void ABP_TerrainDemo::ConfigureTerrainActor()
{
	if (!TerrainActor) return;

	TerrainActor->SetGenerationSettings(GenerationSettings);
	TerrainActor->SetBiomeProfiles(MoveTemp(BiomeProfiles));

	// Trigger generation — uses the actor's own BeginPlay path via GenerateAndRender
	// (called through SetBiome which is public and triggers re-render)
	TerrainActor->SetBiome(GenerationSettings.Biome);
}
