#include "OLCChampionAnimationData.h"

bool UOLCChampionAnimationData::FindAbilityAnim(const FString& InAbilityId, FOLCChampionAbilityAnim& OutAnim) const
{
	if (InAbilityId.IsEmpty())
	{
		return false;
	}

	for (const FOLCChampionAbilityAnim& Anim : AbilityAnims)
	{
		if (Anim.AbilityId.Equals(InAbilityId, ESearchCase::IgnoreCase))
		{
			OutAnim = Anim;
			return true;
		}
	}
	return false;
}
