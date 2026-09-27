#include "OLCGameplayWorldActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/OLCFactionData.h"
#include "Core/OLCPlanetTerrainProfile.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "World/OLCPlanetTerrainActor.h"

AOLCGameplayWorldActor::AOLCGameplayWorldActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = Root;

    CrashedShipComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrashedShip"));
    CrashedShipComponent->SetupAttachment(RootComponent);
    CrashedShipComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CrashedShipComponent->SetMobility(EComponentMobility::Static);
    CrashedShipComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

    TerrainActorClass = AOLCPlanetTerrainActor::StaticClass();

    static ConstructorHelpers::FObjectFinder<UOLCPlanetTerrainProfile> DesertProfile(
        TEXT("/Game/Terrain/Profiles/DA_TerrainProfile_Desert.DA_TerrainProfile_Desert"));
    if (DesertProfile.Succeeded())
    {
        TerrainProfiles.Add(DesertProfile.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CrashedShipMeshFinder(
        TEXT("/Game/Terrain/Actors/TX_Start_CrashedShip.TX_Start_CrashedShip"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(
        TEXT("/Engine/BasicShapes/Cube.Cube"));

    if (CrashedShipMeshFinder.Succeeded())
    {
        CrashedShipMesh = CrashedShipMeshFinder.Object;
    }
    else if (CubeMeshFinder.Succeeded())
    {
        CrashedShipMesh = CubeMeshFinder.Object;
    }

    if (CrashedShipMesh)
    {
        CrashedShipComponent->SetStaticMesh(CrashedShipMesh);
    }
}

void AOLCGameplayWorldActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOLCGameplayWorldActor::InitializeGameplayWorld(EOLCBiomeType Biome, int32 Seed, UOLCFactionData* Faction)
{
    if (bIsInitialized)
    {
        return;
    }

    RequestedBiome = Biome;
    TerrainSettings.MapWidth = 128;
    TerrainSettings.MapHeight = 128;
    TerrainSettings.TileSize = 180.0f;
    TerrainSettings.HeightScale = 180.0f;
    TerrainSettings.Seed = Seed;
    TerrainSettings.Biome = Biome;

    CurrentFaction = Faction;

    SpawnTerrainActor();
    PlaceCrashedShip();
    bIsInitialized = true;
}

void AOLCGameplayWorldActor::SpawnTerrainActor()
{
    if (!GetWorld() || !TerrainActorClass)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    TerrainActor = GetWorld()->SpawnActor<AOLCPlanetTerrainActor>(TerrainActorClass, FTransform::Identity, SpawnParams);
    if (!TerrainActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("[OLC] Failed to spawn terrain actor for gameplay world"));
        return;
    }

    TerrainActor->SetBiomeProfiles(TerrainProfiles);
    TerrainActor->SetGenerationSettings(TerrainSettings);
    TerrainActor->SetBiome(RequestedBiome);
}

void AOLCGameplayWorldActor::PlaceCrashedShip()
{
    if (!TerrainActor || !CrashedShipComponent)
    {
        return;
    }

    const TArray<FOLCTerrainTile>& Tiles = TerrainActor->GetTiles();
    if (Tiles.Num() == 0)
    {
        return;
    }

    TArray<FIntPoint> LandingTileCoords;
    FVector CenterShipLocation = FVector::ZeroVector;
    bool bFoundCenterTile = false;

    for (const FOLCTerrainTile& Tile : Tiles)
    {
        if (Tile.Role == EOLCTerrainTileRole::Landing)
        {
            LandingTileCoords.Add(Tile.Coord);
            if (Tile.Coord == FIntPoint(TerrainSettings.MapWidth / 2, TerrainSettings.MapHeight / 2))
            {
                CenterShipLocation = FVector(
                    (Tile.Coord.X - (TerrainSettings.MapWidth - 1) * 0.5f) * TerrainSettings.TileSize,
                    (Tile.Coord.Y - (TerrainSettings.MapHeight - 1) * 0.5f) * TerrainSettings.TileSize,
                    Tile.Height * TerrainSettings.HeightScale + 20.0f);
                bFoundCenterTile = true;
            }
        }
    }

    if (!LandingTileCoords.Num())
    {
        return;
    }

    if (!bFoundCenterTile)
    {
        const FOLCTerrainTile* FirstLanding = Tiles.FindByPredicate([](const FOLCTerrainTile& Tile) {
            return Tile.Role == EOLCTerrainTileRole::Landing;
        });
        if (FirstLanding)
        {
            CenterShipLocation = FVector(
                (FirstLanding->Coord.X - (TerrainSettings.MapWidth - 1) * 0.5f) * TerrainSettings.TileSize,
                (FirstLanding->Coord.Y - (TerrainSettings.MapHeight - 1) * 0.5f) * TerrainSettings.TileSize,
                FirstLanding->Height * TerrainSettings.HeightScale + 20.0f);
        }
    }

    CenterShipLocation.Z += 90.0f;
    CrashedShipComponent->SetRelativeLocation(CenterShipLocation);
    CrashedShipComponent->SetRelativeScale3D(FVector(5.4f, 5.4f, 1.2f));
    CrashedShipComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

    TerrainActor->MarkTilesOccupied(LandingTileCoords);
}
