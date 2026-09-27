#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OLCCrashSitePrototypeActor.generated.h"

class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UENUM(BlueprintType)
enum class EOLCPlanetTileType : uint8
{
	Dust,
	Rock,
	Buildable,
	Blocked,
	Minerals,
	Fuel,
	Wreckage
};

USTRUCT(BlueprintType)
struct FOLCPlanetTile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Planet")
	FIntPoint Coord = FIntPoint::ZeroValue;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Planet")
	EOLCPlanetTileType Type = EOLCPlanetTileType::Dust;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Planet")
	bool bBuildable = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|Planet")
	bool bOccupied = false;
};

UCLASS()
class OURLASTCHANCE_API AOLCCrashSitePrototypeActor : public AActor
{
	GENERATED_BODY()

public:
	AOLCCrashSitePrototypeActor();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "OLC|Planet")
	const TArray<FOLCPlanetTile>& GetTiles() const { return Tiles; }

	UFUNCTION(BlueprintPure, Category = "OLC|Planet")
	FIntPoint GetMapSize() const { return FIntPoint(MapWidth, MapHeight); }

	UFUNCTION(BlueprintPure, Category = "OLC|Planet")
	bool WorldToTile(const FVector& WorldLocation, FIntPoint& OutTile) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Planet")
	bool CanPlaceFootprint(FIntPoint OriginTile, FIntPoint FootprintSize) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Planet")
	bool TryReserveFootprint(FIntPoint OriginTile, FIntPoint FootprintSize);

private:
	void GenerateCrashSite();
	void ClearGeneratedInstances();
	void AddTileInstance(const FOLCPlanetTile& Tile);
	void AddBorderRocks(int32 X, int32 Y, float EdgeDistance);
	void AddResourceNode(int32 X, int32 Y, EOLCPlanetTileType Type);
	void AddCrashWreck();
	void ConfigureMaterial(UInstancedStaticMeshComponent* Component, const FLinearColor& Color, float Roughness = 0.85f);
	FVector TileToWorld(int32 X, int32 Y, float Z = 0.0f) const;
	bool IsInsideBuildBasin(int32 X, int32 Y) const;
	float TileNoise(int32 X, int32 Y, int32 Salt) const;
	FOLCPlanetTile* FindTileMutable(FIntPoint Coord);
	const FOLCPlanetTile* FindTile(FIntPoint Coord) const;

	UPROPERTY(EditAnywhere, Category = "OLC|Planet")
	int32 MapWidth = 42;

	UPROPERTY(EditAnywhere, Category = "OLC|Planet")
	int32 MapHeight = 34;

	UPROPERTY(EditAnywhere, Category = "OLC|Planet")
	float TileSize = 180.0f;

	UPROPERTY(EditAnywhere, Category = "OLC|Planet")
	int32 Seed = 184736;

	UPROPERTY()
	TArray<FOLCPlanetTile> Tiles;

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> DustTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> BuildableTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> BlockedTiles;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> CliffRocks;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> SmallRocks;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> MineralNodes;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> FuelNodes;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> WreckParts;
};
