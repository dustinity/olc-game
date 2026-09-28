#include "UI/OLCShipModuleManagementWidget.h"
#include "OurLastChance.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Core/OLCUIDataSubsystem.h"

#define LOCTEXT_NAMESPACE "OLCShipModuleManagementWidget"

UOLCShipModuleManagementWidget::UOLCShipModuleManagementWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::RebuildWidget()
{
	InitializeModules();

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
			// Left panel: module grid by category tabs
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 70.0f, 480.0f, 100.0f)
			[ BuildModuleGrid() ]
			// Right panel: hull management + upgrade panel
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(480.0f, 70.0f, 30.0f, 100.0f)
			[ BuildHullManagement() ]
		];
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildTopBar()
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 10.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(LOCTEXT("ModuleTitle", "SHIP MODULE MANAGEMENT"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 20)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("MODULES: {0}/{1}")),
				FText::AsNumber(UsedHullSlots),
				FText::AsNumber(TotalHullSlots)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
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

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildModuleGrid()
{
	TArray<EOLCShipModuleCategory> Categories = {
		EOLCShipModuleCategory::Drives,
		EOLCShipModuleCategory::Storage,
		EOLCShipModuleCategory::Protection,
		EOLCShipModuleCategory::Scanning,
		EOLCShipModuleCategory::Weapons,
		EOLCShipModuleCategory::Labs,
		EOLCShipModuleCategory::Support,
	};

	TArray<FText> CategoryLabels = {
		LOCTEXT("Cat_Drives", "DRIVES"),
		LOCTEXT("Cat_Storage", "STORAGE"),
		LOCTEXT("Cat_Protection", "PROTECTION"),
		LOCTEXT("Cat_Scanning", "SCANNING"),
		LOCTEXT("Cat_Weapons", "WEAPONS"),
		LOCTEXT("Cat_Labs", "LABS"),
		LOCTEXT("Cat_Support", "SUPPORT"),
	};

	// Build category tab strip.
	TSharedRef<SHorizontalBox> TabStrip = SNew(SHorizontalBox);
	for (int32 i = 0; i < Categories.Num(); i++)
	{
		const EOLCShipModuleCategory Category = Categories[i];
		const FText CategoryLabel = CategoryLabels[i];
		bool bActive = (SelectedCategory == Categories[i]);
		TabStrip->AddSlot().AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.OnClicked_Lambda([this, Category]() -> FReply {
				SelectedCategory = Category;
				SelectedGridIndex = -1;
				InvalidateLayoutAndVolatility();
				return FReply::Handled();
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(bActive ? OLCStyleColors::PrimaryOrange : FLinearColor(0.08f, 0.06f, 0.02f, 0.5f))
				.Padding(FMargin(10.0f, 6.0f))
				[ SNew(STextBlock).Text(CategoryLabel)
					.ColorAndOpacity(bActive ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
			]
		];
	}

	// Build module cards for selected category.
	TArray<UOLCShipModuleData*> CategoryModules = GetModulesByCategory(SelectedCategory);

	TSharedRef<SVerticalBox> CardGrid = SNew(SVerticalBox);

	if (CategoryModules.Num() == 0)
	{
		CardGrid->AddSlot().AutoHeight()
		[ SNew(STextBlock).Text(LOCTEXT("NoModules", "NO MODULES IN THIS CATEGORY"))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ];
	}

	for (int32 i = 0; i < CategoryModules.Num(); i++)
	{
		CardGrid->AddSlot().AutoHeight()
		[ BuildModuleCard(CategoryModules[i], SelectedGridIndex == i) ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(8.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[ TabStrip ]
			+ SVerticalBox::Slot().FillHeight(1.0f).Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[ CardGrid ]
			]
		];
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildModuleCard(UOLCShipModuleData* Module, bool bSelected)
{
	if (!Module) return SNew(SBorder);

	FLinearColor RowColor = bSelected
		? FLinearColor(0.2f, 0.12f, 0.05f, 0.6f)
		: FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Module]() -> FReply {
			SelectedGridIndex = -1;
			for (int32 i = 0; i < AllModules.Num(); i++)
			{
				if (AllModules[i].Get() == Module)
				{
					SelectedGridIndex = i;
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
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[ SNew(STextBlock).Text(Module->DisplayName)
					.ColorAndOpacity(bSelected ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 2.0f)
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("TIR {0} — {1} slots")),
					FText::AsNumber(Module->TIRTier),
					FText::AsNumber(Module->HullSlotsRequired)))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
				+ SVerticalBox::Slot().AutoHeight()
				[ SNew(STextBlock).Text(Module->EffectDescription)
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.AutoWrapText(true) ]
			]
		];
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildHullManagement()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("HullTitle", "SHIP HULL"))
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("SLOTS: {0}/{1}")),
		FText::AsNumber(UsedHullSlots),
		FText::AsNumber(TotalHullSlots)))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 16.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::FromString(TEXT("ATTACHMENT POINTS:")))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

	// Draw hull slots in a grid (4 columns × as many rows as needed).
	int32 Rows = FMath::CeilToInt(static_cast<float>(TotalHullSlots) / 4.0f);
	for (int32 row = 0; row < Rows; row++)
	{
		TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);
		for (int32 col = 0; col < 4; col++)
		{
			int32 Index = row * 4 + col;
			if (Index >= TotalHullSlots) break;

			UOLCShipModuleData* Module = (Index < HullSlots.Num()) ? HullSlots[Index].Get() : nullptr;
			HBox->AddSlot().AutoWidth()
			[ BuildHullSlot(Index, Module) ];
		}
		VBox->AddSlot().AutoHeight()
		[ HBox ];
	}

	// Upgrade panel for selected module.
	if (SelectedGridIndex >= 0 && SelectedGridIndex < AllModules.Num())
	{
		VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 8.0f)
		[ SNew(STextBlock).Text(FText::FromString(TEXT("MODULE OPTIONS:")))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

		VBox->AddSlot().AutoHeight()
		[ BuildUpgradePanel() ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildHullSlot(int32 Index, UOLCShipModuleData* ModuleInSlot)
{
	if (ModuleInSlot)
	{
		int32 TIRLevel = (Index < ModuleTIRLevels.Num()) ? ModuleTIRLevels[Index] : 1;

		return SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::TacticalBlue)
			.Padding(FMargin(4.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[ SNew(STextBlock).Text(ModuleInSlot->DisplayName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
				+ SVerticalBox::Slot().AutoHeight()
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("TIR {0}")),
					FText::AsNumber(TIRLevel)))
					.ColorAndOpacity(OLCStyleColors::WarningYellow)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8)) ]
			];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.12f, 0.3f))
		.Padding(FMargin(4.0f))
		[
			SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("#{0}")),
				FText::AsNumber(Index + 1)))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
		];
}

TSharedRef<SWidget> UOLCShipModuleManagementWidget::BuildUpgradePanel()
{
	TArray<UOLCShipModuleData*> CategoryModules = GetModulesByCategory(SelectedCategory);
	if (SelectedGridIndex < 0 || SelectedGridIndex >= CategoryModules.Num()) return SNew(SBorder);

	UOLCShipModuleData* Module = CategoryModules[SelectedGridIndex];
	if (!Module) return SNew(SBorder);

	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(Module->DisplayName)
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(Module->EffectDescription)
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ];

	// Attach button (if hull has free slots).
	bool bHasFreeSlot = (UsedHullSlots < TotalHullSlots);
	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.IsEnabled(bHasFreeSlot)
		.OnClicked_Lambda([this, Module]() -> FReply {
			// Find first empty slot and attach.
			int32 FreeSlot = -1;
			for (int32 i = 0; i < TotalHullSlots; i++)
			{
				if (i >= HullSlots.Num() || !HullSlots[i])
				{
					FreeSlot = i;
					break;
				}
			}

			if (FreeSlot >= 0)
			{
				HullSlots.Add(Module);
				ModuleTIRLevels.Add(Module->TIRTier);
				UsedHullSlots++;

				OnModuleAttached.Broadcast(Module->DisplayName, FreeSlot);
			}

			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(bHasFreeSlot ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(16.0f, 8.0f))
			[ SNew(STextBlock).Text(LOCTEXT("BtnAttach", "ATTACH"))
				.ColorAndOpacity(bHasFreeSlot ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
		]
	];

	// TIR upgrade button (placeholder — would require resources in full impl).
	VBox->AddSlot().AutoHeight()
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.IsEnabled(false) // Disabled until resource wiring implemented
		.OnClicked_Lambda([]() -> FReply {
			UE_LOG(LogOLC, Log, TEXT("[OLC] TIR upgrade clicked (placeholder)"));
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(16.0f, 8.0f))
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("TIR UPGRADE → {0}")),
				FText::AsNumber(Module->TIRTier + 1)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
		]
	];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f))
		[ VBox ];
}

void UOLCShipModuleManagementWidget::InitializeModules()
{
	AllModules.Reset();
	HullSlots.Reset();
	ModuleTIRLevels.Reset();
	UsedHullSlots = 0;

	// Drives (4 modules from Briefing).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_RightDrive", "Right Drive Assembly");
		M1->ModuleCategory = EOLCShipModuleCategory::Drives;
		M1->TIRTier = 1; M1->HullSlotsRequired = 4;
		M1->PowerConsumption = -50.0f;
		M1->EffectDescription = LOCTEXT("Desc_RightDrive", "Primary propulsion drive.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_LeftDrive", "Left Drive Assembly");
		M2->ModuleCategory = EOLCShipModuleCategory::Drives;
		M2->TIRTier = 1; M2->HullSlotsRequired = 4;
		M2->PowerConsumption = -50.0f;
		M2->EffectDescription = LOCTEXT("Desc_LeftDrive", "Secondary propulsion drive.");
		AllModules.Add(M2);

		auto* M3 = NewObject<UOLCShipModuleData>(this);
		M3->DisplayName = LOCTEXT("Mod_EnergyStream", "Energy Stream Drive");
		M3->ModuleCategory = EOLCShipModuleCategory::Drives;
		M3->TIRTier = 3; M3->HullSlotsRequired = 6;
		M3->PowerConsumption = -80.0f;
		M3->EffectDescription = LOCTEXT("Desc_EnergyStream", "Advanced energy-driven propulsion.");
		AllModules.Add(M3);

		auto* M4 = NewObject<UOLCShipModuleData>(this);
		M4->DisplayName = LOCTEXT("Mod_VoidWarp", "Void Warp Drive");
		M4->ModuleCategory = EOLCShipModuleCategory::Drives;
		M4->TIRTier = 5; M4->HullSlotsRequired = 10;
		M4->PowerConsumption = -120.0f;
		M4->EffectDescription = LOCTEXT("Desc_VoidWarp", "Instant travel within 5-system radius.");
		AllModules.Add(M4);
	}

	// Storage (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_StandardCargo", "Standard Cargo Bay");
		M1->ModuleCategory = EOLCShipModuleCategory::Storage;
		M1->TIRTier = 1; M1->HullSlotsRequired = 3;
		M1->PowerConsumption = -5.0f;
		M1->EffectDescription = LOCTEXT("Desc_StandardCargo", "Basic resource storage capacity.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_ExtendedCargo", "Extended Cargo Bay");
		M2->ModuleCategory = EOLCShipModuleCategory::Storage;
		M2->TIRTier = 2; M2->HullSlotsRequired = 5;
		M2->PowerConsumption = -10.0f;
		M2->EffectDescription = LOCTEXT("Desc_ExtendedCargo", "Additional storage for resources and equipment.");
		AllModules.Add(M2);

		auto* M3 = NewObject<UOLCShipModuleData>(this);
		M3->DisplayName = LOCTEXT("Mod_BatteryStorage", "Battery Storage I");
		M3->ModuleCategory = EOLCShipModuleCategory::Storage;
		M3->TIRTier = 1; M3->HullSlotsRequired = 2;
		M3->PowerConsumption = -15.0f;
		M3->EffectDescription = LOCTEXT("Desc_BatteryStorage", "+50 energy capacity on ship.");
		AllModules.Add(M3);
	}

	// Protection (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_ShieldGen", "Shield Generator");
		M1->ModuleCategory = EOLCShipModuleCategory::Protection;
		M1->TIRTier = 2; M1->HullSlotsRequired = 6;
		M1->PowerConsumption = -30.0f;
		M1->EffectDescription = LOCTEXT("Desc_ShieldGen", "Energy shield emitter for defense.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_EnergyShieldI", "Energy Shield Generation I");
		M2->ModuleCategory = EOLCShipModuleCategory::Protection;
		M2->TIRTier = 2; M2->HullSlotsRequired = 8;
		M2->PowerConsumption = -40.0f;
		M2->EffectDescription = LOCTEXT("Desc_EnergyShieldI", "Advanced energy shield system.");
		AllModules.Add(M2);

		auto* M3 = NewObject<UOLCShipModuleData>(this);
		M3->DisplayName = LOCTEXT("Mod_AlienShield", "Alien Shield Generator");
		M3->ModuleCategory = EOLCShipModuleCategory::Protection;
		M3->TIRTier = 5; M3->HullSlotsRequired = 12;
		M3->PowerConsumption = -60.0f;
		M3->EffectDescription = LOCTEXT("Desc_AlienShield", "Immune to physical damage.");
		AllModules.Add(M3);
	}

	// Scanning (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_Sensors", "Long Range Sensors");
		M1->ModuleCategory = EOLCShipModuleCategory::Scanning;
		M1->TIRTier = 2; M1->HullSlotsRequired = 3;
		M1->PowerConsumption = -15.0f;
		M1->EffectDescription = LOCTEXT("Desc_Sensors", "Extended detection range for navigation.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_DeepSonar", "Deep Sonar Array");
		M2->ModuleCategory = EOLCShipModuleCategory::Scanning;
		M2->TIRTier = 3; M2->HullSlotsRequired = 4;
		M2->PowerConsumption = -20.0f;
		M2->EffectDescription = LOCTEXT("Desc_DeepSonar", "Detects buried structures and dungeons.");
		AllModules.Add(M2);
	}

	// Weapons (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_TurretMount", "Ballistic Turret Mount");
		M1->ModuleCategory = EOLCShipModuleCategory::Weapons;
		M1->TIRTier = 1; M1->HullSlotsRequired = 4;
		M1->PowerConsumption = -20.0f;
		M1->EffectDescription = LOCTEXT("Desc_TurretMount", "Hardpoint for ballistic turret systems.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_PlasmaCannon", "Plasma Cannon Mount");
		M2->ModuleCategory = EOLCShipModuleCategory::Weapons;
		M2->TIRTier = 2; M2->HullSlotsRequired = 6;
		M2->PowerConsumption = -35.0f;
		M2->EffectDescription = LOCTEXT("Desc_PlasmaCannon", "Energy weapon deployment point.");
		AllModules.Add(M2);

		auto* M3 = NewObject<UOLCShipModuleData>(this);
		M3->DisplayName = LOCTEXT("Mod_VoidBeam", "Void Beam Emitter");
		M3->ModuleCategory = EOLCShipModuleCategory::Weapons;
		M3->TIRTier = 5; M3->HullSlotsRequired = 10;
		M3->PowerConsumption = -80.0f;
		M3->EffectDescription = LOCTEXT("Desc_VoidBeam", "Extreme damage, ignores armor.");
		AllModules.Add(M3);
	}

	// Labs (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_ResearchLab", "Research Lab Module");
		M1->ModuleCategory = EOLCShipModuleCategory::Labs;
		M1->TIRTier = 3; M1->HullSlotsRequired = 8;
		M1->PowerConsumption = -40.0f;
		M1->EffectDescription = LOCTEXT("Desc_ResearchLab", "On-board research for alien artifacts.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_VoidLab", "Void Lab");
		M2->ModuleCategory = EOLCShipModuleCategory::Labs;
		M2->TIRTier = 5; M2->HullSlotsRequired = 12;
		M2->PowerConsumption = -60.0f;
		M2->EffectDescription = LOCTEXT("Desc_VoidLab", "Outer ring research capability.");
		AllModules.Add(M2);
	}

	// Support (7 modules).
	{
		auto* M1 = NewObject<UOLCShipModuleData>(this);
		M1->DisplayName = LOCTEXT("Mod_LifeSupport", "Life Support System");
		M1->ModuleCategory = EOLCShipModuleCategory::Support;
		M1->TIRTier = 1; M1->HullSlotsRequired = 3;
		M1->PowerConsumption = -25.0f;
		M1->EffectDescription = LOCTEXT("Desc_LifeSupport", "Maintains habitable conditions for crew.");
		AllModules.Add(M1);

		auto* M2 = NewObject<UOLCShipModuleData>(this);
		M2->DisplayName = LOCTEXT("Mod_CommsArray", "Enhanced Comms Array");
		M2->ModuleCategory = EOLCShipModuleCategory::Support;
		M2->TIRTier = 2; M2->HullSlotsRequired = 3;
		M2->PowerConsumption = -10.0f;
		M2->EffectDescription = LOCTEXT("Desc_CommsArray", "+2 control group slots.");
		AllModules.Add(M2);

		auto* M3 = NewObject<UOLCShipModuleData>(this);
		M3->DisplayName = LOCTEXT("Mod_TacticalAI", "Tactical AI Assistant");
		M3->ModuleCategory = EOLCShipModuleCategory::Support;
		M3->TIRTier = 3; M3->HullSlotsRequired = 5;
		M3->PowerConsumption = -20.0f;
		M3->EffectDescription = LOCTEXT("Desc_TacticalAI", "Auto-target priority system.");
		AllModules.Add(M3);
	}
}

TArray<UOLCShipModuleData*> UOLCShipModuleManagementWidget::GetModulesByCategory(EOLCShipModuleCategory Category) const
{
	TArray<UOLCShipModuleData*> Result;
	for (const TObjectPtr<UOLCShipModuleData>& Module : AllModules)
	{
		if (Module && Module->ModuleCategory == Category)
			Result.Add(Module.Get());
	}
	return Result;
}

#undef LOCTEXT_NAMESPACE
