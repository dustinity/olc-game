#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/OLCResourceTypes.h"
#include "OLCGameplayPlayerController.generated.h"

class AOLCUnitBase;
class AOLCPlanetTerrainActor;
class UUserWidget;
class UOLCUIDataSubsystem;

/**
 * Gameplay player controller for RTS controls plus UI overlays.
 * Handles construction mode placement state.
 */
UCLASS()
class OURLASTCHANCE_API AOLCGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOLCGameplayPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/** Rotate the current build placement. */
	void OnRotateBuild();

	/** Cancel construction mode. */
	void OnCancelBuild();

	/** Cycle simulation speed (Space). */
	void OnToggleSimulationSpeed();

	/** WP-107 Step 5: Toggle main HUD overlay (F1 key). */
	void OnToggleMainHUD();

	/** WP-107 Step 5: Pan camera to minimap click location. */
	void OnMinimapClick(FVector2D MinimapCoord);

	/** Left-click: raycast to select a unit. */
	UFUNCTION()
	void OnLeftClick();

	/** Right-click capture: get mouse position and move selected unit. */
	UFUNCTION()
	void OnRightClickCapture();

	void OnBiome1();
	void OnBiome2();
	void OnBiome3();
	void OnBiome4();
	void OnBiome5();
	void OnBiome6();
	void OnBiome7();
	void OnBiome8();
	void OnToggleTerrainRenderMode();
	void OnRegenerateTerrain();
	void OnToggleConstructionMode();
	void OnToggleShipBuilder();
	void MoveCameraX(float Value);
	void MoveCameraY(float Value);
	AOLCPlanetTerrainActor* FindTerrainActor() const;

private:
	// Construction state
	bool bIsInConstructionMode = false;
	int32 BuildRotationDegrees = 0;

	/** WP-107 Step 5: Main HUD overlay visibility state. */
	UPROPERTY()
	bool bIsMainHUDVisible = false;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveHUDWidget;

	/** WP-107 Step 5: Reference to the main HUD widget for minimap click handling. */
	UPROPERTY()
	TObjectPtr<UUserWidget> MainHUDWidget;

	/** Currently selected unit (set by left-click raycast). */
	UPROPERTY()
	TObjectPtr<AOLCUnitBase> SelectedUnit;

	float CameraMoveX = 0.0f;
	float CameraMoveY = 0.0f;

	/** WP-108: Track current biome index for HUD badge display (0-7). */
	UPROPERTY()
	int32 CurrentBiomeIndex = 0;

	UPROPERTY(EditAnywhere, Category = "OLC|Camera")
	float CameraPanSpeed = 2400.0f;
};
