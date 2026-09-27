#include "UI/OLCCombatResultsWidget.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "OLCCombatResultsWidget"

UOLCCombatResultsWidget::UOLCCombatResultsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCCombatResultsWidget::RebuildWidget()
{
	InitializeResults(true); // Prototype: assume victory

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.005f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(0.0f))
		[
			SNew(SOverlay)
			// Top bar with result header
			+ SOverlay::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Fill)
			[ BuildTopBar() ]
			// Left panel: unit results (XP + casualties)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 70.0f, 520.0f, 100.0f)
			[ BuildUnitResults() ]
			// Center panel: loot rewards
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 70.0f, 320.0f, 100.0f)
			[ BuildLootPanel() ]
			// Right panel: artifact notification
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(320.0f, 70.0f, 30.0f, 100.0f)
			[ BuildArtifactPanel() ]
		];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildTopBar()
{
	FText ResultLabel = bVictory ? LOCTEXT("Result_Victory", "VICTORY") : LOCTEXT("Result_Defeat", "DEFEAT");
	FLinearColor HeaderColor = bVictory ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed;

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 12.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(ResultLabel)
				.ColorAndOpacity(HeaderColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 24)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]() -> FReply {
					OnReturnToBase.Broadcast();
					this->RemoveFromParent();
					return FReply::Handled();
				})
				[
					SNew(SBorder)
					.BorderBackgroundColor(OLCStyleColors::PrimaryOrange)
					.Padding(FMargin(18.0f, 10.0f))
					[ SNew(STextBlock).Text(LOCTEXT("BtnReturn", "RETURN TO BASE"))
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
				]
			]
		];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildLootPanel()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("LootTitle", "LOOT REWARDS"))
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::FromString(TEXT("---")) )
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ];

	for (int32 i = 0; i < LootRewards.Num(); i++)
	{
		VBox->AddSlot().AutoHeight()
		[ BuildLootRow(LootRewards[i], LootQuantities[i]) ];
	}

	if (LootRewards.Num() == 0)
	{
		VBox->AddSlot().AutoHeight()
		[ SNew(STextBlock).Text(LOCTEXT("NoLoot", "NO LOOT FOUND"))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildLootRow(const FOLCDungeonReward& Reward, int32 Quantity)
{
	FText ResourceTypeText;
	switch (Reward.ResourceType)
	{
		case EOLCResourceType::Energy:              ResourceTypeText = LOCTEXT("RType_Energy", "ENERGY"); break;
		case EOLCResourceType::Fuel:                ResourceTypeText = LOCTEXT("RType_Fuel", "FUEL"); break;
		case EOLCResourceType::ConstructionMaterial: ResourceTypeText = LOCTEXT("RType_CM", "CONSTR. MAT."); break;
		case EOLCResourceType::Minerals:            ResourceTypeText = LOCTEXT("RType_Minerals", "MINERALS"); break;
		case EOLCResourceType::HullParts:           ResourceTypeText = LOCTEXT("RType_Hull", "HULL PARTS"); break;
		case EOLCResourceType::Survival:            ResourceTypeText = LOCTEXT("RType_Survival", "SURVIVAL"); break;
		case EOLCResourceType::DarkMatterCrystals:  ResourceTypeText = LOCTEXT("RType_DarkMat", "DARK MATTER"); break;
		default:                                    ResourceTypeText = FText::GetEmpty(); break;
	}

	FLinearColor RowColor = Reward.bIsBlueprint ? OLCStyleColors::WarningYellow : OLCStyleColors::ValidGreen;

	return SNew(SBorder)
		.BorderBackgroundColor(RowColor)
		.Padding(FMargin(10.0f, 6.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(Reward.RewardName)
				.ColorAndOpacity(RowColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("+{0} {1}")),
				FText::AsNumber(Quantity),
				ResourceTypeText))
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ]
		];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildUnitResults()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("UnitsTitle", "UNIT RESULTS"))
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::FromString(TEXT("---")))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ];

	for (int32 i = 0; i < UnitNames.Num(); i++)
	{
		VBox->AddSlot().AutoHeight()
		[ BuildUnitEntry(UnitNames[i], UnitXPReceived[i], UnitIsCasualty[i]) ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildUnitEntry(FText UnitName, float XPReceived, bool bIsCasualty)
{
	if (bIsCasualty)
	{
		return SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::DangerRed)
			.Padding(FMargin(10.0f, 6.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[ SNew(STextBlock).Text(UnitName)
					.ColorAndOpacity(OLCStyleColors::DangerRed)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
				[ SNew(STextBlock).Text(LOCTEXT("Casualty", "CASUALTY"))
					.ColorAndOpacity(OLCStyleColors::DangerRed)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.08f, 0.06f, 0.02f, 0.5f))
		.Padding(FMargin(10.0f, 6.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[ SNew(STextBlock).Text(UnitName)
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
			+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 0.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(OLCStyleColors::TacticalBlue)
				.Padding(FMargin(2.0f))
				[ SNew(SBox).WidthOverride(150.0f).HeightOverride(8.0f)
					[ SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::ValidGreen)
						.Padding(FMargin(0.0f))
						[ SNew(SBox).WidthOverride(150.0f * FMath::Clamp(XPReceived / 100.0f, 0.0f, 1.0f)).HeightOverride(8.0f) ] ] ]
			]
			+ SVerticalBox::Slot().AutoHeight()
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("+{0} XP")),
				FText::AsNumber(FMath::RoundToInt(XPReceived))))
				.ColorAndOpacity(OLCStyleColors::TacticalBlue)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
		];
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildArtifactPanel()
{
	if (!bFoundArtifact)
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("NoArtifact", "NO ARTIFACT FOUND"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("ArtifactTitle", "ARTIFACT DISCOVERED"))
		.ColorAndOpacity(OLCStyleColors::WarningYellow)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(ArtifactName)
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(LOCTEXT("ArtifactDesc", "Added to equipment inventory"))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

void UOLCCombatResultsWidget::InitializeResults(bool bVictoryIn)
{
	bVictory = bVictoryIn;
	LootRewards.Reset();
	LootQuantities[0] = LootQuantities[1] = LootQuantities[2] = LootQuantities[3] = 0;
	LootQuantities[4] = LootQuantities[5] = LootQuantities[6] = 0;

	UnitNames.Reset();
	UnitXPReceived.Reset();
	UnitIsCasualty.Reset();

	ArtifactName = FText::GetEmpty();
	bFoundArtifact = false;

	if (!bVictory) return;

	// Prototype loot: Construction Material, Minerals, Fuel.
	LootRewards.Add(FOLCDungeonReward(LOCTEXT("Lw_CM", "Construction Material"), EOLCResourceType::ConstructionMaterial, 50, 150));
	LootQuantities[2] = FMath::RandRange(50, 150);

	LootRewards.Add(FOLCDungeonReward(LOCTEXT("Lw_Minerals", "Minerals"), EOLCResourceType::Minerals, 100, 400));
	LootQuantities[3] = FMath::RandRange(100, 400);

	LootRewards.Add(FOLCDungeonReward(LOCTEXT("Lw_Fuel", "Fuel"), EOLCResourceType::Fuel, 30, 100));
	LootQuantities[1] = FMath::RandRange(30, 100);

	// Prototype unit results.
	UnitNames.Add(LOCTEXT("U_Soldier", "Basic Soldier"));
	UnitXPReceived.Add(FMath::RandRange(25, 75));
	UnitIsCasualty.Add(false);

	UnitNames.Add(LOCTEXT("U_Heavy", "Heavy Gunner"));
	UnitXPReceived.Add(FMath::RandRange(30, 80));
	UnitIsCasualty.Add(false);

	// Random artifact chance.
	if (FMath::FRand() < 0.3f) // 30% chance
	{
		bFoundArtifact = true;
		TArray<FText> Artifacts = {
			LOCTEXT("Art_AlienCore", "Alien Power Core"),
			LOCTEXT("Art_Blueprint", "Tactical AI Blueprint"),
			LOCTEXT("Art_Crystal", "Dark Matter Crystal Shard"),
		};
		ArtifactName = Artifacts[FMath::RandRange(0, Artifacts.Num() - 1)];
	}
}

#undef LOCTEXT_NAMESPACE
