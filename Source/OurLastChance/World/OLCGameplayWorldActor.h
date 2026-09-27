#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/OLCTerrainTypes.h"
#include "OLCGameplayWorldActor.generated.h"

class AOLCPlanetTerrainActor;
class UOLCPlanetTerrainProfile;
class UOLCFactionData;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class OURLASTCHANCE_API AOLCGameplayWorldActor : public AActor
{
    GENERATED_BODY()

public:
    AOLCGameplayWorldActor();

    UFUNCTION(BlueprintCallable, Category = "OLC|Gameplay")
    void InitializeGameplayWorld(EOLCBiomeType Biome, int32 Seed, UOLCFactionData* Faction);

protected:
    virtual void BeginPlay() override;

private:
    void SpawnTerrainActor();
    void PlaceCrashedShip();

    UPROPERTY(EditAnywhere, Category = "OLC|Gameplay")
    TSubclassOf<AOLCPlanetTerrainActor> TerrainActorClass;

    UPROPERTY(EditAnywhere, Category = "OLC|Gameplay")
    TArray<TObjectPtr<UOLCPlanetTerrainProfile>> TerrainProfiles;

    UPROPERTY(EditAnywhere, Category = "OLC|Gameplay")
    TObjectPtr<UStaticMesh> CrashedShipMesh;

    UPROPERTY(VisibleAnywhere, Category = "OLC|Gameplay")
    TObjectPtr<UStaticMeshComponent> CrashedShipComponent;

    UPROPERTY()
    TObjectPtr<AOLCPlanetTerrainActor> TerrainActor;

    UPROPERTY()
    TObjectPtr<UOLCFactionData> CurrentFaction;

    FOLCTerrainGenerationSettings TerrainSettings;
    EOLCBiomeType RequestedBiome = EOLCBiomeType::Desert;
    bool bIsInitialized = false;
};
