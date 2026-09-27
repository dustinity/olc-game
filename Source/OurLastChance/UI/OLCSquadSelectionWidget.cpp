#include "UI/OLCSquadSelectionWidget.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OLCMenuPlayerController.h"
#include "World/OLCUnitBase.h"

#define LOCTEXT_NAMESPACE "OLCSquadSelectionWidget"

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
	if (AvailableUnits.Num() == 0)
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

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.OnClicked_Lambda([this, Unit]() -> FReply {
			AddToSquad(Unit);
			return FReply::Handled();
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

		return SNew(SButton)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.ContentPadding(FMargin(0.0f))
			.OnClicked_Lambda([this, Index]() -> FReply {
				RemoveFromSquad(Index);
				return FReply::Handled();
			})
			[
				SNew(SBorder)
				.BorderBackgroundColor(bIsChampion ? OLCStyleColors::ValidGreen : OLCStyleColors::TacticalBlue)
				.Padding(FMargin(6.0f))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[ SNew(STextBlock).Text(UnitName)
						.ColorAndOpacity(OLCStyleColors::TextWhite)
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

	// Empty slot placeholder.
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.12f, 0.4f))
		.Padding(FMargin(6.0f))
		[
			SNew(STextBlock).Text(FText::Format(
				FText::FromString(TEXT("#{0}")),
				FText::AsNumber(Index + 1)))
			.ColorAndOpacity(OLCStyleColors::TextDim)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
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

	// Total HP
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("TOTAL HP: {0}")),
		FText::AsNumber(FMath::RoundToInt(GetTotalHP()))))
		.ColorAndOpacity(OLCStyleColors::ValidGreen)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Total Damage
	VBox->AddSlot().AutoHeight()
	[ SNew(STextBlock).Text(FText::Format(
		FText::FromString(TEXT("TOTAL DAMAGE: {0}")),
		FText::AsNumber(FMath::RoundToInt(GetTotalDamage()))))
		.ColorAndOpacity(OLCStyleColors::DangerRed)
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14)) ];

	// Ready button
	VBox->AddSlot().AutoHeight().Padding(0.0f, 24.0f, 0.0f, 0.0f)
	[
		SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.IsEnabled(GetSquadCount() > 0)
		.OnClicked_Lambda([this]() -> FReply {
			OnSquadReady.Broadcast();
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
}

void UOLCSquadSelectionWidget::AddToSquad(AOLCUnitBase* Unit)
{
	if (!Unit || SquadSlots.Num() >= 16) return;

	SquadSlots.Add(Unit);
	InvalidateLayoutAndVolatility();
}

void UOLCSquadSelectionWidget::RemoveFromSquad(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= SquadSlots.Num()) return;

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

#undef LOCTEXT_NAMESPACE
