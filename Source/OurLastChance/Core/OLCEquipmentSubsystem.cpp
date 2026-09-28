#include "Core/OLCEquipmentSubsystem.h"

#include "World/OLCUnitBase.h"
#include "World/OLCUnitEquipmentComponent.h"

void UOLCEquipmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	for (UOLCEquipmentData* Item : UOLCEquipmentData::GetDefaultEquipmentCatalog(this))
	{
		if (!Item)
		{
			continue;
		}

		if (Item->bUniqueTemplate)
		{
			continue;
		}

		const int32 Count = Item->Rarity == EOLCEquipmentRarity::Common ? 4 : 2;
		AddToPool(Item, Count);
	}
}

void UOLCEquipmentSubsystem::InitializeFactionDefaults(const FString& FactionId, int32 ColonyTIR)
{
	FactionDefaults.Reset();
	for (UOLCEquipmentData* Item : UOLCEquipmentData::GetDefaultEquipmentCatalog(this))
	{
		if (!Item || Item->RequiresTIR > ColonyTIR || !Item->IsAvailableForFaction(FactionId))
		{
			continue;
		}

		if (Item->EquipmentId == TEXT("EQ_WPN_IMPROVED_RIFLE") ||
			Item->EquipmentId == TEXT("EQ_ARM_COMPOSITE") ||
			(FactionId.Equals(TEXT("NeonPunk"), ESearchCase::IgnoreCase) && Item->EquipmentId == TEXT("EQ_UTIL_STEALTH_COATING")))
		{
			FOLCEquipmentInstance Instance;
			Instance.EquipmentData = Item;
			Instance.Layer = EOLCEquipmentLayer::FactionDefault;
			FactionDefaults.Add(Instance);
		}
	}
}

bool UOLCEquipmentSubsystem::AssignFromPool(EOLCEquipmentSlotType SlotType, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR, FOLCEquipmentInstance& OutInstance)
{
	int32 BestIndex = INDEX_NONE;
	uint8 BestRarity = 0;
	for (int32 Index = 0; Index < EquipmentPool.Num(); ++Index)
	{
		UOLCEquipmentData* Item = EquipmentPool[Index].EquipmentData;
		FText Reason;
		if (Item && Item->CanEquip(SlotType, UnitType, FactionId, ColonyTIR, Reason))
		{
			const uint8 RarityScore = static_cast<uint8>(Item->Rarity);
			if (BestIndex == INDEX_NONE || RarityScore > BestRarity)
			{
				BestIndex = Index;
				BestRarity = RarityScore;
			}
		}
	}

	if (BestIndex == INDEX_NONE)
	{
		return false;
	}

	OutInstance = EquipmentPool[BestIndex];
	OutInstance.Layer = EOLCEquipmentLayer::Pool;
	EquipmentPool.RemoveAt(BestIndex);
	return true;
}

void UOLCEquipmentSubsystem::AddToPool(UOLCEquipmentData* EquipmentData, int32 Count)
{
	if (!EquipmentData)
	{
		return;
	}

	for (int32 Index = 0; Index < Count; ++Index)
	{
		FOLCEquipmentInstance Instance;
		Instance.EquipmentData = EquipmentData;
		Instance.Layer = EOLCEquipmentLayer::Pool;
		EquipmentPool.Add(Instance);
	}
}

void UOLCEquipmentSubsystem::ReturnToPool(const FOLCEquipmentInstance& Instance)
{
	if (!Instance.EquipmentData || Instance.Layer == EOLCEquipmentLayer::Unique || Instance.bUniqueNamedItem)
	{
		return;
	}

	FOLCEquipmentInstance PoolInstance = Instance;
	PoolInstance.Layer = EOLCEquipmentLayer::Pool;
	EquipmentPool.Add(PoolInstance);
}

FOLCEquipmentInstance UOLCEquipmentSubsystem::AddUniqueLoot(UOLCEquipmentData* EquipmentData, const FText& UniqueName)
{
	FOLCEquipmentInstance Instance;
	Instance.EquipmentData = EquipmentData;
	Instance.Layer = EOLCEquipmentLayer::Unique;
	Instance.bUniqueNamedItem = true;
	Instance.CustomName = UniqueName;
	UniqueLoot.Add(Instance);
	return Instance;
}

TArray<FOLCEquipmentInstance> UOLCEquipmentSubsystem::GetAvailableForSlot(EOLCEquipmentSlotType SlotType, EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR) const
{
	TArray<FOLCEquipmentInstance> Result;
	for (const FOLCEquipmentInstance& Instance : EquipmentPool)
	{
		FText Reason;
		if (Instance.EquipmentData && Instance.EquipmentData->CanEquip(SlotType, UnitType, FactionId, ColonyTIR, Reason))
		{
			Result.Add(Instance);
		}
	}
	for (const FOLCEquipmentInstance& Instance : UniqueLoot)
	{
		FText Reason;
		if (Instance.EquipmentData && Instance.EquipmentData->CanEquip(SlotType, UnitType, FactionId, ColonyTIR, Reason))
		{
			Result.Add(Instance);
		}
	}
	return Result;
}

TArray<FOLCEquipmentInstance> UOLCEquipmentSubsystem::GetFactionDefaultsForUnit(EOLCUnitType UnitType, const FString& FactionId, int32 ColonyTIR) const
{
	TArray<FOLCEquipmentInstance> Result;
	for (const FOLCEquipmentInstance& Instance : FactionDefaults)
	{
		FText Reason;
		if (Instance.EquipmentData && Instance.EquipmentData->CanEquip(Instance.EquipmentData->SlotType, UnitType, FactionId, ColonyTIR, Reason))
		{
			Result.Add(Instance);
		}
	}
	return Result;
}

int32 UOLCEquipmentSubsystem::AssignBestToUnits(const TArray<AActor*>& Units, EOLCEquipmentSlotType SlotType, const FString& FactionId, int32 ColonyTIR)
{
	int32 Assigned = 0;
	for (AActor* Actor : Units)
	{
		AOLCUnitBase* Unit = Cast<AOLCUnitBase>(Actor);
		if (!Unit || !Unit->GetEquipmentComponent() || !Unit->GetUnitData())
		{
			continue;
		}

		FOLCEquipmentInstance Instance;
		if (AssignFromPool(SlotType, Unit->GetUnitData()->UnitType, FactionId, ColonyTIR, Instance))
		{
			if (Unit->GetEquipmentComponent()->AssignEquipment(Instance, SlotType, FactionId, ColonyTIR))
			{
				++Assigned;
			}
			else
			{
				ReturnToPool(Instance);
			}
		}
	}
	return Assigned;
}

