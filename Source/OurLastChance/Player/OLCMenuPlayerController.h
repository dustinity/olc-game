#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "OLCMenuPlayerController.generated.h"

class AOLCMenuGameMode;
class ACameraActor;
class UUserWidget;
class UOLCUIDataSubsystem;
class UOLCWelcomeScreenWidget;
class UOLCTechTreeWidget;
class UOLCDungeonEntryWidget;
class UOLCSquadSelectionWidget;
class UOLCTacticalCombatWidget;
class UOLCCombatResultsWidget;
class UScaleBox;
class AOLCPlanetTerrainActor;

/** Screen identifiers for the test switcher hotkeys. */
UENUM(BlueprintType)
enum class EOLCUIScreen : uint8
{
	None              UMETA(DisplayName = "None"),
	TestSwitcher      UMETA(DisplayName = "Test Switcher"),
	MainRTSHUD        UMETA(DisplayName = "Main RTS HUD"),
	Keymap            UMETA(DisplayName = "Keymap"),
	FactionSelect     UMETA(DisplayName = "Faction Select"),
	ConstructionMode  UMETA(DisplayName = "Construction Mode"),
	ColonyResourceNetwork UMETA(DisplayName = "Colony Resource Network"),
	SolarSystem       UMETA(DisplayName = "Solar System"),
	GalaxyMap         UMETA(DisplayName = "Galaxy Map"),
	TacticalDungeon   UMETA(DisplayName = "Tactical Dungeon (S08)"),
	DungeonEntry      UMETA(DisplayName = "Dungeon Entry (S07)"),
	SquadSelection    UMETA(DisplayName = "Squad Selection (S09)"),
	CombatResults     UMETA(DisplayName = "Combat Results (S10)"),
	Research          UMETA(DisplayName = "Research"),
	DropshipRepair    UMETA(DisplayName = "Dropship Repair (S13)"),
	MothershipBuilder UMETA(DisplayName = "Mothership Builder (S14)"),
	Equipment         UMETA(DisplayName = "Equipment"),
};

/**
 * Menu player controller — handles mouse cursor, UI input mode, and test screen switching.
 * Extends existing OLCMenuPlayerController functionality.
 */
UCLASS()
class OURLASTCHANCE_API AOLCMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AOLCMenuPlayerController();

	void OpenMainRTSHUD();
	void OpenKeymap();
	void OpenFactionSelect();
	void OpenConstructionMode();
	void OpenColonyResourceNetwork();
	void OpenSolarSystem();
	void OpenGalaxyMap();
	void OpenTacticalDungeon();
	void OpenResearch();

	UFUNCTION(BlueprintCallable, Category = "OLC|Campaign")
	void ConfirmFactionSelection(const FString& FactionId);
	void OpenDropshipRepair();
	void OpenMothershipBuilder();
	void OpenEquipment();

	/** Blueprint widget class for faction selection. */
	UPROPERTY(EditAnywhere, Category = "OLC|UI")
	TSubclassOf<UUserWidget> FactionSelectWidgetClass;

	/** Open a specific UI screen. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void OpenUIScreen(EOLCUIScreen Screen);

	/** Close the currently active overlay screen. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void CloseActiveScreen();

	/** Show the welcome screen and wire up New Campaign delegate. */
	void ShowWelcomeScreen();

	/** Called when New Campaign is clicked on welcome screen. */
	UFUNCTION()
	void OnNewCampaignClicked();

	/** Fallback: skip crash sequence, go straight to gameplay. */
	void TransitionToGameplayDirect();

	/** Return to test switcher. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void ShowTestSwitcher();

	void SetActiveCamera(ACameraActor* Camera) { ActiveCamera = Camera; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void EnsureMouseCursorVisible(UUserWidget* FocusWidget = nullptr);
	void AddFullscreenWidget(UUserWidget* Widget, int32 ZOrder);

	UPROPERTY()
	TObjectPtr<UOLCWelcomeScreenWidget> WelcomeScreenInstance;

	UPROPERTY()
	TObjectPtr<UUserWidget> TestSwitcherInstance;

	/** Reference to the game mode for crash sequence control. */
	UPROPERTY()
	TObjectPtr<AOLCMenuGameMode> MenuGameMode;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveScreenInstance;

	UPROPERTY()
	TObjectPtr<ACameraActor> ActiveCamera;

	float CurrentOrthoWidth = 7600.0f;

	EOLCUIScreen CurrentScreen = EOLCUIScreen::None;

	/** Zoom camera in/out with mouse wheel. */
	void OnZoomCamera(float Delta);
	bool IsGameplayActive() const;
	AOLCPlanetTerrainActor* FindTerrainActor() const;
	void OpenPrimaryGameplayHUD();
	void OpenGameplayShipBuilder();
	void SetGameplayBiome(int32 BiomeIndex);
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
	void PanCameraUp();
	void PanCameraDown();
	void PanCameraLeft();
	void PanCameraRight();
	void PanCameraBy(const FVector& Delta);

	// ---------------------------------------------------------------------------
	// S11→S12 transition animation (zoom-out effect)
	// ---------------------------------------------------------------------------
	UPROPERTY()
	TObjectPtr<UScaleBox> TransitionOverlay;

	float TransitionProgress = 0.0f;
	float TransitionDuration = 1.5f; // seconds
	bool bIsTransitioning = false;

	FTimeline TransitionTimeline; // Timeline for S11→S12 zoom-out animation

	/** Start a zoom-out transition from S11 to S12. */
	UFUNCTION()
	void StartScreenTransition();

	/** Tick handler for the transition timeline. */
	UFUNCTION()
	void OnTransitionTick(float Value);
};
