#include "UI/OLCDungeonEntryWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

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
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Tactical Combat Dungeon View/Assets") / FileName);
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
		[ SNew(STextBlock).Text(FText::Format(
			FText::FromString(TEXT("{0} × {1}-{2} ({3:P0}% chance)")),
			Reward.RewardName,
			FText::AsNumber(Reward.MinQuantity),
			FText::AsNumber(Reward.MaxQuantity),
			Reward.DropChance * 100.0f))
			.ColorAndOpacity(OLCStyleColors::ValidGreen)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];
	}

	// Deploy button
	VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnClicked_Lambda([this, Dungeon]() -> FReply {
			OnDungeonDeployed.Broadcast(Dungeon);
			this->RemoveFromParent();
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

	// Type 3: Scavenged Vehicle — Tiny, Easy
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_ScavVeh", "Scavenged Vehicle");
		Data->DungeonType = EOLCDungeonType::ScavengedVehicle;
		Data->DungeonSize = EOLCDungeonSize::Tiny;
		Data->Difficulty = EOLCDungeonDifficulty::Easy;
		Data->EstimatedClearMinutes = 7.0f;
		Data->RoomCountMin = 1; Data->RoomCountMax = 2;
		Data->bHasBoss = false;
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Wildlife", "Wildlife"), 1, 3),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_HullParts", "Hull Parts"), EOLCResourceType::HullParts, 30, 80, 0.95f),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 50, 150, 0.80f),
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 20, 50, 0.60f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 1: Abandoned House — Small, Easy
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_AbandHouse", "Abandoned House");
		Data->DungeonType = EOLCDungeonType::AbandonedHouse;
		Data->DungeonSize = EOLCDungeonSize::Small;
		Data->Difficulty = EOLCDungeonDifficulty::Easy;
		Data->EstimatedClearMinutes = 15.0f;
		Data->RoomCountMin = 3; Data->RoomCountMax = 6;
		Data->bHasBoss = false;
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Wildlife", "Wildlife"), 2, 4),
			FOLCDungeonEnemy(LOCTEXT("En_Bandits", "Bandits"), 1, 2),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 20, 60, 0.85f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 30, 100, 0.70f),
			FOLCDungeonReward(LOCTEXT("Rw_Survival", "Survival Rations"), EOLCResourceType::Survival, 5, 15, 0.50f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 2: Enemy Camp — Medium, Moderate
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_EnemyCamp", "Enemy Camp");
		Data->DungeonType = EOLCDungeonType::EnemyCamp;
		Data->DungeonSize = EOLCDungeonSize::Medium;
		Data->Difficulty = EOLCDungeonDifficulty::Moderate;
		Data->EstimatedClearMinutes = 30.0f;
		Data->RoomCountMin = 7; Data->RoomCountMax = 12;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_CampLeader", "Camp Leader");
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Hostiles", "Hostile Units"), 6, 15),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 50, 150, 0.90f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 100, 400, 0.85f),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 30, 100, 0.60f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 4: Military Outpost — Medium-Large, Hard
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_MilOutpost", "Military Outpost");
		Data->DungeonType = EOLCDungeonType::MilitaryOutpost;
		Data->DungeonSize = EOLCDungeonSize::Medium;
		Data->Difficulty = EOLCDungeonDifficulty::Hard;
		Data->EstimatedClearMinutes = 35.0f;
		Data->RoomCountMin = 12; Data->RoomCountMax = 20;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_Commander", "Commander");
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Soldiers", "Soldiers"), 15, 30),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 100, 300, 0.95f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 300, 800, 0.90f),
			FOLCDungeonReward(LOCTEXT("Rw_Fuel", "Fuel Cans"), EOLCResourceType::Fuel, 100, 250, 0.70f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 5: Hospital Complex — Large, Hard
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Hospital", "Hospital Complex");
		Data->DungeonType = EOLCDungeonType::HospitalComplex;
		Data->DungeonSize = EOLCDungeonSize::Large;
		Data->Difficulty = EOLCDungeonDifficulty::Hard;
		Data->EstimatedClearMinutes = 60.0f;
		Data->RoomCountMin = 20; Data->RoomCountMax = 35;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_MegaCreature", "Infected Mega-Creature");
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Mutants", "Mutated Creatures"), 20, 40),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 150, 400, 0.80f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 400, 1200, 0.85f),
			FOLCDungeonReward(LOCTEXT("Rw_Medical", "Medical Supplies"), EOLCResourceType::Survival, 50, 150, 0.70f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 6: Alien Structure — Large-Fortress, Very Hard
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Alien", "Alien Structure");
		Data->DungeonType = EOLCDungeonType::AlienStructure;
		Data->DungeonSize = EOLCDungeonSize::Large;
		Data->Difficulty = EOLCDungeonDifficulty::VeryHard;
		Data->EstimatedClearMinutes = 75.0f;
		Data->RoomCountMin = 25; Data->RoomCountMax = 50;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_AlienCmdr", "Alien Commander");
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Aliens", "Alien Warriors"), 30, 60),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 200, 600, 0.75f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 800, 2000, 0.90f),
			FOLCDungeonReward(LOCTEXT("Rw_DarkMatter", "Dark Matter Crystals"), EOLCResourceType::DarkMatterCrystals, 10, 40, 0.55f),
		};
		SampleDungeons.Add(Data);
	}

	// Type 7: Inner Ring Citadel — Fortress, Extreme
	{
		auto* Data = NewObject<UOLCDungeonData>(this);
		Data->DisplayName = LOCTEXT("Dun_Citadel", "Inner Ring Citadel");
		Data->DungeonType = EOLCDungeonType::InnerRingCitadel;
		Data->DungeonSize = EOLCDungeonSize::Fortress;
		Data->Difficulty = EOLCDungeonDifficulty::Extreme;
		Data->EstimatedClearMinutes = 135.0f;
		Data->RoomCountMin = 40; Data->RoomCountMax = 80;
		Data->bHasBoss = true;
		Data->BossName = LOCTEXT("Boss_CitadelCmdr", "Citadel Commander");
		Data->Enemies = {
			FOLCDungeonEnemy(LOCTEXT("En_Elite", "Elite Units"), 50, 100),
		};
		Data->Rewards = {
			FOLCDungeonReward(LOCTEXT("Rw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 500, 1500, 0.90f),
			FOLCDungeonReward(LOCTEXT("Rw_Minerals", "Minerals"), EOLCResourceType::Minerals, 2000, 5000, 0.95f),
			FOLCDungeonReward(LOCTEXT("Rw_DarkMatter", "Dark Matter Crystals"), EOLCResourceType::DarkMatterCrystals, 50, 200, 0.80f),
		};
		SampleDungeons.Add(Data);
	}
}

#undef LOCTEXT_NAMESPACE
