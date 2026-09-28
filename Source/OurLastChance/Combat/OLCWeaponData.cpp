#include "Combat/OLCWeaponData.h"

namespace
{
	const float UpgradeMultipliers[10] = { 1.05f, 1.10f, 1.16f, 1.23f, 1.31f, 1.40f, 1.50f, 1.61f, 1.73f, 1.81f };

	FLinearColor ColorForDamageType(EOLCWeaponDamageType Type)
	{
		switch (Type)
		{
			case EOLCWeaponDamageType::Ballistic: return FLinearColor(1.0f, 0.46f, 0.08f, 1.0f);
			case EOLCWeaponDamageType::Energy: return FLinearColor(0.05f, 0.72f, 1.0f, 1.0f);
			case EOLCWeaponDamageType::Rocket: return FLinearColor(1.0f, 0.16f, 0.05f, 1.0f);
			case EOLCWeaponDamageType::Ion: return FLinearColor(0.55f, 0.2f, 1.0f, 1.0f);
			case EOLCWeaponDamageType::Void: return FLinearColor(0.0f, 1.0f, 0.42f, 1.0f);
		}
		return FLinearColor::White;
	}

	void AddUpgradeLevels(UOLCWeaponData* Weapon)
	{
		Weapon->TIRUpgradeLevels.Reset();
		for (int32 Index = 0; Index < 10; ++Index)
		{
			FOLCWeaponTIRUpgradeLevel Upgrade;
			Upgrade.Level = Index + 1;
			Upgrade.DamageMultiplier = UpgradeMultipliers[Index];
			const float CostScale = static_cast<float>(Index + 1);
			Upgrade.MaterialCost.Emplace(EOLCResourceType::ConstructionMaterial, 20.0f * CostScale, 20.0f * CostScale);
			Upgrade.MaterialCost.Emplace(EOLCResourceType::Minerals, 15.0f * CostScale, 15.0f * CostScale);
			Weapon->TIRUpgradeLevels.Add(Upgrade);
		}
	}

	void AddVisualVariants(UOLCWeaponData* Weapon)
	{
		Weapon->VisualVariants.Reset();
		for (int32 Tier = 1; Tier <= 5; ++Tier)
		{
			FOLCWeaponVisualVariant Variant;
			Variant.TIRTier = Tier;
			Variant.GlowColor = ColorForDamageType(Weapon->DamageType);
			Variant.GlowIntensity = 0.65f + Tier * 0.35f;
			Variant.ProjectileScale = 0.85f + Tier * 0.15f;
			switch (Tier)
			{
				case 1: Variant.EffectDescription = FText::FromString(TEXT("Simple muzzle flash, small projectile")); break;
				case 2: Variant.EffectDescription = FText::FromString(TEXT("Colored glow with larger projectile")); break;
				case 3: Variant.EffectDescription = FText::FromString(TEXT("Particle trail and distinct report")); break;
				case 4: Variant.EffectDescription = FText::FromString(TEXT("Beam effect with screen shake")); break;
				default: Variant.EffectDescription = FText::FromString(TEXT("Large beam or vortex with ambient particles")); break;
			}
			Weapon->VisualVariants.Add(Variant);
		}
	}

	UOLCWeaponData* MakeWeapon(UObject* Outer, const TCHAR* Id, const TCHAR* Name, EOLCWeaponDamageType Type, int32 TIR, float Damage, float Range, float FireRate, EOLCTurretMountType Mount, EOLCTargetingMode Mode, EOLCWeaponAmmoType Ammo = EOLCWeaponAmmoType::Infinite, int32 Burst = 1, float EnergyCost = 0.0f, float ShieldPen = 0.0f, bool bIgnoresArmor = false)
	{
		UOLCWeaponData* Weapon = NewObject<UOLCWeaponData>(Outer ? Outer : GetTransientPackage(), NAME_None, RF_Transient);
		Weapon->WeaponId = FName(Id);
		Weapon->DisplayName = FText::FromString(Name);
		Weapon->DamageType = Type;
		Weapon->TIRTier = TIR;
		Weapon->BaseDamage = Damage;
		Weapon->Range = Range;
		Weapon->FireRate = FireRate;
		Weapon->RequiredMount = Mount;
		Weapon->PreferredTargetingMode = Mode;
		Weapon->AmmoType = Ammo;
		Weapon->BurstCount = Burst;
		Weapon->EnergyCostPerShot = EnergyCost;
		Weapon->ShieldPenetration = ShieldPen;
		Weapon->bIgnoresArmor = bIgnoresArmor;
		AddUpgradeLevels(Weapon);
		AddVisualVariants(Weapon);
		return Weapon;
	}
}

float UOLCWeaponData::GetDamageWithTIRUpgrade(int32 UpgradeLevel) const
{
	if (TIRUpgradeLevels.Num() == 0)
	{
		const int32 Index = FMath::Clamp(UpgradeLevel, 1, 10) - 1;
		return BaseDamage * UpgradeMultipliers[Index];
	}

	const int32 Index = FMath::Clamp(UpgradeLevel, 1, TIRUpgradeLevels.Num()) - 1;
	return BaseDamage * TIRUpgradeLevels[Index].DamageMultiplier;
}

FOLCWeaponVisualVariant UOLCWeaponData::GetVisualVariantForTier(int32 InTIRTier) const
{
	if (VisualVariants.Num() == 0)
	{
		FOLCWeaponVisualVariant Variant;
		Variant.TIRTier = FMath::Clamp(InTIRTier, 1, 5);
		Variant.GlowColor = ColorForDamageType(DamageType);
		Variant.GlowIntensity = 1.0f;
		Variant.ProjectileScale = 1.0f;
		return Variant;
	}

	const int32 ClampedTier = FMath::Clamp(InTIRTier, 1, 5);
	for (const FOLCWeaponVisualVariant& Variant : VisualVariants)
	{
		if (Variant.TIRTier == ClampedTier)
		{
			return Variant;
		}
	}
	return VisualVariants.Last();
}

EOLCDamageType UOLCWeaponData::ToCombatDamageType() const
{
	switch (DamageType)
	{
		case EOLCWeaponDamageType::Ballistic: return EOLCDamageType::Kinetic;
		case EOLCWeaponDamageType::Energy: return EOLCDamageType::Energy;
		case EOLCWeaponDamageType::Rocket: return EOLCDamageType::Explosive;
		case EOLCWeaponDamageType::Ion: return EOLCDamageType::Energy;
		case EOLCWeaponDamageType::Void: return EOLCDamageType::Void;
	}
	return EOLCDamageType::Kinetic;
}

TArray<UOLCWeaponData*> UOLCWeaponData::GetDefaultWeaponCatalog(UObject* Outer)
{
	TArray<UOLCWeaponData*> Weapons;
	Weapons.Reserve(25);

	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_BAL_MACHINE_GUN"), TEXT("Machine Gun"), EOLCWeaponDamageType::Ballistic, 1, 8.0f, 200.0f, 4.0f, EOLCTurretMountType::Light, EOLCTargetingMode::Auto));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_BAL_AUTOCANNON"), TEXT("Autocannon"), EOLCWeaponDamageType::Ballistic, 2, 15.0f, 275.0f, 2.4f, EOLCTurretMountType::Light, EOLCTargetingMode::Auto));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_BAL_HEAVY_CANNON"), TEXT("Heavy Cannon"), EOLCWeaponDamageType::Ballistic, 3, 25.0f, 350.0f, 1.5f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Focus));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_BAL_GAUSS_CANNON"), TEXT("Gauss Cannon"), EOLCWeaponDamageType::Ballistic, 4, 42.0f, 480.0f, 0.9f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Focus));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_BAL_RAILGUN"), TEXT("Railgun"), EOLCWeaponDamageType::Ballistic, 5, 60.0f, 600.0f, 0.5f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Limited));

	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_EN_LASER"), TEXT("Laser"), EOLCWeaponDamageType::Energy, 1, 12.0f, 250.0f, 3.0f, EOLCTurretMountType::Light, EOLCTargetingMode::Auto, EOLCWeaponAmmoType::Infinite, 1, 2.0f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_EN_PULSE_LASER"), TEXT("Pulse Laser"), EOLCWeaponDamageType::Energy, 2, 18.0f, 320.0f, 2.2f, EOLCTurretMountType::Light, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 3.0f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_EN_PLASMA_BEAM"), TEXT("Plasma Beam"), EOLCWeaponDamageType::Energy, 3, 30.0f, 400.0f, 1.2f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Infinite, 1, 5.0f, 0.25f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_EN_FUSION_LANCE"), TEXT("Fusion Lance"), EOLCWeaponDamageType::Energy, 4, 48.0f, 500.0f, 0.8f, EOLCTurretMountType::Dual, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Infinite, 1, 7.0f, 0.35f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_EN_PARTICLE_ACCELERATOR"), TEXT("Particle Accelerator"), EOLCWeaponDamageType::Energy, 5, 70.0f, 550.0f, 0.45f, EOLCTurretMountType::Dual, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Infinite, 1, 10.0f, 0.45f));

	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_RKT_MISSILE_POD"), TEXT("Missile Pod"), EOLCWeaponDamageType::Rocket, 1, 20.0f, 300.0f, 1.0f, EOLCTurretMountType::Light, EOLCTargetingMode::Spread, EOLCWeaponAmmoType::Burst, 3));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_RKT_SWARM_RACK"), TEXT("Swarm Rack"), EOLCWeaponDamageType::Rocket, 2, 32.0f, 420.0f, 0.8f, EOLCTurretMountType::Dual, EOLCTargetingMode::Spread, EOLCWeaponAmmoType::Burst, 5));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_RKT_BARRAGE_LAUNCHER"), TEXT("Barrage Launcher"), EOLCWeaponDamageType::Rocket, 3, 45.0f, 500.0f, 0.6f, EOLCTurretMountType::Quad, EOLCTargetingMode::Spread, EOLCWeaponAmmoType::Burst, 8));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_RKT_SIEGE_ROCKET"), TEXT("Siege Rocket"), EOLCWeaponDamageType::Rocket, 4, 68.0f, 620.0f, 0.35f, EOLCTurretMountType::Quad, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Limited, 4));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_RKT_TORPEDO_ARRAY"), TEXT("Torpedo Array"), EOLCWeaponDamageType::Rocket, 5, 90.0f, 700.0f, 0.25f, EOLCTurretMountType::Quad, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Burst, 2));

	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_ION_EMP_PULSE"), TEXT("EMP Pulse"), EOLCWeaponDamageType::Ion, 2, 15.0f, 200.0f, 0.8f, EOLCTurretMountType::Light, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 4.0f, 0.4f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_ION_ARC_THROWER"), TEXT("Arc Thrower"), EOLCWeaponDamageType::Ion, 2, 20.0f, 280.0f, 1.1f, EOLCTurretMountType::Light, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 4.0f, 0.45f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_ION_DISRUPTOR_CANNON"), TEXT("Disruptor Cannon"), EOLCWeaponDamageType::Ion, 3, 25.0f, 350.0f, 0.7f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 6.0f, 0.5f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_ION_NULL_FIELD"), TEXT("Null Field Emitter"), EOLCWeaponDamageType::Ion, 4, 32.0f, 410.0f, 0.5f, EOLCTurretMountType::Dual, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 8.0f, 0.65f));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_ION_SINGULARITY_PROJECTOR"), TEXT("Singularity Projector"), EOLCWeaponDamageType::Ion, 5, 40.0f, 450.0f, 0.3f, EOLCTurretMountType::Quad, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 12.0f, 0.8f));

	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_VOID_PHASE_BLASTER"), TEXT("Phase Blaster"), EOLCWeaponDamageType::Void, 4, 50.0f, 400.0f, 0.8f, EOLCTurretMountType::Heavy, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Infinite, 1, 9.0f, 0.6f, true));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_VOID_EVENT_RIPPER"), TEXT("Event Ripper"), EOLCWeaponDamageType::Void, 4, 64.0f, 450.0f, 0.55f, EOLCTurretMountType::Dual, EOLCTargetingMode::Focus, EOLCWeaponAmmoType::Infinite, 1, 11.0f, 0.75f, true));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_VOID_REALITY_FRACTURE"), TEXT("Reality Fracture"), EOLCWeaponDamageType::Void, 5, 80.0f, 500.0f, 0.35f, EOLCTurretMountType::Quad, EOLCTargetingMode::Spread, EOLCWeaponAmmoType::Infinite, 1, 15.0f, 1.0f, true));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_VOID_GRAVITY_LENS"), TEXT("Gravity Lens"), EOLCWeaponDamageType::Void, 5, 96.0f, 560.0f, 0.25f, EOLCTurretMountType::Quad, EOLCTargetingMode::Pulse, EOLCWeaponAmmoType::Infinite, 1, 18.0f, 1.0f, true));
	Weapons.Add(MakeWeapon(Outer, TEXT("WPN_VOID_ALIEN_CORE"), TEXT("Alien Core Weapon"), EOLCWeaponDamageType::Void, 6, 120.0f, 600.0f, 0.2f, EOLCTurretMountType::Quad, EOLCTargetingMode::Spread, EOLCWeaponAmmoType::Infinite, 1, 25.0f, 1.0f, true));

	return Weapons;
}

TArray<UOLCWeaponData*> UOLCWeaponData::GetWeaponsByDamageType(UObject* Outer, EOLCWeaponDamageType InDamageType, int32 MaxTIR)
{
	TArray<UOLCWeaponData*> Filtered;
	for (UOLCWeaponData* Weapon : GetDefaultWeaponCatalog(Outer))
	{
		if (Weapon && Weapon->DamageType == InDamageType && Weapon->TIRTier <= MaxTIR)
		{
			Filtered.Add(Weapon);
		}
	}
	return Filtered;
}

