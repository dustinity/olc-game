#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/OLCTerrainTypes.h"
#include "OLCPlanetTerrainActor.generated.h"

class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UProceduralMeshComponent;
class UOLCPlanetTerrainProfile;

UCLASS()
class OURLASTCHANCE_API AOLCPlanetTerrainActor : public AActor
{
	GENERATED_BODY()

public:
	AOLCPlanetTerrainActor();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	void SetBiome(EOLCBiomeType NewBiome);

	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	void SetBiomeByIndex(int32 BiomeIndex);

	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	void ToggleRenderMode();

	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	void Regenerate(int32 SeedOffset = 1);

	/** Get the current biome being displayed (may be transitioning). */
	UFUNCTION(BlueprintPure, Category = "OLC|Terrain")
	EOLCBiomeType GetCurrentBiome() const { return CurrentBiome; }

	/** Get the target biome being transitioned to. */
	UFUNCTION(BlueprintPure, Category = "OLC|Terrain")
	EOLCBiomeType GetTargetBiome() const { return TargetBiome; }

	/** Get crossfade progress 0.0-1.0. */
	UFUNCTION(BlueprintPure, Category = "OLC|Terrain")
	float GetBiomeCrossfadeProgress() const { return bCrossfading ? CrossfadeProgress : 1.0f; }

	UFUNCTION(BlueprintPure, Category = "OLC|Terrain")
	const TArray<FOLCTerrainTile>& GetTiles() const { return Tiles; }

	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain")
	void MarkTilesOccupied(const TArray<FIntPoint>& TileCoords);

	// -----------------------------------------------------------------------
	// Building placement (WP-103 Step 3)
	// -----------------------------------------------------------------------

	/** Check if a building can be placed at the given grid origin with the given footprint size. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain|Placement")
	bool CanPlaceBuilding(FIntPoint Origin, FVector2D Footprint, int32 RotationDegrees = 0) const;

	/** Place a building: marks tiles occupied and returns world position of center tile. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Terrain|Placement")
	bool PlaceBuilding(FIntPoint Origin, FVector2D Footprint, FVector& OutWorldPosition, int32 RotationDegrees = 0);

	/** Get world position for a grid coordinate (center of tile at height). */
	UFUNCTION(BlueprintPure, Category = "OLC|Terrain|Placement")
	FVector GetTileWorldPosition(FIntPoint Coord) const;

	/** Get map dimensions in tiles. */
	UFUNCTION(BlueprintPure, Category = "OLC|Terrain|Placement")
	FIntPoint GetMapDimensions() const { return FIntPoint(GenerationSettings.MapWidth, GenerationSettings.MapHeight); }

	// GetCurrentBiome already declared above (returns CurrentBiome for crossfade support)

	// Allow demo wrapper to configure settings before triggering generation
	void SetGenerationSettings(const FOLCTerrainGenerationSettings& InSettings)
	{
		GenerationSettings = InSettings;
	}

	void SetBiomeProfiles(TArray<TObjectPtr<UOLCPlanetTerrainProfile>> InProfiles)
	{
		BiomeProfiles = MoveTemp(InProfiles);
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "OLC|Terrain")
	FOLCTerrainGenerationSettings GenerationSettings;

	UPROPERTY(EditAnywhere, Category = "OLC|Terrain")
	EOLCTerrainRenderMode RenderMode = EOLCTerrainRenderMode::SurfaceMesh;

	UPROPERTY(EditAnywhere, Category = "OLC|Terrain")
	TArray<TObjectPtr<UOLCPlanetTerrainProfile>> BiomeProfiles;

	/** Crossfade duration between biomes in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|BiomeSwitching", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float BiomeCrossfadeDuration = 0.3f;

private:
	void GenerateAndRender();
	void RenderSurfaceMesh();
	void RenderDebugTiles();
	void RenderProps();
	void ClearInstances();
	void ApplyBiomeMaterials();
	void ConfigureMaterial(UInstancedStaticMeshComponent* Component, const FLinearColor& Color, float Roughness = 0.85f);
	void ApplyAtmosphere(const UOLCPlanetTerrainProfile* Profile);
	FVector TileToWorld(int32 X, int32 Y, float Height01 = 0.0f) const;
	FLinearColor GetDebugColor(EOLCTerrainTileRole Role) const;
	FLinearColor GetBiomeSurfaceColor(EOLCBiomeType Biome) const;
	FLinearColor GetBiomeBlockerColor(EOLCBiomeType Biome) const;
	const UOLCPlanetTerrainProfile* FindActiveProfile() const;
	void UpdateBiomeCrossfade(float DeltaTime);
	void UpdateSurfaceMeshCrossfade(const FLinearColor& Color);
	void UpdatePropColorsCrossfade(const FLinearColor& BlockerColor);

	// --- Biome ambient VFX hook (WP-126 step-9; append-only) ------------------
	/** Stop the currently active biome ambient and start the new one via the VFX subsystem. */
	void StartBiomeAmbientVFX(EOLCBiomeType NewBiome);

	UPROPERTY()
	TWeakObjectPtr<AActor> BiomeAmbientActor;

	UPROPERTY()
	TArray<FOLCTerrainTile> Tiles;

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> SurfaceMesh;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugBuildableTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugRestrictedTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugBlockedTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugWaterTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugResourceTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugDungeonTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugLandingTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DebugOutsideTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> BlockerProps;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> ResourceProps;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DungeonProps;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> PlantProps;

	// WP-108 Step 4: Billboard sprite components for PNG-based props
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> BillboardPlantProps;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> BillboardResourceProps;

	// Material for billboard sprites with chroma-key support
	UPROPERTY(EditAnywhere, Category = "OLC|Terrain|Billboard")
	TObjectPtr<UMaterialInterface> BillboardMaterial;

	// Biome switching crossfade state (WP-108)
	EOLCBiomeType CurrentBiome = EOLCBiomeType::Desert;
	EOLCBiomeType TargetBiome = EOLCBiomeType::Desert;
	bool bCrossfading = false;
	float CrossfadeProgress = 0.0f;

private:
	void CreateBillboardSprites();
};
