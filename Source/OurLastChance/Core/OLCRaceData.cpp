#include "OLCRaceData.h"

UOLCRaceData::UOLCRaceData()
{
}

float UOLCRaceData::GetSpawnWeightForBiome(EOLCRaceBiomeType InBiome) const
{
	const float* Weight = BiomeSpawnWeights.Find(InBiome);
	if (Weight)
	{
		return *Weight;
	}
	return 0.0f;
}

bool UOLCRaceData::IsRaceInFamily(EOLCRaceFamily InFamily) const
{
	return RaceFamily == InFamily;
}
