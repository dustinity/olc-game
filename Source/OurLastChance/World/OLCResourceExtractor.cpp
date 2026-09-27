#include "OLCResourceExtractor.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "World/OLCPlanetTerrainActor.h"

AOLCResourceExtractor::AOLCResourceExtractor()
{
        PrimaryActorTick.bCanEverTick = true;

        // Create resource marker component
        ResourceMarkerComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ResourceMarker"));
        ResourceMarkerComponent->SetupAttachment(RootComponent);
        ResourceMarkerComponent->SetVisibility(true);
}

void AOLCResourceExtractor::BeginPlay()
{
        Super::BeginPlay();

        // Find nearest resource tile for this extractor
        FindNearestResourceTile();

        // Spawn visual marker on the connected resource tile
        if (ConnectedResourceTileIndex >= 0)
        {
                SpawnResourceMarker();
        }

        // Start production timer if building has output defined
        if (BuildingData.OutputPerTick.Num() > 0 && IsPowered())
        {
                UE_LOG(LogTemp, Log, TEXT("[OLC] Resource extractor '%s' started production (biome=%s, multiplier=%.2f)"),
                        *BuildingData.DisplayName.ToString(), *UEnum::GetValueAsString(CurrentBiome), ProductionMultiplier);
        }
}

void AOLCResourceExtractor::Tick(float DeltaTime)
{
        Super::Tick(DeltaTime);

        if (!IsPowered()) return;

        // Update marker pulse animation
        UpdateMarkerPulse(DeltaTime);

        // Accumulate time and produce when interval reached
        ProductionAccumulator += DeltaTime;

        if (ProductionAccumulator >= ProductionInterval)
        {
                ProductionAccumulator = 0.0f;

                // Calculate production amount with richness scaling
                float Multiplier = CalculateProductionMultiplier();

                // Add each output resource to the subsystem counters via AddResource()
                if (UGameInstance* GI = GetWorld()->GetGameInstance())
                {
                        if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
                        {
                                for (const auto& Output : BuildingData.OutputPerTick)
                                {
                                        float BaseAmount = Output.Delta > 0 ? Output.Delta : Output.CurrentValue;
                                        float ScaledAmount = BaseAmount * Multiplier;

                                        // Apply biome modifiers from building data
                                        float BiomeMod = BuildingData.GetBiomeMultiplier(CurrentBiome);
                                        ScaledAmount *= BiomeMod;

                                        Data->AddResource(Output.ResourceType, ScaledAmount);

                                        UE_LOG(LogTemp, Verbose, TEXT("[OLC] Extractor '%s' produced %.1f %s (base=%.1f, richness=%.2f, biome=%.2f)"),
                                                *BuildingData.DisplayName.ToString(),
                                                ScaledAmount,
                                                *UEnum::GetValueAsString(Output.ResourceType),
                                                BaseAmount,
                                                ProductionMultiplier,
                                                BiomeMod);
                                }
                        }
                }
        }
}

void AOLCResourceExtractor::FindNearestResourceTile()
{
        ConnectedResourceTileIndex = -1;
        ProductionMultiplier = 1.0f;

        // Get terrain actor
        if (!GetWorld()) return;

        AActor* TerrainActor = UGameplayStatics::GetActorOfClass(GetWorld(), AOLCPlanetTerrainActor::StaticClass());
        if (!TerrainActor)
        {
                UE_LOG(LogTemp, Warning, TEXT("[OLC] Resource extractor: no terrain actor found"));
                return;
        }

        AOLCPlanetTerrainActor* Terrain = Cast<AOLCPlanetTerrainActor>(TerrainActor);
        if (!Terrain) return;

        const TArray<FOLCTerrainTile>& Tiles = Terrain->GetTiles();
        if (Tiles.IsEmpty()) return;

        // Get building world location
        FVector BuildingLocation = GetActorLocation();

        float MinDistance = TNumericLimits<float>::Max();
        int32 NearestIndex = -1;

        // Search for nearest Resource tile within radius
        for (int32 i = 0; i < Tiles.Num(); ++i)
        {
                if (Tiles[i].Role != EOLCTerrainTileRole::Resource) continue;

                // Convert tile coord to world position
                FVector TileLocation = Terrain->GetTileWorldPosition(Tiles[i].Coord);
                float Distance = FVector::Dist(BuildingLocation, TileLocation);

                // Check if within search radius (convert tile distance to world distance)
                float TileSize = Terrain->GetTiles()[0].Coord.X == 0 ? 180.0f : 180.0f; // Default tile size
                float MaxDistance = ResourceSearchRadius * TileSize;

                if (Distance < MinDistance && Distance <= MaxDistance)
                {
                        MinDistance = Distance;
                        NearestIndex = i;
                }
        }

        if (NearestIndex >= 0)
        {
                ConnectedResourceTileIndex = NearestIndex;
                ProductionMultiplier = FMath::Clamp(Tiles[NearestIndex].ResourceRichness, 0.35f, 1.0f);

                UE_LOG(LogTemp, Log, TEXT("[OLC] Resource extractor '%s' connected to resource tile at index %d (richness=%.2f, multiplier=%.2f)"),
                        *BuildingData.DisplayName.ToString(), NearestIndex, Tiles[NearestIndex].ResourceRichness, ProductionMultiplier);
        }
        else
        {
                // No resource tile found — use scavenging mode
                ProductionMultiplier = ScavengingMultiplier;
                UE_LOG(LogTemp, Warning, TEXT("[OLC] Resource extractor '%s' no resource tile found within %f tiles — using scavenging mode (multiplier=%.2f)"),
                        *BuildingData.DisplayName.ToString(), ResourceSearchRadius, ScavengingMultiplier);
        }
}

float AOLCResourceExtractor::CalculateProductionMultiplier() const
{
        // Base multiplier from resource richness
        float Multiplier = ProductionMultiplier;

        // If no resource tile found, use scavenging multiplier
        if (ConnectedResourceTileIndex < 0)
        {
                Multiplier = ScavengingMultiplier;
        }

        return Multiplier;
}

void AOLCResourceExtractor::SpawnResourceMarker()
{
        if (ConnectedResourceTileIndex < 0) return;

        // Get terrain actor
        AActor* TerrainActor = UGameplayStatics::GetActorOfClass(GetWorld(), AOLCPlanetTerrainActor::StaticClass());
        if (!TerrainActor) return;

        AOLCPlanetTerrainActor* Terrain = Cast<AOLCPlanetTerrainActor>(TerrainActor);
        if (!Terrain) return;

        const TArray<FOLCTerrainTile>& Tiles = Terrain->GetTiles();
        if (ConnectedResourceTileIndex >= Tiles.Num()) return;

        // Get resource tile location
        FVector TileLocation = Terrain->GetTileWorldPosition(Tiles[ConnectedResourceTileIndex].Coord);

        // Set marker component location to resource tile
        ResourceMarkerComponent->SetWorldLocation(TileLocation);

        // Create material instance dynamic for pulse animation
        if (ResourceMarkerComponent->GetMaterial(0))
        {
                MarkerMaterialInstance = UMaterialInstanceDynamic::Create(ResourceMarkerComponent->GetMaterial(0), this);
                ResourceMarkerComponent->SetMaterial(0, MarkerMaterialInstance);
        }

        UE_LOG(LogTemp, Log, TEXT("[OLC] Resource marker spawned at tile %d (%s)"),
                ConnectedResourceTileIndex, *UEnum::GetValueAsString(Tiles[ConnectedResourceTileIndex].ResourceType));
}

void AOLCResourceExtractor::UpdateMarkerPulse(float DeltaTime)
{
        if (!MarkerMaterialInstance) return;

        // Update pulse phase
        PulsePhase += DeltaTime * 2.0f; // 2 Hz pulse frequency

        // Calculate pulse value (sine wave from 0 to 1)
        float PulseValue = (FMath::Sin(PulsePhase) + 1.0f) * 0.5f;

        // Apply pulse to material scalar parameter
        if (PulseValue > 0.5f)
        {
                MarkerMaterialInstance->SetScalarParameterValue(TEXT("Pulse"), PulseValue);
        }
}
