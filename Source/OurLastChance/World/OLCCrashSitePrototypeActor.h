#pragma once

// WP-129 Step 2: tutorial interact sphere + proximity-scan hooks added to this actor.

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OLCCrashSitePrototypeActor.generated.h"

class UInstancedStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class USphereComponent;
class UPrimitiveComponent;
class UInputComponent;
class UOLCTutorialSubsystem;

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

	/** Throttled proximity scan for tutorial objectives (WP-129 Step 2). */
	virtual void Tick(float DeltaTime) override;

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

	// -----------------------------------------------------------------------
	// WP-129 Step 2 — public tutorial entry points (player-controller routing)
	// -----------------------------------------------------------------------

	/**
	 * Public interact entry point: complete InspectCrashSite once and grant
	 * the dropship-storage reward. Guarded internally (IsObjectiveComplete),
	 * so it is safe to call from any trigger — overlap sphere or the
	 * player-controller Interact key (E).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Tutorial")
	void InteractWithCrashSite();

	/**
	 * True when WorldLocation is within the wreck interact-sphere radius.
	 * Horizontal (XY) distance only — the RTS camera sits far above the
	 * terrain, so Z is intentionally ignored.
	 */
	bool IsWithinInteractProximity(const FVector& WorldLocation) const;

	/**
	 * World-space location of the mineral-node cluster nearest to From
	 * (WP-129 Step 3 deposit-highlight target). Returns false when no
	 * mineral nodes were recorded.
	 */
	bool GetNearestMineralNodeLocation(const FVector& From, FVector& OutLocation) const;

	/**
	 * Tutorial-only mode: skip all visual instance generation (tile terrain,
	 * rocks, node clusters, wreck) while keeping the invisible interact
	 * sphere and the recorded mineral-node positions. Used when this actor is
	 * spawned into the real gameplay flow (OLCMenuGameMode::TransitionToGameplay)
	 * so it does not render a second planet on top of the existing terrain.
	 */
	void SetTutorialOnly(bool bInTutorialOnly);

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

	// -----------------------------------------------------------------------
	// WP-129 Step 2 — tutorial objective hooks (guarded, idempotent)
	// -----------------------------------------------------------------------

	/** Overlap handler for the wreck interact sphere (unit/pawn entering proximity). UE 5.8 six-param overlap signature. */
	UFUNCTION()
	void OnInteractSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);

	/** Interact action handler (UInputComponent "Interact" binding). */
	void OnInteractCrashSite();

	/** Throttled scan: complete DiscoverDeposit when a unit or the player view is near a mineral node. */
	void ScanTutorialProximity();

	/** World-space centers of generated mineral-node clusters (for the proximity scan). */
	TArray<FVector> MineralNodePositions;

	/** Accumulator for the throttled tutorial proximity scan. */
	float TutorialScanAccumulator = 0.0f;

	/**
	 * WP-129 Step 3: presence already inside the interact sphere when BeginPlay
	 * ran (e.g. the player pawn standing at PlayerStart since PIE start). Their
	 * initial overlap must NOT auto-complete InspectCrashSite — the intended
	 * tutorial start is an explicit E-key press near the wreck. Without this
	 * gate, TransitionToGameplay's spawn registered the sphere around a pawn
	 * that was already inside and completed objective 1 instantly, before the
	 * Step-3 highlight could ever be shown for it.
	 */
	TSet<TWeakObjectPtr<AActor>> PreExistingOverlapActors;

	/**
	 * WP-129 Step 3: world time until which overlap events are ignored. UE defers
	 * a component's INITIAL overlap sweep to the first tick after registration,
	 * so capturing GetOverlappingActors() in BeginPlay can come back empty and
	 * the pre-existing presence's event arrives just AFTER BeginPlay — a pure
	 * "was it already inside" check races that sweep. A short settling window
	 * covers it; presence entering after the window still triggers normally.
	 */
	float InteractGraceEndTime = -1.0f;

	UPROPERTY(EditAnywhere, Category = "OLC|Tutorial")
	float InteractRadius = 900.0f;

	UPROPERTY(EditAnywhere, Category = "OLC|Tutorial")
	float DepositProximityRadius = 1200.0f;

	/** When true, GenerateCrashSite() records positions but adds no visual instances (WP-129 Step 2). */
	UPROPERTY(EditAnywhere, Category = "OLC|Tutorial")
	bool bTutorialOnly = false;

	UPROPERTY()
	TObjectPtr<USphereComponent> InteractSphere;

	/** Input component hosting the tutorial interact action binding (WP-129 Step 2). */
	UPROPERTY()
	TObjectPtr<UInputComponent> TutorialInput;

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
