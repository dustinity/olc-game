#pragma once

#include "CoreMinimal.h"
#include "World/OLCBuildingBase.h"
#include "OLCResourceExtractor.generated.h"

class AOLCPlanetTerrainActor;

/**
 * Resource extractor buildings (mines, oil pumps, harvesters).
 * Produces resources on a timed tick via UOLCUIDataSubsystem.
 * Mines wire to terrain resource tiles for richness-based output scaling.
 */
UCLASS()
class OURLASTCHANCE_API AOLCResourceExtractor : public AOLCBuildingBase
{
        GENERATED_BODY()

public:
        AOLCResourceExtractor();

        virtual void Tick(float DeltaTime) override;

        UFUNCTION(BlueprintPure, Category = "OLC|Extraction")
        bool HasConnectedResourceTile() const { return ConnectedResourceTileIndex >= 0; }

        UFUNCTION(BlueprintPure, Category = "OLC|Extraction")
        float GetExtractionMultiplier() const { return CalculateProductionMultiplier(); }

        UFUNCTION(BlueprintPure, Category = "OLC|Extraction")
        EOLCResourceType GetExtractedResourceType() const { return ExtractedResourceType; }

protected:
        virtual void BeginPlay() override;

        /** Find nearest resource tile within range and store reference. */
        void FindNearestResourceTile();

        /** Calculate production multiplier based on resource richness and biome. */
        float CalculateProductionMultiplier() const;

        /** Spawn visual marker on the extracted resource tile. */
        void SpawnResourceMarker();

        /** Update marker pulse animation. */
        void UpdateMarkerPulse(float DeltaTime);

        // -----------------------------------------------------------------------
        // Resource extraction settings
        // -----------------------------------------------------------------------

        /** Maximum distance in tiles to search for a resource tile. */
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Extraction", meta = (AllowPrivateAccess = "true"))
        float ResourceSearchRadius = 5.0f;

        /** Base production rate when no resource tile is found (scavenging mode). */
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Extraction", meta = (AllowPrivateAccess = "true"))
        float ScavengingMultiplier = 0.25f;

        // -----------------------------------------------------------------------
        // Resource tile reference
        // -----------------------------------------------------------------------

        /** Index of the resource tile this extractor is connected to. */
        UPROPERTY()
        int32 ConnectedResourceTileIndex = -1;

        /** Multiplier applied to production based on richness and biome. */
        UPROPERTY()
        float ProductionMultiplier = 1.0f;

        UPROPERTY()
        EOLCResourceType ExtractedResourceType = EOLCResourceType::Minerals;

        // -----------------------------------------------------------------------
        // Visual marker for resource tile
        // -----------------------------------------------------------------------

        /** Instanced static mesh component for the resource marker. */
        UPROPERTY(VisibleAnywhere, Category = "OLC|Extraction")
        TObjectPtr<UInstancedStaticMeshComponent> ResourceMarkerComponent;

        /** Material instance dynamic for pulse animation. */
        UPROPERTY()
        TObjectPtr<UMaterialInstanceDynamic> MarkerMaterialInstance;

        /** Current pulse phase for animation. */
        UPROPERTY()
        float PulsePhase = 0.0f;

        /** Timer handle for marker pulse update. */
        FTimerHandle MarkerPulseTimer;
};
