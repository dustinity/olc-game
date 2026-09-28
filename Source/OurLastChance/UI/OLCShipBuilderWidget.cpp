#include "OLCShipBuilderWidget.h"
#include "OurLastChance.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#include "Core/OLCUIDataSubsystem.h"
#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCShipBuilderWidget"

UOLCShipBuilderWidget::UOLCShipBuilderWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCShipBuilderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = GetGameInstance())
	{
		DataSubsystem = GI->GetSubsystem<UOLCUIDataSubsystem>();
	}

	bIsVisible = true;
	RefreshModuleCatalog();
	UpdateHullSlots();
}

void UOLCShipBuilderWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Refresh catalog periodically to stay in sync with subsystem state
	if (DataSubsystem && bIsVisible)
	{
		RefreshModuleCatalog();
		UpdateHullSlots();
	}
}

void UOLCShipBuilderWidget::ToggleVisibility()
{
	bIsVisible = !bIsVisible;
	SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (bIsVisible)
	{
		RefreshModuleCatalog();
		UpdateHullSlots();
	}
}

void UOLCShipBuilderWidget::CloseShipBuilder()
{
	bIsVisible = false;
	SetVisibility(ESlateVisibility::Hidden);
}

void UOLCShipBuilderWidget::OnInstallClicked()
{
	if (!DataSubsystem || SelectedModuleIndex < 0) return;
	if (SelectedModuleIndex >= CurrentCategoryModules.Num()) return;

	const FOLCShipModuleViewData& Module = CurrentCategoryModules[SelectedModuleIndex];

	// Check TIR compatibility
	if (Module.TIRTier > DataSubsystem->GetColonyTIR())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Cannot install %s — requires TIR %d, colony TIR is %d"),
			*Module.DisplayName.ToString(), Module.TIRTier, DataSubsystem->GetColonyTIR());
		return;
	}

	// Check if already installed in this category
	TArray<FOLCShipModuleViewData> Installed = DataSubsystem->GetInstalledModules(Module.Category);
	bool bAlreadyHasModule = false;
	for (const auto& InstMod : Installed)
	{
		if (InstMod.Category == Module.Category)
		{
			bAlreadyHasModule = true;
			break;
		}
	}

	if (bAlreadyHasModule)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Already have module in category %d — use SWAP instead"), (int32)Module.Category);
		return;
	}

	// Check resources available (use repair cost as install cost for offline modules)
	TArray<FOLCResourceAmount> Cost = Module.RepairCost;
	if (!DataSubsystem->CanAffordBuild(Cost))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Insufficient resources to install %s"), *Module.DisplayName.ToString());
		return;
	}

	// Deduct cost and apply effects
	DataSubsystem->ConsumeResourcesForBuild(Cost);

	// Update module state in available modules list using helper method
	DataSubsystem->UpdateModuleState(Module.DisplayName, EOLCModuleState::Installed, 1.0f);

	// Add to installed modules using helper method
	FOLCShipModuleViewData InstalledMod = Module;
	InstalledMod.State = EOLCModuleState::Installed;
	InstalledMod.IntegrityPercent = 1.0f;
	DataSubsystem->AddInstalledModule(InstalledMod);

	// Apply module-specific effects
	switch (Module.Category)
	{
		case EOLCShipModuleCategory::Drives:
			DataSubsystem->SetDriveTier(Module.TIRTier);
			DataSubsystem->SetDriveStatus(EOLCModuleState::Installed);
			break;
		case EOLCShipModuleCategory::Protection:
			if (Module.PowerConsumption < 0.0f)
			{
				DataSubsystem->SetShieldStatus(EOLCModuleState::Installed);
			}
			break;
		case EOLCShipModuleCategory::Storage:
			DataSubsystem->AddStorageBonus(200);
			break;
		default:
			break;
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Installed module: %s (category: %d)"), *Module.DisplayName.ToString(), (int32)Module.Category);

	// Refresh display
	RefreshModuleCatalog();
	UpdateHullSlots();
}

void UOLCShipBuilderWidget::OnSwapClicked()
{
	if (!DataSubsystem || SelectedModuleIndex < 0) return;
	if (SelectedModuleIndex >= CurrentCategoryModules.Num()) return;

	const FOLCShipModuleViewData& NewModule = CurrentCategoryModules[SelectedModuleIndex];

	// Check TIR compatibility
	if (NewModule.TIRTier > DataSubsystem->GetColonyTIR())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Cannot swap — requires TIR %d, colony TIR is %d"),
			NewModule.TIRTier, DataSubsystem->GetColonyTIR());
		return;
	}

	// Remove old module in same category and refund 50% using helper method
	TArray<FOLCShipModuleViewData> Installed = DataSubsystem->GetInstalledModules(NewModule.Category);
	for (const auto& OldModule : Installed)
	{
		if (OldModule.Category == NewModule.Category)
		{
			UE_LOG(LogOLC, Log, TEXT("[OLC] Swapped out module: %s"), *OldModule.DisplayName.ToString());

			// Get refund using helper method
			TArray<FOLCResourceAmount> Refund = DataSubsystem->GetModuleRefund(OldModule);
			for (const auto& RefundEntry : Refund)
			{
				DataSubsystem->AddResource(RefundEntry.ResourceType, RefundEntry.CurrentValue);
			}

			break;
		}
	}

	// Remove old installed module by category using helper method
	DataSubsystem->RemoveInstalledModuleByCategory(NewModule.Category);

	// Check resources for new module
	TArray<FOLCResourceAmount> Cost = NewModule.RepairCost;
	if (!DataSubsystem->CanAffordBuild(Cost))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Insufficient resources to swap to %s"), *NewModule.DisplayName.ToString());
		return;
	}

	// Deduct cost
	DataSubsystem->ConsumeResourcesForBuild(Cost);

	// Update module state in available modules list using helper method
	DataSubsystem->UpdateModuleState(NewModule.DisplayName, EOLCModuleState::Installed, 1.0f);

	// Add new module to installed using helper method
	FOLCShipModuleViewData InstalledMod = NewModule;
	InstalledMod.State = EOLCModuleState::Installed;
	InstalledMod.IntegrityPercent = 1.0f;
	DataSubsystem->AddInstalledModule(InstalledMod);

	// Apply module-specific effects (same as install)
	switch (NewModule.Category)
	{
		case EOLCShipModuleCategory::Drives:
			DataSubsystem->SetDriveTier(NewModule.TIRTier);
			DataSubsystem->SetDriveStatus(EOLCModuleState::Installed);
			break;
		case EOLCShipModuleCategory::Protection:
			if (NewModule.PowerConsumption < 0.0f)
			{
				DataSubsystem->SetShieldStatus(EOLCModuleState::Installed);
			}
			break;
		case EOLCShipModuleCategory::Storage:
			DataSubsystem->AddStorageBonus(200);
			break;
		default:
			break;
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Swapped to module: %s (category: %d)"), *NewModule.DisplayName.ToString(), (int32)NewModule.Category);

	// Refresh display
	RefreshModuleCatalog();
	UpdateHullSlots();
}

void UOLCShipBuilderWidget::OnRepairClicked()
{
	if (!DataSubsystem) return;

	// Delegate to subsystem — it handles cost deduction, module state update, and drive status.
	bool bSuccess = DataSubsystem->RepairModule(SelectedCategory);

	if (bSuccess)
	{
		// Update drive status if applicable
		if (SelectedCategory == EOLCShipModuleCategory::Drives)
		{
			DataSubsystem->SetDriveStatus(EOLCModuleState::Installed);
		}

		// Refresh display
		RefreshModuleCatalog();
		UpdateHullSlots();
	}
	else
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] OnRepairClicked: no damaged module found in category %d"), (int32)SelectedCategory);
	}
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildHullPanel()
{
	// Helper to get slot color for overlay indicators
	auto GetSlotColorForOverlay = [this](EOLCShipModuleCategory Category) -> FLinearColor
	{
		if (!DataSubsystem) return OLCStyleColors::DangerRed;

		switch (Category)
		{
			case EOLCShipModuleCategory::Drives:
				return (DataSubsystem->GetDriveStatus() == EOLCModuleState::Installed)
					? OLCStyleColors::ValidGreen
					: (DataSubsystem->GetDriveStatus() == EOLCModuleState::Damaged ? OLCStyleColors::WarningYellow : OLCStyleColors::DangerRed);

			case EOLCShipModuleCategory::Protection:
				return (DataSubsystem->GetShieldStatus() == EOLCModuleState::Installed)
					? OLCStyleColors::ValidGreen
					: OLCStyleColors::DangerRed;

			case EOLCShipModuleCategory::Storage:
				return (DataSubsystem->GetStorageBonusPerResource() > 0)
					? OLCStyleColors::ValidGreen
					: OLCStyleColors::DangerRed;

			case EOLCShipModuleCategory::Weapons:
				return OLCStyleColors::DangerRed;

			default:
				return OLCStyleColors::DangerRed;
		}
	};

	// Slot labels for each attachment point
	const FText DriveLabel = LOCTEXT("Hull_Drive", "DRIVE");
	const FText ShieldLabel = LOCTEXT("Hull_Shield", "SHIELD");
	const FText StorageLabel = LOCTEXT("Hull_Storage", "STORAGE");
	const FText WeaponsLabel = LOCTEXT("Hull_Weapons", "WEAPONS");

	// Slot colors for overlay
	const FLinearColor DriveColor = GetSlotColorForOverlay(EOLCShipModuleCategory::Drives);
	const FLinearColor ShieldColor = GetSlotColorForOverlay(EOLCShipModuleCategory::Protection);
	const FLinearColor StorageColor = GetSlotColorForOverlay(EOLCShipModuleCategory::Storage);
	const FLinearColor WeaponsColor = GetSlotColorForOverlay(EOLCShipModuleCategory::Weapons);

	// Load crashed ship image (once)
	static FName ShipBrushName(TEXT("CrashedShipHull"));
	static TUniquePtr<FSlateDynamicImageBrush> ShipBrush;
	static bool bShipLoaded = false;

	if (!bShipLoaded)
	{
		FString ShipImagePath = FPaths::ConvertRelativePathToFull(
			FPaths::ProjectDir() / TEXT("../../UE5/Assets/Terrain/Desert/Props/StartLocation/TX_Start_CrashedShip.png"));

		if (FPaths::FileExists(ShipImagePath))
		{
			ShipBrush = MakeUnique<FSlateDynamicImageBrush>(ShipBrushName, FVector2D(280.0f, 320.0f));
			UE_LOG(LogOLC, Log, TEXT("[OLC] Ship Builder: Loaded crashed ship image from %s"), *ShipImagePath);
		}
		else
		{
			UE_LOG(LogOLC, Warning, TEXT("[OLC] Ship Builder: Crashed ship image not found — using placeholder"));
		}
		bShipLoaded = true;
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HullTitle", "DROPSHIP HULL"))
			.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.015f, 0.025f, 0.03f, 0.95f))
			.Padding(FMargin(0.0f))
			[
				SNew(SOverlay)
				// Dropship hull image background (or placeholder)
				+ SOverlay::Slot()
				[
					SNew(SBox)
					.WidthOverride(280.0f)
					.HeightOverride(320.0f)
					[
						SNew(SImage)
						.Image(ShipBrush.Get())
						.ColorAndOpacity(FLinearColor(0.95f, 0.95f, 0.95f, 0.6f))
					]
				]
				// Slot indicator: Drive (left side)
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(FMargin(30.0f, 0.0f, 0.0f, 0.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(DriveColor.R, DriveColor.G, DriveColor.B, 0.9f))
						.Padding(FMargin(6.0f))
						[
							SNew(SBox)
							.WidthOverride(12.0f)
							.HeightOverride(12.0f)
						]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(DriveLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
				]
				// Slot indicator: Shield (center top)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				.Padding(FMargin(0.0f, 20.0f, 0.0f, 0.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(ShieldColor.R, ShieldColor.G, ShieldColor.B, 0.9f))
						.Padding(FMargin(6.0f))
						[
							SNew(SBox)
							.WidthOverride(12.0f)
							.HeightOverride(12.0f)
						]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(ShieldLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
				]
				// Slot indicator: Storage (center bottom)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Bottom)
				.Padding(FMargin(0.0f, 0.0f, 0.0f, 20.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(StorageColor.R, StorageColor.G, StorageColor.B, 0.9f))
						.Padding(FMargin(6.0f))
						[
							SNew(SBox)
							.WidthOverride(12.0f)
							.HeightOverride(12.0f)
						]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(StorageLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
				]
				// Slot indicator: Weapons (right side)
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(FMargin(0.0f, 30.0f, 0.0f, 0.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(WeaponsColor.R, WeaponsColor.G, WeaponsColor.B, 0.9f))
						.Padding(FMargin(6.0f))
						[
							SNew(SBox)
							.WidthOverride(12.0f)
							.HeightOverride(12.0f)
						]
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(WeaponsLabel)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
					]
				]
			]
		];
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildModuleCatalog()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("CatalogTitle", "MODULE CATALOG"))
			.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[ BuildCategoryTabs() ]
		+ SVerticalBox::Slot().FillHeight(1.0f).Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[ BuildModuleList() ]
		+ SVerticalBox::Slot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[ BuildDetailsPanel() ];
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildCategoryTabs()
{
	TArray<EOLCShipModuleCategory> Categories = {
		EOLCShipModuleCategory::Drives,
		EOLCShipModuleCategory::Protection,
		EOLCShipModuleCategory::Storage,
		EOLCShipModuleCategory::Weapons
	};

	TSharedRef<SHorizontalBox> Tabs = SNew(SHorizontalBox);

	for (const auto& Category : Categories)
	{
		FText TabText;
		switch (Category)
		{
			case EOLCShipModuleCategory::Drives:      TabText = LOCTEXT("Tab_Drives", "DRIVES"); break;
			case EOLCShipModuleCategory::Protection:  TabText = LOCTEXT("Tab_Protection", "PROTECTION"); break;
			case EOLCShipModuleCategory::Storage:     TabText = LOCTEXT("Tab_Storage", "STORAGE"); break;
			case EOLCShipModuleCategory::Weapons:     TabText = LOCTEXT("Tab_Weapons", "WEAPONS"); break;
			default:                                  TabText = FText(); break;
		}

		const bool bSelected = (Category == SelectedCategory);
		FLinearColor TabColor = bSelected ? OLCStyleColors::PrimaryOrange : OLCStyleColors::TextDim;

		Tabs->AddSlot()
			.FillWidth(1.0f)
			.Padding(2.0f)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this, Category]()
					{
						SelectedCategory = Category;
						SelectedModuleIndex = -1;
						RefreshModuleCatalog();
						return FReply::Handled();
					})
				[
					SNew(STextBlock)
					.Text(TabText)
					.ColorAndOpacity(TabColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
			];
	}

	return Tabs;
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildModuleList()
{
	if (CurrentCategoryModules.IsEmpty())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.015f, 0.025f, 0.03f, 0.95f))
			.Padding(FMargin(16.0f))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NoModules", "No modules in this category"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			];
	}

	TSharedRef<SVerticalBox> List = SNew(SVerticalBox);

	for (int32 i = 0; i < CurrentCategoryModules.Num(); ++i)
	{
		List->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 2.0f)
			[ BuildModuleCard(CurrentCategoryModules[i], i == SelectedModuleIndex) ];
	}

	return List;
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildModuleCard(const FOLCShipModuleViewData& Module, bool bSelected)
{
	FText StateText = GetStateText(Module.State);
	FLinearColor StateColor = (Module.State == EOLCModuleState::Installed)
		? OLCStyleColors::ValidGreen
		: (Module.State == EOLCModuleState::Damaged ? OLCStyleColors::WarningYellow : OLCStyleColors::DangerRed);

	return SNew(SBorder)
		.BorderBackgroundColor(bSelected
			? FLinearColor(0.09f, 0.105f, 0.12f, 0.96f)
			: FLinearColor(0.025f, 0.04f, 0.05f, 0.90f))
		.Padding(FMargin(8.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(Module.DisplayName)
				.ColorAndOpacity(OLCStyleColors::TextWhite)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
			[
				SNew(STextBlock)
				.Text(FText::Format(FText::FromString(TEXT("TIR {0} | Slots: {1}")),
					FText::AsNumber(Module.TIRTier),
					FText::AsNumber(Module.HullSlotsRequired)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
			[
				SNew(STextBlock)
				.Text(FText::Format(FText::FromString(TEXT("Power: {0}")),
					FText::AsNumber(Module.PowerConsumption)))
				.ColorAndOpacity(Module.PowerConsumption < 0.0f ? OLCStyleColors::ValidGreen : OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
			[
				SNew(STextBlock)
				.Text(StateText)
				.ColorAndOpacity(StateColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9))
			]
		];
}

TSharedRef<SWidget> UOLCShipBuilderWidget::BuildDetailsPanel()
{
	if (SelectedModuleIndex < 0 || SelectedModuleIndex >= CurrentCategoryModules.Num())
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.015f, 0.025f, 0.03f, 0.95f))
			.Padding(FMargin(12.0f))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Details_NoSelection", "— SELECT A MODULE —"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			];
	}

	const FOLCShipModuleViewData& Module = CurrentCategoryModules[SelectedModuleIndex];

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.015f, 0.025f, 0.03f, 0.95f))
			.Padding(FMargin(12.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(Module.DisplayName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
				[
					SNew(STextBlock)
					.Text(Module.EffectDescription)
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
					.AutoWrapText(true)
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(FMargin(2.0f))
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]()
					{
						OnInstallClicked();
						return FReply::Handled();
					})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Btn_Install", "INSTALL"))
					.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(FMargin(2.0f))
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]()
					{
						OnSwapClicked();
						return FReply::Handled();
					})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Btn_Swap", "SWAP"))
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(FMargin(2.0f))
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked_Lambda([this]()
					{
						OnRepairClicked();
						return FReply::Handled();
					})
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Btn_Repair", "REPAIR"))
					.ColorAndOpacity(OLCStyleColors::WarningYellow)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
			]
		];
}

void UOLCShipBuilderWidget::UpdateHullSlots()
{
	if (!DataSubsystem) return;

	// Update slot colors based on subsystem state
	// Drives: Damaged = yellow, Installed = green, Offline = red
	FLinearColor DriveColor = (DataSubsystem->GetDriveStatus() == EOLCModuleState::Installed)
		? OLCStyleColors::ValidGreen
		: (DataSubsystem->GetDriveStatus() == EOLCModuleState::Damaged ? OLCStyleColors::WarningYellow : OLCStyleColors::DangerRed);

	// Shields: Installed = green, Offline = red
	FLinearColor ShieldColor = (DataSubsystem->GetShieldStatus() == EOLCModuleState::Installed)
		? OLCStyleColors::ValidGreen
		: OLCStyleColors::DangerRed;

	UE_LOG(LogOLC, Verbose, TEXT("[OLC] Ship Builder: Drive=%d, Shield=%d"),
		(int32)DataSubsystem->GetDriveStatus(), (int32)DataSubsystem->GetShieldStatus());
}

void UOLCShipBuilderWidget::RefreshModuleCatalog()
{
	if (!DataSubsystem) return;

	TArray<FOLCShipModuleViewData> AllModules = DataSubsystem->GetAvailableModules();
	CurrentCategoryModules.Reset();

	for (const auto& Module : AllModules)
	{
		if (Module.Category == SelectedCategory)
		{
			CurrentCategoryModules.Add(Module);
		}
	}

	SelectedModuleIndex = -1;
}

void UOLCShipBuilderWidget::UpdateDetailsPanel()
{
	// Details panel is rebuilt on each tick when a module is selected
}

FLinearColor UOLCShipBuilderWidget::GetSlotColor(EOLCShipModuleCategory Category) const
{
	if (!DataSubsystem) return OLCStyleColors::DangerRed;

	switch (Category)
	{
		case EOLCShipModuleCategory::Drives:
			return (DataSubsystem->GetDriveStatus() == EOLCModuleState::Installed)
				? OLCStyleColors::ValidGreen
				: (DataSubsystem->GetDriveStatus() == EOLCModuleState::Damaged ? OLCStyleColors::WarningYellow : OLCStyleColors::DangerRed);

		case EOLCShipModuleCategory::Protection:
			return (DataSubsystem->GetShieldStatus() == EOLCModuleState::Installed)
				? OLCStyleColors::ValidGreen
				: OLCStyleColors::DangerRed;

		case EOLCShipModuleCategory::Storage:
			return (DataSubsystem->GetStorageBonusPerResource() > 0)
				? OLCStyleColors::ValidGreen
				: OLCStyleColors::DangerRed;

		case EOLCShipModuleCategory::Weapons:
			// Weapons not yet implemented — show red (empty)
			return OLCStyleColors::DangerRed;

		default:
			return OLCStyleColors::DangerRed;
	}
}

FText UOLCShipBuilderWidget::GetStateText(EOLCModuleState State) const
{
	switch (State)
	{
		case EOLCModuleState::Installed: return LOCTEXT("State_Installed", "INSTALLED");
		case EOLCModuleState::Damaged:   return LOCTEXT("State_Damaged", "DAMAGED");
		case EOLCModuleState::Offline:   return LOCTEXT("State_Offline", "OFFLINE");
		default:                         return FText();
	}
}

#undef LOCTEXT_NAMESPACE
