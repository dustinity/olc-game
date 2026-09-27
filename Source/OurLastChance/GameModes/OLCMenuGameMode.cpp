#include "OLCMenuGameMode.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Core/OLCResourceTypes.h"
#include "Core/OLCResearchSubsystem.h"
#include "Core/OLCUIDataSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Player/OLCGameplayPlayerController.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCGameplayWorldActor.h"
#include "Core/OLCFactionData.h"
#include "EngineUtils.h"

AOLCMenuGameMode::AOLCMenuGameMode()
{
	// Phase durations for crash animation sequence (seconds):
	// 0: Side-view flight approach     — 4s
	// 1: Atmospheric entry / descent   — 3s
	// 2: Crash impact + dust cloud     — 2s
	// 3: Post-crash scene              — 3s
	// 4: Door opens, transition ready  — 2s
	CrashPhaseDurations = { 4.0f, 3.0f, 2.0f, 3.0f, 2.0f };

	PrimaryActorTick.bCanEverTick = true;

	// Use menu player controller for welcome screen and crash sequence.
	PlayerControllerClass = AOLCMenuPlayerController::StaticClass();
}

void AOLCMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start in menu state — welcome screen is shown by the menu player controller.
	CurrentGameState = EOLCGameState::Menu;
	CurrentCrashPhase = 0;

	UE_LOG(LogTemp, Log, TEXT("[OLC] MenuGameMode initialized — waiting for New Game"));
}

void AOLCMenuGameMode::StartCrashSequence()
{
	CurrentGameState = EOLCGameState::CrashSequence;
	CurrentCrashPhase = 0;
	bWelcomeShown = true;

	UE_LOG(LogTemp, Log, TEXT("[OLC] Crash sequence started — phase 0: side-view flight"));

	// Set up the side-view camera for the crash animation.
	SetupSequenceCamera();

	// Start the first phase timer.
	StartPhaseTimer();
}

void AOLCMenuGameMode::TransitionToGameplay()
{
	CurrentGameState = EOLCGameState::Gameplay;
	CurrentCrashPhase = -1; // No more phases

	// Cancel any remaining crash timers.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CrashPhaseTimer);
	}

	// Spawn gameplay world inside the same map before switching to RTS view.
	SpawnGameplayWorld();

	// Destroy the sequence camera.
	if (SequenceCamera)
	{
		SequenceCamera->Destroy();
		SequenceCamera = nullptr;
	}

	// Switch camera to top-down RTS view.
	SetupRTSCamera();

	// WP-109: Transition player controller from Menu to Gameplay for proper input handling.
	// The menu PC handles F-keys for screen switching, gameplay PC handles terrain/building controls.
	TransitionPlayerControllerToGameplay();

	// Show the main HUD and set tutorial objectives from Briefing Phase 1.
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Data->ResetResourcesToZero();

			TArray<FOLCMissionObjectiveViewData> TutorialObjectives;

			// 1. Explore crash site — Active from start.
			TutorialObjectives.Emplace();
			TutorialObjectives.Last().ObjectiveName = FText::FromString(TEXT("Explore crash site"));
			TutorialObjectives.Last().Description = FText::FromString(TEXT("Scavenge remaining supplies from the dropship storage module"));
			TutorialObjectives.Last().Progress = 0.0f;
			TutorialObjectives.Last().TargetProgress = 1.0f;
			TutorialObjectives.Last().State = EOLCProgressState::Active;

			// 2. Collect construction material — Idle until objective 1 complete.
			TutorialObjectives.Emplace();
			TutorialObjectives.Last().ObjectiveName = FText::FromString(TEXT("Collect construction material"));
			TutorialObjectives.Last().Description = FText::FromString(TEXT("Manual mining of stone/concrete from nearby area"));
			TutorialObjectives.Last().Progress = 0.0f;
			TutorialObjectives.Last().TargetProgress = 1.0f;
			TutorialObjectives.Last().State = EOLCProgressState::Idle;

			// 3. Discover mineral deposit — Idle until objective 2 complete.
			TutorialObjectives.Emplace();
			TutorialObjectives.Last().ObjectiveName = FText::FromString(TEXT("Discover mineral deposit"));
			TutorialObjectives.Last().Description = FText::FromString(TEXT("Find and scan mineral deposits using basic radar"));
			TutorialObjectives.Last().Progress = 0.0f;
			TutorialObjectives.Last().TargetProgress = 1.0f;
			TutorialObjectives.Last().State = EOLCProgressState::Idle;

			// 4. Build solar panel — Idle until objective 3 complete.
			TutorialObjectives.Emplace();
			TutorialObjectives.Last().ObjectiveName = FText::FromString(TEXT("Build solar panel"));
			TutorialObjectives.Last().Description = FText::FromString(TEXT("First structure placement — open construction mode (F2)"));
			TutorialObjectives.Last().Progress = 0.0f;
			TutorialObjectives.Last().TargetProgress = 1.0f;
			TutorialObjectives.Last().State = EOLCProgressState::Idle;

			// 5. Repair left drive — Idle until objective 4 complete.
			TutorialObjectives.Emplace();
			TutorialObjectives.Last().ObjectiveName = FText::FromString(TEXT("Repair left drive"));
			TutorialObjectives.Last().Description = FText::FromString(TEXT("First research unlock — 15 minutes at forge bench"));
			TutorialObjectives.Last().Progress = 0.0f;
			TutorialObjectives.Last().TargetProgress = 1.0f;
			TutorialObjectives.Last().State = EOLCProgressState::Idle;

			Data->SetMissionObjectives(TutorialObjectives);

			// Also add dropship starting resources per Briefing Dropship README:
			// Construction Material ~80, Minerals 50, Fuel 30, Survival 40, Hull Parts 25
			Data->AddResource(EOLCResourceType::ConstructionMaterial, 80.0f);
			Data->AddResource(EOLCResourceType::Minerals, 50.0f);
			Data->AddResource(EOLCResourceType::Fuel, 30.0f);
			Data->AddResource(EOLCResourceType::Survival, 40.0f);
			Data->AddResource(EOLCResourceType::HullParts, 25.0f);

			UE_LOG(LogTemp, Display, TEXT("[OLC] Tutorial objectives initialized — %d steps"), TutorialObjectives.Num());
		}

		// Register starter techs and auto-complete Core ring.
		if (UOLCResearchSubsystem* Research = GI->GetSubsystem<UOLCResearchSubsystem>())
		{
			Research->RegisterStarterTechs();
			Research->AutoCompleteCoreTechs();

			UE_LOG(LogTemp, Display, TEXT("[OLC] Tech tree initialized — starter techs registered and Core auto-unlocked"));
		}
	}

	UE_LOG(LogTemp, Display, TEXT("[OLC] Transitioned to gameplay — top-down RTS view active"));
}

/** WP-109: Switch player controller from Menu PC to Gameplay PC for proper input handling. */
void AOLCMenuGameMode::TransitionPlayerControllerToGameplay()
{
	if (!GetWorld()) return;

	APlayerController* CurrentPC = GetWorld()->GetFirstPlayerController();
	if (!CurrentPC) return;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	CurrentPC->SetInputMode(InputMode);
	CurrentPC->SetIgnoreMoveInput(false);
	CurrentPC->SetIgnoreLookInput(false);
	CurrentPC->bShowMouseCursor = true;

	UE_LOG(LogTemp, Display, TEXT("[OLC] Player controller transitioned to gameplay input mode"));
}

void AOLCMenuGameMode::SpawnGameplayWorld()
{
	if (!GetWorld())
	{
		return;
	}

	AOLCGameplayWorldActor* GameplayWorldActor = nullptr;
	for (TActorIterator<AOLCGameplayWorldActor> It(GetWorld()); It; ++It)
	{
		GameplayWorldActor = *It;
		break;
	}

	if (!GameplayWorldActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GameplayWorldActor = GetWorld()->SpawnActor<AOLCGameplayWorldActor>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	if (!GameplayWorldActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Failed to spawn gameplay world actor"));
		return;
	}

	UOLCFactionData* SelectedFactionData = LoadFactionDataById(SelectedFactionId);
	if (!SelectedFactionData && !SelectedFactionId.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Could not load faction data asset for '%s'"), *SelectedFactionId);
	}

	GameplayWorldActor->InitializeGameplayWorld(GameplayBiome, GameplaySeed, SelectedFactionData);
}

UOLCFactionData* AOLCMenuGameMode::LoadFactionDataById(const FString& FactionId) const
{
	if (FactionId.IsEmpty())
	{
		return nullptr;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.ClassPaths.Add(UOLCFactionData::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(TEXT("/Game/Data/Factions"));
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	for (const FAssetData& AssetData : AssetList)
	{
		FString AssetFactionId;
		if (AssetData.GetTagValue(TEXT("FactionId"), AssetFactionId) && AssetFactionId == FactionId)
		{
			return Cast<UOLCFactionData>(AssetData.GetAsset());
		}
	}

	return nullptr;
}

void AOLCMenuGameMode::SetSelectedFactionId(const FString& InFactionId)
{
	SelectedFactionId = InFactionId;
}

FString AOLCMenuGameMode::GetSelectedFactionId() const
{
	return SelectedFactionId;
}

void AOLCMenuGameMode::SetSelectedChampionId(const FString& InChampionId)
{
	SelectedChampionId = InChampionId;
}

FString AOLCMenuGameMode::GetSelectedChampionId() const
{
	return SelectedChampionId;
}

void AOLCMenuGameMode::SetupSequenceCamera()
{
	if (!GetWorld()) return;

	// Find or create a side-view camera actor.
	// Use a fixed side-view position looking at the crash site center.
	FVector CameraLocation(0.0f, -2500.0f, 120.0f); // Side view, offset Y, low Z
	FRotator CameraRotation(0.0f, 90.0f, 0.0f);     // Looking along +X axis (side profile)

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SequenceCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);

	if (SequenceCamera)
	{
		// Set as the view target for the first player controller.
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetViewTarget(SequenceCamera);
			UE_LOG(LogTemp, Log, TEXT("[OLC] Sequence camera placed at side-view position"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Failed to spawn sequence camera"));
	}
}

void AOLCMenuGameMode::SetupRTSCamera()
{
	if (!GetWorld()) return;

	// Destroy the sequence camera.
	if (SequenceCamera)
	{
		SequenceCamera->Destroy();
		SequenceCamera = nullptr;
	}

	// Create a top-down RTS orthographic camera.
	FVector CameraLocation(0.0f, 0.0f, 4500.0f); // High above center
	FRotator CameraRotation(-80.0f, 90.0f, 0.0f); // Top-down angle looking at +X plane

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACameraActor* RTSCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);

	if (RTSCamera)
	{
		// Set orthographic view for RTS perspective.
		RTSCamera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		RTSCamera->GetCameraComponent()->SetOrthoWidth(7600.0f);

		// Smoothly transition the player's view target to the RTS camera.
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetViewTargetWithBlend(RTSCamera, 1.5f); // 1.5s smooth transition

			// Update menu player controller's camera reference for zoom support.
			if (AOLCMenuPlayerController* MenuPC = Cast<AOLCMenuPlayerController>(PC))
			{
				MenuPC->SetActiveCamera(RTSCamera);
			}

			UE_LOG(LogTemp, Log, TEXT("[OLC] RTS camera placed at top-down position"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[OLC] Failed to spawn RTS camera"));
	}
}

void AOLCMenuGameMode::AdvanceCrashPhase()
{
	if (CurrentCrashPhase < 0 || CurrentCrashPhase >= CrashPhaseDurations.Num())
	{
		// All phases complete — transition to gameplay.
		TransitionToGameplay();
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Crash phase %d/%d complete"),
		CurrentCrashPhase + 1, CrashPhaseDurations.Num());

	CurrentCrashPhase++;

	if (CurrentCrashPhase < CrashPhaseDurations.Num())
	{
		// Log what's happening in this phase.
		const FString PhaseNames[] = {
			TEXT("Side-view flight approach"),
			TEXT("Atmospheric entry / descent"),
			TEXT("Crash impact + dust cloud"),
			TEXT("Post-crash scene — smoke rising"),
			TEXT("Door opens — ready for gameplay")
		};

		if (CurrentCrashPhase < UE_ARRAY_COUNT(PhaseNames))
		{
			UE_LOG(LogTemp, Log, TEXT("[OLC] Phase %d: %s"), CurrentCrashPhase, *PhaseNames[CurrentCrashPhase]);
		}

		StartPhaseTimer();
	}
	else
	{
		TransitionToGameplay();
	}
}

void AOLCMenuGameMode::StartPhaseTimer()
{
	if (!GetWorld() || CurrentCrashPhase < 0 || CurrentCrashPhase >= CrashPhaseDurations.Num())
		return;

	float Duration = CrashPhaseDurations[CurrentCrashPhase];
	GetWorld()->GetTimerManager().SetTimer(CrashPhaseTimer, this,
		&AOLCMenuGameMode::AdvanceCrashPhase, Duration, false);
}

void AOLCMenuGameMode::OnCrashPhaseTimer()
{
	// Legacy — now handled by AdvanceCrashPhase via timer. Kept for compatibility.
	AdvanceCrashPhase();
}
