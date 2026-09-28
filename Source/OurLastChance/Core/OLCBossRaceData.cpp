#include "OLCBossRaceData.h"

UOLCBossRaceData::UOLCBossRaceData()
{
}

FOLCBossPhaseData UOLCBossRaceData::GetPhaseForHPPercent(float HPPercent) const
{
	// Phases are stored intro-first; walk from the end so the first phase
	// whose threshold the boss has fallen to or below wins.
	for (int32 Index = Phases.Num() - 1; Index >= 0; --Index)
	{
		if (HPPercent <= Phases[Index].HPThresholdPercent)
		{
			return Phases[Index];
		}
	}
	return Phases.Num() > 0 ? Phases[0] : FOLCBossPhaseData();
}
