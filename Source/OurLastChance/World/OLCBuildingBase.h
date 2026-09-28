#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/OLCBuildingData.h"
#include "OLCBuildingBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTextRenderComponent;
class UNiagaraComponent;

/**
 * Base actor for all planet buildings.
 * Provides mesh rendering, grid snap, building data reference,
 * and lifecycle hooks that category subclasses extend.
 */
UCLASS(Blueprintable)
class OURLASTCHANCE_API AOLCBuildingBase : public AActor
{
	GENERATED_BODY()

public:
	AOLCBuildingBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Get the building data asset. */
	UFUNCTION(BlueprintPure, Category = "OLC|Building")
	const FOLCBuildingConfig& GetBuildingData() const { return BuildingData; }

	/** Set the building data asset. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building")
	void SetBuildingData(const FOLCBuildingConfig& InData);

	/** Snap actor location to grid based on BuildingData->GridSize. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building")
	void SnapToGrid();

	/** Rotate building 90 degrees around Y axis. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building")
	void RotateBuild();

	/** Check if this building is currently powered (has power connection). */
	UFUNCTION(BlueprintPure, Category = "OLC|Building")
	bool IsPowered() const { return bIsPowered; }

	/** Set powered state. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building")
	void SetPowered(bool bNewPowered);

	UFUNCTION(BlueprintCallable, Category = "OLC|Building|Production")
	void ShowProduction(const FText& ProductionText, bool bDeficit);

	// --- VFX integration hooks (WP-126 step-7; append-only) ------------------

	/** Play the one-shot building destruction debris burst at this building's location. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building|VFX")
	void PlayDestructionVFX();

	/** Attach a looping construction progress glow to SceneRoot. No-op if already active. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building|VFX")
	void StartConstructionGlow();

	/** Detach and destroy the construction glow component if active. No-op if not glowing. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Building|VFX")
	void StopConstructionGlow();

protected:
	/** Scene root component. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	/** Building mesh visual. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Collision footprint for placement overlap checks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Components")
	TObjectPtr<UBoxComponent> Footprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building|Components")
	TObjectPtr<UTextRenderComponent> ProductionTextComponent;

	/** Configurable building data — set in constructor or editor. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Building|Data")
	FOLCBuildingConfig BuildingData;

	/** Whether the building has a valid power connection. */
	UPROPERTY(BlueprintReadWrite, Category = "Building|Power")
	bool bIsPowered = false;

	/** Current rotation in degrees (0, 90, 180, 270). */
	UPROPERTY(BlueprintReadWrite, Category = "Building|Rotation")
	int32 BuildRotationDegrees = 0;

	/** Grid cell size in world units — derived from map tile size. */
	UPROPERTY(BlueprintReadWrite, Category = "Building|Grid")
	float GridCellSize = 180.0f;

	// -----------------------------------------------------------------------
	// Biome — current planet biome (set by game state or subsystem)
	// Default is Desert if not explicitly set.
	// -----------------------------------------------------------------------
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Biome")
	EOLCBiomeType CurrentBiome = EOLCBiomeType::Desert;

	/** Get the biome multiplier for this building based on current planet biome. */
	UFUNCTION(BlueprintPure, Category = "OLC|Building|Biome")
	float GetBiomeMultiplier() const
	{
		return BuildingData.GetBiomeMultiplier(CurrentBiome);
	}

	// --- VFX integration state (WP-126 step-7; append-only) ------------------

	/** Active construction glow component attached to SceneRoot (null when not glowing). */
	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> ConstructionGlow;
};
