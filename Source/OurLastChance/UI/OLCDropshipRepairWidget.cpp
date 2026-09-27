#include "UI/OLCDropshipRepairWidget.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Core/OLCUIDataSubsystem.h"

#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCDropshipRepairWidget"

// ---------------------------------------------------------------------------
// Screen-specific asset paths (WP-13 Step 4)
// ---------------------------------------------------------------------------
namespace DropshipAssetPath
{
	FString RepairIcon(const FString& FileName)
	{
		return FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/UI/Dropship Repair View/Assets") / FileName);
	}

	bool AssetExists(const FString& Path) { return FPaths::FileExists(Path); }
}

UOLCDropshipRepairWidget::UOLCDropshipRepairWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UOLCDropshipRepairWidget::RebuildWidget()
{
	InitializeShipModules();

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
			// Left panel: available modules/replacements list
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 70.0f, 380.0f, 100.0f)
			[ BuildModuleList() ]
			// Right panel: ship hull schematic with attachment points
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(380.0f, 70.0f, 30.0f, 100.0f)
			[ BuildHullSchematic() ]
		];
}

TSharedRef<SWidget> UOLCDropshipRepairWidget::BuildTopBar()
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 10.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(LOCTEXT("RepairTitle", "DROPSHIP REPAIR"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("HULL: {0:P1}%")),
				HullIntegrityPercent * 100.0f))
				.ColorAndOpacity(HullIntegrityPercent > 0.5f ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("SLOTS: {0}/{1}")),
				FText::AsNumber(UsedHullSlots),
				FText::AsNumber(TotalHullSlots)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
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

TSharedRef<SWidget> UOLCDropshipRepairWidget::BuildModuleList()
{
	if (AvailableModules.Num() == 0)
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("NoModules", "NO MODULES AVAILABLE"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	for (int32 i = 0; i < AvailableModules.Num(); i++)
	{
		VBox->AddSlot().AutoHeight()
		[ BuildModuleEntry(AvailableModules[i], SelectedIndex == i) ];
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

TSharedRef<SWidget> UOLCDropshipRepairWidget::BuildModuleEntry(const FOLCShipModuleViewData& Module, bool bSelected)
{
	FText CategoryLabel;
	switch (Module.Category)
	{
		case EOLCShipModuleCategory::Drives:     CategoryLabel = LOCTEXT("Cat_Drives", "DRIVES"); break;
		case EOLCShipModuleCategory::Storage:    CategoryLabel = LOCTEXT("Cat_Storage", "STORAGE"); break;
		case EOLCShipModuleCategory::Protection: CategoryLabel = LOCTEXT("Cat_Protection", "PROTECTION"); break;
		case EOLCShipModuleCategory::Scanning:   CategoryLabel = LOCTEXT("Cat_Scanning", "SCANNING"); break;
		case EOLCShipModuleCategory::Weapons:    CategoryLabel = LOCTEXT("Cat_Weapons", "WEAPONS"); break;
		case EOLCShipModuleCategory::Labs:       CategoryLabel = LOCTEXT("Cat_Labs", "LABS"); break;
		case EOLCShipModuleCategory::Support:    CategoryLabel = LOCTEXT("Cat_Support", "SUPPORT"); break;
		default:                                 CategoryLabel = FText::GetEmpty(); break;
	}

	FLinearColor StateColor;
	switch (Module.State)
	{
		case EOLCModuleState::Installed: StateColor = OLCStyleColors::ValidGreen; break;
		case EOLCModuleState::Damaged:   StateColor = OLCStyleColors::WarningYellow; break;
		case EOLCModuleState::Offline:   StateColor = OLCStyleColors::TextDim; break;
		default:                         StateColor = OLCStyleColors::TextDim; break;
	}

	FLinearColor RowColor = bSelected
		? FLinearColor(0.2f, 0.12f, 0.05f, 0.6f)
		: FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, &Module]() -> FReply {
			SelectedIndex = -1;
			for (int32 i = 0; i < AvailableModules.Num(); i++)
			{
				if (&AvailableModules[i] == &Module)
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
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[ SNew(STextBlock).Text(Module.DisplayName)
					.ColorAndOpacity(bSelected ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ]
				+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 4.0f, 0.0f, 2.0f)
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("{0} — TIR {1} — {2} slots")),
					CategoryLabel,
					FText::AsNumber(Module.TIRTier),
					FText::AsNumber(Module.HullSlotsRequired)))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(SBorder)
					.BorderBackgroundColor(StateColor)
					.Padding(FMargin(6.0f, 2.0f))
					[ SNew(STextBlock).Text(bSelected && Module.State == EOLCModuleState::Damaged
						? LOCTEXT("Mod_Damaged", "DAMAGED")
						: LOCTEXT("Mod_OK", "OK"))
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
				]
			]
		];
}

TSharedRef<SWidget> UOLCDropshipRepairWidget::BuildHullSchematic()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("HullTitle", "SHIP HULL SCHEMATIC"))
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	// Hull integrity bar.
	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::FromString(TEXT("HULL INTEGRITY:")))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

	VBox->AddSlot().AutoHeight()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.08f, 0.06f, 0.02f, 0.5f))
		.Padding(FMargin(2.0f))
		[ SNew(SBox).WidthOverride(300.0f).HeightOverride(12.0f)
			[ SNew(SBorder)
				.BorderBackgroundColor(HullIntegrityPercent > 0.5f ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed)
				.Padding(FMargin(0.0f))
				[ SNew(SBox).WidthOverride(300.0f * HullIntegrityPercent).HeightOverride(12.0f) ] ] ]
	];

	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("{0:P1}%")),
		HullIntegrityPercent * 100.0f))
		.ColorAndOpacity(HullIntegrityPercent > 0.5f ? OLCStyleColors::ValidGreen : OLCStyleColors::DangerRed)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	VBox->AddSlot().AutoHeight().Padding(0.0f, 20.0f, 0.0f, 8.0f)
	[ SNew(STextBlock).Text(FText::FromString(TEXT("ATTACHMENT POINTS:")))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ];

	// Draw attachment points in a 4×3 grid.
	for (int32 row = 0; row < 3; row++)
	{
		TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);
		for (int32 col = 0; col < 4; col++)
		{
			int32 Index = row * 4 + col;
			FText PointName = FText::Format(
				FText::FromString(TEXT("P{0}")),
				FText::AsNumber(Index + 1));

			EOLCModuleState State = (Index < UsedHullSlots)
				? EOLCModuleState::Installed
				: EOLCModuleState::Offline;

			HBox->AddSlot().AutoWidth()
			[ BuildAttachmentPoint(PointName, State, Index < UsedHullSlots) ];
		}
		VBox->AddSlot().AutoHeight()
		[ HBox ];
	}

	// Repair button (for selected damaged module).
	if (SelectedIndex >= 0 && SelectedIndex < AvailableModules.Num())
	{
		const auto& Module = AvailableModules[SelectedIndex];
		bool bIsDamaged = (Module.State == EOLCModuleState::Damaged);

		VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 0.0f)
		[
			SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.IsEnabled(bIsDamaged && CanRepair(Module.RepairCost))
			.OnClicked_Lambda([this, &Module]() -> FReply {
				CurrentRepairingModule = Module.DisplayName;
				CurrentRepairDuration = 5.0f; // 5s prototype repair time
				CurrentRepairProgress = 0.0f;

				OnRepairStarted.Broadcast(Module.DisplayName, CurrentRepairDuration);

				// Deduct resources for prototype.
				if (UGameInstance* GI = GetWorld()->GetGameInstance())
				{
					if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
					{
						for (const auto& Cost : Module.RepairCost)
						{
							Data->AddResource(Cost.ResourceType, -Cost.CurrentValue);
						}
					}
				}

				return FReply::Handled();
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(bIsDamaged && CanRepair(Module.RepairCost)
					? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
				.Padding(FMargin(18.0f, 10.0f))
				[ SNew(STextBlock).Text(LOCTEXT("BtnRepair", "REPAIR"))
					.ColorAndOpacity(bIsDamaged && CanRepair(Module.RepairCost)
						? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ]
			]
		];

		// Repair progress bar.
		if (CurrentRepairingModule.EqualTo(Module.DisplayName) && CurrentRepairProgress < CurrentRepairDuration)
		{
			VBox->AddSlot().AutoHeight()
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("REPAIRING: {0:P1}%")),
				(CurrentRepairProgress / CurrentRepairDuration) * 100.0f))
				.ColorAndOpacity(OLCStyleColors::TacticalBlue)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];

			VBox->AddSlot().AutoHeight()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.08f, 0.06f, 0.02f, 0.5f))
				.Padding(FMargin(2.0f))
				[ SNew(SBox).WidthOverride(300.0f).HeightOverride(10.0f)
					[ SNew(SBorder)
						.BorderBackgroundColor(OLCStyleColors::TacticalBlue)
						.Padding(FMargin(0.0f))
						[ SNew(SBox).WidthOverride(300.0f * (CurrentRepairProgress / CurrentRepairDuration)).HeightOverride(10.0f) ] ] ]
			];
		}
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

TSharedRef<SWidget> UOLCDropshipRepairWidget::BuildAttachmentPoint(const FText& Name, EOLCModuleState State, bool bHasModule)
{
	FLinearColor PointColor;
	switch (State)
	{
		case EOLCModuleState::Installed: PointColor = OLCStyleColors::ValidGreen; break;
		case EOLCModuleState::Damaged:   PointColor = OLCStyleColors::WarningYellow; break;
		default:                         PointColor = FLinearColor(0.1f, 0.1f, 0.12f, 0.4f); break;
	}

	return SNew(SBorder)
		.BorderBackgroundColor(PointColor)
		.Padding(FMargin(6.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[ SNew(STextBlock).Text(Name)
				.ColorAndOpacity(bHasModule ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
			+ SVerticalBox::Slot().AutoHeight()
			[ SNew(STextBlock).Text(bHasModule
				? LOCTEXT("Pt_Occupied", "OCCUPIED")
				: LOCTEXT("Pt_Free", "FREE"))
				.ColorAndOpacity(PointColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 8)) ]
		];
}

bool UOLCDropshipRepairWidget::CanRepair(const TArray<FOLCResourceAmount>& Cost)
{
	if (!GetWorld()) return false;

	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (!GI) return false;

	UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>();
	if (!Data) return false;

	const auto& Counters = Data->GetResourceCounters();

	for (const auto& Req : Cost)
	{
		for (const auto& Counter : Counters)
		{
			if (Counter.ResourceType == Req.ResourceType && Counter.Value < Req.CurrentValue)
				return false;
		}
	}
	return true;
}

void UOLCDropshipRepairWidget::InitializeShipModules()
{
	AvailableModules.Reset();

	// Drives — prototype modules from Briefing/ShipModules.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_RightDrive", "Right Drive Assembly"),
		EOLCShipModuleCategory::Drives, 1, 4, -50.0f,
		LOCTEXT("Desc_RightDrive", "Primary propulsion drive. Currently functional but empty fuel tank.")));

	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_LeftDrive", "Left Drive Assembly"),
		EOLCShipModuleCategory::Drives, 1, 4, -50.0f,
		LOCTEXT("Desc_LeftDrive", "Secondary propulsion drive. Currently damaged — 50% thrust output.")));

	// Protection — Shield generator.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_ShieldGen", "Shield Generator"),
		EOLCShipModuleCategory::Protection, 2, 6, -30.0f,
		LOCTEXT("Desc_ShieldGen", "Energy shield emitter. Protects dropship from incoming fire.")));

	// Scanning — Long range sensor array.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_Sensors", "Long Range Sensors"),
		EOLCShipModuleCategory::Scanning, 2, 3, -15.0f,
		LOCTEXT("Desc_Sensors", "Extended detection range for planet scanning and navigation.")));

	// Weapons — Ballistic turret mount.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_Turret", "Ballistic Turret Mount"),
		EOLCShipModuleCategory::Weapons, 1, 4, -20.0f,
		LOCTEXT("Desc_Turret", "Hardpoint for ballistic turret weapon systems.")));

	// Storage — Extended cargo bay.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_CargoBay", "Extended Cargo Bay"),
		EOLCShipModuleCategory::Storage, 1, 5, -10.0f,
		LOCTEXT("Desc_CargoBay", "Additional storage capacity for resources and equipment.")));

	// Support — Life support system.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_LifeSupport", "Life Support System"),
		EOLCShipModuleCategory::Support, 1, 3, -25.0f,
		LOCTEXT("Desc_LifeSupport", "Maintains habitable conditions for crew during transit.")));

	// Labs — Research lab module.
	AvailableModules.Add(FOLCShipModuleViewData(
		LOCTEXT("Mod_ResearchLab", "Research Lab Module"),
		EOLCShipModuleCategory::Labs, 3, 8, -40.0f,
		LOCTEXT("Desc_ResearchLab", "On-board research facility for analyzing alien artifacts and blueprints.")));

	UsedHullSlots = AvailableModules.Num(); // All slots occupied in prototype.
}

#undef LOCTEXT_NAMESPACE
