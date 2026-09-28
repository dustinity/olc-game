#include "UI/OLCDungeonEntryWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include "Core/OLCDungeonGenerationData.h" // UOLCDungeonStateSubsystem
#include "Player/OLCMenuPlayerController.h"
#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCDungeonEntryWidget"

// ---------------------------------------------------------------------------
// Screen-specific asset paths (WP-13 Step 4)
// ---------------------------------------------------------------------------
namespace DungeonAssetPath
{
	FString TacticalIcon(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../../Assets/UI/Tactical Combat Dungeon View/Assets") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

UOLCDungeonEntryWidget::UOLCDungeonEntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCDungeonEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	SampleDungeons.Reset();
	SelectedIndex = -1;
}

TSharedRef<SWidget> UOLCDungeonEntryWidget::RebuildWidget()
{
	InitializeDungeons();

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
			// Left panel: dungeon list
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 70.0f, 350.0f, 80.0f)
			[ BuildDungeonList() ]
			// Right panel: reward preview
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(350.0f, 70.0f, 30.0f, 80.0f)
			[ BuildRewardPreview() ]
		];
}

TSharedRef<SWidget> UOLCDungeonEntryWidget::BuildTopBar()
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 10.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(LOCTEXT("DungeonTitle", "DUNGEON ENTRY"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]() -> FReply {
					this->RemoveFromParent();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::PrimaryOrange)
					.Padding(FMargin(14.0f, 8.0f))
					[ SNew(STextBlock).Text(LOCTEXT("BtnBack", "← BACK"))
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				]
			]
		];
}

TSharedRef<SWidget> UOLCDungeonEntryWidget::BuildDungeonList()
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f))
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)
				// Header row
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 8.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[ SNew(STextBlock).Text(LOCTEXT("ColZone", "ZONE"))
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SHorizontalBox::Slot().AutoWidth().Padding(12.0f, 0.0f, 12.0f, 0.0f)
					[ SNew(STextBlock).Text(LOCTEXT("ColDifficulty", "DIFFICULTY"))
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SHorizontalBox::Slot().AutoWidth()
					[ SNew(STextBlock).Text(LOCTEXT("ColRooms", "ROOMS"))
						.ColorAndOpacity(OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
				]
			]
		];

	// Build individual entries — we inline them in a separate approach.
	// Rebuild with actual entries.
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	// Header row
	VBox->AddSlot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 8.0f)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(1.0f)
		[ SNew(STextBlock).Text(LOCTEXT("ColZone", "ZONE"))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
		+ SHorizontalBox::Slot().AutoWidth().Padding(12.0f, 0.0f, 12.0f, 0.0f)
		[ SNew(STextBlock).Text(LOCTEXT("ColDifficulty", "DIFFICULTY"))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
		+ SHorizontalBox::Slot().AutoWidth()
		[ SNew(STextBlock).Text(LOCTEXT("ColRooms", "ROOMS"))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
	];

	for (int32 i = 0; i < SampleDungeons.Num(); i++)
	{
		const auto* Dungeon = SampleDungeons[i].Get();
		if (!Dungeon) continue;

		VBox->AddSlot().AutoHeight()
		[ BuildDungeonEntry(Dungeon, SelectedIndex == i) ];
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

TSharedRef<SWidget> UOLCDungeonEntryWidget::BuildDungeonEntry(const UOLCDungeonData* Dungeon, bool bSelected)
{
	if (!Dungeon) return SNew(SBorder);

	FLinearColor RowColor = bSelected
		? FLinearColor(0.2f, 0.12f, 0.05f, 0.6f) // Orange tint for selected
		: FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Transparent

	FText DifficultyLabel;
	EOLCColorRole DiffColor;
	FLinearColor DifficultyColor = OLCStyleColors::TextDim;
	switch (Dungeon->Difficulty)
	{
		case EOLCDungeonDifficulty::Easy:       DifficultyLabel = LOCTEXT("Diff_Easy", "EASY");     DiffColor = EOLCColorRole::Success; DifficultyColor = OLCStyleColors::ValidGreen; break;
		case EOLCDungeonDifficulty::Moderate:   DifficultyLabel = LOCTEXT("Diff_Moderate", "MODERATE"); DiffColor = EOLCColorRole::Warning; DifficultyColor = OLCStyleColors::WarningYellow; break;
		case EOLCDungeonDifficulty::Hard:       DifficultyLabel = LOCTEXT("Diff_Hard", "HARD");     DiffColor = EOLCColorRole::Danger; DifficultyColor = OLCStyleColors::DangerRed; break;
		case EOLCDungeonDifficulty::VeryHard:   DifficultyLabel = LOCTEXT("Diff_VeryHard", "VERY HARD"); DiffColor = EOLCColorRole::Danger; DifficultyColor = OLCStyleColors::DangerRed; break;
		case EOLCDungeonDifficulty::Extreme:    DifficultyLabel = LOCTEXT("Diff_Extreme", "EXTREME");  DiffColor = EOLCColorRole::Danger; DifficultyColor = OLCStyleColors::DangerRed; break;
		default:                                DifficultyLabel = FText::GetEmpty();               DiffColor = EOLCColorRole::Default; DifficultyColor = OLCStyleColors::TextDim; break;
	}

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Dungeon]() -> FReply {
			SelectedIndex = -1;
			for (int32 i = 0; i < SampleDungeons.Num(); i++)
			{
				if (SampleDungeons[i].Get() == Dungeon)
				{
					SelectedIndex = i;
					break;
				}
			}
			InvalidateLayoutAndVolatility();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(RowColor)
			.Padding(FMargin(8.0f, 6.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().FillWidth(1.0f)
				[ SNew(STextBlock).Text(Dungeon->DisplayName)
					.ColorAndOpacity(bSelected ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ]
				+ SHorizontalBox::Slot().AutoWidth().Padding(12.0f, 0.0f, 12.0f, 0.0f)
				[
					SNew(SBorder)
					.BorderBackgroundColor(DifficultyColor)
					.Padding(FMargin(6.0f, 2.0f))
					[ SNew(STextBlock).Text(DifficultyLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
				]
				+ SHorizontalBox::Slot().AutoWidth()
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("{0}-{1}")),
					FText::AsNumber(Dungeon->RoomCountMin),
					FText::AsNumber(Dungeon->RoomCountMax)))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ]
			]
		];
}

TSharedRef<SWidget> UOLCDungeonEntryWidget::BuildRewardPreview()
{
	if (SelectedIndex < 0 || SelectedIndex >= SampleDungeons.Num())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("PreviewSelect", "SELECT A ZONE"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	const auto* Dungeon = SampleDungeons[SelectedIndex].Get();
	if (!Dungeon) return SNew(SBorder);

	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	// Dungeon name
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(Dungeon->DisplayName)
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20)) ];

	// Size badge
	FText SizeLabel;
	switch (Dungeon->DungeonSize)
	{
		case EOLCDungeonSize::Tiny:   SizeLabel = LOCTEXT("Size_Tiny", "TINY"); break;
		case EOLCDungeonSize::Small:  SizeLabel = LOCTEXT("Size_Small", "SMALL"); break;
		case EOLCDungeonSize::Medium: SizeLabel = LOCTEXT("Size_Medium", "MEDIUM"); break;
		case EOLCDungeonSize::Large:  SizeLabel = LOCTEXT("Size_Large", "LARGE"); break;
		case EOLCDungeonSize::Fortress: SizeLabel = LOCTEXT("Size_Fortress", "FORTRESS"); break;
		default: SizeLabel = FText::GetEmpty(); break;
	}

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("{0} — {1} min clear")),
		SizeLabel,
		FText::AsNumber(FMath::RoundToInt(Dungeon->EstimatedClearMinutes))))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];

	// Layout seed + boss phase indicator (WP-130).
	VBox->AddSlot().AutoHeight().Padding(0.0f, 2.0f, 0.0f, 0.0f)
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("Seed {0}{1}")),
		GetLayoutSeedText(Dungeon->LayoutSeed),
		Dungeon->bHasBoss ? FText::Format(FText::FromString(TEXT("  •  {0}")), GetPhaseIndicator(Dungeon)) : FText::GetEmpty()))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9)) ];

	// Respawn status (WP-130: enemies 48h, materials 72h, bosses 168h, blueprints once).
	{
		FText StatusText;
		FLinearColor StatusColor = OLCStyleColors::TextDim;
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UOLCDungeonStateSubsystem* State = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
			{
				const double Now = FDateTime::UtcNow().ToUnixTimestamp();
				if (Dungeon->bHasBoss && !State->IsBossAvailable(Dungeon->DungeonType, Now))
				{
					StatusText = LOCTEXT("Status_BossRespawn", "BOSS RESPAWNING");
					StatusColor = OLCStyleColors::WarningYellow;
				}
				else if (!State->AreEnemiesRespawned(Dungeon->DungeonType, Now))
				{
					StatusText = LOCTEXT("Status_EnemyRespawn", "ENEMIES RESPAWNING");
					StatusColor = OLCStyleColors::WarningYellow;
				}
				else if (Dungeon->DungeonSize == EOLCDungeonSize::Fortress && State->HasBlueprintClaimed(Dungeon->DungeonType))
				{
					StatusText = LOCTEXT("Status_BPClaimed", "BLUEPRINT CLAIMED");
				}
				else if (Dungeon->bHasBoss)
				{
					StatusText = LOCTEXT("Status_BossReady", "BOSS AVAILABLE");
					StatusColor = OLCStyleColors::ValidGreen;
				}
				else
				{
					StatusText = LOCTEXT("Status_Ready", "READY");
					StatusColor = OLCStyleColors::ValidGreen;
				}
			}
		}

		if (!StatusText.IsEmpty())
		{
			VBox->AddSlot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(StatusText)
				.ColorAndOpacity(StatusColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];
		}
	}

	// Enemies section
	VBox->AddSlot().AutoHeight().Padding(0.0f, 16.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(LOCTEXT("PreviewEnemies", "ENEMIES"))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

	for (const auto& Enemy : Dungeon->Enemies)
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 2.0f)
		[ SNew(STextBlock).Text(FText::Format(
			FText::FromString(TEXT("{0} × {1}-{2}")),
			Enemy.EnemyType,
			FText::AsNumber(Enemy.MinCount),
			FText::AsNumber(Enemy.MaxCount)))
			.ColorAndOpacity(OLCStyleColors::DangerRed)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];
	}

	// Boss indicator
	if (Dungeon->bHasBoss)
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
		[ SNew(SBorder).BorderBackgroundColor(OLCStyleColors::DangerRed)
			.Padding(FMargin(6.0f, 3.0f))
			[ SNew(STextBlock).Text(Dungeon->BossName)
				.ColorAndOpacity(OLCStyleColors::WarningYellow)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ] ];
	}

	// Rewards section
	VBox->AddSlot().AutoHeight().Padding(0.0f, 16.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(LOCTEXT("PreviewRewards", "REWARDS"))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

	for (const auto& Reward : Dungeon->Rewards)
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 2.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("{0} × {1}-{2} ({3:P0}% chance)")),
				Reward.RewardName,
				FText::AsNumber(Reward.MinQuantity),
				FText::AsNumber(Reward.MaxQuantity),
				Reward.DropChance * 100.0f))
				.ColorAndOpacity(GetRarityColor(Reward.RarityTier))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(6.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(GetRarityDisplayName(Reward.RarityTier))
				.ColorAndOpacity(GetRarityColor(Reward.RarityTier))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
		];
	}

	for (const auto& Blueprint : Dungeon->BlueprintRewards)
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 2.0f)
		[ SNew(STextBlock).Text(FText::Format(
			FText::FromString(TEXT("BLUEPRINT: {0}")),
			Blueprint.BlueprintName))
			.ColorAndOpacity(OLCStyleColors::WarningYellow)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11)) ];
	}

	// Deploy button
	VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnClicked_Lambda([this, Dungeon]() -> FReply {
			OnDungeonDeployed.Broadcast(Dungeon);

			if (UGameInstance* GI = GetGameInstance())
			{
				if (UOLCDungeonStateSubsystem* State = GI->GetSubsystem<UOLCDungeonStateSubsystem>())
				{
					// WP-130: generates the layout (LayoutSeed) and stores it as the pending
					// expedition for S09 (squad confirm) and S08 (tactical combat init) to consume.
					State->BeginExpedition(Dungeon);
				}
			}

			if (AOLCMenuPlayerController* PC = Cast<AOLCMenuPlayerController>(GetOwningPlayer()))
			{
				PC->OpenUIScreen(EOLCUIScreen::SquadSelection);
			}
			else
			{
				this->RemoveFromParent();
			}
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::PrimaryOrange)
			.Padding(FMargin(24.0f, 12.0f))
			[ SNew(STextBlock).Text(LOCTEXT("BtnDeploy", "DEPLOY"))
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16)) ]
		]
	];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

void UOLCDungeonEntryWidget::InitializeDungeons()
{
	SampleDungeons.Reset();

	// Archetype 1: Abandoned House — Tiny, Easy, Common loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_AbandonedHouse", "Abandoned House");
		Data->DungeonType = EOLCDungeonType::AbandonedHouse;
		Data->DungeonSize = EOLCDungeonSize::Tiny;
		Data->Difficulty = EOLCDungeonDifficulty::Easy;
		Data->EstimatedClearMinutes = 7.0f;
		Data->RoomCountMin = 1; Data->RoomCountMax = 2;
		Data->bHasBoss = false;
		Data->LayoutSeed = 20240101;
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_HullParts", "Hull Parts"), EOLCResourceType::HullParts, 30, 80, 0.95f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 50, 150, 0.80f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 20, 50, 0.60f, EOLCDungeonRarityTier::Common),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 2: Enemy Camp — Small, Moderate, Uncommon loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_EnemyCamp", "Enemy Camp");
		Data->DungeonType = EOLCDungeonType::EnemyCamp;
		Data->DungeonSize = EOLCDungeonSize::Small;
		Data->Difficulty = EOLCDungeonDifficulty::Moderate;
		Data->EstimatedClearMinutes = 30.0f;
		Data->RoomCountMin = 3; Data->RoomCountMax = 6;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_CampLeader", "Camp Leader");
		Data->BossRaceId = TEXT("HumanoidEliteCommanders"); // WP-130 A2 mapping
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Hostiles", "Hostile Units"), 2, 4, EOLCRaceFamily::Humanoid),
			FOLCDungeonEnemy(LOCTEXT("En_Bandits", "Bandits"), 1, 2, EOLCRaceFamily::Humanoid),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 20, 60, 0.85f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 30, 100, 0.70f, EOLCDungeonRarityTier::Uncommon),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 30, 100, 0.60f, EOLCDungeonRarityTier::Common),
		};
		Data->LayoutSeed = 20240102;
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(70.0f, LOCTEXT("Phase1_Threshold", "Phase 1: 70% HP")),
			FOLCDungeonBossPhase(40.0f, LOCTEXT("Phase2_Threshold", "Phase 2: 40% HP")),
			FOLCDungeonBossPhase(15.0f, LOCTEXT("Phase3_Threshold", "Phase 3: 15% HP"), true),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 3: Scavenged Vehicle — Tiny, Easy, Common loot, no boss
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_ScavengedVehicle", "Scavenged Vehicle");
		Data->DungeonType = EOLCDungeonType::ScavengedVehicle;
		Data->DungeonSize = EOLCDungeonSize::Tiny;
		Data->Difficulty = EOLCDungeonDifficulty::Easy;
		Data->EstimatedClearMinutes = 7.0f;
		Data->RoomCountMin = 1; Data->RoomCountMax = 2;
		Data->bHasBoss = false;
		Data->LayoutSeed = 20240103;
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_HullParts", "Hull Parts"), EOLCResourceType::HullParts, 30, 80, 0.95f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 50, 150, 0.80f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 20, 50, 0.60f, EOLCDungeonRarityTier::Common),
		};
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(0.0f, LOCTEXT("NoBoss", "No Boss")),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 4: Military Outpost — Medium, Hard, Uncommon loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_MilitaryOutpost", "Military Outpost");
		Data->DungeonType = EOLCDungeonType::MilitaryOutpost;
		Data->DungeonSize = EOLCDungeonSize::Medium;
		Data->Difficulty = EOLCDungeonDifficulty::Hard;
		Data->EstimatedClearMinutes = 35.0f;
		Data->RoomCountMin = 7; Data->RoomCountMax = 12;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_Commander", "Commander");
		Data->BossRaceId = TEXT("ReptilianHydraColonies"); // WP-130 A2 mapping
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Soldiers", "Soldiers"), 6, 15, EOLCRaceFamily::Reptilian),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 50, 150, 0.90f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 100, 400, 0.85f, EOLCDungeonRarityTier::Uncommon),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 30, 100, 0.60f, EOLCDungeonRarityTier::Common),
		};
		Data->LayoutSeed = 20240104;
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(70.0f, LOCTEXT("Phase1_Threshold", "Phase 1: 70% HP")),
			FOLCDungeonBossPhase(40.0f, LOCTEXT("Phase2_Threshold", "Phase 2: 40% HP")),
			FOLCDungeonBossPhase(15.0f, LOCTEXT("Phase3_Threshold", "Phase 3: 15% HP"), true),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 5: Hospital Complex — Large, Hard, Rare loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_HospitalComplex", "Hospital Complex");
		Data->DungeonType = EOLCDungeonType::HospitalComplex;
		Data->DungeonSize = EOLCDungeonSize::Large;
		Data->Difficulty = EOLCDungeonDifficulty::Hard;
		Data->EstimatedClearMinutes = 60.0f;
		Data->RoomCountMin = 20; Data->RoomCountMax = 35;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_MegaCreature", "Infected Mega-Creature");
		Data->BossRaceId = TEXT("MolluskoidVoidOctopuses"); // WP-130 A2 mapping
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Mutants", "Mutated Creatures"), 20, 40, EOLCRaceFamily::Molluskoid),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 150, 400, 0.80f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 400, 1200, 0.85f, EOLCDungeonRarityTier::Rare),
			FOLCDungeonReward(LOCTEXT("Rw_Medical", "Medical Supplies"), EOLCResourceType::Survival, 50, 150, 0.70f, EOLCDungeonRarityTier::Common),
		};
		Data->LayoutSeed = 20240105;
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(70.0f, LOCTEXT("Phase1_Threshold", "Phase 1: 70% HP")),
			FOLCDungeonBossPhase(40.0f, LOCTEXT("Phase2_Threshold", "Phase 2: 40% HP")),
			FOLCDungeonBossPhase(15.0f, LOCTEXT("Phase3_Threshold", "Phase 3: 15% HP"), true),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 6: Alien Structure — Large, Very Hard, Epic loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_AlienStructure", "Alien Structure");
		Data->DungeonType = EOLCDungeonType::AlienStructure;
		Data->DungeonSize = EOLCDungeonSize::Large;
		Data->Difficulty = EOLCDungeonDifficulty::VeryHard;
		Data->EstimatedClearMinutes = 75.0f;
		Data->RoomCountMin = 25; Data->RoomCountMax = 50;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_AlienCmdr", "Alien Commander");
		Data->BossRaceId = TEXT("CrystalloidDarkMatterShapers"); // WP-130 A2 mapping
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Aliens", "Alien Warriors"), 30, 60, EOLCRaceFamily::Crystalloid),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 200, 600, 0.75f, EOLCDungeonRarityTier::Uncommon),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 800, 2000, 0.90f, EOLCDungeonRarityTier::Rare),
			FOLCDungeonReward(LOCTEXT("Rw_DarkMatter", "Dark Matter Crystals"), EOLCResourceType::DarkMatterCrystals, 10, 40, 0.55f, EOLCDungeonRarityTier::Epic),
		};
		Data->LayoutSeed = 20240106;
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(70.0f, LOCTEXT("Phase1_Threshold", "Phase 1: 70% HP")),
			FOLCDungeonBossPhase(40.0f, LOCTEXT("Phase2_Threshold", "Phase 2: 40% HP")),
			FOLCDungeonBossPhase(15.0f, LOCTEXT("Phase3_Threshold", "Phase 3: 15% HP"), true),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		SampleDungeons.Add(Data);
	}

	// Archetype 7: Inner Ring Citadel — Fortress, Extreme, Legendary loot
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Archetype_InnerRingCitadel", "Inner Ring Citadel");
		Data->DungeonType = EOLCDungeonType::InnerRingCitadel;
		Data->DungeonSize = EOLCDungeonSize::Fortress;
		Data->Difficulty = EOLCDungeonDifficulty::Extreme;
		Data->EstimatedClearMinutes = 135.0f;
		Data->RoomCountMin = 40; Data->RoomCountMax = 80;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_CitadelCmdr", "Citadel Commander");
		Data->BossRaceId = TEXT("InsectoidCrystalHive"); // WP-130 A2 mapping (Fortress-tier boss for the only Fortress-size dungeon)
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Elite", "Elite Units"), 50, 100, EOLCRaceFamily::Insectoid),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 500, 1500, 0.90f, EOLCDungeonRarityTier::Common),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 2000, 5000, 0.95f, EOLCDungeonRarityTier::Rare),
			FOLCDungeonReward(LOCTEXT("Rw_DarkMatter", "Dark Matter Crystals"), EOLCResourceType::DarkMatterCrystals, 50, 200, 0.80f, EOLCDungeonRarityTier::Legendary),
		};
		Data->LayoutSeed = 20240107;
		Data->BossPhaseThresholds = {
			FOLCDungeonBossPhase(70.0f, LOCTEXT("Phase1_Threshold", "Phase 1: 70% HP")),
			FOLCDungeonBossPhase(40.0f, LOCTEXT("Phase2_Threshold", "Phase 2: 40% HP")),
			FOLCDungeonBossPhase(15.0f, LOCTEXT("Phase3_Threshold", "Phase 3: 15% HP"), true),
		};
		Data->RespawnRules = UOLCDungeonData::GetDefaultRespawnRules();
		Data->BlueprintRewards = {
			FOLCDungeonReward(LOCTEXT("Bp_CitadelUplink", "Citadel Command Uplink"), EOLCResourceType::ConstructionMaterial, 1, 1, 1.0f, EOLCDungeonRarityTier::Legendary),
		};
		Data->BlueprintRewards[0].bIsBlueprint = true;
		Data->BlueprintRewards[0].BlueprintName = LOCTEXT("Bp_CitadelUplink_Name", "Citadel Command Uplink");
		SampleDungeons.Add(Data);
	}
}

FLinearColor UOLCDungeonEntryWidget::GetRarityColor(EOLCDungeonRarityTier Tier)
{
	switch (Tier)
	{
		case EOLCDungeonRarityTier::Common:     return FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);    // Gray
		case EOLCDungeonRarityTier::Uncommon:   return FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);   // Green
		case EOLCDungeonRarityTier::Rare:       return FLinearColor(0.2f, 0.4f, 0.8f, 1.0f);   // Blue
		case EOLCDungeonRarityTier::Epic:       return FLinearColor(0.5f, 0.2f, 0.8f, 1.0f);  // Purple
		case EOLCDungeonRarityTier::Legendary:  return FLinearColor(1.0f, 0.84f, 0.0f, 1.0f); // Gold
		default: return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

FText UOLCDungeonEntryWidget::GetRarityDisplayName(EOLCDungeonRarityTier Tier)
{
	switch (Tier)
	{
		case EOLCDungeonRarityTier::Common:     return LOCTEXT("Rarity_Common", "Common");
		case EOLCDungeonRarityTier::Uncommon:   return LOCTEXT("Rarity_Uncommon", "Uncommon");
		case EOLCDungeonRarityTier::Rare:       return LOCTEXT("Rarity_Rare", "Rare");
		case EOLCDungeonRarityTier::Epic:       return LOCTEXT("Rarity_Epic", "Epic");
		case EOLCDungeonRarityTier::Legendary:  return LOCTEXT("Rarity_Legendary", "Legendary");
		default: return FText::GetEmpty();
	}
}

FText UOLCDungeonEntryWidget::GetPhaseIndicator(const UOLCDungeonData* Dungeon)
{
	if (!Dungeon || !Dungeon->BossPhaseThresholds.Num()) return LOCTEXT("Phase_Indicator_1", "Phase 1");

	// Display Phase 1 as the starting phase; full tracking happens during combat
	// Thresholds: 70% (Phase 2), 40% (Phase 3), 15% (Phase 4/enraged)
	return LOCTEXT("Phase_Indicator_1", "Phase 1");
}

FText UOLCDungeonEntryWidget::GetLayoutSeedText(int32 Seed)
{
	return FText::FromString(FString::FromInt(Seed));
}

#undef LOCTEXT_NAMESPACE
