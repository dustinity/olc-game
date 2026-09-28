#include "OLCUITestGameMode.h"
#include "OurLastChance.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "EngineUtils.h"
#include "Engine/SkyLight.h"
#include "Kismet/GameplayStatics.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCCrashSitePrototypeActor.h"

AOLCUITestGameMode::AOLCUITestGameMode()
{
	PlayerControllerClass = AOLCMenuPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AOLCUITestGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->SpawnActor<AOLCCrashSitePrototypeActor>(AOLCCrashSitePrototypeActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

	const FVector CameraLocation(-3900.0f, -4300.0f, 4200.0f);
	const FRotator CameraRotation = (FVector::ZeroVector - CameraLocation).Rotation();
	ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation);
	if (Camera)
	{
		Camera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		Camera->GetCameraComponent()->SetOrthoWidth(7600.0f);
		Camera->GetCameraComponent()->SetConstraintAspectRatio(false);

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->SetViewTarget(Camera);

			// Pass camera reference to menu controller for zoom.
			if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PlayerController))
			{
				MenuPC->SetActiveCamera(Camera);
			}
		}
	}

	ADirectionalLight* KeyLight = nullptr;
	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		KeyLight = *It;
		break;
	}
	if (!KeyLight)
	{
		KeyLight = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), FVector(-1200.0f, -1600.0f, 2400.0f), FRotator(-52.0f, -35.0f, 0.0f));
	}
	if (KeyLight)
	{
		KeyLight->GetLightComponent()->SetIntensity(3.5f);
	}

	ASkyLight* SkyLight = nullptr;
	for (TActorIterator<ASkyLight> It(World); It; ++It)
	{
		SkyLight = *It;
		break;
	}
	if (!SkyLight)
	{
		SkyLight = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	}
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(0.35f);
	}

	// Reset all resources to 0 for fresh game start.
	if (UOLCUIDataSubsystem* Data = GetUIDataSubsystem())
	{
		Data->ResetResourcesToZero();
		UE_LOG(LogOLC, Log, TEXT("[OLC] Resources reset to 0 for new game start"));
	}
}

UOLCUIDataSubsystem* AOLCUITestGameMode::GetUIDataSubsystem()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		return GI->GetSubsystem<UOLCUIDataSubsystem>();
	}
	return nullptr;
}
