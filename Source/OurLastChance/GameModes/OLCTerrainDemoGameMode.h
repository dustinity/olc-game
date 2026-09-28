#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OLCTerrainDemoGameMode.generated.h"

class APlayerController;
class ACameraActor;

/**
 * Game mode for the Terrain Demo level.
 * Uses OLCGameplayPlayerController for terrain biome switching (keys 1-8, G, T) and camera panning.
 * Spawns a 45-degree RTS camera on BeginPlay so the player can immediately see the terrain.
 */
UCLASS()
class OURLASTCHANCE_API AOLCTerrainDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOLCTerrainDemoGameMode();

protected:
	virtual void BeginPlay() override;

private:
	void SetupRTSCamera(APlayerController* PC);

	UPROPERTY()
	TObjectPtr<ACameraActor> RTSCamera;
};
