#include "UI/OLCTacticalCombatWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Combat/OLCCombatLogSubsystem.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCDungeonGenerator.h"
#include "World/OLCUnitBase.h"
#include "Core/OLCBossRaceData.h"
#include "Core/OLCRaceSubsystem.h"

#define LOCTEXT_NAMESPACE "OLCTacticalCombatWidget"

UOLCTacticalCombatWidget::UOLCTacticalCombatWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCTacticalCombatWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ArenaSize = MyGeometry.GetLocalSize();

	if (bCombatEnded || !GetWorld()) return;

	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Process combat AI ticks at fixed interval.
	if (!bIsPaused && (CurrentTime - LastCombatTickTime) >= CombatTickInterval)
	{
		LastCombatTickTime = CurrentTime;

		// Check boss phase transitions before AI processing.
		CheckBossPhaseTransition();

		// Enemy AI: each enemy attacks nearest player unit within range.
		for (AOLCUnitBase* Enemy : EnemyUnits)
		{
			if (!Enemy || Enemy->IsDead()) continue;
			float BestDist = Enemy->UnitData ? Enemy->UnitData->Range : 20.0f;
			AOLCUnitBase* Target = nullptr;

			for (AOLCUnitBase* PlayerUnit : PlayerUnits)
			{
				if (!PlayerUnit || PlayerUnit->IsDead()) continue;
				float Dist = FVector::Dist(Enemy->GetActorLocation(), PlayerUnit->GetActorLocation());
				if (Dist <= BestDist)
				{
					Target = PlayerUnit;
					BestDist = Dist;
				}
			}

			if (Target)
			{
				if (Enemy == BossUnit && Enemy->UnitData && BossBaseDamage > 0.0f)
				{
					// Phase-scaled outgoing damage (WP-118 DamageMultiplier), reapplied to the base value each hit.
					Enemy->UnitData->AttackDamage = BossBaseDamage * CurrentBossDamageMultiplier;
				}
				Enemy->Attack(Target);
				LogCombatEvent(FText::Format(
					FText::FromString(TEXT("{0} attacks {1} for {2} damage")),
					Enemy->UnitData ? Enemy->UnitData->DisplayName : FText::GetEmpty(),
					Target->UnitData ? Target->UnitData->DisplayName : FText::GetEmpty(),
					FText::AsNumber(FMath::RoundToInt(Enemy->UnitData ? Enemy->UnitData->AttackDamage : 0))));
			}
		}

		// Check win/lose conditions.
		if (CheckVictoryCondition())
		{
			bCombatEnded = true;
			LogCombatEvent(LOCTEXT("CombatVictory", "VICTORY!"));
			if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(BossEnrageTimer);
			if (bHasExpedition)
			{
				const FDungeonCompletionResult Result = BuildCompletionResult(true);
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UOLCDungeonStateSubsystem* State = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
					{
						State->SetLastResult(Result);
						const bool bBlueprintGranted = Result.BlueprintGrants.Num() > 0;
						State->RecordClear(ExpeditionDungeon, true, bBlueprintGranted);
					}
				}
			}
			OnCombatEnded.Broadcast(true);
			return;
		}

		if (CheckDefeatCondition())
		{
			bCombatEnded = true;
			LogCombatEvent(LOCTEXT("CombatDefeat", "DEFEAT — All units lost"));
			if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(BossEnrageTimer);
			if (bHasExpedition)
			{
				const FDungeonCompletionResult Result = BuildCompletionResult(false);
				if (UGameInstance* GI = GetGameInstance())
				{
					if (UOLCDungeonStateSubsystem* State = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
					{
						State->SetLastResult(Result);
					}
				}
			}
			OnCombatEnded.Broadcast(false);
			return;
		}
	}
}

void UOLCTacticalCombatWidget::CheckBossPhaseTransition()
{
	if (!bHasActiveBoss || !BossUnit || BossUnit->IsDead() || !BossRaceData) return;

	const float MaxHP = BossUnit->GetMaxHP();
	if (MaxHP <= 0.0f) return;
	const float HPPercent = FMath::Clamp(BossUnit->GetCurrentHP() / MaxHP, 0.0f, 1.0f) * 100.0f;

	const FOLCBossPhaseData Phase = BossRaceData->GetPhaseForHPPercent(HPPercent);
	if (Phase.PhaseName.EqualTo(LastBossPhaseName)) return;

	LastBossPhaseName = Phase.PhaseName;
	CurrentBossPhase = BossRaceData->Phases.IndexOfByPredicate([&Phase](const FOLCBossPhaseData& P) { return P.HPThresholdPercent == Phase.HPThresholdPercent; }) + 1;
	CurrentBossDamageMultiplier = Phase.DamageMultiplier;

	OnBossPhaseChanged.Broadcast(CurrentBossPhase, HPPercent);

	LogCombatEvent(FText::Format(
		FText::FromString(TEXT("BOSS phase transition: {0} at {1}% HP — {2}")),
		Phase.PhaseName,
		FText::AsNumber(FMath::RoundToInt(HPPercent)),
		Phase.PhaseDescription));

	if (Phase.bSummonsAdds)
	{
		int32 CurrentAdds = 0;
		for (AOLCUnitBase* Enemy : EnemyUnits)
		{
			if (Enemy && !Enemy->IsDead() && Enemy != BossUnit) CurrentAdds++;
		}
		const int32 AddsToSpawn = FMath::Max(0, FMath::Min(4, AddCap - CurrentAdds));
		if (AddsToSpawn > 0 && GetWorld())
		{
			UOLCRaceSubsystem* Races = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCRaceSubsystem>() : nullptr;
			TArray<UOLCRaceData*> Pool = Races ? Races->GetRacesByFamily(BossRaceData->RaceFamily) : TArray<UOLCRaceData*>();
			const FVector SpawnBase = BossUnit->GetActorLocation();
			for (int32 i = 0; i < AddsToSpawn; i++)
			{
				UOLCRaceData* Race = Pool.Num() > 0 ? Pool[i % Pool.Num()] : nullptr;
				const FVector SpawnLoc = SpawnBase + FVector(FMath::FRandRange(-150.0f, 150.0f), FMath::FRandRange(-150.0f, 150.0f), 0.0f);
				if (AOLCUnitBase* Add = GetWorld()->SpawnActor<AOLCUnitBase>(AOLCUnitBase::StaticClass(), SpawnLoc, FRotator::ZeroRotator))
				{
					UOLCUnitData* AddData = NewObject<UOLCUnitData>(this);
					AddData->DisplayName = Race ? Race->DisplayName : LOCTEXT("Add_Generic", "Summoned Add");
					AddData->MaxHP = Race ? Race->BaseHP * 0.5f : 50.0f;
					AddData->AttackDamage = Race ? Race->BaseDamage * 0.5f : 5.0f;
					Add->SetUnitData(AddData);
					EnemyUnits.Add(Add);
					UnitCanvasPositions.Add(Add, UnitCanvasPositions.FindRef(BossUnit) + FVector2D(FMath::FRandRange(-20.0f, 20.0f), FMath::FRandRange(-20.0f, 20.0f)));
				}
			}
			LogCombatEvent(FText::Format(FText::FromString(TEXT("{0} summons {1} adds")), Phase.PhaseName, FText::AsNumber(AddsToSpawn)));
		}
	}

	// Final phase: an enrage timer that force-ends the encounter in the boss's favor if not defeated in time.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(BossEnrageTimer);
	}
	const bool bIsFinalPhase = BossRaceData->Phases.Num() > 0 && Phase.HPThresholdPercent <= BossRaceData->Phases.Last().HPThresholdPercent;
	if (bIsFinalPhase && GetWorld() && BossRaceData->FinalPhaseEnrageTimerSeconds > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(BossEnrageTimer, this, &UOLCTacticalCombatWidget::OnBossEnrageExpired, BossRaceData->FinalPhaseEnrageTimerSeconds, false);
	}
}

void UOLCTacticalCombatWidget::OnBossEnrageExpired()
{
	if (bCombatEnded) return;
	bCombatEnded = true;
	LogCombatEvent(LOCTEXT("Log_BossEnrage", "The boss's final-phase enrage overwhelms the squad — DEFEAT"));
	if (bHasExpedition)
	{
		const FDungeonCompletionResult Result = BuildCompletionResult(false);
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UOLCDungeonStateSubsystem* State = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
			{
				State->SetLastResult(Result);
			}
		}
	}
	OnCombatEnded.Broadcast(false);
}

void UOLCTacticalCombatWidget::NativeDestruct()
{
	Super::NativeDestruct();
	PlayerUnits.Reset();
	EnemyUnits.Reset();
	CombatLogEntries.Reset();
	UnitCanvasPositions.Reset();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CombatTickTimer);
		GetWorld()->GetTimerManager().ClearTimer(BossEnrageTimer);
	}
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::RebuildWidget()
{
	// Get player controller reference.
	if (APlayerController* PC = GetOwningPlayer())
		MenuPC = Cast<AOLCMenuPlayerController>(PC);

	// Scan world for units to populate combat encounter.
	// Player units: those that were in the squad (we'll accept them as a parameter via Blueprint).
	// Enemy units: random spawn based on dungeon difficulty.
	InitializeCombatEncounter();

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.005f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(0.0f))
		[
			SNew(SOverlay)
			// Top bar
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[ BuildTopBar() ]
			// Combat arena (center canvas)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 70.0f, 280.0f, 130.0f)
			[ BuildCombatArena() ]
			// Right panel: combat log
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(280.0f, 70.0f, 30.0f, 130.0f)
			[ BuildCombatLog() ]
			// Bottom bar: ability hotbar + pause button
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 30.0f, 0.0f, 10.0f)
			[ BuildAbilityHotbar() ]
		];
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildTopBar()
{
	FText PauseLabel = bIsPaused ? LOCTEXT("BtnResume", "RESUME") : LOCTEXT("BtnPause", "PAUSE");

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 10.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(LOCTEXT("CombatTitle", "TACTICAL COMBAT"))
				.ColorAndOpacity(OLCStyleColors::DangerRed)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("ALIVE: {0}P / {1}E")),
				FText::AsNumber(PlayerUnits.Num()),
				FText::AsNumber(EnemyUnits.Num())))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]() -> FReply {
					TogglePause();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(bIsPaused ? OLCStyleColors::ValidGreen : OLCStyleColors::WarningYellow)
					.Padding(FMargin(14.0f, 8.0f))
					[ SNew(STextBlock).Text(PauseLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				]
			]
		];
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildCombatArena()
{
	TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas);

	// Background — dark grid pattern.
	Canvas->AddSlot()
		.Offset(FMargin((FVector2D::ZeroVector).X, (FVector2D::ZeroVector).Y, 0.0f, 0.0f)).AutoSize(true)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.01f, 0.015f, 0.02f, 1.0f))
			.Padding(FMargin(0.0f))
		];

	// Draw player units (blue markers).
	for (AOLCUnitBase* Unit : PlayerUnits)
	{
		if (!Unit || Unit->IsDead()) continue;

		const FVector2D Pos = UnitCanvasPositions.FindRef(Unit);
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2D(15, 15)).X, (Pos - FVector2D(15, 15)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildUnitMarker(Unit, true) ];

		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2D(15, 30)).X, (Pos - FVector2D(15, 30)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildHealthBar(Unit) ];
	}

	// Draw enemy units (red markers).
	for (AOLCUnitBase* Unit : EnemyUnits)
	{
		if (!Unit || Unit->IsDead()) continue;

		const FVector2D Pos = UnitCanvasPositions.FindRef(Unit);
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2D(15, 15)).X, (Pos - FVector2D(15, 15)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildUnitMarker(Unit, false) ];

		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2D(15, 30)).X, (Pos - FVector2D(15, 30)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildHealthBar(Unit) ];
	}

	return Canvas;
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildUnitMarker(AOLCUnitBase* Unit, bool bPlayerUnit)
{
	if (!Unit) return SNew(SBorder);

	FLinearColor MarkerColor = bPlayerUnit ? OLCStyleColors::TacticalBlue : OLCStyleColors::DangerRed;

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		[
			SNew(SBorder)
			.BorderBackgroundColor(MarkerColor)
			.Padding(FMargin(0.0f))
			[ SNew(SBox).WidthOverride(30.0f).HeightOverride(30.0f) ]
		];
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildHealthBar(AOLCUnitBase* Unit)
{
	if (!Unit || !Unit->UnitData) return SNew(SBorder);

	float HPPercent = FMath::Clamp(Unit->UnitData->MaxHP > 0.0f
		? (Unit->GetCurrentHP() / Unit->UnitData->MaxHP)
		: 1.0f, 0.0f, 1.0f);

	FLinearColor HPColor;
	if (HPPercent > 0.6f)      HPColor = OLCStyleColors::ValidGreen;
	else if (HPPercent > 0.3f) HPColor = OLCStyleColors::WarningYellow;
	else                       HPColor = OLCStyleColors::DangerRed;

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.8f))
		.Padding(FMargin(1.0f))
		[
			SNew(SBox).WidthOverride(50.0f).HeightOverride(6.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(HPColor)
				.Padding(FMargin(0.0f))
				[ SNew(SBox).WidthOverride(50.0f * HPPercent).HeightOverride(6.0f) ]
			]
		];
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildAbilityHotbar()
{
	TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);

	// Define abilities for hotbar.
	TArray<FText> AbilityLabels = {
		LOCTEXT("Ab_Attack", "ATTACK"),
		LOCTEXT("Ab_Defend", "DEFEND"),
		LOCTEXT("Ab_Focus", "FOCUS"),
		LOCTEXT("Ab_Retreat", "RETREAT"),
		LOCTEXT("Ab_Special", "SPECIAL"),
	};

	for (int32 i = 0; i < AbilityLabels.Num(); i++)
	{
		HBox->AddSlot().AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.OnClicked_Lambda([this, i]() -> FReply {
				if (PlayerUnits.Num() > 0 && PlayerUnits[0])
				{
					AActor* Target = EnemyUnits.Num() > 0 ? EnemyUnits[0] : nullptr;
					PlayerUnits[0]->ActivateAbility(i, Target);
				}
				return FReply::Handled();
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
				.Padding(FMargin(12.0f, 8.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(AbilityLabels[i])
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(FText::Format(
						FText::FromString(TEXT("[{0}] {1}s")),
						FText::AsNumber(i + 1),
						FText::AsNumber(PlayerUnits.Num() > 0 && PlayerUnits[0] ? FMath::RoundToInt(PlayerUnits[0]->GetAbilityCooldownRemaining(i)) : 0)))
						.ColorAndOpacity(OLCStyleColors::TacticalBlue)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
				]
			]
		];
	}

	// Add a small spacer then the pause button.
	HBox->AddSlot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnClicked_Lambda([this]() -> FReply {
			TogglePause();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(bIsPaused ? OLCStyleColors::ValidGreen : OLCStyleColors::WarningYellow)
			.Padding(FMargin(16.0f, 8.0f))
			[ SNew(STextBlock).Text(bIsPaused ? LOCTEXT("BtnResume", "RESUME") : LOCTEXT("BtnPause", "PAUSE"))
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
		]
	];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f))
		[ HBox ];
}

TSharedRef<SWidget> UOLCTacticalCombatWidget::BuildCombatLog()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	for (int32 i = CombatLogEntries.Num() - 1; i >= FMath::Max(0, CombatLogEntries.Num() - 20); i--)
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 2.0f)
		[ SNew(STextBlock).Text(CombatLogEntries[i])
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f))
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[ VBox ]
		];
}

void UOLCTacticalCombatWidget::TogglePause()
{
	bIsPaused = !bIsPaused;
	LogCombatEvent(bIsPaused ? LOCTEXT("Log_Paused", "COMBAT PAUSED") : LOCTEXT("Log_Resume", "COMBAT RESUMED"));
	InvalidateLayoutAndVolatility();
}

bool UOLCTacticalCombatWidget::CheckVictoryCondition()
{
	for (AOLCUnitBase* Enemy : EnemyUnits)
	{
		if (Enemy && !Enemy->IsDead()) return false;
	}
	return true; // All enemies dead.
}

bool UOLCTacticalCombatWidget::CheckDefeatCondition()
{
	for (AOLCUnitBase* PlayerUnit : PlayerUnits)
	{
		if (PlayerUnit && !PlayerUnit->IsDead()) return false;
	}
	return true; // All player units dead.
}

void UOLCTacticalCombatWidget::LogCombatEvent(const FText& Message)
{
	CombatLogEntries.Add(Message);
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UOLCCombatLogSubsystem* Log = GI->GetSubsystem<UOLCCombatLogSubsystem>())
		{
			Log->AddCombatLogEntry(Message, EOLCCombatLogType::System);
		}
	}

	// Keep log bounded to last 100 entries.
	if (CombatLogEntries.Num() > 100)
	{
		CombatLogEntries.RemoveAt(0, CombatLogEntries.Num() - 100);
	}
}

void UOLCTacticalCombatWidget::InitializeFromExpedition(const UOLCDungeonData* Dungeon, const FOLCDungeonGenerationResult& Layout, const FOLCSquadDeploymentData& Squad)
{
	ExpeditionDungeon = const_cast<UOLCDungeonData*>(Dungeon);
	ExpeditionLayout = Layout;
	ExpeditionSquad = Squad;
	bHasExpedition = Dungeon != nullptr && Layout.bValid;
}

FVector2D UOLCTacticalCombatWidget::ProjectRoomToCanvas(const FOLCDungeonRoom& Room) const
{
	if (ExpeditionLayout.Rooms.Num() == 0 || ArenaSize.X <= 0.0f || ArenaSize.Y <= 0.0f)
	{
		return ArenaSize * 0.5f;
	}

	float MinX = TNumericLimits<float>::Max(), MinY = TNumericLimits<float>::Max();
	float MaxX = TNumericLimits<float>::Lowest(), MaxY = TNumericLimits<float>::Lowest();
	for (const FOLCDungeonRoom& R : ExpeditionLayout.Rooms)
	{
		MinX = FMath::Min(MinX, static_cast<float>(R.GridX));
		MinY = FMath::Min(MinY, static_cast<float>(R.GridY));
		MaxX = FMath::Max(MaxX, static_cast<float>(R.GridX + R.WidthTiles));
		MaxY = FMath::Max(MaxY, static_cast<float>(R.GridY + R.HeightTiles));
	}
	const float SpanX = FMath::Max(1.0f, MaxX - MinX);
	const float SpanY = FMath::Max(1.0f, MaxY - MinY);

	const FVector2D Center = Room.GetCenterTile();
	const float NormX = (Center.X - MinX) / SpanX;
	const float NormY = (Center.Y - MinY) / SpanY;

	constexpr float Margin = 50.0f;
	return FVector2D(
		Margin + NormX * FMath::Max(1.0f, ArenaSize.X - 2.0f * Margin),
		Margin + NormY * FMath::Max(1.0f, ArenaSize.Y - 2.0f * Margin));
}

void UOLCTacticalCombatWidget::InitializeCombatEncounter()
{
	PlayerUnits.Reset();
	EnemyUnits.Reset();
	CombatLogEntries.Reset();
	UnitCanvasPositions.Reset();
	BossUnit = nullptr;
	BossRaceData = nullptr;
	bHasActiveBoss = false;
	CurrentBossPhase = 1;
	CurrentBossDamageMultiplier = 1.0f;
	BossBaseDamage = 0.0f;
	LastBossPhaseName = FText::GetEmpty();
	bCombatEnded = false;
	bIsPaused = false;
	LastCombatTickTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	if (!GetWorld()) return;

	if (bHasExpedition && ExpeditionDungeon)
	{
		// Player units: live actor refs from the squad confirmed on S09 (same menu world, no map transition).
		for (AOLCUnitBase* Unit : ExpeditionSquad.SquadUnits)
		{
			if (!Unit || Unit->IsDead()) continue;
			PlayerUnits.Add(Unit);
			const FOLCDungeonRoom* EntranceRoom = ExpeditionLayout.FindRoom(ExpeditionLayout.EntranceRoomId);
			const FVector2D BasePos = EntranceRoom ? ProjectRoomToCanvas(*EntranceRoom) : ArenaSize * 0.5f;
			UnitCanvasPositions.Add(Unit, BasePos + FVector2D(FMath::FRandRange(-25.0f, 25.0f), FMath::FRandRange(-25.0f, 25.0f)));
		}
		if (PlayerUnits.Num() == 0)
		{
			LogCombatEvent(LOCTEXT("Log_NoUnits", "No player units available — using prototype fallback"));
		}

		// Enemies: composition rolled per-clear (run seed), not the stable layout seed.
		UOLCRaceSubsystem* Races = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCRaceSubsystem>() : nullptr;
		int32 ClearCount = 0;
		if (UOLCDungeonStateSubsystem* State = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCDungeonStateSubsystem>() : nullptr)
		{
			if (const FOLCDungeonInstanceState* Instance = State->GetInstanceState(ExpeditionDungeon->DungeonType))
			{
				ClearCount = Instance->ClearCount;
			}
		}
		const int32 RunSeed = UOLCDungeonGenerator::ComputeRunSeed(ExpeditionLayout.Seed, ClearCount);
		FRandomStream RunRng(RunSeed);
		UOLCDungeonGenerator::PopulateEnemies(ExpeditionLayout, ExpeditionDungeon, RunRng, Races);

		// World spawn locations cluster near the squad's actual location (an existing menu-world
		// position we don't otherwise control) so the enemy AI's range check below can find
		// targets; the room layout instead drives the 2D canvas position used for display.
		//
		// The per-unit scatter offset below (FMath::FRandRange) is intentionally NOT drawn from
		// the seeded run Rng: it only jitters cosmetic 3D placement within the cluster and has no
		// effect on determinism-sensitive outcomes (enemy composition/count/loot all already come
		// from RunRng above), so it doesn't need to be reproducible.
		const FVector WorldOrigin = (PlayerUnits.Num() > 0 && PlayerUnits[0]) ? PlayerUnits[0]->GetActorLocation() : FVector::ZeroVector;
		constexpr float EnemyEngageRange = 1500.0f;

		for (const FOLCDungeonRoom& Room : ExpeditionLayout.Rooms)
		{
			if (Room.Type != EOLCDungeonRoomType::Combat || Room.EnemyCount <= 0) continue;
			const FVector2D RoomPos = ProjectRoomToCanvas(Room);

			for (int32 i = 0; i < Room.EnemyCount; i++)
			{
				const FVector SpawnLoc = WorldOrigin + FVector(FMath::FRandRange(-400.0f, 400.0f), FMath::FRandRange(-400.0f, 400.0f), 0.0f);
				AOLCUnitBase* Enemy = GetWorld()->SpawnActor<AOLCUnitBase>(AOLCUnitBase::StaticClass(), SpawnLoc, FRotator::ZeroRotator);
				if (!Enemy) continue;

				UOLCUnitData* EnemyData = NewObject<UOLCUnitData>(this);
				UOLCRaceData* Race = (Races && Room.EnemyRaceIds.IsValidIndex(i)) ? Races->FindRaceById(Room.EnemyRaceIds[i]) : nullptr;
				EnemyData->DisplayName = Race ? Race->DisplayName : Room.EnemyGroupLabel;
				EnemyData->MaxHP = Race ? Race->BaseHP : 100.0f;
				EnemyData->AttackDamage = Race ? Race->BaseDamage : 10.0f;
				EnemyData->Range = EnemyEngageRange;
				Enemy->SetUnitData(EnemyData);

				EnemyUnits.Add(Enemy);
				UnitCanvasPositions.Add(Enemy, RoomPos + FVector2D(FMath::FRandRange(-20.0f, 20.0f), FMath::FRandRange(-20.0f, 20.0f)));
			}
		}

		// Boss: resolved from WP-118 race data; occupies the deepest room.
		if (ExpeditionLayout.BossRoomId != INDEX_NONE)
		{
			const FOLCDungeonBossEncounter Encounter = UOLCDungeonGenerator::ResolveBossEncounter(ExpeditionDungeon, ExpeditionLayout, Races);
			UOLCBossRaceData* Boss = Races ? Cast<UOLCBossRaceData>(Races->FindRaceById(Encounter.BossRaceId)) : nullptr;
			const FOLCDungeonRoom* BossRoom = ExpeditionLayout.FindRoom(ExpeditionLayout.BossRoomId);

			if (Boss && BossRoom)
			{
				const FVector2D RoomPos = ProjectRoomToCanvas(*BossRoom);
				const FVector SpawnLoc = WorldOrigin + FVector(FMath::FRandRange(-400.0f, 400.0f), FMath::FRandRange(-400.0f, 400.0f), 0.0f);
				if (AOLCUnitBase* Spawned = GetWorld()->SpawnActor<AOLCUnitBase>(AOLCUnitBase::StaticClass(), SpawnLoc, FRotator::ZeroRotator))
				{
					UOLCUnitData* BossData = NewObject<UOLCUnitData>(this);
					BossData->DisplayName = Boss->DisplayName;
					BossData->MaxHP = Boss->BaseHP;
					BossData->AttackDamage = Boss->BaseDamage;
					BossData->Range = EnemyEngageRange;
					Spawned->SetUnitData(BossData);

					BossUnit = Spawned;
					BossRaceData = Boss;
					BossBaseDamage = Boss->BaseDamage;
					bHasActiveBoss = true;
					EnemyUnits.Add(Spawned);
					UnitCanvasPositions.Add(Spawned, RoomPos);

					LogCombatEvent(FText::Format(FText::FromString(TEXT("BOSS ENCOUNTER: {0}")), Boss->DisplayName));
				}
			}
		}

		LogCombatEvent(FText::Format(
			FText::FromString(TEXT("Encounter: {0} enemy units detected")),
			FText::AsNumber(EnemyUnits.Num())));
		return;
	}

	// Prototype fallback (no expedition data — e.g. AOLCCombatTriggerActor's standalone flow).
	TArray<AActor*> FoundUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCUnitBase::StaticClass(), FoundUnits);

	int32 PlayerCount = 0;
	for (AActor* Actor : FoundUnits)
	{
		AOLCUnitBase* Unit = Cast<AOLCUnitBase>(Actor);
		if (!Unit || Unit->IsDead()) continue;
		if (PlayerCount < 4) // Take up to 4 units for prototype.
		{
			PlayerUnits.Add(Unit);
			UnitCanvasPositions.Add(Unit, ArenaSize * 0.5f + FVector2D(FMath::FRandRange(-25.0f, 25.0f), FMath::FRandRange(-25.0f, 25.0f)));
			PlayerCount++;
		}
	}

	if (PlayerUnits.Num() == 0)
	{
		LogCombatEvent(LOCTEXT("Log_NoUnits", "No player units available — using prototype enemies"));
	}

	const int32 EnemyCount = FMath::RandRange(3, 6); // 3-6 enemies for prototype.
	LogCombatEvent(FText::Format(
		FText::FromString(TEXT("Encounter: {0} enemy units detected")),
		FText::AsNumber(EnemyCount)));

	// Note: without expedition data, enemies remain a placeholder — real spawning is the
	// WP-130 expedition path above; this branch only exists for the pre-WP-130 trigger-actor flow.
}

FDungeonCompletionResult UOLCTacticalCombatWidget::BuildCompletionResult(bool bVictory)
{
	FDungeonCompletionResult Result;
	Result.bVictory = bVictory;
	if (!ExpeditionDungeon) return Result;

	TSet<AOLCUnitBase*> SurvivingSet;
	for (AOLCUnitBase* Unit : PlayerUnits)
	{
		if (Unit && !Unit->IsDead()) SurvivingSet.Add(Unit);
	}

	for (AOLCUnitBase* Unit : ExpeditionSquad.SquadUnits)
	{
		if (!Unit) continue;
		const FText Name = Unit->GetUnitData() ? Unit->GetUnitData()->DisplayName : FText::FromString(TEXT("Unit"));

		if (SurvivingSet.Contains(Unit))
		{
			const bool bIsChampion = ExpeditionSquad.SquadUnits.IndexOfByKey(Unit) == ExpeditionSquad.ChampionSlotIndex;
			float XP = FMath::RandRange(25.0f, 75.0f) * (1.0f + static_cast<int32>(ExpeditionDungeon->Difficulty) * 0.25f);
			if (bIsChampion) XP *= 1.5f;
			Result.UnitXPGains.Add(Name.ToString(), XP);
		}
		else
		{
			Result.Casualties.Add(Name);
		}
	}

	if (!bVictory) return Result; // No loot on defeat.

	int32 ClearCount = 0;
	bool bBlueprintClaimed = false;
	if (UOLCDungeonStateSubsystem* State = GetGameInstance() ? GetGameInstance()->GetSubsystem<UOLCDungeonStateSubsystem>() : nullptr)
	{
		if (const FOLCDungeonInstanceState* Instance = State->GetInstanceState(ExpeditionDungeon->DungeonType))
		{
			ClearCount = Instance->ClearCount;
			bBlueprintClaimed = Instance->bBlueprintClaimed;
		}
	}

	// Loot uses the same per-clear run seed as enemy composition, offset so the two draw
	// sequences are independent (both otherwise consuming from RunSeed's stream identically).
	const int32 RunSeed = UOLCDungeonGenerator::ComputeRunSeed(ExpeditionLayout.Seed, ClearCount);
	FRandomStream LootRng(RunSeed ^ 0x5bd1e995);

	const TArray<FOLCDungeonLootRoll> Rolls = UOLCDungeonGenerator::RollDungeonLoot(ExpeditionDungeon, ExpeditionLayout, LootRng, bBlueprintClaimed, this);
	Result.LootRolls = Rolls;

	for (const FOLCDungeonLootRoll& Roll : Rolls)
	{
		if (!Roll.bDropped) continue;

		if (!Roll.EquipmentTemplate.IsValid())
		{
			float& Existing = Result.ResourcesGained.FindOrAdd(Roll.Reward.ResourceType);
			Existing += Roll.Quantity;
		}
		if (Roll.Reward.bIsBlueprint)
		{
			Result.BlueprintGrants.Add(Roll.Reward.BlueprintName);
			Result.ResearchUnlocks.Add(FText::Format(LOCTEXT("Research_Blueprint", "Blueprint Analysis: {0}"), Roll.Reward.BlueprintName));
		}
	}

	return Result;
}

#undef LOCTEXT_NAMESPACE


