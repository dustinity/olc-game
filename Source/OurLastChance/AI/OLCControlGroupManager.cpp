#include "AI/OLCControlGroupManager.h"

#include "AI/OLCFormationManager.h"
#include "World/OLCUnitBase.h"

void UOLCControlGroupManager::AssignGroup(int32 GroupNumber, const TArray<AOLCUnitBase*>& Units)
{
	if (GroupNumber < 1 || GroupNumber > 9)
	{
		return;
	}
	FOLCControlGroup& Group = Groups.FindOrAdd(GroupNumber);
	Group.Units.Reset();
	if (Champion)
	{
		Group.Units.Add(Champion);
	}
	for (AOLCUnitBase* Unit : Units)
	{
		if (Unit && Unit != Champion)
		{
			Group.Units.Add(Unit);
		}
	}
}

TArray<AOLCUnitBase*> UOLCControlGroupManager::SelectGroup(int32 GroupNumber) const
{
	TArray<AOLCUnitBase*> Result;
	if (const FOLCControlGroup* Group = Groups.Find(GroupNumber))
	{
		for (AOLCUnitBase* Unit : Group->Units)
		{
			if (Unit)
			{
				Unit->SetSelected(true);
				Result.Add(Unit);
			}
		}
	}
	return Result;
}

void UOLCControlGroupManager::RecallFormation(int32 GroupNumber, const FVector& AnchorLocation)
{
	if (FOLCControlGroup* Group = Groups.Find(GroupNumber))
	{
		UOLCFormationManager* Manager = NewObject<UOLCFormationManager>(this);
		TArray<AOLCUnitBase*> Units;
		for (AOLCUnitBase* Unit : Group->Units)
		{
			Units.Add(Unit);
		}
		Manager->ApplyFormation(Units, AnchorLocation, Group->LastFormation);
	}
}

void UOLCControlGroupManager::SetChampion(AOLCUnitBase* ChampionUnit)
{
	Champion = ChampionUnit;
}

bool UOLCControlGroupManager::IsChampionIncluded(int32 GroupNumber) const
{
	const FOLCControlGroup* Group = Groups.Find(GroupNumber);
	return Group && Champion && Group->Units.Num() > 0 && Group->Units[0] == Champion;
}
