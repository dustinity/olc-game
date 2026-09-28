#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OLCGameplayDemoGameMode.generated.h"

class APlayerController;
class ACameraActor;

UCLASS()
class OURLASTCHANCE_API AOLCGameplayDemoGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOLCGameplayDemoGameMode();

protected:
    virtual void BeginPlay() override;

private:
    void SetupRTSCamera(APlayerController* PC);
    void SetupDemoBuildings();

    UPROPERTY()
    TObjectPtr<ACameraActor> RTSCamera;

    FTimerHandle DemoSetupTimer;
};
