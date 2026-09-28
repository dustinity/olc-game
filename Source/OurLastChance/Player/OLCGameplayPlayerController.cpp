#include "OLCGameplayPlayerController.h"
#include "OurLastChance.h"

#include "World/OLCUnitBase.h"
#include "World/OLCGroundUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Core/OLCNavigationSubsystem.h"
#include "UI/OLCHUDWidgets.h"
#include "UI/OLCSharedWidgets.h"
#include "UI/OLCShipBuilderWidget.h"
#include "Blueprint/UserWidget.h"
#include "World/OLCPlanetTerrainActor.h"
#include "EngineUtils.h"

AOLCGameplayPlayerController::AOLCGameplayPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AOLCGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AOLCGameplayPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Per-frame driver for the navigation subsystem's scan timer.
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCNavigationSubsystem* NavSub = GI->GetSubsystem<UOLCNavigationSubsystem>())
		{
			NavSub->TickScans(DeltaTime);
		}
	}

	if (FMath::IsNearlyZero(CameraMoveX) && FMath::IsNearlyZero(CameraMoveY))
	{
		return;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector Delta(CameraMoveX * CameraPanSpeed * DeltaTime, CameraMoveY * CameraPanSpeed * DeltaTime, 0.0f);
		ControlledPawn->AddActorWorldOffset(Delta, true);
	}
}

void AOLCGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;

	// R - Rotate build placement
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &AOLCGameplayPlayerController::OnRotateBuild);

	// Esc - Cancel construction
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AOLCGameplayPlayerController::OnCancelBuild);

	// Space - Toggle simulation speed
	InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleSimulationSpeed);

	// WP-107 Step 5: F1 - Toggle main HUD overlay
	InputComponent->BindKey(EKeys::F1, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleMainHUD);

	// H - Toggle keymap / controls reference.
	InputComponent->BindKey(EKeys::H, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleKeymap);

	// Left click - Select unit (raycast) or confirm build in construction mode
	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AOLCGameplayPlayerController::OnLeftClick);

	// Right click - Move selected unit
	InputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AOLCGameplayPlayerController::OnRightClickCapture);

	InputComponent->BindKey(EKeys::One, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome1);
	InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome2);
	InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome3);
	InputComponent->BindKey(EKeys::Four, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome4);
	InputComponent->BindKey(EKeys::Five, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome5);
	InputComponent->BindKey(EKeys::Six, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome6);
	InputComponent->BindKey(EKeys::Seven, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome7);
	InputComponent->BindKey(EKeys::Eight, IE_Pressed, this, &AOLCGameplayPlayerController::OnBiome8);
	InputComponent->BindKey(EKeys::G, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleTerrainRenderMode);
	InputComponent->BindKey(EKeys::T, IE_Pressed, this, &AOLCGameplayPlayerController::OnRegenerateTerrain);
	InputComponent->BindKey(EKeys::F2, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleConstructionMode);
	InputComponent->BindKey(EKeys::M, IE_Pressed, this, &AOLCGameplayPlayerController::OnToggleShipBuilder);

	InputComponent->BindAxis(TEXT("OLC_CameraMoveX"), this, &AOLCGameplayPlayerController::MoveCameraX);
	InputComponent->BindAxis(TEXT("OLC_CameraMoveY"), this, &AOLCGameplayPlayerController::MoveCameraY);
}

void AOLCGameplayPlayerController::OnRotateBuild()
{
	if (!bIsInConstructionMode) return;
	if (UUserWidget* W = ActiveHUDWidget.Get())
	{
		if (UOLCConstructionOverlayWidget* Construction = Cast<UOLCConstructionOverlayWidget>(W))
		{
			Construction->RotateBuild();
		}
	}
	UE_LOG(LogOLC, Log, TEXT("[OLC] Build rotation requested"));
}

void AOLCGameplayPlayerController::OnCancelBuild()
{
	if (KeymapWidget)
	{
		KeymapWidget->RemoveFromParent();
		KeymapWidget = nullptr;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Keymap closed"));
		return;
	}

	if (bIsInConstructionMode)
	{
		bIsInConstructionMode = false;

		if (UUserWidget* W = ActiveHUDWidget.Get())
		{
			if (UOLCConstructionOverlayWidget* Construction = Cast<UOLCConstructionOverlayWidget>(W))
			{
				Construction->CancelBuild();
			}
			W->RemoveFromParent();
		}

		ActiveHUDWidget = nullptr;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Construction mode cancelled"));
	}
}

void AOLCGameplayPlayerController::OnToggleConstructionMode()
{
	if (bIsInConstructionMode)
	{
		OnCancelBuild();
		return;
	}

	if (ActiveHUDWidget)
	{
		ActiveHUDWidget->RemoveFromParent();
		ActiveHUDWidget = nullptr;
	}

	bIsInConstructionMode = true;
	if (UOLCConstructionOverlayWidget* Construction = CreateWidget<UOLCConstructionOverlayWidget>(this, UOLCConstructionOverlayWidget::StaticClass()))
	{
		Construction->AddToViewport(100);
		ActiveHUDWidget = Construction;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Construction mode opened"));
	}
}

void AOLCGameplayPlayerController::OnToggleShipBuilder()
{
	if (ActiveHUDWidget)
	{
		ActiveHUDWidget->RemoveFromParent();
		ActiveHUDWidget = nullptr;
		bIsInConstructionMode = false;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Ship builder closed"));
		return;
	}

	if (UOLCShipBuilderWidget* ShipBuilder = CreateWidget<UOLCShipBuilderWidget>(this, UOLCShipBuilderWidget::StaticClass()))
	{
		bIsInConstructionMode = false;
		ShipBuilder->AddToViewport(100);
		ActiveHUDWidget = ShipBuilder;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Ship builder opened"));
	}
}

void AOLCGameplayPlayerController::OnToggleSimulationSpeed()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Data->CycleSimulationSpeed();
		}
	}
}

// ---------------------------------------------------------------------------
// WP-107 Step 5: F1 HUD toggle + minimap click-to-pan
// ---------------------------------------------------------------------------
void AOLCGameplayPlayerController::OnToggleKeymap()
{
	if (KeymapWidget)
	{
		KeymapWidget->RemoveFromParent();
		KeymapWidget = nullptr;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Keymap closed"));
		return;
	}

	if (UOLCKeymapWidget* Keymap = CreateWidget<UOLCKeymapWidget>(this, UOLCKeymapWidget::StaticClass()))
	{
		const FVector2D KeymapSize(820.0f, 610.0f);
		int32 ViewportX = 1280;
		int32 ViewportY = 720;
		GetViewportSize(ViewportX, ViewportY);
		if (ViewportX <= 0 || ViewportY <= 0)
		{
			ViewportX = 1280;
			ViewportY = 720;
		}

		const FVector2D KeymapPosition(
			FMath::Max(0.0f, (static_cast<float>(ViewportX) - KeymapSize.X) * 0.5f),
			FMath::Max(0.0f, (static_cast<float>(ViewportY) - KeymapSize.Y) * 0.5f));

		Keymap->SetAnchorsInViewport(FAnchors(0.0f, 0.0f));
		Keymap->SetAlignmentInViewport(FVector2D::ZeroVector);
		Keymap->SetPositionInViewport(KeymapPosition, false);
		Keymap->SetDesiredSizeInViewport(KeymapSize);
		Keymap->AddToViewport(300);
		KeymapWidget = Keymap;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Keymap opened"));
	}
}

void AOLCGameplayPlayerController::OnToggleMainHUD()
{
	if (bIsMainHUDVisible)
	{
		// Close main HUD
		if (MainHUDWidget)
		{
			MainHUDWidget->RemoveFromParent();
			MainHUDWidget = nullptr;
		}
		bIsMainHUDVisible = false;
		UE_LOG(LogOLC, Log, TEXT("[OLC] Main HUD closed"));
		return;
	}

	// Open main HUD
	if (UOLCMainRTSHUDWidget* MainHUD = CreateWidget<UOLCMainRTSHUDWidget>(this, UOLCMainRTSHUDWidget::StaticClass()))
	{
		MainHUD->AddToViewport(50); // Lower z-order than construction mode (100)
		MainHUDWidget = MainHUD;
		bIsMainHUDVisible = true;

		// Wire minimap click-to-pan: when minimap is clicked, pan camera to that location
		// This is handled by the HUD widget's internal Slate click handler
		UE_LOG(LogOLC, Log, TEXT("[OLC] Main HUD opened"));
	}
}

void AOLCGameplayPlayerController::OnMinimapClick(FVector2D MinimapCoord)
{
	if (!bIsMainHUDVisible || !MainHUDWidget) return;

	// WP-107 Step 5: Pan camera to minimap click location
	// Convert minimap coordinates (0-1) to world position
	AOLCPlanetTerrainActor* Terrain = FindTerrainActor();
	if (!Terrain) return;

	FIntPoint MapDims = Terrain->GetMapDimensions();
	int32 TileX = FMath::Clamp(static_cast<int32>(MinimapCoord.X * MapDims.X), 0, MapDims.X - 1);
	int32 TileY = FMath::Clamp(static_cast<int32>(MinimapCoord.Y * MapDims.Y), 0, MapDims.Y - 1);

	FVector WorldPos = Terrain->GetTileWorldPosition(FIntPoint(TileX, TileY));

	// Pan camera to world position
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector CameraTarget = WorldPos + FVector(0.0f, 0.0f, 500.0f); // Offset above ground
		ControlledPawn->SetActorLocation(CameraTarget, false, nullptr, ETeleportType::None);
		UE_LOG(LogOLC, Log, TEXT("[OLC] Camera panned to minimap click: Tile(%d,%d) World(%.0f,%.0f,%.0f)"),
			TileX, TileY, WorldPos.X, WorldPos.Y, WorldPos.Z);
	}
}

void AOLCGameplayPlayerController::OnLeftClick()
{
	// WP-103 Step 6: In construction mode, left-click confirms building placement.
	if (bIsInConstructionMode)
	{
		if (UUserWidget* W = ActiveHUDWidget.Get())
		{
			if (UOLCConstructionOverlayWidget* Construction = Cast<UOLCConstructionOverlayWidget>(W))
			{
				Construction->ConfirmBuild();
			}
		}
		return;
	}

	// Deproject mouse screen position to world direction.
	FVector Start, Direction;
	if (!DeprojectMousePositionToWorld(Start, Direction))
	{
		return;
	}

	// Raycast forward from camera position.
	const FVector End = Start + Direction * 10000.0f;
	TArray<FHitResult> Hits;

	bool bHit = GetWorld()->LineTraceMultiByChannel(Hits, Start, End, ECC_Visibility);

	for (const FHitResult& Hit : Hits)
	{
		if (AOLCUnitBase* Unit = Cast<AOLCUnitBase>(Hit.GetActor()))
		{
			// Deselect previous unit.
			if (SelectedUnit && SelectedUnit != Unit)
			{
				SelectedUnit->SetSelected(false);
			}

			SelectedUnit = Unit;
			SelectedUnit->SetSelected(true);

			UE_LOG(LogOLC, Log, TEXT("[OLC] Selected unit '%s'"),
				Unit->GetUnitData() ? *Unit->GetUnitData()->DisplayName.ToString() : TEXT("Unknown"));
			return;
		}
	}

	// No unit hit — deselect current.
	if (SelectedUnit)
	{
		SelectedUnit->SetSelected(false);
		SelectedUnit = nullptr;
	}
}

void AOLCGameplayPlayerController::OnRightClickCapture()
{
	// Get mouse screen position.
	float MouseX = 0.0f;
	float MouseY = 0.0f;
	GetMousePosition(MouseX, MouseY);

	// Deproject to world space.
	FVector Start, Direction;
	if (!DeprojectMousePositionToWorld(Start, Direction))
	{
		return;
	}

	// Raycast to find ground target location.
	const FVector End = Start + Direction * 10000.0f;
	FHitResult Hit(ForceInit);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	if (bHit)
	{
		// Move selected unit to hit location.
		if (SelectedUnit && !SelectedUnit->IsDead())
		{
			if (AOLCGroundUnit* GroundUnit = Cast<AOLCGroundUnit>(SelectedUnit))
			{
				GroundUnit->MoveTo(Hit.Location);
			}
			UE_LOG(LogOLC, Log, TEXT("[OLC] Selected unit moving to %s"), *Hit.Location.ToString());
		}
	}
}

void AOLCGameplayPlayerController::OnBiome1() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(0); CurrentBiomeIndex = 0; } }
void AOLCGameplayPlayerController::OnBiome2() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(1); CurrentBiomeIndex = 1; } }
void AOLCGameplayPlayerController::OnBiome3() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(2); CurrentBiomeIndex = 2; } }
void AOLCGameplayPlayerController::OnBiome4() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(3); CurrentBiomeIndex = 3; } }
void AOLCGameplayPlayerController::OnBiome5() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(4); CurrentBiomeIndex = 4; } }
void AOLCGameplayPlayerController::OnBiome6() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(5); CurrentBiomeIndex = 5; } }
void AOLCGameplayPlayerController::OnBiome7() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(6); CurrentBiomeIndex = 6; } }
void AOLCGameplayPlayerController::OnBiome8() { if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor()) { Terrain->SetBiomeByIndex(7); CurrentBiomeIndex = 7; } }

void AOLCGameplayPlayerController::OnToggleTerrainRenderMode()
{
	if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor())
	{
		Terrain->ToggleRenderMode();
	}
}

void AOLCGameplayPlayerController::OnRegenerateTerrain()
{
	if (AOLCPlanetTerrainActor* Terrain = FindTerrainActor())
	{
		Terrain->Regenerate();
	}
}

void AOLCGameplayPlayerController::MoveCameraX(float Value)
{
	CameraMoveX = Value;
}

void AOLCGameplayPlayerController::MoveCameraY(float Value)
{
	CameraMoveY = Value;
}

AOLCPlanetTerrainActor* AOLCGameplayPlayerController::FindTerrainActor() const
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
