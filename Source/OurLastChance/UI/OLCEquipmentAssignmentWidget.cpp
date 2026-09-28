#include "UI/OLCEquipmentAssignmentWidget.h"

#include "Core/OLCEquipmentSubsystem.h"
#include "UI/OLCSharedWidgets.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "World/OLCUnitBase.h"
#include "World/OLCUnitEquipmentComponent.h"

#define LOCTEXT_NAMESPACE "OLCEquipmentAssignmentWidget"

void UOLCEquipmentAssignmentWidget::InitializeForUnit(AOLCUnitBase* InUnit, const FString& InFactionId, int32 InColonyTIR)
{
	TargetUnit = InUnit;
	FactionId = InFactionId;
	ColonyTIR = InColonyTIR;
	InvalidateLayoutAndVolatility();
}

bool UOLCEquipmentAssignmentWidget::AssignEquipmentToSlot(const FOLCEquipmentInstance& Instance, EOLCEquipmentSlotType SlotType)
{
	if (!TargetUnit || !TargetUnit->GetEquipmentComponent())
	{
		return false;
	}
	return TargetUnit->GetEquipmentComponent()->AssignEquipment(Instance, SlotType, FactionId, ColonyTIR);
}

TSharedRef<SWidget> UOLCEquipmentAssignmentWidget::RebuildWidget()
{
	TSharedRef<SVerticalBox> Slots = SNew(SVerticalBox);
	Slots->AddSlot().AutoHeight()[BuildSlot(EOLCEquipmentSlotType::Primary)];
	Slots->AddSlot().AutoHeight()[BuildSlot(EOLCEquipmentSlotType::Armor)];
	Slots->AddSlot().AutoHeight()[BuildSlot(EOLCEquipmentSlotType::Utility)];
	Slots->AddSlot().AutoHeight()[BuildSlot(EOLCEquipmentSlotType::Secondary)];
	Slots->AddSlot().AutoHeight()[BuildSlot(EOLCEquipmentSlotType::Tertiary)];

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.006f, 0.012f, 0.016f, 0.98f))
		.Padding(12.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(0.45f)[Slots]
			+ SHorizontalBox::Slot().FillWidth(0.55f).Padding(12.0f, 0.0f, 0.0f, 0.0f)[BuildInventory()]
		];
}

TSharedRef<SWidget> UOLCEquipmentAssignmentWidget::BuildSlot(EOLCEquipmentSlotType SlotType)
{
	FText ItemName = LOCTEXT("EmptySlot", "EMPTY");
	FLinearColor BorderColor = OLCStyleColors::GunmetalBlack;

	if (TargetUnit && TargetUnit->GetEquipmentComponent())
	{
		const TMap<EOLCEquipmentSlotType, FOLCEquipmentInstance>& Slots = TargetUnit->GetEquipmentComponent()->GetEquipmentSlots();
		if (const FOLCEquipmentInstance* Instance = Slots.Find(SlotType))
		{
			if (Instance->EquipmentData)
			{
				ItemName = Instance->CustomName.IsEmpty() ? Instance->EquipmentData->DisplayName : Instance->CustomName;
				BorderColor = Instance->EquipmentData->GetRarityColor();
			}
		}
	}

	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.OnClicked_Lambda([this, SlotType]() -> FReply
		{
			if (TargetUnit && TargetUnit->GetEquipmentComponent())
			{
				TargetUnit->GetEquipmentComponent()->RemoveEquipment(SlotType);
				InvalidateLayoutAndVolatility();
			}
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(BorderColor)
			.Padding(8.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock).Text(GetSlotLabel(SlotType))
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
				+ SHorizontalBox::Slot().FillWidth(1.0f).HAlign(HAlign_Right)
				[
					SNew(STextBlock).Text(ItemName)
					.ColorAndOpacity(OLCStyleColors::TextWhite)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
				]
			]
		];
}

TSharedRef<SWidget> UOLCEquipmentAssignmentWidget::BuildInventory()
{
	TSharedRef<SVerticalBox> Items = SNew(SVerticalBox);
	UOLCEquipmentSubsystem* EquipmentSubsystem = nullptr;
	if (UGameInstance* GI = GetGameInstance())
	{
		EquipmentSubsystem = GI->GetSubsystem<UOLCEquipmentSubsystem>();
	}

	const EOLCUnitType UnitType = TargetUnit && TargetUnit->GetUnitData() ? TargetUnit->GetUnitData()->UnitType : EOLCUnitType::Infantry;
	const TArray<EOLCEquipmentSlotType> SlotTypes = { EOLCEquipmentSlotType::Primary, EOLCEquipmentSlotType::Secondary, EOLCEquipmentSlotType::Tertiary, EOLCEquipmentSlotType::Armor, EOLCEquipmentSlotType::Utility };

	if (EquipmentSubsystem)
	{
		for (EOLCEquipmentSlotType SlotType : SlotTypes)
		{
			for (const FOLCEquipmentInstance& Instance : EquipmentSubsystem->GetAvailableForSlot(SlotType, UnitType, FactionId, ColonyTIR))
			{
				if (!Instance.EquipmentData)
				{
					continue;
				}
				const FOLCEquipmentInstance LocalInstance = Instance;
				Items->AddSlot().AutoHeight()
				[
					SNew(SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.OnClicked_Lambda([this, LocalInstance, SlotType]() -> FReply
					{
						AssignEquipmentToSlot(LocalInstance, SlotType);
						InvalidateLayoutAndVolatility();
						return FReply::Handled();
					})
					[
						SNew(SBorder)
						.BorderBackgroundColor(LocalInstance.EquipmentData->GetRarityColor())
						.Padding(7.0f)
						[
							SNew(STextBlock)
							.Text(LocalInstance.EquipmentData->DisplayName)
							.ColorAndOpacity(OLCStyleColors::TextWhite)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 11))
						]
					]
				];
			}
		}
	}

	return SNew(SScrollBox) + SScrollBox::Slot()[Items];
}

FText UOLCEquipmentAssignmentWidget::GetSlotLabel(EOLCEquipmentSlotType SlotType) const
{
	switch (SlotType)
	{
		case EOLCEquipmentSlotType::Primary: return LOCTEXT("Primary", "PRIMARY");
		case EOLCEquipmentSlotType::Secondary: return LOCTEXT("Secondary", "SECONDARY");
		case EOLCEquipmentSlotType::Tertiary: return LOCTEXT("Tertiary", "TERTIARY");
		case EOLCEquipmentSlotType::Armor: return LOCTEXT("Armor", "ARMOR");
		case EOLCEquipmentSlotType::Utility: return LOCTEXT("Utility", "UTILITY");
	}
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE

