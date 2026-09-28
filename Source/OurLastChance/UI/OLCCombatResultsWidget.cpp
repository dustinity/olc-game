#include "UI/OLCCombatResultsWidget.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include "Core/OLCEquipmentSubsystem.h"
#include "Core/OLCUIDataSubsystem.h"
#include "Engine/GameInstance.h"

#define LOCTEXT_NAMESPACE "OLCCombatResultsWidget"

UOLCCombatResultsWidget::UOLCCombatResultsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCCombatResultsWidget::RebuildWidget()
{
	if (!bHasRealResult)
	{
		InitializeResults(true); // Prototype fallback: assume victory
	}

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
					FeedCompletionResults();
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

	int32 NumRowsShown = 0;
	if (bHasRealResult)
	{
		for (const FOLCDungeonLootRoll& Roll : RealResult.LootRolls)
		{
			if (!Roll.bDropped) continue;
			VBox->AddSlot().AutoHeight()
			[ BuildLootRollRow(Roll) ];
			NumRowsShown++;
		}
	}
	else
	{
		for (int32 i = 0; i < LootRewards.Num(); i++)
		{
			VBox->AddSlot().AutoHeight()
			[ BuildLootRow(LootRewards[i], LootQuantities[i]) ];
			NumRowsShown++;
		}
	}

	if (NumRowsShown == 0)
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

FLinearColor UOLCCombatResultsWidget::GetRarityColor(EOLCDungeonRarityTier Tier)
{
	switch (Tier)
	{
		case EOLCDungeonRarityTier::Common:    return FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);
		case EOLCDungeonRarityTier::Uncommon:  return FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);
		case EOLCDungeonRarityTier::Rare:      return FLinearColor(0.2f, 0.4f, 0.8f, 1.0f);
		case EOLCDungeonRarityTier::Epic:      return FLinearColor(0.5f, 0.2f, 0.8f, 1.0f);
		case EOLCDungeonRarityTier::Legendary: return FLinearColor(1.0f, 0.84f, 0.0f, 1.0f);
		default:                               return FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

TSharedRef<SWidget> UOLCCombatResultsWidget::BuildLootRollRow(const FOLCDungeonLootRoll& Roll)
{
	const bool bNamedUnique = Roll.bIsUnique && !Roll.UniqueItemName.IsEmpty();
	const FLinearColor RowColor = bNamedUnique ? OLCStyleColors::WarningYellow : GetRarityColor(Roll.Rarity);
	const FText DisplayName = bNamedUnique ? Roll.UniqueItemName : Roll.Reward.RewardName;

	FText QuantityText;
	if (Roll.EquipmentTemplate.IsValid())
	{
		QuantityText = LOCTEXT("LootRoll_Equipment", "EQUIPPABLE");
	}
	else
	{
		QuantityText = FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(Roll.Quantity));
	}

	return SNew(SBorder)
		.BorderBackgroundColor(RowColor)
		.Padding(FMargin(10.0f, 6.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(DisplayName)
				.ColorAndOpacity(RowColor)
				.Font(FCoreStyle::GetDefaultFontStyle(bNamedUnique ? "Bold" : "Regular", 12)) ]
			+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
			[ SNew(STextBlock).Text(QuantityText)
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

void UOLCCombatResultsWidget::InitializeFromResult(const FDungeonCompletionResult& Result)
{
	bHasRealResult = true;
	RealResult = Result;
	bVictory = Result.bVictory;

	UnitNames.Reset();
	UnitXPReceived.Reset();
	UnitIsCasualty.Reset();

	for (const auto& Pair : Result.UnitXPGains)
	{
		UnitNames.Add(FText::FromString(Pair.Key));
		UnitXPReceived.Add(Pair.Value);
		UnitIsCasualty.Add(false);
	}
	for (const FText& Casualty : Result.Casualties)
	{
		UnitNames.Add(Casualty);
		UnitXPReceived.Add(0.0f);
		UnitIsCasualty.Add(true);
	}
}

void UOLCCombatResultsWidget::FeedCompletionResults()
{
	FDungeonCompletionResult Result;

	if (bHasRealResult)
	{
		Result = RealResult;
	}
	else
	{
		// Prototype fallback (pre-WP-130 AOLCCombatTriggerActor flow): build a result from the sample data.
		Result.bVictory = bVictory;
		if (bVictory)
		{
			for (const auto& Reward : LootRewards)
			{
				const float Amount = FMath::RandRange(Reward.MinQuantity, Reward.MaxQuantity) * Reward.DropChance;
				float& Existing = Result.ResourcesGained.FindOrAdd(Reward.ResourceType);
				Existing += Amount;

				if (Reward.bIsBlueprint)
				{
					Result.BlueprintGrants.Add(Reward.BlueprintName);
				}
			}

			for (int32 i = 0; i < UnitNames.Num(); i++)
			{
				if (!UnitIsCasualty[i])
				{
					const float XPBase = UnitXPReceived.IsValidIndex(i) ? UnitXPReceived[i] : 0.0f;
					Result.UnitXPGains.Add(UnitNames[i].ToString(), XPBase * 0.8f); // 80% of prototype XP survives
				}
				else
				{
					Result.Casualties.Add(UnitNames[i]);
				}
			}

			if (Result.ResourcesGained.Contains(EOLCResourceType::DarkMatterCrystals))
			{
				TArray<FText> PrototypeResearch = {
					LOCTEXT("Research_TacticalAI", "Tactical AI Operation"),
					LOCTEXT("Research_CrystalTech", "Crystalloid Technology"),
					LOCTEXT("Research_AdvancedCombat", "Advanced Combat Tactics"),
				};
				Result.ResearchUnlocks.Add(PrototypeResearch[FMath::RandRange(0, PrototypeResearch.Num() - 1)]);
			}
			for (const auto& BPName : Result.BlueprintGrants)
			{
				Result.ResearchUnlocks.Add(FText::Format(LOCTEXT("Research_Blueprint", "Blueprint Analysis: {0}"), BPName));
			}
		}
	}

	if (Result.bVictory)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			// Resources -> canonical resource counters.
			if (UOLCUIDataSubsystem* UIData = GI->GetSubsystem<UOLCUIDataSubsystem>())
			{
				for (const auto& Pair : Result.ResourcesGained)
				{
					UIData->AddResource(Pair.Key, Pair.Value);
				}
			}

			// Equipment -> canonical equipment pool / unique loot.
			if (UOLCEquipmentSubsystem* Equip = GI->GetSubsystem<UOLCEquipmentSubsystem>())
			{
				for (const FOLCDungeonLootRoll& Roll : Result.LootRolls)
				{
					if (!Roll.bDropped || !Roll.EquipmentTemplate.IsValid()) continue;

					if (Roll.bIsUnique)
					{
						Equip->AddUniqueLoot(Roll.EquipmentTemplate.Get(), Roll.UniqueItemName);
					}
					else
					{
						Equip->AddToPool(Roll.EquipmentTemplate.Get(), 1);
					}
				}
			}

			// Blueprints -> research unlock log. Tech-node creation is WP-120's research domain;
			// OnDungeonCompletion (broadcast below) remains the hook for that consumer.
		}
	}

	OnDungeonCompletion.Broadcast(Result);
}

#undef LOCTEXT_NAMESPACE
