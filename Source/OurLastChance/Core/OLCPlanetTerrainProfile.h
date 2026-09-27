#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCTerrainTypes.h"
#include "OLCPlanetTerrainProfile.generated.h"

class UMaterialInterface;
class UStaticMesh;

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCPlanetTerrainProfile : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Generation")
	float WaterLevel = 0.24f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Generation")
	float BlockerDensity = 0.12f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Generation")
	float ResourceDensity = 0.018f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Generation")
	float DecorationDensity = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Gameplay")
	float MovementModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Gameplay")
	float DefenseModifier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	FLinearColor SurfaceColor = FLinearColor(0.25f, 0.22f, 0.16f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	FLinearColor BlockerColor = FLinearColor(0.10f, 0.10f, 0.10f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	TObjectPtr<UMaterialInterface> TerrainMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	TObjectPtr<UStaticMesh> BlockerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	TObjectPtr<UStaticMesh> ResourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Visual")
	TObjectPtr<UStaticMesh> DungeonMesh;

	// WP-108: Biome-specific atmosphere settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Atmosphere")
	float FogDensity = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Atmosphere")
	FLinearColor FogColor = FLinearColor(0.85f, 0.75f, 0.60f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Atmosphere")
	FLinearColor DirectionalLightColor = FLinearColor(1.0f, 0.95f, 0.85f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Atmosphere")
	float PostProcessExposureCompensation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Terrain|Atmosphere")
	float PostProcessContrast = 1.0f;
};
