#include "OLCUnitData.h"

UOLCUnitData::UOLCUnitData()
{
	ProductionTimeSeconds = TrainingTime;
}

namespace
{
	void AddCost(UOLCUnitData* Unit, EOLCResourceType Type, float Amount)
	{
		Unit->BuildCost.Emplace(Type, Amount, Amount);
	}

	UOLCUnitData* MakeUnit(UObject* Outer, const TCHAR* Id, const TCHAR* Name, EOLCUnitType Type, EOLCUnitCategory Category, float HP, float Speed, float Damage, float Range, float Time, FIntPoint Grid, int32 TIR = 1, const TCHAR* FactionId = TEXT(""))
	{
		UOLCUnitData* Unit = NewObject<UOLCUnitData>(Outer ? Outer : GetTransientPackage());
		Unit->UnitId = Id;
		Unit->DisplayName = FText::FromString(Name);
		Unit->UnitType = Type;
		Unit->UnitCategory = Category;
		Unit->MaxHP = HP;
		Unit->MovementSpeed = Speed;
		Unit->AttackDamage = Damage;
		Unit->Range = Range;
		Unit->TrainingTime = Time;
		Unit->ProductionTimeSeconds = Time;
		Unit->GridSize = Grid;
		Unit->TIRRequirement = TIR;
		Unit->FactionId = FactionId;
		Unit->FactionExclusivity = FText::FromString(FactionId);
		Unit->FuelConsumptionPerMinute = Type == EOLCUnitType::Vehicle || Type == EOLCUnitType::Aerial ? FMath::Max(1.0f, Grid.X * Grid.Y * 0.75f) : 0.0f;
		Unit->FuelConsumptionPerTick = Unit->FuelConsumptionPerMinute / 60.0f;
		return Unit;
	}

	void ApplySharedBiomeRules(UOLCUnitData* Unit)
	{
		auto Add = [Unit](EOLCBiomeType Biome, float Speed, float Damage, float Range, bool bCanSpawn = true)
		{
			FOLCUnitBiomeModifier Modifier;
			Modifier.Biome = Biome;
			Modifier.SpeedMultiplier = Speed;
			Modifier.DamageMultiplier = Damage;
			Modifier.RangeMultiplier = Range;
			Modifier.bCanSpawn = bCanSpawn;
			Unit->BiomeModifiers.Add(Modifier);
		};

		const bool bVehicle = Unit->UnitType == EOLCUnitType::Vehicle;
		const bool bAerial = Unit->UnitType == EOLCUnitType::Aerial;
		const bool bHovercraft = Unit->UnitId.Equals(TEXT("VEH_HOVERCRAFT"));
		Add(EOLCBiomeType::Desert, bVehicle ? 1.1f : 1.0f, 1.0f, bAerial ? 1.05f : 1.0f);
		Add(EOLCBiomeType::Rocky, bVehicle ? 0.8f : 1.0f, Unit->UnitType == EOLCUnitType::Infantry ? 1.1f : 1.0f, 1.0f);
		Add(EOLCBiomeType::Water, bVehicle ? (bHovercraft ? 1.0f : 0.0f) : 0.7f, 1.0f, 1.0f, !bVehicle || bHovercraft);
		Add(EOLCBiomeType::Swamp, 0.85f, 1.0f, 1.0f, !bAerial);
		Add(EOLCBiomeType::Jungle, bVehicle ? 0.75f : 1.0f, 1.0f, 1.0f);
		Add(EOLCBiomeType::Ice, Unit->UnitType == EOLCUnitType::Infantry ? 0.9f : 1.0f, bVehicle ? 1.1f : 1.0f, 1.0f);
	}
}

FOLCUnitBiomeModifier UOLCUnitData::GetModifierForBiome(EOLCBiomeType Biome) const
{
	for (const FOLCUnitBiomeModifier& Modifier : BiomeModifiers)
	{
		if (Modifier.Biome == Biome)
		{
			return Modifier;
		}
	}
	return FOLCUnitBiomeModifier();
}

bool UOLCUnitData::IsAvailableForFaction(const FString& InFactionId) const
{
	return FactionId.IsEmpty() || FactionId.Equals(InFactionId, ESearchCase::IgnoreCase);
}

TArray<UOLCUnitData*> UOLCUnitData::GetDefaultUnitCatalog(UObject* Outer)
{
	TArray<UOLCUnitData*> Units;
	auto Add = [&Units, Outer](const TCHAR* Id, const TCHAR* Name, EOLCUnitType Type, EOLCUnitCategory Category, float HP, float Speed, float Damage, float Range, float Time, FIntPoint Grid, int32 TIR = 1, const TCHAR* FactionId = TEXT("")) -> UOLCUnitData*
	{
		UOLCUnitData* Unit = MakeUnit(Outer, Id, Name, Type, Category, HP, Speed, Damage, Range, Time, Grid, TIR, FactionId);
		ApplySharedBiomeRules(Unit);
		Units.Add(Unit);
		return Unit;
	};

	Add(TEXT("INF_SCOUT"), TEXT("Scout"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 55, 360, 8, 900, 18, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 5); AddCost(Units.Last(), EOLCResourceType::Minerals, 3);
	Add(TEXT("INF_RIFLEMAN"), TEXT("Rifleman"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 85, 260, 15, 1100, 24, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 8); AddCost(Units.Last(), EOLCResourceType::Minerals, 5); AddCost(Units.Last(), EOLCResourceType::Fuel, 2);
	Add(TEXT("INF_GRENADIER"), TEXT("Grenadier"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 80, 220, 28, 850, 30, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 10); AddCost(Units.Last(), EOLCResourceType::Minerals, 7); AddCost(Units.Last(), EOLCResourceType::Fuel, 3);
	Add(TEXT("INF_ENGINEER"), TEXT("Engineer"), EOLCUnitType::Infantry, EOLCUnitCategory::Support, 75, 240, 7, 500, 28, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 12); AddCost(Units.Last(), EOLCResourceType::Minerals, 6);
	Add(TEXT("INF_MEDIC"), TEXT("Medic"), EOLCUnitType::Infantry, EOLCUnitCategory::Support, 70, 250, 4, 450, 26, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 8); AddCost(Units.Last(), EOLCResourceType::Minerals, 5); AddCost(Units.Last(), EOLCResourceType::Survival, 4);
	Add(TEXT("INF_SNIPER"), TEXT("Sniper"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 50, 220, 45, 2200, 34, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 10); AddCost(Units.Last(), EOLCResourceType::Minerals, 8); AddCost(Units.Last(), EOLCResourceType::Fuel, 2);
	Add(TEXT("INF_DEMOLITIONS"), TEXT("Demolitions"), EOLCUnitType::Infantry, EOLCUnitCategory::Support, 70, 220, 50, 600, 36, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 12); AddCost(Units.Last(), EOLCResourceType::Minerals, 6); AddCost(Units.Last(), EOLCResourceType::Fuel, 3);
	Add(TEXT("INF_HEAVY_GUNNER"), TEXT("Heavy Gunner"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 120, 170, 34, 1200, 42, FIntPoint(2, 1), 1, TEXT("DarkRealistic")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 15); AddCost(Units.Last(), EOLCResourceType::Minerals, 10); AddCost(Units.Last(), EOLCResourceType::Fuel, 5);
	Add(TEXT("INF_FLAMER"), TEXT("Flamer"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 90, 200, 24, 420, 32, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 10); AddCost(Units.Last(), EOLCResourceType::Minerals, 7); AddCost(Units.Last(), EOLCResourceType::Fuel, 4);
	Add(TEXT("INF_ROCKETEER"), TEXT("Rocketeer"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 75, 210, 48, 1300, 38, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 12); AddCost(Units.Last(), EOLCResourceType::Minerals, 8); AddCost(Units.Last(), EOLCResourceType::Fuel, 6);
	Add(TEXT("INF_STEALTH_OPERATIVE"), TEXT("Stealth Operative"), EOLCUnitType::Infantry, EOLCUnitCategory::Infantry, 65, 300, 22, 800, 40, FIntPoint(1, 1), 1, TEXT("NeonPunk")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 15); AddCost(Units.Last(), EOLCResourceType::Minerals, 10); AddCost(Units.Last(), EOLCResourceType::Survival, 5);
	Add(TEXT("INF_DRONE_OPERATOR"), TEXT("Drone Operator"), EOLCUnitType::Infantry, EOLCUnitCategory::Support, 70, 250, 10, 1000, 34, FIntPoint(1, 1), 1, TEXT("NeonPunk")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 10); AddCost(Units.Last(), EOLCResourceType::Minerals, 8); AddCost(Units.Last(), EOLCResourceType::Fuel, 4);
	Add(TEXT("INF_CHAMPION"), TEXT("Champion"), EOLCUnitType::Champion, EOLCUnitCategory::Infantry, 160, 280, 36, 1200, 60, FIntPoint(1, 1));

	Add(TEXT("VEH_APC"), TEXT("APC"), EOLCUnitType::Vehicle, EOLCUnitCategory::LightVehicle, 220, 220, 18, 900, 80, FIntPoint(2, 2)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 30); AddCost(Units.Last(), EOLCResourceType::Minerals, 20); AddCost(Units.Last(), EOLCResourceType::Fuel, 15);
	Add(TEXT("VEH_TANK"), TEXT("Tank"), EOLCUnitType::Vehicle, EOLCUnitCategory::HeavyVehicle, 360, 150, 65, 1300, 120, FIntPoint(2, 2)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 40); AddCost(Units.Last(), EOLCResourceType::Minerals, 30); AddCost(Units.Last(), EOLCResourceType::Fuel, 20);
	Add(TEXT("VEH_HOVERCRAFT"), TEXT("Hovercraft"), EOLCUnitType::Vehicle, EOLCUnitCategory::LightVehicle, 180, 330, 26, 900, 70, FIntPoint(2, 1), 1, TEXT("CartoonSciFi")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 25); AddCost(Units.Last(), EOLCResourceType::Minerals, 18); AddCost(Units.Last(), EOLCResourceType::Fuel, 12);
	Add(TEXT("VEH_MISSILE_LAUNCHER"), TEXT("Missile Launcher"), EOLCUnitType::Vehicle, EOLCUnitCategory::HeavyVehicle, 190, 130, 85, 2200, 110, FIntPoint(2, 2)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 35); AddCost(Units.Last(), EOLCResourceType::Minerals, 25); AddCost(Units.Last(), EOLCResourceType::Fuel, 18);
	Add(TEXT("VEH_REPAIR"), TEXT("Repair Vehicle"), EOLCUnitType::Vehicle, EOLCUnitCategory::Support, 170, 170, 5, 400, 76, FIntPoint(2, 2), 1, TEXT("BrightRealistic")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 20); AddCost(Units.Last(), EOLCResourceType::Minerals, 15); AddCost(Units.Last(), EOLCResourceType::Fuel, 10);
	Add(TEXT("VEH_RECON_BIKE"), TEXT("Recon Bike"), EOLCUnitType::Vehicle, EOLCUnitCategory::LightVehicle, 90, 420, 12, 700, 46, FIntPoint(1, 1), 1, TEXT("CartoonSciFi")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 10); AddCost(Units.Last(), EOLCResourceType::Minerals, 6); AddCost(Units.Last(), EOLCResourceType::Fuel, 5);
	Add(TEXT("VEH_ARTILLERY"), TEXT("Artillery"), EOLCUnitType::Vehicle, EOLCUnitCategory::HeavyVehicle, 210, 110, 120, 3200, 150, FIntPoint(3, 2), 2, TEXT("DarkRealistic")); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 50); AddCost(Units.Last(), EOLCResourceType::Minerals, 40); AddCost(Units.Last(), EOLCResourceType::Fuel, 25);
	Add(TEXT("VEH_TRANSPORT"), TEXT("Transport Truck"), EOLCUnitType::Vehicle, EOLCUnitCategory::HeavyVehicle, 240, 180, 8, 400, 95, FIntPoint(3, 2)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 35); AddCost(Units.Last(), EOLCResourceType::Minerals, 20); AddCost(Units.Last(), EOLCResourceType::Fuel, 20);

	Add(TEXT("AIR_DRONE"), TEXT("Drone"), EOLCUnitType::Aerial, EOLCUnitCategory::Aerial, 45, 520, 0, 1800, 54, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::Minerals, 8); AddCost(Units.Last(), EOLCResourceType::Energy, 5);
	Add(TEXT("AIR_ATTACK_HELI"), TEXT("Attack Helicopter"), EOLCUnitType::Aerial, EOLCUnitCategory::Aerial, 180, 360, 52, 1500, 130, FIntPoint(2, 2)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 30); AddCost(Units.Last(), EOLCResourceType::Minerals, 20); AddCost(Units.Last(), EOLCResourceType::Fuel, 15);
	Add(TEXT("AIR_DROPSHIP_SUPPORT"), TEXT("Dropship Support"), EOLCUnitType::Aerial, EOLCUnitCategory::Aerial, 320, 260, 18, 1200, 180, FIntPoint(3, 3)); AddCost(Units.Last(), EOLCResourceType::ConstructionMaterial, 50); AddCost(Units.Last(), EOLCResourceType::Minerals, 40); AddCost(Units.Last(), EOLCResourceType::Fuel, 30);
	Add(TEXT("AIR_SCOUT_PLANE"), TEXT("Scout Plane"), EOLCUnitType::Aerial, EOLCUnitCategory::Aerial, 80, 620, 0, 2400, 72, FIntPoint(1, 1)); AddCost(Units.Last(), EOLCResourceType::Minerals, 10); AddCost(Units.Last(), EOLCResourceType::Energy, 8);
	for (UOLCUnitData* Unit : Units)
	{
		if (Unit && Unit->UnitType == EOLCUnitType::Aerial)
		{
			Unit->bRequiresAirfield = true;
		}
	}
	return Units;
}

TArray<UOLCUnitData*> UOLCUnitData::GetUnitsByType(UObject* Outer, EOLCUnitType Type)
{
	TArray<UOLCUnitData*> Result;
	for (UOLCUnitData* Unit : GetDefaultUnitCatalog(Outer))
	{
		if (Unit && Unit->UnitType == Type)
		{
			Result.Add(Unit);
		}
	}
	return Result;
}

TArray<UOLCUnitData*> UOLCUnitData::GetUnitsByFaction(UObject* Outer, const FString& InFactionId, int32 ColonyTIR)
{
	TArray<UOLCUnitData*> Result;
	for (UOLCUnitData* Unit : GetDefaultUnitCatalog(Outer))
	{
		if (Unit && Unit->IsAvailableForFaction(InFactionId) && Unit->IsUnlockedAtTIR(ColonyTIR))
		{
			Result.Add(Unit);
		}
	}
	return Result;
}
