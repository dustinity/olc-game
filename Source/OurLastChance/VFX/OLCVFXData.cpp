#include "VFX/OLCVFXData.h"

TArray<FOLCCombatVFXMapping> UOLCVFXData::GetDefaultCombatMappings()
{
	TArray<FOLCCombatVFXMapping> Mappings;

	// One row per EOLCDamageType — null-tolerant (NiagaraSystem is null until an asset is assigned).
	FOLCCombatVFXMapping Kinetic;
	Kinetic.DamageType = EOLCDamageType::Kinetic;
	Mappings.Add(Kinetic);

	FOLCCombatVFXMapping Energy;
	Energy.DamageType = EOLCDamageType::Energy;
	Mappings.Add(Energy);

	FOLCCombatVFXMapping Explosive;
	Explosive.DamageType = EOLCDamageType::Explosive;
	Mappings.Add(Explosive);

	FOLCCombatVFXMapping Void;
	Void.DamageType = EOLCDamageType::Void;
	Mappings.Add(Void);

	return Mappings;
}

TArray<FOLCEnvironmentalVFXMapping> UOLCVFXData::GetDefaultEnvironmentalMappings()
{
	TArray<FOLCEnvironmentalVFXMapping> Mappings;

	// Standard environmental event keys — null-tolerant.
	FOLCEnvironmentalVFXMapping EMP;
	EMP.EventKey = FName(TEXT("EMP"));
	Mappings.Add(EMP);

	FOLCEnvironmentalVFXMapping DustStorm;
	DustStorm.EventKey = FName(TEXT("DustStorm"));
	Mappings.Add(DustStorm);

	FOLCEnvironmentalVFXMapping Lightning;
	Lightning.EventKey = FName(TEXT("Lightning"));
	Mappings.Add(Lightning);

	FOLCEnvironmentalVFXMapping SporeCloud;
	SporeCloud.EventKey = FName(TEXT("SporeCloud"));
	Mappings.Add(SporeCloud);

	FOLCEnvironmentalVFXMapping CrystalResonance;
	CrystalResonance.EventKey = FName(TEXT("CrystalResonance"));
	Mappings.Add(CrystalResonance);

	return Mappings;
}

TArray<FOLCBiomeAmbientMapping> UOLCVFXData::GetDefaultBiomeAmbientMappings()
{
	TArray<FOLCBiomeAmbientMapping> Mappings;

	// One row per EOLCBiomeType (all 8 biomes) — null-tolerant.
	FOLCBiomeAmbientMapping Desert;
	Desert.BiomeType = EOLCBiomeType::Desert;
	Mappings.Add(Desert);

	FOLCBiomeAmbientMapping Dusty;
	Dusty.BiomeType = EOLCBiomeType::Dusty;
	Mappings.Add(Dusty);

	FOLCBiomeAmbientMapping Rocky;
	Rocky.BiomeType = EOLCBiomeType::Rocky;
	Mappings.Add(Rocky);

	FOLCBiomeAmbientMapping Water;
	Water.BiomeType = EOLCBiomeType::Water;
	Mappings.Add(Water);

	FOLCBiomeAmbientMapping Swamp;
	Swamp.BiomeType = EOLCBiomeType::Swamp;
	Mappings.Add(Swamp);

	FOLCBiomeAmbientMapping Jungle;
	Jungle.BiomeType = EOLCBiomeType::Jungle;
	Mappings.Add(Jungle);

	FOLCBiomeAmbientMapping LightSnow;
	LightSnow.BiomeType = EOLCBiomeType::LightSnow;
	Mappings.Add(LightSnow);

	FOLCBiomeAmbientMapping Ice;
	Ice.BiomeType = EOLCBiomeType::Ice;
	Mappings.Add(Ice);

	return Mappings;
}
