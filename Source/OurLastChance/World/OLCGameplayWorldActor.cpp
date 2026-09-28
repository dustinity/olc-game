#include "OLCGameplayWorldActor.h"
#include "OurLastChance.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Core/OLCChampionData.h"
#include "Core/OLCFactionData.h"
#include "Core/OLCPlanetTerrainProfile.h"
#include "Core/OLCResearchSubsystem.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "World/OLCPlanetTerrainActor.h"

// Same "search a fixed Content path, load candidates, match by ID field"
// pattern already used by UOLCUIDataSubsystem::PopulateFakeBuildCards() for
// building DataAssets — no champion DataAssets have been authored yet
// (Content/OurLastChance/Data has only Buildings/ so far), so this returns
// nullptr until some exist, which GetResearchSpeedMultiplier() already
// handles gracefully (no champion bonus applied).
static UOLCChampionData* FindChampionDataById(const FString& ChampionId)
{
	if (ChampionId.IsEmpty()) return nullptr;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	FARFilter Filter;
	Filter.ClassPaths.Add(UOLCChampionData::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(TEXT("/Game/OurLastChance/Data/Champions"));
	Filter.bRecursivePaths = true;
	TArray<FAssetData> ChampionAssets;
	AssetRegistryModule.Get().GetAssets(Filter, ChampionAssets);
	for (const FAssetData& Asset : ChampionAssets)
	{
		if (UOLCChampionData* Data = Cast<UOLCChampionData>(Asset.GetAsset()))
		{
			if (Data->ChampionId.Equals(ChampionId, ESearchCase::IgnoreCase)) return Data;
		}
	}
	return nullptr;
}

AOLCGameplayWorldActor::AOLCGameplayWorldActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = Root;

    CrashedShipComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrashedShip"));
    CrashedShipComponent->SetupAttachment(RootComponent);
    CrashedShipComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CrashedShipComponent->SetMobility(EComponentMobility::Movable);
    CrashedShipComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

    TerrainActorClass = AOLCPlanetTerrainActor::StaticClass();

    static ConstructorHelpers::FObjectFinder<UOLCPlanetTerrainProfile> DesertProfile(
        TEXT("/Game/Terrain/Profiles/DA_TerrainProfile_Desert.DA_TerrainProfile_Desert"));
    if (DesertProfile.Succeeded())
    {
        TerrainProfiles.Add(DesertProfile.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CrashedShipMeshFinder(
        TEXT("/Game/Terrain/Desert/Props/StartLocation/TX_Start_CrashedShip.TX_Start_CrashedShip"));
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

    if (!bIsInitialized)
    {
        InitializeGameplayWorld(RequestedBiome, TerrainSettings.Seed, CurrentFaction, SelectedChampionId);
    }
}

FVector AOLCGameplayWorldActor::GetCrashedShipLocation() const
{
    return CrashedShipComponent ? CrashedShipComponent->GetComponentLocation() : FVector::ZeroVector;
}

void AOLCGameplayWorldActor::InitializeGameplayWorld(EOLCBiomeType Biome, int32 Seed, UOLCFactionData* Faction, const FString& ChampionId)
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
    SelectedChampionId = ChampionId;

    if (UGameInstance* GI = UGameplayStatics::GetGameInstance(this))
    {
        if (UOLCResearchSubsystem* Research = GI->GetSubsystem<UOLCResearchSubsystem>())
        {
            Research->SetActiveFactionAndChampion(Faction, FindChampionDataById(ChampionId));
        }
    }

    SpawnTerrainActor();
    PlaceCrashedShip();
    SpawnSelectedChampionMarker();
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
        UE_LOG(LogOLC, Warning, TEXT("[OLC] Failed to spawn terrain actor for gameplay world"));
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
            const FIntPoint Center(TerrainSettings.MapWidth / 2, TerrainSettings.MapHeight / 2);
            if (FMath::Abs(Tile.Coord.X - Center.X) <= 1 && FMath::Abs(Tile.Coord.Y - Center.Y) <= 1)
            {
                LandingTileCoords.Add(Tile.Coord);
            }
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

void AOLCGameplayWorldActor::SpawnSelectedChampionMarker()
{
    if (!GetWorld() || SelectedChampionId.IsEmpty())
    {
        return;
    }

    FVector SpawnLocation = FVector(360.0f, 540.0f, 160.0f);
    if (CrashedShipComponent)
    {
        SpawnLocation = CrashedShipComponent->GetComponentLocation() + FVector(360.0f, 540.0f, 80.0f);
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AStaticMeshActor* Marker = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (!Marker)
    {
        return;
    }

    if (UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")))
    {
        Marker->GetStaticMeshComponent()->SetStaticMesh(SphereMesh);
    }
    Marker->SetActorScale3D(FVector(0.45f, 0.45f, 0.9f));
#if WITH_EDITOR
    Marker->SetActorLabel(FString::Printf(TEXT("SelectedChampion_%s"), *SelectedChampionId));
#endif
    ChampionMarkerActor = Marker;

    UTextRenderComponent* Label = NewObject<UTextRenderComponent>(Marker, TEXT("ChampionLabel"));
    if (Label)
    {
        Label->RegisterComponent();
        Label->AttachToComponent(Marker->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        Label->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
        Label->SetHorizontalAlignment(EHTA_Center);
        Label->SetWorldSize(44.0f);
        Label->SetText(FText::FromString(SelectedChampionId));
    }

    UE_LOG(LogOLC, Display, TEXT("[OLC] Spawned selected champion marker '%s' near crashed dropship ramp"), *SelectedChampionId);
}
