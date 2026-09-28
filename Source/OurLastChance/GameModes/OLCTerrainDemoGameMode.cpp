#include "OLCTerrainDemoGameMode.h"
#include "OurLastChance.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Player/OLCGameplayPlayerController.h"
#include "Logging/LogMacros.h"

AOLCTerrainDemoGameMode::AOLCTerrainDemoGameMode()
{
	// Use the gameplay player controller — it has all terrain demo keybindings (1-8, G, T, arrow keys)
	PlayerControllerClass = AOLCGameplayPlayerController::StaticClass();

	PrimaryActorTick.bCanEverTick = false;
}

void AOLCTerrainDemoGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogOLC, Display, TEXT("[OLC] Terrain Demo started — keys 1-8 switch biome, G toggles debug, T regenerates"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		SetupRTSCamera(PC);
	}
}

void AOLCTerrainDemoGameMode::SetupRTSCamera(APlayerController* PC)
{
	check(PC);

	// 45-degree RTS camera — positioned above center looking down at 45 degrees
	FVector CameraLocation(0.0f, 0.0f, 3600.0f);
	FRotator CameraRotation(-45.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RTSCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);

	if (RTSCamera)
	{
		// Perspective camera at 45 degrees — not orthographic for depth perception
		RTSCamera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Perspective);
		RTSCamera->GetCameraComponent()->SetFieldOfView(60.0f);

		PC->SetViewTargetWithBlend(RTSCamera, 0.5f);

		UE_LOG(LogOLC, Log, TEXT("[OLC] 45-degree RTS camera placed above terrain center"));
	}
	else
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Failed to spawn RTS camera for Terrain Demo"));
	}
}
