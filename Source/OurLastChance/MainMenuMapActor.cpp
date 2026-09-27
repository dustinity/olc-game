// MainMenuMapActor.cpp
// Minimal map actor for MainMenu.umap — provides lighting and PlayerStart
// This file is referenced by the MainMenu map which should be created in UE5 Editor.
// For testing, this actor can be placed directly in any map.

#include "MainMenuMapActor.h"

#include "Camera/CameraActor.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AMainMenuMapActor::AMainMenuMapActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Directional light for sky visibility
	DirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MainLight"));
	DirectionalLight->SetupAttachment(RootComponent);
	DirectionalLight->SetRelativeRotation(FRotator(30.0f, 50.0f, 0.0f));
	DirectionalLight->Intensity = 3000.0f;
}

void AMainMenuMapActor::BeginPlay()
{
	Super::BeginPlay();

	bool bHasPlayerStart = false;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		bHasPlayerStart = true;
		break;
	}

	if (!bHasPlayerStart && GetWorld())
	{
		GetWorld()->SpawnActor<APlayerStart>(GetActorLocation(), GetActorRotation());
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] MainMenuMapActor initialized — lighting and spawn ready"));
}
