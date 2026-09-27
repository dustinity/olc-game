#include "OLCGameplayDemoGameMode.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Player/OLCGameplayPlayerController.h"
#include "Logging/LogMacros.h"

AOLCGameplayDemoGameMode::AOLCGameplayDemoGameMode()
{
    PlayerControllerClass = AOLCGameplayPlayerController::StaticClass();
    PrimaryActorTick.bCanEverTick = false;
}

void AOLCGameplayDemoGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Display, TEXT("[OLC] Gameplay Demo started — RTS camera and gameplay controls active"));

    if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
    {
        SetupRTSCamera(PC);
    }
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
        UE_LOG(LogTemp, Warning, TEXT("[OLC] Failed to spawn gameplay RTS camera"));
        return;
    }

    RTSCamera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
    RTSCamera->GetCameraComponent()->SetOrthoWidth(7600.0f);
    PC->SetViewTargetWithBlend(RTSCamera, 0.5f);
}
