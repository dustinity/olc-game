#include "OLCGameplayDemoGameMode.h"
#include "OurLastChance.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Player/OLCGameplayPlayerController.h"
#include "Logging/LogMacros.h"
#include "Core/OLCBuildingData.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "World/OLCBuildingBase.h"
#include "World/OLCPlanetTerrainActor.h"

AOLCGameplayDemoGameMode::AOLCGameplayDemoGameMode()
{
    PlayerControllerClass = AOLCGameplayPlayerController::StaticClass();
    PrimaryActorTick.bCanEverTick = false;
}

void AOLCGameplayDemoGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogOLC, Display, TEXT("[OLC] Gameplay Demo started — RTS camera and gameplay controls active"));

    if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
    {
        SetupRTSCamera(PC);
    }

    GetWorldTimerManager().SetTimer(DemoSetupTimer, this, &AOLCGameplayDemoGameMode::SetupDemoBuildings, 0.25f, false);
}

void AOLCGameplayDemoGameMode::SetupDemoBuildings()
{
    UWorld* World = GetWorld();
    AOLCPlanetTerrainActor* Terrain = World ? Cast<AOLCPlanetTerrainActor>(UGameplayStatics::GetActorOfClass(World, AOLCPlanetTerrainActor::StaticClass())) : nullptr;
    if (!Terrain) return;

    auto SpawnDemoBuilding = [World, Terrain](const TCHAR* BlueprintPath, const TCHAR* DataPath, FIntPoint PreferredOrigin)
    {
        UOLCBuildingData* DataAsset = LoadObject<UOLCBuildingData>(nullptr, DataPath);
        TSubclassOf<AOLCBuildingBase> BuildingClass = LoadClass<AOLCBuildingBase>(nullptr, BlueprintPath);
        if (!DataAsset || !BuildingClass) return;

        FVector WorldPosition;
        bool bPlaced = false;
        for (int32 Radius = 0; Radius <= 5 && !bPlaced; ++Radius)
        {
            for (int32 Y = -Radius; Y <= Radius && !bPlaced; ++Y)
            {
                for (int32 X = -Radius; X <= Radius && !bPlaced; ++X)
                {
                    bPlaced = Terrain->PlaceBuilding(PreferredOrigin + FIntPoint(X, Y), DataAsset->GridSize, WorldPosition);
                }
            }
        }
        if (!bPlaced) return;

        AOLCBuildingBase* Building = World->SpawnActor<AOLCBuildingBase>(BuildingClass, WorldPosition, FRotator::ZeroRotator);
        if (!Building) return;
        Building->SetBuildingData(DataAsset->ToConfig());
        Building->SetPowered(true);
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UOLCUIDataSubsystem* UIData = GI->GetSubsystem<UOLCUIDataSubsystem>()) UIData->OnBuildingPlaced(Building, DataAsset);
        }
    };

    FIntPoint ResourceCoord = Terrain->GetMapDimensions() / 2;
    for (const FOLCTerrainTile& Tile : Terrain->GetTiles())
    {
        if (Tile.Role == EOLCTerrainTileRole::Resource)
        {
            ResourceCoord = Tile.Coord;
            break;
        }
    }

    SpawnDemoBuilding(TEXT("/Game/OurLastChance/Buildings/BP_SolarArray.BP_SolarArray_C"), TEXT("/Game/OurLastChance/Data/Buildings/DA_Building_SolarArray.DA_Building_SolarArray"), ResourceCoord + FIntPoint(-4, 0));
    SpawnDemoBuilding(TEXT("/Game/OurLastChance/Buildings/BP_Mine.BP_Mine_C"), TEXT("/Game/OurLastChance/Data/Buildings/DA_Building_Mine.DA_Building_Mine"), ResourceCoord + FIntPoint(1, 0));
}

void AOLCGameplayDemoGameMode::SetupRTSCamera(APlayerController* PC)
{
    check(PC);

    FVector CameraLocation(0.0f, 0.0f, 4500.0f);
    FRotator CameraRotation(-80.0f, 90.0f, 0.0f);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    RTSCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);
    if (!RTSCamera)
    {
        UE_LOG(LogOLC, Warning, TEXT("[OLC] Failed to spawn gameplay RTS camera"));
        return;
    }

    RTSCamera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
    RTSCamera->GetCameraComponent()->SetOrthoWidth(7600.0f);
    PC->SetViewTargetWithBlend(RTSCamera, 0.5f);
}
