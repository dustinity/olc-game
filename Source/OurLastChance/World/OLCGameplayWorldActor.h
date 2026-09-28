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
    void InitializeGameplayWorld(EOLCBiomeType Biome, int32 Seed, UOLCFactionData* Faction, const FString& ChampionId = FString());

    /** World location of the crashed dropship (WP-129 Step 2 tutorial spawn anchor). */
    UFUNCTION(BlueprintPure, Category = "OLC|Gameplay")
    FVector GetCrashedShipLocation() const;

protected:
    virtual void BeginPlay() override;

private:
    void SpawnTerrainActor();
    void PlaceCrashedShip();
    void SpawnSelectedChampionMarker();

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

    UPROPERTY()
    TObjectPtr<AActor> ChampionMarkerActor;

    FOLCTerrainGenerationSettings TerrainSettings;
    FString SelectedChampionId;
    EOLCBiomeType RequestedBiome = EOLCBiomeType::Desert;
    bool bIsInitialized = false;
};
