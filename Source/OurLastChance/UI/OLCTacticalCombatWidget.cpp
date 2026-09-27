#include "UI/OLCTacticalCombatWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCUnitBase.h"

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
			OnCombatEnded.Broadcast(true);
			return;
		}

		if (CheckDefeatCondition())
		{
			bCombatEnded = true;
			LogCombatEvent(LOCTEXT("CombatDefeat", "DEFEAT — All units lost"));
			OnCombatEnded.Broadcast(false);
			return;
		}
	}
}

void UOLCTacticalCombatWidget::NativeDestruct()
{
	Super::NativeDestruct();
	PlayerUnits.Reset();
	EnemyUnits.Reset();
	CombatLogEntries.Reset();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(CombatTickTimer);
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

		// Position unit on canvas based on world location projected to screen.
		FVector2D Pos = FVector2D::ZeroVector; // Would need camera projection in real impl
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2d(15, 15)).X, (Pos - FVector2d(15, 15)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildUnitMarker(Unit, true) ];

		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2d(15, 30)).X, (Pos - FVector2d(15, 30)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildHealthBar(Unit) ];
	}

	// Draw enemy units (red markers).
	for (AOLCUnitBase* Unit : EnemyUnits)
	{
		if (!Unit || Unit->IsDead()) continue;

		FVector2D Pos = FVector2D::ZeroVector;
		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2d(15, 15)).X, (Pos - FVector2d(15, 15)).Y, 0.0f, 0.0f)).AutoSize(true)
			[ BuildUnitMarker(Unit, false) ];

		Canvas->AddSlot()
			.Offset(FMargin((Pos - FVector2d(15, 30)).X, (Pos - FVector2d(15, 30)).Y, 0.0f, 0.0f)).AutoSize(true)
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
			.OnClicked_Lambda([i]() -> FReply {
				UE_LOG(LogTemp, Log, TEXT("[OLC] Ability %d activated (placeholder)"), i);
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
						FText::FromString(TEXT("[{0}]")),
						FText::AsNumber(i + 1)))
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

	// Keep log bounded to last 100 entries.
	if (CombatLogEntries.Num() > 100)
	{
		CombatLogEntries.RemoveAt(0, CombatLogEntries.Num() - 100);
	}
}

void UOLCTacticalCombatWidget::InitializeCombatEncounter()
{
	PlayerUnits.Reset();
	EnemyUnits.Reset();
	CombatLogEntries.Reset();
	bCombatEnded = false;
	bIsPaused = false;
	LastCombatTickTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	if (!GetWorld()) return;

	// In a real implementation, player units would be passed from the squad selection screen.
	// For prototype: spawn some placeholder units.
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
			PlayerCount++;
		}
	}

	// Spawn enemy units if none found (prototype fallback).
	if (PlayerUnits.Num() == 0)
	{
		LogCombatEvent(LOCTEXT("Log_NoUnits", "No player units available — using prototype enemies"));
	}

	// Spawn enemies based on difficulty. For prototype, add some dummy enemies.
	int32 EnemyCount = FMath::RandRange(3, 6); // 3-6 enemies for prototype.
	LogCombatEvent(FText::Format(
		FText::FromString(TEXT("Encounter: {0} enemy units detected")),
		FText::AsNumber(EnemyCount)));

	// Note: In full implementation, enemies would be spawned from dungeon config.
	// This is a placeholder — real spawning happens in the dungeon game mode.
}

#undef LOCTEXT_NAMESPACE


