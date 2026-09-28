// MainMenuMapActor.h
// Minimal map actor for MainMenu.umap — provides lighting and PlayerStart

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuMapActor.generated.h"

class UDirectionalLightComponent;

UCLASS()
class OURLASTCHANCE_API AMainMenuMapActor : public AActor
{
	GENERATED_BODY()

public:
	AMainMenuMapActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "OLC|Map")
	TObjectPtr<UDirectionalLightComponent> DirectionalLight;
};
