#include "Combat/OLCDamageCalculator.h"

float UOLCDamageCalculator::GetArmorMultiplier(EOLCDamageType DamageType, float Armor)
{
	const float ArmorRatio = FMath::Clamp(Armor / 100.0f, 0.0f, 0.85f);
	switch (DamageType)
	{
		case EOLCDamageType::Kinetic: return 1.0f - ArmorRatio;
		case EOLCDamageType::Energy: return 1.0f - (ArmorRatio * 0.65f);
		case EOLCDamageType::Explosive: return 1.15f - (ArmorRatio * 0.45f);
		case EOLCDamageType::Void: return 1.25f - (ArmorRatio * 0.25f);
		default: return 1.0f;
	}
}

FOLCDamageResult UOLCDamageCalculator::ResolveDamage(const FOLCDamageInput& Input, float CurrentShield, float CurrentHull, float Armor, int32 DefenderTIR)
{
	FOLCDamageResult Result;
	const float TierDelta = FMath::Clamp(static_cast<float>(Input.WeaponTIR - DefenderTIR), -3.0f, 3.0f);
	const float Penetration = FMath::Clamp(0.2f + TierDelta * 0.12f + (Input.DamageType == EOLCDamageType::Void ? 0.3f : 0.0f), 0.05f, 0.85f);
	const bool bCrit = FMath::FRand() <= FMath::Clamp(Input.CriticalChance + Input.WeaponTIR * 0.015f, 0.0f, 0.65f);
	const float CritMultiplier = bCrit ? 1.5f : 1.0f;
	const float TypedDamage = Input.RawDamage * GetArmorMultiplier(Input.DamageType, Armor) * CritMultiplier;

	const float ShieldPortion = CurrentShield > 0.0f ? TypedDamage * (1.0f - Penetration) : 0.0f;
	Result.ShieldDamage = FMath::Min(CurrentShield, ShieldPortion);
	const float Overflow = FMath::Max(0.0f, ShieldPortion - Result.ShieldDamage);
	Result.HullDamage = FMath::Max(0.0f, TypedDamage * Penetration + Overflow);
	Result.FinalDamage = Result.ShieldDamage + Result.HullDamage;
	Result.bCriticalHit = bCrit;
	Result.bDestroyed = (CurrentHull - Result.HullDamage) <= 0.0f;
	return Result;
}

// ---------------------------------------------------------------------------
// Alien Shield Generator modifier (WP-125 Step 1)
// ---------------------------------------------------------------------------

FOLCDamageResult UOLCDamageCalculator::ApplyAlienShieldModifier(const FOLCDamageResult& InResult, EOLCDamageType DamageType)
{
	FOLCDamageResult Modified = InResult;

	switch (DamageType)
	{
		case EOLCDamageType::Kinetic:
			// Alien Shield: complete immunity to physical/kinetic damage.
			Modified.ShieldDamage = 0.0f;
			Modified.HullDamage = 0.0f;
			Modified.FinalDamage = 0.0f;
			Modified.bDestroyed = false;
			break;

		case EOLCDamageType::Energy:
			// Alien Shield: energy damage reduced to 50% effectiveness.
			Modified.ShieldDamage *= 0.5f;
			Modified.HullDamage *= 0.5f;
			Modified.FinalDamage = Modified.ShieldDamage + Modified.HullDamage;
			break;

		case EOLCDamageType::Explosive:
		case EOLCDamageType::Void:
		default:
			// Unaffected by Alien Shield — return input unchanged.
			break;
	}

	return Modified;
}
