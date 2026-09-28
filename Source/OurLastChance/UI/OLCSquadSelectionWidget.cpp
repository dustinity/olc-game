#include "UI/OLCSquadSelectionWidget.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCUnitBase.h"
#include "World/OLCProductionFacility.h"

#define LOCTEXT_NAMESPACE "OLCSquadSelectionWidget"

class SOLCSquadDragDropSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOLCSquadDragDropSlot) {}
		SLATE_ARGUMENT(AOLCUnitBase*, DragUnit)
		SLATE_ARGUMENT(int32, SourceSlot)
		SLATE_ARGUMENT(int32, TargetSlot)
		SLATE_ARGUMENT(TFunction<void(int32)>, OnClicked)
		SLATE_ARGUMENT(TFunction<FReply(TSharedPtr<ODragDrop>, int32)>, OnDropped)
		SLATE_ARGUMENT(TFunction<bool(TSharedPtr<ODragDrop>, int32)>, IsValidDrop)
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		DragUnit = InArgs._DragUnit;
		SourceSlot = InArgs._SourceSlot;
		TargetSlot = InArgs._TargetSlot;
		OnClicked = InArgs._OnClicked;
		OnDropped = InArgs._OnDropped;
		IsValidDrop = InArgs._IsValidDrop;
		ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[ InArgs._Content.Widget ]
			+ SOverlay::Slot()
			[
				SAssignNew(HighlightBorder, SBorder)
				.BorderBackgroundColor(FLinearColor::Transparent)
				.Visibility(EVisibility::HitTestInvisible)
				[ SNullWidget::NullWidget ]
			]
		];
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && OnClicked)
		{
			OnClicked(TargetSlot);
			return FReply::Handled();
		}
		return FReply::Unhandled();
	}

	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		return DragUnit ? FReply::Handled().BeginDragDrop(ODragDrop::New(DragUnit, SourceSlot)) : FReply::Unhandled();
	}

	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		if (HighlightBorder.IsValid())
		{
			HighlightBorder->SetBorderBackgroundColor(FLinearColor::Transparent);
		}
		return OnDropped ? OnDropped(DragDropEvent.GetOperationAs<ODragDrop>(), TargetSlot) : FReply::Unhandled();
	}

	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		if (!HighlightBorder.IsValid()) return;
		const bool bValid = IsValidDrop ? IsValidDrop(DragDropEvent.GetOperationAs<ODragDrop>(), TargetSlot) : true;
		HighlightBorder->SetBorderBackgroundColor(bValid
			? FLinearColor(0.2f, 1.0f, 0.2f, 0.35f)
			: FLinearColor(1.0f, 0.2f, 0.2f, 0.35f));
	}

	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override
	{
		if (HighlightBorder.IsValid())
		{
			HighlightBorder->SetBorderBackgroundColor(FLinearColor::Transparent);
		}
	}

private:
	AOLCUnitBase* DragUnit = nullptr;
	int32 SourceSlot = -1;
	int32 TargetSlot = -1;
	TFunction<void(int32)> OnClicked;
	TFunction<FReply(TSharedPtr<ODragDrop>, int32)> OnDropped;
	TFunction<bool(TSharedPtr<ODragDrop>, int32)> IsValidDrop;
	TSharedPtr<SBorder> HighlightBorder;
};

UOLCSquadSelectionWidget::UOLCSquadSelectionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCSquadSelectionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CanvasSize = MyGeometry.GetLocalSize();

	// Periodically refresh available units from game world.
	if (FMath::RandRange(0, 60) == 0) // ~once per second at 60fps
	{
		RefreshRoster();
	}
}

TSharedRef<SWidget> UOLCSquadSelectionWidget::RebuildWidget()
{
	// Scan world for available units (units not in a squad, alive).
	RefreshRoster();
	EnsureChampionSlot();

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
			// Left panel: unit roster
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			.Padding(0.0f, 70.0f, 520.0f, 80.0f)
			[ BuildUnitRoster() ]
			// Center panel: squad slots (4×4 grid = 16 max)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(0.0f, 70.0f, 320.0f, 80.0f)
			[ BuildSquadSlots() ]
			// Right panel: stats summary + Ready button
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(320.0f, 70.0f, 30.0f, 80.0f)
			[ BuildStatsSummary() ]
		];
}

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildTopBar()
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f, 10.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[ SNew(STextBlock).Text(LOCTEXT("SquadTitle", "SQUAD SELECTION"))
				.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22)) ]
			+ SHorizontalBox::Slot().AutoWidth().Padding(16.0f, 0.0f, 0.0f, 0.0f)
			[ SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("{0} / 16")),
				FText::AsNumber(GetSquadCount())))
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

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildUnitRoster()
{
	if (AvailableUnits.Num() == 0 && ProductionFacilities.Num() == 0)
	{
		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(16.0f))
			[ SNew(STextBlock).Text(LOCTEXT("RosterEmpty", "NO UNITS AVAILABLE"))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)) ];
	}

	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	for (AOLCUnitBase* Unit : AvailableUnits)
	{
		if (!Unit) continue;
		VBox->AddSlot().AutoHeight()
		[ BuildRosterEntry(Unit) ];
	}

	// Units currently being produced aren't AOLCUnitBase actors yet — show them as
	// grayed-out, non-draggable rows with a progress readout.
	for (AOLCProductionFacility* Facility : ProductionFacilities)
	{
		if (!Facility) continue;
		UOLCUnitData* InProgressUnit = nullptr;
		float Progress = 0.0f;
		if (!Facility->GetActiveUnitProduction(InProgressUnit, Progress) || !InProgressUnit) continue;

		VBox->AddSlot().AutoHeight()
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.06f, 0.85f))
			.Padding(FMargin(8.0f, 6.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[ SNew(STextBlock).Text(InProgressUnit->DisplayName)
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Italic", 12)) ]
				+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("IN PRODUCTION {0}%")),
					FText::AsNumber(FMath::RoundToInt(Progress * 100.0f))))
					.ColorAndOpacity(OLCStyleColors::WarningYellow)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
			]
		];
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

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildRosterEntry(AOLCUnitBase* Unit)
{
	if (!Unit) return SNew(SBorder);

	FText UnitName = Unit->UnitData ? Unit->UnitData->DisplayName : FText::FromString(TEXT("Unknown"));

	return SNew(SOLCSquadDragDropSlot)
		.DragUnit(Unit)
		.SourceSlot(-1)
		.TargetSlot(-1)
		.OnDropped([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> FReply {
			if (!Op.IsValid()) return FReply::Unhandled();
			const int32 SourceSlot = Op->GetSourceSlot();
			if (SourceSlot <= 0) return FReply::Unhandled(); // already in roster, or champion (locked)
			RemoveFromSquad(SourceSlot);
			return FReply::Handled();
		})
		.IsValidDrop([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> bool {
			return Op.IsValid() && Op->GetSourceSlot() > 0;
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(8.0f, 6.0f))
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[ SNew(STextBlock).Text(UnitName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12)) ]
				+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
				[ SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("HP:{0} DMG:{1}")),
					FText::AsNumber(FMath::RoundToInt(Unit->UnitData ? Unit->UnitData->MaxHP : 0)),
					FText::AsNumber(FMath::RoundToInt(Unit->UnitData ? Unit->UnitData->AttackDamage : 0))))
					.ColorAndOpacity(OLCStyleColors::TextDim)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) ]
			]
		];
}

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildSquadSlots()
{
	if (SquadSlots.Num() == 0)
	{
		// Show empty 4×4 grid of placeholder slots.
		TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

		for (int32 row = 0; row < 4; row++)
		{
			TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);
			for (int32 col = 0; col < 4; col++)
			{
				int32 Index = row * 4 + col;
				HBox->AddSlot().AutoWidth()
				[ BuildSquadSlot(Index, nullptr) ];
			}
			VBox->AddSlot().AutoHeight()
			[ HBox ];
		}

		return SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
			.Padding(FMargin(12.0f))
			[ VBox ];
	}

	// Show filled slots + empty placeholders.
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	for (int32 row = 0; row < 4; row++)
	{
		TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);
		for (int32 col = 0; col < 4; col++)
		{
			int32 Index = row * 4 + col;
			AOLCUnitBase* Unit = (Index < SquadSlots.Num()) ? SquadSlots[Index] : nullptr;
			HBox->AddSlot().AutoWidth()
			[ BuildSquadSlot(Index, Unit) ];
		}
		VBox->AddSlot().AutoHeight()
		[ HBox ];
	}

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(12.0f))
		[ VBox ];
}

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildSquadSlot(int32 Index, AOLCUnitBase* UnitInSlot)
{
	if (UnitInSlot)
	{
		FText UnitName = UnitInSlot->UnitData ? UnitInSlot->UnitData->DisplayName : FText::FromString(TEXT("Unit"));
		bool bIsChampion = (Index == 0);

		// Champion slot gets a grayed-out appearance to visually distinguish it.
		const FLinearColor SlotBorderColor = bIsChampion
			? FLinearColor(0.35f, 0.35f, 0.38f, 0.85f)  // Desaturated gray for champion
			: OLCStyleColors::TacticalBlue;

		const FLinearColor UnitNameColor = bIsChampion
			? OLCStyleColors::TextDim  // Dimmed text for champion
			: OLCStyleColors::TextWhite;

		return SNew(SOLCSquadDragDropSlot)
			.DragUnit(UnitInSlot)
			.SourceSlot(Index)
			.TargetSlot(Index)
			.OnClicked([this](int32 SlotIndex) { RemoveFromSquad(SlotIndex); })
			.OnDropped([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> FReply {
				if (!Op.IsValid()) return FReply::Unhandled();
				AOLCUnitBase* DroppedUnit = Op->GetDragSource();
				if (!DroppedUnit) return FReply::Unhandled();
				const int32 SourceSlot = Op->GetSourceSlot();
				if (SourceSlot == SlotIndex) return FReply::Unhandled();
				if (SourceSlot == -1)
				{
					if (SlotIndex == 0) return FReply::Unhandled();
					AddToSquad(DroppedUnit);
					return FReply::Handled();
				}
				MoveUnitToSlot(SourceSlot, SlotIndex);
				return FReply::Handled();
			})
			.IsValidDrop([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> bool {
				return Op.IsValid() && IsValidDropTarget(Op->GetDragSource(), Op->GetSourceSlot(), SlotIndex);
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(SlotBorderColor)
				.Padding(FMargin(6.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(UnitName)
						.ColorAndOpacity(UnitNameColor)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10)) ]
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(FText::Format(
						FText::FromString(TEXT("#{0}")),
						FText::AsNumber(Index + 1)))
						.ColorAndOpacity(bIsChampion ? OLCStyleColors::WarningYellow : OLCStyleColors::TextDim)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 9)) ]
				]
			];
	}

	// Empty slot placeholder — also accepts drops.
	return SNew(SOLCSquadDragDropSlot)
		.TargetSlot(Index)
		.OnDropped([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> FReply {
			if (!Op.IsValid()) return FReply::Unhandled();
			AOLCUnitBase* DroppedUnit = Op->GetDragSource();
			if (!DroppedUnit) return FReply::Unhandled();
			const int32 SourceSlot = Op->GetSourceSlot();
			if (SourceSlot == -1)
			{
				if (SlotIndex == 0) return FReply::Unhandled();
				AddToSquad(DroppedUnit);
				return FReply::Handled();
			}
			MoveUnitToSlot(SourceSlot, SlotIndex);
			return FReply::Handled();
		})
		.IsValidDrop([this](TSharedPtr<ODragDrop> Op, int32 SlotIndex) -> bool {
			return Op.IsValid() && IsValidDropTarget(Op->GetDragSource(), Op->GetSourceSlot(), SlotIndex);
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.12f, 0.4f))
			.Padding(FMargin(6.0f))
			[
				SNew(STextBlock).Text(FText::Format(
					FText::FromString(TEXT("#{0}")),
					FText::AsNumber(Index + 1)))
				.ColorAndOpacity(OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
			]
		];
}

TSharedRef<SWidget> UOLCSquadSelectionWidget::BuildStatsSummary()
{
	TSharedRef<SVerticalBox> VBox = SNew(SVerticalBox);

	// Title
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(LOCTEXT("StatsTitle", "SQUAD SUMMARY"))
		.ColorAndOpacity(OLCStyleColors::PrimaryOrange)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18)) ];

	// Unit count
	VBox->AddSlot().AutoHeight().Padding(0.0f, 12.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("UNITS: {0}/16")),
		FText::AsNumber(GetSquadCount())))
		.ColorAndOpacity(OLCStyleColors::TextWhite)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Unit-type breakdown
	VBox->AddSlot().AutoHeight().Padding(0.0f, 0.0f, 0.0f, 4.0f)
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("INF:{0}  VEH:{1}  AIR:{2}")),
		FText::AsNumber(GetUnitTypeCount(EOLCUnitType::Infantry)),
		FText::AsNumber(GetUnitTypeCount(EOLCUnitType::Vehicle)),
		FText::AsNumber(GetUnitTypeCount(EOLCUnitType::Aerial))))
		.ColorAndOpacity(OLCStyleColors::TextDim)
		.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];

	// Total HP — color reflects squad capacity fill (16 max), a quick "are we deploying enough" signal.
	const float CapacityRatio = GetSquadCount() / 16.0f;
	const FLinearColor CapacityColor = CapacityRatio > 0.7f ? OLCStyleColors::ValidGreen
		: (CapacityRatio > 0.3f ? OLCStyleColors::WarningYellow : OLCStyleColors::DangerRed);
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("TOTAL HP: {0}")),
		FText::AsNumber(FMath::RoundToInt(GetTotalHP()))))
		.ColorAndOpacity(CapacityColor)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Total Damage
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("TOTAL DAMAGE: {0}")),
		FText::AsNumber(FMath::RoundToInt(GetTotalDamage()))))
		.ColorAndOpacity(OLCStyleColors::DangerRed)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Total Speed
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("TOTAL SPEED: {0}")),
		FText::AsNumber(FMath::RoundToInt(GetTotalSpeed()))))
		.ColorAndOpacity(FLinearColor(0.3f, 0.7f, 1.0f))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Target race — only shown once an upstream flow (expedition/dungeon-entry) sets
	// TargetRaceFamily. No caller currently feeds this; it's a hook, not a wired stat.
	if (TargetRaceFamily != EOLCRaceFamily::Unknown)
	{
		const UEnum* FamilyEnum = StaticEnum<EOLCRaceFamily>();
		const FText FamilyName = FamilyEnum ? FamilyEnum->GetDisplayNameTextByValue(static_cast<int64>(TargetRaceFamily)) : FText::GetEmpty();
		VBox->AddSlot().AutoHeight().Padding(0.0f, 8.0f, 0.0f, 0.0f)
		[ SNew(STextBlock).Text(FText::Format(
			FText::FromString(TEXT("TARGET: {0}")),
			FamilyName))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11)) ];
	}

	// Ready button
	VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.IsEnabled(GetSquadCount() > 0)
		.OnClicked_Lambda([this]() -> FReply {
			const FOLCSquadDeploymentData Payload = BuildDeploymentPayload();
			if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
			{
				if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
				{
					Data->SetCurrentSquad(Payload);
				}
			}
			OnSquadReady.Broadcast(Payload);
			this->RemoveFromParent();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(GetSquadCount() > 0 ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(24.0f, 12.0f))
			[ SNew(STextBlock).Text(LOCTEXT("BtnReady", "READY"))
				.ColorAndOpacity(GetSquadCount() > 0 ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16)) ]
		]
	];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(FMargin(16.0f))
		[ VBox ];
}

void UOLCSquadSelectionWidget::RefreshRoster()
{
	if (!GetWorld()) return;

	TArray<AActor*> FoundUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCUnitBase::StaticClass(), FoundUnits);

	AvailableUnits.Reset();
	for (AActor* Actor : FoundUnits)
	{
		AOLCUnitBase* Unit = Cast<AOLCUnitBase>(Actor);
		if (!Unit) continue;
		// Exclude units already in squad.
		bool bAlreadySquad = false;
		for (AOLCUnitBase* SquadUnit : SquadSlots)
		{
			if (SquadUnit == Unit) { bAlreadySquad = true; break; }
		}
		if (!bAlreadySquad && !Unit->IsDead())
		{
			AvailableUnits.Add(Unit);
		}
	}

	bHasUnits = AvailableUnits.Num() > 0;

	TArray<AActor*> FoundFacilities;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCProductionFacility::StaticClass(), FoundFacilities);
	ProductionFacilities.Reset();
	for (AActor* Actor : FoundFacilities)
	{
		if (AOLCProductionFacility* Facility = Cast<AOLCProductionFacility>(Actor))
		{
			ProductionFacilities.Add(Facility);
		}
	}

	EnsureChampionSlot();
}

void UOLCSquadSelectionWidget::AddToSquad(AOLCUnitBase* Unit)
{
	if (!Unit) return;

	// Check if unit is already in squad.
	for (AOLCUnitBase* ExistingUnit : SquadSlots)
	{
		if (ExistingUnit == Unit)
		{
			return; // Unit already in squad, do nothing.
		}
	}

	// Check squad size limit (max 16).
	if (SquadSlots.Num() >= 16)
	{
		return;
	}

	// Handle champion units: always place at slot 0.
	if (Unit->UnitData && Unit->UnitData->UnitType == EOLCUnitType::Champion)
	{
		// Remove champion from any existing position.
		SquadSlots.Remove(Unit);
		// Insert at slot 0.
		SquadSlots.Insert(Unit, 0);
		ChampionUnit = Unit;
	}
	else
	{
		// Find first empty slot (skip slot 0 if champion is present).
		int32 InsertIndex = 0;
		if (SquadSlots.Num() > 0 && SquadSlots[0] && SquadSlots[0]->UnitData && SquadSlots[0]->UnitData->UnitType == EOLCUnitType::Champion)
		{
			InsertIndex = 1; // Skip champion slot.
		}

		// Find first null slot from InsertIndex.
		while (InsertIndex < SquadSlots.Num() && SquadSlots[InsertIndex] != nullptr)
		{
			InsertIndex++;
		}

		// If no empty slot found, add to end.
		if (InsertIndex >= SquadSlots.Num())
		{
			SquadSlots.Add(Unit);
		}
		else
		{
			SquadSlots[InsertIndex] = Unit;
		}
	}

	// Remove unit from AvailableUnits.
	AvailableUnits.Remove(Unit);

	InvalidateLayoutAndVolatility();
}

void UOLCSquadSelectionWidget::MoveUnitToSlot(int32 FromIndex, int32 ToIndex)
{
	if (FromIndex < 0 || FromIndex >= SquadSlots.Num()) return;
	if (ToIndex < 0 || ToIndex >= SquadSlots.Num()) return;
	if (FromIndex == ToIndex) return;

	AOLCUnitBase* Unit = SquadSlots[FromIndex];
	if (!Unit) return;

	// Protect champion slot: cannot move champion out of slot 0.
	if (FromIndex == 0 && Unit->UnitData && Unit->UnitData->UnitType == EOLCUnitType::Champion)
	{
		return;
	}

	// Cannot drop onto champion slot unless it's the champion itself.
	if (ToIndex == 0)
	{
		if (!SquadSlots[0] || SquadSlots[0] != Unit)
		{
			return;
		}
	}

	// Remove from source.
	SquadSlots.RemoveAt(FromIndex);

	// Adjust target index if source was before target.
	int32 AdjustedToIndex = ToIndex;
	if (FromIndex < ToIndex)
	{
		AdjustedToIndex--;
	}

	// Clamp to valid range.
	if (AdjustedToIndex < 0) AdjustedToIndex = 0;
	if (AdjustedToIndex > SquadSlots.Num()) AdjustedToIndex = SquadSlots.Num();

	// Insert at target.
	SquadSlots.Insert(Unit, AdjustedToIndex);

	InvalidateLayoutAndVolatility();
}

void UOLCSquadSelectionWidget::EnsureChampionSlot()
{
	for (AOLCUnitBase* Unit : AvailableUnits)
	{
		if (Unit && Unit->UnitData && Unit->UnitData->UnitType == EOLCUnitType::Champion)
		{
			ChampionUnit = Unit;
			if (!SquadSlots.Contains(Unit))
			{
				SquadSlots.Insert(Unit, 0);
			}
			else
			{
				SquadSlots.Remove(Unit);
				SquadSlots.Insert(Unit, 0);
			}
			break;
		}
	}
}

void UOLCSquadSelectionWidget::RemoveFromSquad(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= SquadSlots.Num()) return;
	if (SlotIndex == 0 && SquadSlots[0] && SquadSlots[0]->UnitData && SquadSlots[0]->UnitData->UnitType == EOLCUnitType::Champion)
	{
		return;
	}

	SquadSlots.RemoveAt(SlotIndex);
	InvalidateLayoutAndVolatility();
}

float UOLCSquadSelectionWidget::GetTotalHP() const
{
	float Total = 0.0f;
	for (AOLCUnitBase* Unit : SquadSlots)
	{
		if (!Unit || !Unit->UnitData) continue;
		Total += Unit->UnitData->MaxHP;
	}
	return Total;
}

float UOLCSquadSelectionWidget::GetTotalDamage() const
{
	float Total = 0.0f;
	for (AOLCUnitBase* Unit : SquadSlots)
	{
		if (!Unit || !Unit->UnitData) continue;
		Total += Unit->UnitData->AttackDamage;
	}
	return Total;
}

float UOLCSquadSelectionWidget::GetTotalSpeed() const
{
	float Total = 0.0f;
	for (AOLCUnitBase* Unit : SquadSlots)
	{
		if (!Unit || !Unit->UnitData) continue;
		Total += Unit->UnitData->MovementSpeed;
	}
	return Total;
}

int32 UOLCSquadSelectionWidget::GetUnitTypeCount(EOLCUnitType Type) const
{
	int32 Count = 0;
	for (AOLCUnitBase* Unit : SquadSlots)
	{
		if (Unit && Unit->UnitData && Unit->UnitData->UnitType == Type)
		{
			Count++;
		}
	}
	return Count;
}

bool UOLCSquadSelectionWidget::IsValidDropTarget(AOLCUnitBase* DraggedUnit, int32 SourceSlot, int32 TargetSlot) const
{
	if (!DraggedUnit) return false;
	if (SourceSlot == TargetSlot) return false;

	// The champion's locked slot 0 only ever accepts the champion itself (a no-op drop).
	if (TargetSlot == 0)
	{
		return SourceSlot == 0;
	}

	if (SourceSlot == -1)
	{
		// From roster: needs room, and the champion is never offered from the roster
		// (EnsureChampionSlot keeps it out of AvailableUnits), but guard anyway.
		return SquadSlots.Num() < 16 && DraggedUnit != ChampionUnit;
	}

	// Moving an existing squad member — the champion is locked in slot 0 and can't be moved out.
	return SourceSlot != 0;
}

FOLCSquadDeploymentData UOLCSquadSelectionWidget::BuildDeploymentPayload() const
{
	FOLCSquadDeploymentData Payload;
	Payload.ChampionSlotIndex = 0;

	for (AOLCUnitBase* Unit : SquadSlots)
	{
		if (!Unit) continue;
		Payload.UnitIds.Add(Unit->UnitData ? Unit->UnitData->UnitId : FString());
		Payload.SquadUnits.Add(Unit);
	}

	Payload.bIsValid = Payload.SquadUnits.Num() > 0;
	return Payload;
}

#undef LOCTEXT_NAMESPACE
