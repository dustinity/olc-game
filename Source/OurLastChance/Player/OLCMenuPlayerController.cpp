#include "OLCMenuPlayerController.h"
#include "OurLastChance.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"
#include "Core/OLCUIDataSubsystem.h"
#include "GameModes/OLCMenuGameMode.h"
#include "UI/OLCHUDWidgets.h"
#include "UI/OLCSharedWidgets.h"
#include "UI/OLCWelcomeScreenWidget.h"
#include "UI/OLCTechTreeWidget.h"
#include "UI/OLCSolarSystemWidget.h"
#include "UI/OLCGalaxyMapWidget.h"
#include "UI/OLCDungeonEntryWidget.h"
#include "UI/OLCSquadSelectionWidget.h"
#include "UI/OLCTacticalCombatWidget.h"
#include "UI/OLCCombatResultsWidget.h"
#include "UI/OLCDropshipRepairWidget.h"
#include "UI/OLCShipBuilderWidget.h"
#include "UI/OLCShipModuleManagementWidget.h"
#include "UI/OLCChampionSelectWidget.h"
#include "UI/WBP_FactionSelect.h"
#include "Core/OLCTechData.h"
#include "Core/OLCResearchSubsystem.h"
#include "Core/OLCDungeonGenerationData.h"
#include "Core/OLCRaceSubsystem.h"
#include "Core/OLCTutorialSubsystem.h"
#include "Core/OLCTutorialTestConfig.h"
#include "UObject/ConstructorHelpers.h"
#include "World/OLCCrashSitePrototypeActor.h"
#include "World/OLCPlanetTerrainActor.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "Widgets/Layout/SScaleBox.h"

#define LOCTEXT_NAMESPACE "OLCMenuPlayerController"

AOLCMenuPlayerController::AOLCMenuPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> FactionSelectWidgetFinder(
		TEXT("/Game/UI/FactionSelection/Widgets/WBP_FactionSelect.WBP_FactionSelect_C"));
	if (FactionSelectWidgetFinder.Succeeded())
	{
		FactionSelectWidgetClass = FactionSelectWidgetFinder.Class;
	}
}

void AOLCMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;

	// Cache reference to the game mode for crash sequence control.
	MenuGameMode = Cast<AOLCMenuGameMode>(GetWorld()->GetAuthGameMode());

	// Boot directly into the welcome screen for the opening experience.
	ShowWelcomeScreen();

	// WP-129 Step 3 (test-only, off by default): deterministic TransitionToGameplay
	// for PIE verification — the in-game console and UMG buttons are not reachable
	// from the MCP toolsets, so this is the "console" equivalent (see
	// Core/OLCTutorialTestConfig.h). Dismisses the welcome screen exactly like the
	// real New Campaign flow, then calls TransitionToGameplay directly.
	if (const UOLCTutorialTestConfig* Test = UOLCTutorialTestConfig::Load())
	{
		if (Test->AutoTransitionSeconds > 0.0f)
		{
			GetWorldTimerManager().SetTimer(TestAutoTransitionTimer, this, &AOLCMenuPlayerController::TriggerTestTransition, Test->AutoTransitionSeconds, false);
			UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial test hook: auto-transition to gameplay in %.1f s"), Test->AutoTransitionSeconds);
		}
	}

	// PIE worlds are duplicated rather than created, so UOLCTutorialSubsystem's
	// FWorldDelegates::OnPostWorldCreation-based scheduling never fires for them.
	// BeginPlay is a reliable PIE-safe fallback trigger for the same test hooks.
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCTutorialSubsystem* Tutorial = GI->GetSubsystem<UOLCTutorialSubsystem>())
		{
			Tutorial->ScheduleTestHooksIfNeeded(GetWorld());
		}
	}
}

void AOLCMenuPlayerController::TriggerTestTransition()
{
	if (WelcomeScreenInstance)
	{
		// RemoveFromParent: the 5.8 replacement for the deprecated RemoveFromViewport.
		WelcomeScreenInstance->RemoveFromParent();
		WelcomeScreenInstance = nullptr;
	}

	if (MenuGameMode)
	{
		MenuGameMode->TransitionToGameplay();
	}
	else
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Tutorial test hook: no MenuGameMode — transition skipped"));
	}
}

/** Show the welcome screen and wire up New Campaign delegate to start crash sequence. */
void AOLCMenuPlayerController::ShowWelcomeScreen()
{
	if (!WelcomeScreenInstance)
	{
		WelcomeScreenInstance = CreateWidget<UOLCWelcomeScreenWidget>(this, UOLCWelcomeScreenWidget::StaticClass());
	}

	if (WelcomeScreenInstance)
	{
		// Wire New Campaign button to start crash sequence.
		if (WelcomeScreenInstance->OnNewCampaign.IsBound())
		{
			WelcomeScreenInstance->OnNewCampaign.Clear();
		}
		WelcomeScreenInstance->OnNewCampaign.AddDynamic(this, &AOLCMenuPlayerController::OnNewCampaignClicked);

		AddFullscreenWidget(WelcomeScreenInstance.Get(), 200);
		CurrentScreen = EOLCUIScreen::None; // Not an overlay screen — it's the boot screen.

		// Every other screen-opening path calls this to switch to
		// FInputModeGameAndUI so clicks actually reach Slate/UMG widgets --
		// this is the very first screen shown (from BeginPlay), and without
		// it the controller stays in the engine's default GameOnly input
		// mode. bShowMouseCursor=true only makes the cursor visible; it
		// doesn't route clicks to the UI layer, so every button here was
		// unclickable regardless of the widget's own hit-test setup.
		EnsureMouseCursorVisible(WelcomeScreenInstance.Get());
	}
}

/** Called when New Campaign is clicked on welcome screen — starts crash sequence. */
void AOLCMenuPlayerController::OnNewCampaignClicked()
{
	if (WelcomeScreenInstance)
	{
		WelcomeScreenInstance->RemoveFromParent();
		WelcomeScreenInstance = nullptr;
	}

	if (FactionSelectWidgetClass)
	{
		OpenUIScreen(EOLCUIScreen::FactionSelect);
		if (CurrentScreen == EOLCUIScreen::FactionSelect)
		{
			return;
		}
		UE_LOG(LogOLC, Warning, TEXT("[OLC] FactionSelect widget failed to open — falling back to crash sequence"));
	}

	// Start the crash animation sequence via the game mode.
	if (MenuGameMode)
	{
		MenuGameMode->StartCrashSequence();
	}
	else
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] No MenuGameMode found — starting gameplay directly"));
		TransitionToGameplayDirect();
	}
}

void AOLCMenuPlayerController::HandleFactionSelectBack()
{
	if (ActiveScreenInstance)
	{
		ActiveScreenInstance->RemoveFromParent();
		ActiveScreenInstance = nullptr;
	}

	CurrentScreen = EOLCUIScreen::None;
	ShowWelcomeScreen();
}

void AOLCMenuPlayerController::ConfirmFactionSelection(const FString& FactionId)
{
	if (MenuGameMode)
	{
		MenuGameMode->SetSelectedFactionId(FactionId);
	}

	if (ActiveScreenInstance)
	{
		ActiveScreenInstance->RemoveFromParent();
		ActiveScreenInstance = nullptr;
	}

	CurrentScreen = EOLCUIScreen::None;
	OpenChampionSelect();
}

/** Fallback: skip crash sequence and go straight to gameplay. */
void AOLCMenuPlayerController::ConfirmChampionSelection(const FString& ChampionId)
{
	if (!MenuGameMode || ChampionId.IsEmpty()) return;
	MenuGameMode->SetSelectedChampionId(ChampionId);
	if (ActiveScreenInstance) { ActiveScreenInstance->RemoveFromParent(); ActiveScreenInstance=nullptr; }
	CurrentScreen=EOLCUIScreen::None;
	MenuGameMode->StartCrashSequence();
}

void AOLCMenuPlayerController::TransitionToGameplayDirect()
{
	if (WelcomeScreenInstance)
	{
		WelcomeScreenInstance->RemoveFromParent();
		WelcomeScreenInstance = nullptr;
	}

	if (MenuGameMode)
	{
		MenuGameMode->TransitionToGameplay();
		OpenMainRTSHUD();
		return;
	}

	// Fallback: switch camera to top-down RTS view without menu game mode support.
	FVector CameraLocation(0.0f, 0.0f, 4500.0f);
	FRotator CameraRotation(-80.0f, 90.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACameraActor* RTSCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);
	if (RTSCamera)
	{
		RTSCamera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		RTSCamera->GetCameraComponent()->SetOrthoWidth(7600.0f);

		SetViewTarget(RTSCamera);
		ActiveCamera = RTSCamera;
	}

	// Show main HUD.
	OpenMainRTSHUD();

	UE_LOG(LogOLC, Display, TEXT("[OLC] Transitioned to gameplay (direct — no crash sequence)"));
}

void AOLCMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
		return;

	InputComponent->BindKey(EKeys::F1, IE_Pressed, this, &AOLCMenuPlayerController::OpenPrimaryGameplayHUD);
	// H — keymap/controls reference, always available (F1 only reaches it pre-gameplay).
	InputComponent->BindKey(EKeys::H, IE_Pressed, this, &AOLCMenuPlayerController::OpenKeymap);
	InputComponent->BindKey(EKeys::B, IE_Pressed, this, &AOLCMenuPlayerController::OpenConstructionMode);
	InputComponent->BindKey(EKeys::F2, IE_Pressed, this, &AOLCMenuPlayerController::OpenConstructionMode);
	InputComponent->BindKey(EKeys::F3, IE_Pressed, this, &AOLCMenuPlayerController::OpenColonyResourceNetwork);
	InputComponent->BindKey(EKeys::F4, IE_Pressed, this, &AOLCMenuPlayerController::OpenSolarSystem);
	InputComponent->BindKey(EKeys::F5, IE_Pressed, this, &AOLCMenuPlayerController::OpenGalaxyMap);
	InputComponent->BindKey(EKeys::F6, IE_Pressed, this, &AOLCMenuPlayerController::OpenTacticalDungeon);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AOLCMenuPlayerController::OpenResearch);
	InputComponent->BindKey(EKeys::F8, IE_Pressed, this, &AOLCMenuPlayerController::OpenDropshipRepair);
	InputComponent->BindKey(EKeys::M, IE_Pressed, this, &AOLCMenuPlayerController::OpenMothershipBuilder);
	InputComponent->BindKey(EKeys::F9, IE_Pressed, this, &AOLCMenuPlayerController::OpenMothershipBuilder);
	InputComponent->BindKey(EKeys::F10, IE_Pressed, this, &AOLCMenuPlayerController::OpenEquipment);

	// Esc - close active screen / return to switcher.
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AOLCMenuPlayerController::CloseActiveScreen);

	InputComponent->BindKey(EKeys::One, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome1);
	InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome2);
	InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome3);
	InputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome4);
	InputComponent->BindKey(EKeys::Five, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome5);
	InputComponent->BindKey(EKeys::Six, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome6);
	InputComponent->BindKey(EKeys::Seven, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome7);
	InputComponent->BindKey(EKeys::Eight, IE_Pressed, this, &AOLCMenuPlayerController::OnBiome8);
	InputComponent->BindKey(EKeys::G, IE_Pressed, this, &AOLCMenuPlayerController::OnToggleTerrainRenderMode);
	InputComponent->BindKey(EKeys::T, IE_Pressed, this, &AOLCMenuPlayerController::OnRegenerateTerrain);
	InputComponent->BindKey(EKeys::Up, IE_Pressed, this, &AOLCMenuPlayerController::PanCameraUp);
	InputComponent->BindKey(EKeys::Down, IE_Pressed, this, &AOLCMenuPlayerController::PanCameraDown);
	InputComponent->BindKey(EKeys::Left, IE_Pressed, this, &AOLCMenuPlayerController::PanCameraLeft);
	InputComponent->BindKey(EKeys::Right, IE_Pressed, this, &AOLCMenuPlayerController::PanCameraRight);

	// WP-129 Step 2: E — context-sensitive interact (inspect wreck / manual mine).
	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AOLCMenuPlayerController::OnInteract);

	// Mouse wheel - Zoom camera (orthographic width)
	InputComponent->BindAxis(TEXT("MouseWheel"), this, &AOLCMenuPlayerController::OnZoomCamera);
}

void AOLCMenuPlayerController::OpenMainRTSHUD() { OpenUIScreen(EOLCUIScreen::MainRTSHUD); }
void AOLCMenuPlayerController::OpenKeymap() { OpenUIScreen(EOLCUIScreen::Keymap); }
void AOLCMenuPlayerController::OpenConstructionMode() { OpenUIScreen(EOLCUIScreen::ConstructionMode); }
void AOLCMenuPlayerController::OpenColonyResourceNetwork() { OpenUIScreen(EOLCUIScreen::ColonyResourceNetwork); }
void AOLCMenuPlayerController::OpenSolarSystem() { OpenUIScreen(EOLCUIScreen::SolarSystem); }
void AOLCMenuPlayerController::OpenGalaxyMap() { OpenUIScreen(EOLCUIScreen::GalaxyMap); }
void AOLCMenuPlayerController::OpenFactionSelect() { OpenUIScreen(EOLCUIScreen::FactionSelect); }
void AOLCMenuPlayerController::OpenChampionSelect() { OpenUIScreen(EOLCUIScreen::ChampionSelect); }
void AOLCMenuPlayerController::OpenTacticalDungeon() { OpenUIScreen(EOLCUIScreen::TacticalDungeon); }
void AOLCMenuPlayerController::OpenResearch() { OpenUIScreen(EOLCUIScreen::Research); }
void AOLCMenuPlayerController::OpenDropshipRepair() { OpenUIScreen(EOLCUIScreen::DropshipRepair); }
void AOLCMenuPlayerController::OpenMothershipBuilder() { OpenUIScreen(EOLCUIScreen::MothershipBuilder); }
void AOLCMenuPlayerController::OpenEquipment() { OpenUIScreen(EOLCUIScreen::Equipment); }

void AOLCMenuPlayerController::HandleSquadReady(const FOLCSquadDeploymentData& SquadData)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCDungeonStateSubsystem* DungeonState = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
		{
			DungeonState->ConfirmSquad(SquadData);
			if (DungeonState->GetPendingExpedition().bValid)
			{
				OpenUIScreen(EOLCUIScreen::TacticalDungeon);
				return;
			}
		}
	}
	// No pending expedition (e.g. squad selection reached outside the S07 dungeon flow): keep current behavior.
}

bool AOLCMenuPlayerController::IsGameplayActive() const
{
	return MenuGameMode && MenuGameMode->GetCurrentGameState() == EOLCGameState::Gameplay;
}

void AOLCMenuPlayerController::OpenPrimaryGameplayHUD()
{
	if (IsGameplayActive())
	{
		OpenMainRTSHUD();
		return;
	}

	OpenKeymap();
}

void AOLCMenuPlayerController::OpenGameplayShipBuilder()
{
	if (ActiveScreenInstance)
	{
		ActiveScreenInstance->RemoveFromParent();
		ActiveScreenInstance = nullptr;
		CurrentScreen = EOLCUIScreen::None;
		return;
	}

	ActiveScreenInstance = CreateWidget<UOLCShipBuilderWidget>(this, UOLCShipBuilderWidget::StaticClass());
	if (ActiveScreenInstance)
	{
		AddFullscreenWidget(ActiveScreenInstance, 200);
		CurrentScreen = EOLCUIScreen::MothershipBuilder;
		EnsureMouseCursorVisible(ActiveScreenInstance.Get());
		UE_LOG(LogOLC, Display, TEXT("[OLC] Ship Builder opened"));
	}
}

AOLCPlanetTerrainActor* AOLCMenuPlayerController::FindTerrainActor() const
{
	if (!GetWorld())
	{
		return nullptr;
	}

	for (TActorIterator<AOLCPlanetTerrainActor> It(GetWorld()); It; ++It)
	{
		return *It;
	}

	return nullptr;
}

void AOLCMenuPlayerController::SetGameplayBiome(int32 BiomeIndex)
{
	if (IsGameplayActive())
	{
		if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor())
		{
			Terrain->SetBiomeByIndex(BiomeIndex);
		}
	}
}

void AOLCMenuPlayerController::OnBiome1() { SetGameplayBiome(0); }
void AOLCMenuPlayerController::OnBiome2() { SetGameplayBiome(1); }
void AOLCMenuPlayerController::OnBiome3() { SetGameplayBiome(2); }
void AOLCMenuPlayerController::OnBiome4() { SetGameplayBiome(3); }
void AOLCMenuPlayerController::OnBiome5() { SetGameplayBiome(4); }
void AOLCMenuPlayerController::OnBiome6() { SetGameplayBiome(5); }
void AOLCMenuPlayerController::OnBiome7() { SetGameplayBiome(6); }
void AOLCMenuPlayerController::OnBiome8() { SetGameplayBiome(7); }

void AOLCMenuPlayerController::OnToggleTerrainRenderMode()
{
	if (IsGameplayActive())
	{
		if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor())
		{
			Terrain->ToggleRenderMode();
		}
	}
}

void AOLCMenuPlayerController::OnRegenerateTerrain()
{
	if (IsGameplayActive())
	{
		if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor())
		{
			Terrain->Regenerate();
		}
	}
}

void AOLCMenuPlayerController::PanCameraUp() { PanCameraBy(FVector(0.0f, 600.0f, 0.0f)); }
void AOLCMenuPlayerController::PanCameraDown() { PanCameraBy(FVector(0.0f, -600.0f, 0.0f)); }
void AOLCMenuPlayerController::PanCameraLeft() { PanCameraBy(FVector(-600.0f, 0.0f, 0.0f)); }
void AOLCMenuPlayerController::PanCameraRight() { PanCameraBy(FVector(600.0f, 0.0f, 0.0f)); }

void AOLCMenuPlayerController::PanCameraBy(const FVector& Delta)
{
	if (!IsGameplayActive() || !ActiveCamera)
	{
		return;
	}

	ActiveCamera->AddActorWorldOffset(Delta, false);
}

// ---------------------------------------------------------------------------
// WP-129 Step 2: E — context-sensitive interact (tutorial objectives 1 & 2)
// ---------------------------------------------------------------------------
void AOLCMenuPlayerController::OnInteract()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Player presence for proximity checks: the controlled pawn when one
	// exists, otherwise the RTS camera view target (the top-down gameplay
	// flow has no possessed pawn).
	FVector PresenceLocation = GetFocalLocation();
	if (const APawn* ControlledPawn = GetPawn())
	{
		PresenceLocation = ControlledPawn->GetActorLocation();
	}

	// 1) Near the crash-site wreck: inspect it. Completes objective 1 and
	// grants the dropship-storage reward exactly once (guarded in the actor).
	for (TActorIterator<AOLCCrashSitePrototypeActor> It(World); It; ++It)
	{
		if (It->IsWithinInteractProximity(PresenceLocation))
		{
			It->InteractWithCrashSite();
			return;
		}
	}

	// 2) Otherwise: manual mining — hand-collect construction material from
	// the nearby area (objective 2 path). Gameplay state only, and not while
	// an overlay screen is open. The >=50 CM completion check lives in
	// AddResourceFromManualMining and is guarded there (no re-trigger).
	if (!IsGameplayActive() || ActiveScreenInstance)
	{
		return;
	}

	if (UGameInstance* GI = World->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			constexpr float HandMineYieldPerPress = 5.0f; // RES-CM-01: manual collection = 5 material/cycle
			Data->AddResourceFromManualMining(EOLCResourceType::ConstructionMaterial, HandMineYieldPerPress);
			UE_LOG(LogOLC, Display, TEXT("[OLC] Tutorial: manual mining +%.0f construction material"), HandMineYieldPerPress);
		}
	}
}

void AOLCMenuPlayerController::OpenUIScreen(EOLCUIScreen Screen)
{
	UE_LOG(LogOLC, Log, TEXT("[OLC] Opening UI screen: %d"), (int32)Screen);

	// Remove any currently active screen widget.
	if (ActiveScreenInstance)
	{
		ActiveScreenInstance->RemoveFromParent();
		ActiveScreenInstance = nullptr;
	}

	if (TestSwitcherInstance && Screen != EOLCUIScreen::TestSwitcher)
	{
		TestSwitcherInstance->RemoveFromParent();
		TestSwitcherInstance = nullptr;
	}

	if (Screen == EOLCUIScreen::Keymap && WelcomeScreenInstance)
	{
		WelcomeScreenInstance->RemoveFromParent();
		WelcomeScreenInstance = nullptr;
	}

	EOLCUIScreen PreviousScreen = CurrentScreen;
	CurrentScreen = EOLCUIScreen::None;

	switch (Screen)
	{
		case EOLCUIScreen::TestSwitcher:
			ShowTestSwitcher();
			return;

		case EOLCUIScreen::MainRTSHUD:
		{
			UOLCMainRTSHUDWidget* MainRTSHUDWidget = CreateWidget<UOLCMainRTSHUDWidget>(this, UOLCMainRTSHUDWidget::StaticClass());
			ActiveScreenInstance = MainRTSHUDWidget;
			if (MainRTSHUDWidget)
			{
				// Bind ship status indicator click to open ship builder
				MainRTSHUDWidget->OnShipStatusClicked.AddDynamic(this, &AOLCMenuPlayerController::OpenMothershipBuilder);
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Main RTS HUD opened"));
			}
			break;
		}

		case EOLCUIScreen::Keymap:
		{
			ActiveScreenInstance = CreateWidget<UOLCKeymapWidget>(this, UOLCKeymapWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddCenteredWidget(ActiveScreenInstance, 300, FVector2D(820.0f, 610.0f));
				UE_LOG(LogOLC, Display, TEXT("[OLC] Keymap opened"));
			}
			break;
		}

		case EOLCUIScreen::ConstructionMode:
		{
			ActiveScreenInstance = CreateWidget<UOLCConstructionOverlayWidget>(this, UOLCConstructionOverlayWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Construction Mode opened"));
			}
			break;
		}

		case EOLCUIScreen::FactionSelect:
		{
			if (FactionSelectWidgetClass)
			{
				ActiveScreenInstance = CreateWidget<UUserWidget>(this, FactionSelectWidgetClass);
				if (ActiveScreenInstance)
				{
					if (UOLCFactionSelectWidget* FactionWidget = Cast<UOLCFactionSelectWidget>(ActiveScreenInstance))
					{
						FactionWidget->OnFactionConfirmed.AddDynamic(this, &AOLCMenuPlayerController::ConfirmFactionSelection);
						FactionWidget->OnBackPressed.AddDynamic(this, &AOLCMenuPlayerController::HandleFactionSelectBack);
					}
					AddFullscreenWidget(ActiveScreenInstance, 200);
					CurrentScreen = Screen;
					UE_LOG(LogOLC, Display, TEXT("[OLC] Faction Select opened"));
				}
				else
				{
					UE_LOG(LogOLC, Warning, TEXT("[OLC] Failed to instantiate FactionSelect widget"));
				}
			}
			else
			{
				UE_LOG(LogOLC, Warning, TEXT("[OLC] FactionSelectWidgetClass not assigned"));
			}
			break;
		}

		case EOLCUIScreen::ChampionSelect:
		{
			UOLCChampionSelectWidget* W=CreateWidget<UOLCChampionSelectWidget>(this,UOLCChampionSelectWidget::StaticClass()); ActiveScreenInstance=W;
			if(W){W->InitializeForFaction(MenuGameMode?MenuGameMode->GetSelectedFactionId():FString());AddFullscreenWidget(W,200);CurrentScreen=Screen;}
			break;
		}

		case EOLCUIScreen::Research:
		{
			// S05 Tech Tree screen.
			ActiveScreenInstance = CreateWidget<UOLCTechTreeWidget>(this, UOLCTechTreeWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Tech Tree screen opened"));
			}
			break;
		}

		case EOLCUIScreen::SolarSystem:
		{
			// S11 Solar System View.
			ActiveScreenInstance = CreateWidget<UOLCSolarSystemWidget>(this, UOLCSolarSystemWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Solar System View opened"));
			}
			break;
		}

		case EOLCUIScreen::GalaxyMap:
		{
			// S12 Galaxy Map — if coming from Solar System (S11), play transition.
			if (PreviousScreen == EOLCUIScreen::SolarSystem)
			{
				StartScreenTransition();
			}

			ActiveScreenInstance = CreateWidget<UOLCGalaxyMapWidget>(this, UOLCGalaxyMapWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Galaxy Map opened"));
			}
			break;
		}

		case EOLCUIScreen::DungeonEntry:
		{
			// S07 Dungeon Entry screen.
			ActiveScreenInstance = CreateWidget<UOLCDungeonEntryWidget>(this, UOLCDungeonEntryWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Dungeon Entry screen opened"));
			}
			break;
		}

		case EOLCUIScreen::SquadSelection:
		{
			// S09 Squad Selection screen.
			UOLCSquadSelectionWidget* SquadWidget = CreateWidget<UOLCSquadSelectionWidget>(this, UOLCSquadSelectionWidget::StaticClass());
			ActiveScreenInstance = SquadWidget;
			if (SquadWidget)
			{
				SquadWidget->OnSquadReady.AddDynamic(this, &AOLCMenuPlayerController::HandleSquadReady);

				// WP-130: if a dungeon expedition is pending, set the boss race family so the
				// "Effective vs Target Race" stat line (WP-119 Step 4) can show (completes that gap).
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UOLCDungeonStateSubsystem* DungeonState = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
					{
						const FOLCPendingExpedition& Pending = DungeonState->GetPendingExpedition();
						if (Pending.bValid && Pending.Dungeon && Pending.Dungeon->bHasBoss)
						{
							if (UOLCRaceSubsystem* Races = GI->GetSubsystem<UOLCRaceSubsystem>())
							{
								if (UOLCRaceData* BossRace = Races->FindRaceById(Pending.Dungeon->BossRaceId))
								{
									SquadWidget->TargetRaceFamily = BossRace->RaceFamily;
								}
							}
						}
					}
				}

				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Squad Selection screen opened"));
			}
			break;
		}

		case EOLCUIScreen::TacticalDungeon:
		{
			// S08 Tactical Combat View.
			UOLCTacticalCombatWidget* TacticalWidget = CreateWidget<UOLCTacticalCombatWidget>(this, UOLCTacticalCombatWidget::StaticClass());
			ActiveScreenInstance = TacticalWidget;
			if (TacticalWidget)
			{
				// WP-130: initialize from the pending expedition (generated layout + confirmed squad)
				// when one exists; otherwise the widget falls back to its prototype init.
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UOLCDungeonStateSubsystem* DungeonState = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
					{
						FOLCPendingExpedition Expedition;
						if (DungeonState->ConsumePendingExpedition(Expedition))
						{
							TacticalWidget->InitializeFromExpedition(Expedition.Dungeon, Expedition.Layout, Expedition.Squad);
						}
					}
				}

				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Tactical Combat View opened"));
			}
			break;
		}

		case EOLCUIScreen::CombatResults:
		{
			// S10 Combat Results screen.
			UOLCCombatResultsWidget* ResultsWidget = CreateWidget<UOLCCombatResultsWidget>(this, UOLCCombatResultsWidget::StaticClass());
			ActiveScreenInstance = ResultsWidget;
			if (ResultsWidget)
			{
				// WP-130: feed the real completion result recorded by the tactical widget, when present.
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UOLCDungeonStateSubsystem* DungeonState = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
					{
						FDungeonCompletionResult Result;
						if (DungeonState->ConsumeLastResult(Result))
						{
							ResultsWidget->InitializeFromResult(Result);
						}
					}
				}

				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Combat Results opened"));
			}
			break;
		}

		case EOLCUIScreen::DropshipRepair:
		{
			// S13 Dropship Repair View.
			ActiveScreenInstance = CreateWidget<UOLCDropshipRepairWidget>(this, UOLCDropshipRepairWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Dropship Repair View opened"));
			}
			break;
		}

		case EOLCUIScreen::MothershipBuilder:
		{
			if (IsGameplayActive())
			{
				OpenGameplayShipBuilder();
				return;
			}

			// S14 Ship Module Management.
			ActiveScreenInstance = CreateWidget<UOLCShipModuleManagementWidget>(this, UOLCShipModuleManagementWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
				UE_LOG(LogOLC, Display, TEXT("[OLC] Ship Module Management opened"));
			}
			break;
		}

		default:
			UE_LOG(LogOLC, Warning, TEXT("[OLC] Screen %d not yet implemented — showing placeholder"), (int32)Screen);
			ActiveScreenInstance = CreateWidget<UOLCTestSwitcherWidget>(this, UOLCTestSwitcherWidget::StaticClass());
			if (ActiveScreenInstance)
			{
				AddFullscreenWidget(ActiveScreenInstance, 200);
			}
			break;
	}

		if (ActiveScreenInstance)
		{
			CurrentScreen = Screen;
		}

		EnsureMouseCursorVisible(ActiveScreenInstance.Get());
}

void AOLCMenuPlayerController::CloseActiveScreen()
{
	if (CurrentScreen != EOLCUIScreen::TestSwitcher && CurrentScreen != EOLCUIScreen::None)
	{
		if (ActiveScreenInstance)
		{
			ActiveScreenInstance->RemoveFromParent();
			ActiveScreenInstance = nullptr;
		}

		if (CurrentScreen == EOLCUIScreen::Keymap)
		{
			CurrentScreen = EOLCUIScreen::None;
			return;
		}

		ShowTestSwitcher();
		return;
	}

	// If we're on the test switcher, Esc does nothing.
	UE_LOG(LogOLC, Log, TEXT("[OLC] Already on test switcher — Esc ignored"));
}

void AOLCMenuPlayerController::ShowTestSwitcher()
{
	// Remove any active screen first.
	if (ActiveScreenInstance && ActiveScreenInstance != TestSwitcherInstance)
	{
		ActiveScreenInstance->RemoveFromParent();
		ActiveScreenInstance = nullptr;
	}

	if (!TestSwitcherInstance)
	{
		TestSwitcherInstance = CreateWidget<UOLCTestSwitcherWidget>(this, UOLCTestSwitcherWidget::StaticClass());
	}

	if (TestSwitcherInstance)
	{
		AddFullscreenWidget(TestSwitcherInstance, 200);
		ActiveScreenInstance = TestSwitcherInstance;
		CurrentScreen = EOLCUIScreen::TestSwitcher;
	}

	EnsureMouseCursorVisible(TestSwitcherInstance.Get());
}

void AOLCMenuPlayerController::EnsureMouseCursorVisible(UUserWidget* FocusWidget)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	if (FocusWidget)
	{
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	}
	SetInputMode(InputMode);
}

void AOLCMenuPlayerController::AddFullscreenWidget(UUserWidget* Widget, int32 ZOrder)
{
	if (!Widget)
	{
		return;
	}

	Widget->SetAnchorsInViewport(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Widget->SetAlignmentInViewport(FVector2D::ZeroVector);
	Widget->SetDesiredSizeInViewport(FVector2D::ZeroVector);
	Widget->AddToViewport(ZOrder);
}

void AOLCMenuPlayerController::AddCenteredWidget(UUserWidget* Widget, int32 ZOrder, const FVector2D& WidgetSize)
{
	if (!Widget)
	{
		return;
	}

	int32 ViewportX = 1280;
	int32 ViewportY = 720;
	GetViewportSize(ViewportX, ViewportY);
	if (ViewportX <= 0 || ViewportY <= 0)
	{
		ViewportX = 1280;
		ViewportY = 720;
	}

	const FVector2D ViewportSize(static_cast<float>(ViewportX), static_cast<float>(ViewportY));
	const FVector2D PopupPosition(
		FMath::Max(0.0f, (ViewportSize.X - WidgetSize.X) * 0.5f),
		FMath::Max(0.0f, (ViewportSize.Y - WidgetSize.Y) * 0.5f));

	Widget->SetAnchorsInViewport(FAnchors(0.0f, 0.0f));
	Widget->SetAlignmentInViewport(FVector2D::ZeroVector);
	Widget->SetPositionInViewport(PopupPosition, false);
	Widget->SetDesiredSizeInViewport(WidgetSize);
	Widget->AddToViewport(ZOrder);
}

void AOLCMenuPlayerController::OnZoomCamera(float Delta)
{
	if (!ActiveCamera || !ActiveCamera->GetCameraComponent()) return;

	CurrentOrthoWidth = FMath::Clamp(CurrentOrthoWidth - Delta * 400.0f, 1500.0f, 20000.0f);
	ActiveCamera->GetCameraComponent()->SetOrthoWidth(CurrentOrthoWidth);

	UE_LOG(LogOLC, Log, TEXT("[OLC] Camera zoom: OrthoWidth = %.0f"), CurrentOrthoWidth);
}

// ---------------------------------------------------------------------------
// S11→S12 transition animation (zoom-out effect)
// ---------------------------------------------------------------------------

void AOLCMenuPlayerController::StartScreenTransition()
{
	if (bIsTransitioning) return;
	bIsTransitioning = true;
	TransitionProgress = 0.0f;
	OnTransitionTick(1.0f);
}

void AOLCMenuPlayerController::OnTransitionTick(float Value)
{
	TransitionProgress = Value;

	if (TransitionOverlay)
	{
		TransitionOverlay = nullptr;
	}

	// At completion: remove overlay, finalize galaxy map display.
	if (Value >= 1.0f)
	{
		bIsTransitioning = false;
		if (TransitionOverlay)
		{
			TransitionOverlay = nullptr;
		}
	}
}

#undef LOCTEXT_NAMESPACE
