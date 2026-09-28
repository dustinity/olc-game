#include "World/OLCUnitEquipmentComponent.h"

#include "Core/OLCEquipmentSubsystem.h"
#include "World/OLCUnitBase.h"

UOLCUnitEquipmentComponent::UOLCUnitEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOLCUnitEquipmentComponent::InitializeDefaultSlots(EOLCUnitType UnitType, bool bChampion)
{
	EquipmentSlots.Reset();
	EquipmentSlots.Add(EOLCEquipmentSlotType::Primary);
	EquipmentSlots.Add(EOLCEquipmentSlotType::Armor);
	EquipmentSlots.Add(EOLCEquipmentSlotType::Utility);

	if (UnitType == EOLCUnitType::Vehicle)
	{
		EquipmentSlots.Add(EOLCEquipmentSlotType::Secondary);
	}
	if (bChampion)
	{
		EquipmentSlots.Add(EOLCEquipmentSlotType::Secondary);
		EquipmentSlots.Add(EOLCEquipmentSlotType::Tertiary);
	}
}

bool UOLCUnitEquipmentComponent::AssignEquipment(const FOLCEquipmentInstance& Instance, EOLCEquipmentSlotType SlotType, const FString& FactionId, int32 ColonyTIR)
{
	AOLCUnitBase* OwnerUnit = Cast<AOLCUnitBase>(GetOwner());
	if (!OwnerUnit || !OwnerUnit->GetUnitData() || !Instance.EquipmentData)
	{
		return false;
	}

	if (!EquipmentSlots.Contains(SlotType))
	{
		InitializeDefaultSlots(OwnerUnit->GetUnitData()->UnitType, OwnerUnit->GetUnitData()->UnitType == EOLCUnitType::Champion);
	}

	FText Reason;
	if (!Instance.EquipmentData->CanEquip(SlotType, OwnerUnit->GetUnitData()->UnitType, FactionId, ColonyTIR, Reason))
	{
		return false;
	}

	EquipmentSlots.FindOrAdd(SlotType) = Instance;
	return true;
}

bool UOLCUnitEquipmentComponent::RemoveEquipment(EOLCEquipmentSlotType SlotType, bool bReturnToPool)
{
	FOLCEquipmentInstance* Existing = EquipmentSlots.Find(SlotType);
	if (!Existing || !Existing->EquipmentData)
	{
		return false;
	}

	if (bReturnToPool)
	{
		if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
		{
			if (UOLCEquipmentSubsystem* EquipmentSubsystem = GI->GetSubsystem<UOLCEquipmentSubsystem>())
			{
				EquipmentSubsystem->ReturnToPool(*Existing);
			}
		}
	}

	EquipmentSlots.FindOrAdd(SlotType) = FOLCEquipmentInstance();
	return true;
}

void UOLCUnitEquipmentComponent::OnUnitDeath()
{
	if (!GetWorld())
	{
		return;
	}

	UOLCEquipmentSubsystem* EquipmentSubsystem = nullptr;
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		EquipmentSubsystem = GI->GetSubsystem<UOLCEquipmentSubsystem>();
	}

	for (TPair<EOLCEquipmentSlotType, FOLCEquipmentInstance>& Pair : EquipmentSlots)
	{
		FOLCEquipmentInstance& Instance = Pair.Value;
		if (!Instance.EquipmentData)
		{
			continue;
		}

		const EOLCEquipmentRarity Rarity = Instance.EquipmentData->Rarity;
		const bool bAlwaysRecovered = Instance.bUniqueNamedItem || Instance.Layer == EOLCEquipmentLayer::Unique || Rarity == EOLCEquipmentRarity::Alien;
		const bool bBasicRecovered = Rarity == EOLCEquipmentRarity::Common || Rarity == EOLCEquipmentRarity::Uncommon || Rarity == EOLCEquipmentRarity::Rare;
		const bool bHighRarityRecovered = (Rarity == EOLCEquipmentRarity::Epic || Rarity == EOLCEquipmentRarity::Legendary) && FMath::FRand() >= 0.5f;

		if (EquipmentSubsystem && (bAlwaysRecovered || bBasicRecovered || bHighRarityRecovered))
		{
			EquipmentSubsystem->ReturnToPool(Instance);
		}
		Instance = FOLCEquipmentInstance();
	}
}

FOLCEquipmentStats UOLCUnitEquipmentComponent::GetTotalEquipmentStats() const
{
	FOLCEquipmentStats Total;
	for (const TPair<EOLCEquipmentSlotType, FOLCEquipmentInstance>& Pair : EquipmentSlots)
	{
		if (!Pair.Value.EquipmentData)
		{
			continue;
		}
		Total.DamageBonus += Pair.Value.EquipmentData->Stats.DamageBonus;
		Total.HPBonus += Pair.Value.EquipmentData->Stats.HPBonus;
		Total.SpeedModifier += Pair.Value.EquipmentData->Stats.SpeedModifier;
		Total.ShieldBonus += Pair.Value.EquipmentData->Stats.ShieldBonus;
		Total.UtilityBonus += Pair.Value.EquipmentData->Stats.UtilityBonus;
	}
	return Total;
}

