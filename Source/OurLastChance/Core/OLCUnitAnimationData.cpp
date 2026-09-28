#include "OLCUnitAnimationData.h"

bool UOLCUnitAnimationSet::FindEntry(EOLCAnimSlot Slot, FOLCAnimationEntry& OutEntry) const
{
	for (const FOLCAnimationEntry& Entry : Entries)
	{
		if (Entry.Slot == Slot)
		{
			OutEntry = Entry;
			return true;
		}
	}
	return false;
}

bool UOLCUnitAnimationSet::HasEntry(EOLCAnimSlot Slot) const
{
	FOLCAnimationEntry Entry;
	return FindEntry(Slot, Entry) && Entry.AnimationAsset != nullptr;
}
