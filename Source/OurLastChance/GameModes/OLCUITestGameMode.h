#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OLCUITestGameMode.generated.h"

class APlayerController;
class UUserWidget;
class UOLCUIDataSubsystem;

/**
 * Dedicated test game mode for opening any UI screen without full gameplay.
 * Spawns the test switcher widget on BeginPlay.
 */
UCLASS()
class OURLASTCHANCE_API AOLCUITestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOLCUITestGameMode();

protected:
	virtual void BeginPlay() override;

	/** Get the UI data subsystem. */
	UFUNCTION(BlueprintPure, Category = "OLC|UI")
	UOLCUIDataSubsystem* GetUIDataSubsystem();
};
