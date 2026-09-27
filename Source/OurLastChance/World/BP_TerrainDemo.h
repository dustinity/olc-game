#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/OLCTerrainTypes.h"
#include "BP_TerrainDemo.generated.h"

class AOLCPlanetTerrainActor;
class UOLCPlanetTerrainProfile;

/**
 * Terrain demo actor — wraps the planet terrain actor with demo defaults.
 * Place this in the TerrainDemo level alongside a PlayerStart and DirectionalLight.
 */
UCLASS()
class OURLASTCHANCE_API ABP_TerrainDemo : public AActor
{
	GENERATED_BODY()

public:
	ABP_TerrainDemo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	FOLCTerrainGenerationSettings GenerationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	TArray<TObjectPtr<UOLCPlanetTerrainProfile>> BiomeProfiles;

private:
	void ConfigureTerrainActor();

	UPROPERTY()
	TObjectPtr<AOLCPlanetTerrainActor> TerrainActor;

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;
};
