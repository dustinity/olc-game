#include "OLCRaceSubsystem.h"
#include "OurLastChance.h"

#include "Logging/LogMacros.h"

void UOLCRaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Race subsystem initialized"));
}

void UOLCRaceSubsystem::RegisterRace(UOLCRaceData* Race)
{
	if (!Race || AllRaces.Contains(Race)) return;

	AllRaces.Add(Race);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Registered race: %s (%s)"),
		*Race->RaceId, *Race->DisplayName.ToString());
}

UOLCRaceData* UOLCRaceSubsystem::FindRaceById(const FString& RaceId) const
{
	for (const TObjectPtr<UOLCRaceData>& Race : AllRaces)
	{
		if (Race && Race->RaceId.Equals(RaceId, ESearchCase::IgnoreCase))
		{
			return Race.Get();
		}
	}
	return nullptr;
}

TArray<UOLCRaceData*> UOLCRaceSubsystem::GetRacesByFamily(EOLCRaceFamily Family) const
{
	TArray<UOLCRaceData*> Result;
	for (const TObjectPtr<UOLCRaceData>& Race : AllRaces)
	{
		if (Race && Race->IsRaceInFamily(Family))
		{
			Result.Add(Race.Get());
		}
	}
	return Result;
}

TArray<UOLCRaceData*> UOLCRaceSubsystem::GetSpawnableRaces(EOLCRaceBiomeType Biome) const
{
	TArray<UOLCRaceData*> Result;
	for (const TObjectPtr<UOLCRaceData>& Race : AllRaces)
	{
		if (Race && Race->GetSpawnWeightForBiome(Biome) > 0.0f)
		{
			Result.Add(Race.Get());
		}
	}
	Result.Sort([](const UOLCRaceData& A, const UOLCRaceData& B)
	{
		return A.GetSpawnWeightForBiome(A.PreferredBiomes.Num() > 0 ? A.PreferredBiomes[0] : EOLCRaceBiomeType::All) >
			B.GetSpawnWeightForBiome(B.PreferredBiomes.Num() > 0 ? B.PreferredBiomes[0] : EOLCRaceBiomeType::All);
	});
	return Result;
}

TArray<UOLCBossRaceData*> UOLCRaceSubsystem::GetAllBosses() const
{
	TArray<UOLCBossRaceData*> Result;
	for (const TObjectPtr<UOLCRaceData>& Race : AllRaces)
	{
		if (UOLCBossRaceData* Boss = Cast<UOLCBossRaceData>(Race.Get()))
		{
			Result.Add(Boss);
		}
	}
	return Result;
}

void UOLCRaceSubsystem::RegisterFactionRaceBonus(UOLCFactionRaceBonusData* Bonus)
{
	if (!Bonus || FactionRaceBonuses.Contains(Bonus)) return;

	FactionRaceBonuses.Add(Bonus);
	UE_LOG(LogOLC, Log, TEXT("[OLC] Registered faction race bonus: %s vs %s"),
		*Bonus->FactionId, *UEnum::GetValueAsString(Bonus->TargetRaceFamily));
}

UOLCFactionRaceBonusData* UOLCRaceSubsystem::FindFactionRaceBonus(const FString& FactionId, EOLCRaceFamily Family) const
{
	for (const TObjectPtr<UOLCFactionRaceBonusData>& Bonus : FactionRaceBonuses)
	{
		if (Bonus && Bonus->FactionId.Equals(FactionId, ESearchCase::IgnoreCase) && Bonus->TargetRaceFamily == Family)
		{
			return Bonus.Get();
		}
	}
	return nullptr;
}

// ---------------------------------------------------------------------------
// RegisterStarterRaces — the canonical 33-race roster (31 hostile + 2
// neutral) from Briefing/factions/Races/, grouped into 5 hostile families
// (Insectoid x6, Reptilian x6, Molluskoid x6, Humanoid x7, Crystalloid x6)
// plus 2 neutral races. Registered directly in C++ as transient NewObject()s
// — mirrors UOLCResearchSubsystem's tech registration pattern, so no MCP
// DataAsset-creation round trip is needed.
//
// Names/families/stats are re-derived from the per-race Briefing files
// (the authoritative design-canon source), replacing the earlier invented
// taxonomy (Insectoid/Mechanical/Organic/Crystal/Shadow) that never matched
// the canon despite sharing its 31+2=33 total.
//
// One boss per family (+1 neutral) mirrors the prior structure: Crystal
// Hive (Insectoid), Hydra Colonies (Reptilian), Void Octopuses (Molluskoid),
// Elite Commanders (Humanoid), Dark Matter Shapers (Crystalloid), and
// Ancient Ruin Keepers (Neutral, puzzle-boss) — each is explicitly called
// out as boss/elite-tier or is its family's highest-TIR entry in the
// source specs.
// ---------------------------------------------------------------------------
void UOLCRaceSubsystem::RegisterStarterRaces()
{
	// ===== INSECTOID FAMILY (6) — segmented exoskeletons, swarm/adapt doctrine =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_InsectoidChitinBugs"));
		Race->RaceId = TEXT("InsectoidChitinBugs");
		Race->DisplayName = FText::FromString(TEXT("Chitin Bugs"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 25.0f;
		Race->BaseDamage = 9.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Desert, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 1.4f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.8f);
		Race->SpecialAbility = FText::FromString(TEXT("Swarm attacker (groups of 5-15) — death explosion releases 2-4 smaller buglets at 50% original damage."));
		Race->Weakness = FText::FromString(TEXT("Fire damage deals +50%."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_InsectoidBurrowers"));
		Race->RaceId = TEXT("InsectoidBurrowers");
		Race->DisplayName = FText::FromString(TEXT("Burrowers"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 300.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::Desert };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.3f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 0.9f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Underground ambush predator — untargetable while burrowed, 2-second ground-tremor warning before surfacing."));
		Race->Weakness = FText::FromString(TEXT("Area damage reveals and damages them underground."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_InsectoidWingStingers"));
		Race->RaceId = TEXT("InsectoidWingStingers");
		Race->DisplayName = FText::FromString(TEXT("Wing Stingers"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 40.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 410.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Jungle, EOLCRaceBiomeType::LightSnow };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Jungle, 1.3f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::LightSnow, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Aerial dive-bomb harassment — flying, immune to ground-based traps and terrain effects."));
		Race->Weakness = FText::FromString(TEXT("Fragile, easy anti-air target."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_InsectoidSporeQueens"));
		Race->RaceId = TEXT("InsectoidSporeQueens");
		Race->DisplayName = FText::FromString(TEXT("Spore Queens"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 5.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp, EOLCRaceBiomeType::Jungle };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.3f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Jungle, 1.1f);
		Race->SpecialAbility = FText::FromString(TEXT("Support unit — spawns insect waves every 30s, deploys a slowing spore cloud (-40% in 10m radius)."));
		Race->Weakness = FText::FromString(TEXT("High-value target — destroying the Queen stops the spawns."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_InsectoidArmoredMantis"));
		Race->RaceId = TEXT("InsectoidArmoredMantis");
		Race->DisplayName = FText::FromString(TEXT("Armored Mantis"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 36.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Jungle, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Jungle, 0.9f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Elite melee unit — telegraphed charge pierces walls and structures (+100% building damage)."));
		Race->Weakness = FText::FromString(TEXT("Charge is telegraphed 1 second in advance — dodge or block."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_InsectoidCrystalHive"));
		Race->RaceId = TEXT("InsectoidCrystalHive");
		Race->DisplayName = FText::FromString(TEXT("Crystal Hive"));
		Race->RaceFamily = EOLCRaceFamily::Insectoid;
		Race->BaseHP = 700.0f;
		Race->BaseDamage = 32.0f;
		Race->MovementSpeed = 220.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::CrystalCaves, EOLCRaceBiomeType::Ice };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CrystalCaves, 0.15f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 0.15f);
		Race->SpecialAbility = FText::FromString(TEXT("Boss-tier insect infused with crystal energy — reflects 50% of projectile damage back at the attacker, spawns crystal shards on death."));
		Race->Weakness = FText::FromString(TEXT("Energy weapons deal +75% damage; ice-based attacks slow it."));
		Race->BossTier = EOLCBossTier::Fortress;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Reflects 50% of projectile damage; energy weapons bypass the reflection for +75% damage, ice attacks slow it."));
		Race->FinalPhaseEnrageTimerSeconds = 60.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Basic melee/spit attacks; passive projectile reflection active."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.3f, true, FText::FromString(TEXT("Summons Chitin Bugs and Burrowers waves, +30% damage."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.6f, true, FText::FromString(TEXT("Crystal shard barrage, self-repair, elite Armored Mantis add."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 2.0f, true, FText::FromString(TEXT("Reflection intensifies to 75%; one-shot enrage if not defeated within 60s."))));
		RegisterRace(Race);
	}

	// ===== REPTILIAN FAMILY (6) — scaled skin, terrain-adapted ambush/pack doctrine =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_ReptilianTerradons"));
		Race->RaceId = TEXT("ReptilianTerradons");
		Race->DisplayName = FText::FromString(TEXT("Terradons"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 22.0f;
		Race->MovementSpeed = 190.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Jungle, EOLCRaceBiomeType::Desert };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Jungle, 1.2f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Dinosaur-like brute — charge-and-bite attacks, stomp creates a knockback shockwave (3m radius)."));
		Race->Weakness = FText::FromString(TEXT("Slow to turn around — attack from behind."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_ReptilianScalebacks"));
		Race->RaceId = TEXT("ReptilianScalebacks");
		Race->DisplayName = FText::FromString(TEXT("Scalebacks"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 240.0f;
		Race->BaseDamage = 9.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Desert };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.2f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Turtle-like defensive unit — retracts into its shell, immune to all damage for 5s (30s cooldown)."));
		Race->Weakness = FText::FromString(TEXT("Rear attacks while extended deal +100% damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_ReptilianViperPacks"));
		Race->RaceId = TEXT("ReptilianViperPacks");
		Race->DisplayName = FText::FromString(TEXT("Viper Packs"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 25.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 330.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.2f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.8f);
		Race->SpecialAbility = FText::FromString(TEXT("Hunts in packs of 3-8 — venomous bite applies stacking poison (2 dmg/sec for 10s, stacks to 5)."));
		Race->Weakness = FText::FromString(TEXT("Fragile — killed quickly by area damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_ReptilianFrostDrakes"));
		Race->RaceId = TEXT("ReptilianFrostDrakes");
		Race->DisplayName = FText::FromString(TEXT("Frost Drakes"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 22.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::LightSnow };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 1.3f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::LightSnow, 1.1f);
		Race->SpecialAbility = FText::FromString(TEXT("Ice-breathing reptile — breath attack freezes targets solid for 3s (breakable with damage)."));
		Race->Weakness = FText::FromString(TEXT("Fire weapons deal +100% damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_ReptilianMagmaLizards"));
		Race->RaceId = TEXT("ReptilianMagmaLizards");
		Race->DisplayName = FText::FromString(TEXT("Magma Lizards"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 36.0f;
		Race->MovementSpeed = 290.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Desert };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Heat-resistant, leaves a lava trail — death explosion creates a persistent 5m lava zone (10 dmg/sec for 30s)."));
		Race->Weakness = FText::FromString(TEXT("Cold damage slows its movement by -60%."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_ReptilianHydraColonies"));
		Race->RaceId = TEXT("ReptilianHydraColonies");
		Race->DisplayName = FText::FromString(TEXT("Hydra Colonies"));
		Race->RaceFamily = EOLCRaceFamily::Reptilian;
		Race->BaseHP = 720.0f;
		Race->BaseDamage = 34.0f;
		Race->MovementSpeed = 210.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 0.15f);
		Race->SpecialAbility = FText::FromString(TEXT("Multi-head regenerating boss — each head attacks independently; sheds a head at 66% and 33% HP but regrows 2 heads if the main body survives 10 seconds."));
		Race->Weakness = FText::FromString(TEXT("Focus fire to kill all heads before regeneration completes."));
		Race->BossTier = EOLCBossTier::Fortress;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Independent heads attack separately; sheds a head at 66%/33% HP but regrows 2 if left alone for 10s — focus fire is mandatory."));
		Race->FinalPhaseEnrageTimerSeconds = 60.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Three heads attack independently with basic bites."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.3f, true, FText::FromString(TEXT("First head lost — regeneration timer begins, summons Viper Pack adds."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.6f, true, FText::FromString(TEXT("Second head lost — regrowth accelerates, Terradon stomp adds, +30% damage."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 2.0f, true, FText::FromString(TEXT("All heads snapping at once; one-shot enrage if not defeated within 60s."))));
		RegisterRace(Race);
	}

	// ===== MOLLUSKOID FAMILY (6) — soft bodies/shells/tentacles, defense/area-denial doctrine =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_MolluskoidShellSnails"));
		Race->RaceId = TEXT("MolluskoidShellSnails");
		Race->DisplayName = FText::FromString(TEXT("Shell Snails"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 5.0f;
		Race->MovementSpeed = 90.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.3f);
		Race->SpecialAbility = FText::FromString(TEXT("Slow but heavily armored — shell is nearly impenetrable from the front, leaves a slime trail that slows enemies by -50%."));
		Race->Weakness = FText::FromString(TEXT("A rare, telegraphed flip attack exposes its vulnerable underside."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_MolluskoidAcidSpitters"));
		Race->RaceId = TEXT("MolluskoidAcidSpitters");
		Race->DisplayName = FText::FromString(TEXT("Acid Spitters"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 30.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.2f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.8f);
		Race->SpecialAbility = FText::FromString(TEXT("Ranged attacker — acid projectiles corrode armor, each hit reduces target defense by -10% (stacks to 5)."));
		Race->Weakness = FText::FromString(TEXT("Fragile, killed quickly by focused fire."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_MolluskoidTentacleCrawlers"));
		Race->RaceId = TEXT("MolluskoidTentacleCrawlers");
		Race->DisplayName = FText::FromString(TEXT("Tentacle Crawlers"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.3f);
		Race->SpecialAbility = FText::FromString(TEXT("Grappling melee unit — pulls enemies closer and tangles them, unable to move or attack for 5s (allies can break them free)."));
		Race->Weakness = FText::FromString(TEXT("Fire deals +50% damage; area damage hits all tentacles simultaneously."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_MolluskoidGlowSquids"));
		Race->RaceId = TEXT("MolluskoidGlowSquids");
		Race->DisplayName = FText::FromString(TEXT("Glow Squids"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 9.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.1f);
		Race->SpecialAbility = FText::FromString(TEXT("Bioluminescent flash — all units in a 10m radius miss their next attack (3s cooldown per unit)."));
		Race->Weakness = FText::FromString(TEXT("Dark biomes reduce effectiveness; light-based weapons stun them."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_MolluskoidIronClams"));
		Race->RaceId = TEXT("MolluskoidIronClams");
		Race->DisplayName = FText::FromString(TEXT("Iron Clams"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 240.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 0.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Swamp };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Stationary turret-like unit — reflects 20% damage to adjacent melee attackers, spawns smaller mollusks when destroyed."));
		Race->Weakness = FText::FromString(TEXT("Easy to kite — attack from range before reaching it."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_MolluskoidVoidOctopuses"));
		Race->RaceId = TEXT("MolluskoidVoidOctopuses");
		Race->DisplayName = FText::FromString(TEXT("Void Octopuses"));
		Race->RaceFamily = EOLCRaceFamily::Molluskoid;
		Race->BaseHP = 680.0f;
		Race->BaseDamage = 34.0f;
		Race->MovementSpeed = 260.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::CrystalCaves, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CrystalCaves, 0.15f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.15f);
		Race->SpecialAbility = FText::FromString(TEXT("Camouflage and teleportation elite — blinks to a random location within 20m every 8 seconds, leaving an afterimage decoy."));
		Race->Weakness = FText::FromString(TEXT("Radar/sonar reveals its true position; area-denial weapons can predict teleport targets."));
		Race->BossTier = EOLCBossTier::Fortress;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Teleports every 8s leaving a decoy; radar/sonar reveals the real target through the camouflage."));
		Race->FinalPhaseEnrageTimerSeconds = 60.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Camouflaged, occasional tentacle strikes."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.3f, true, FText::FromString(TEXT("Teleport frequency doubles, summons Tentacle Crawler adds, +30% damage."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.6f, true, FText::FromString(TEXT("Ink cloud blinds targeting, self-heal, elite Iron Clam add."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 2.0f, true, FText::FromString(TEXT("Rapid teleport spam; one-shot enrage if not defeated within 60s."))));
		RegisterRace(Race);
	}

	// ===== HUMANOID FAMILY (7) — bipedal, organization/tactics doctrine =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidScavengers"));
		Race->RaceId = TEXT("HumanoidScavengers");
		Race->DisplayName = FText::FromString(TEXT("Scavengers"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 25.0f;
		Race->BaseDamage = 5.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::All };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::All, 1.2f);
		Race->SpecialAbility = FText::FromString(TEXT("Weak but numerous (spawn in groups of 10-30), uses found/looted equipment — 50% chance to drop bonus resources on death."));
		Race->Weakness = FText::FromString(TEXT("Flees when the leader unit is killed (morale break)."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidPunkRaiders"));
		Race->RaceId = TEXT("HumanoidPunkRaiders");
		Race->DisplayName = FText::FromString(TEXT("Punk Raiders"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 290.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Desert, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 1.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Armed with crude but effective weapons, aggressive patrols — War Paint grants +20% damage when in groups of 3+."));
		Race->Weakness = FText::FromString(TEXT("Predictable individual attack patterns."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidRustbornClan"));
		Race->RaceId = TEXT("HumanoidRustbornClan");
		Race->DisplayName = FText::FromString(TEXT("Rustborn Clan"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 22.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Desert, EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Ruins, EOLCRaceBiomeType::IndustrialWreckage };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 1.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ruins, 0.8f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::IndustrialWreckage, 0.8f);
		Race->SpecialAbility = FText::FromString(TEXT("Always-hostile ambush patrols with welded scrap fortifications — Blood Oath grants nearby clan members +15% damage resistance after the first clan unit dies."));
		Race->Weakness = FText::FromString(TEXT("Poor shields and weak long-range discipline; vulnerable to organized kiting and ion disruption."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidMedicColonies"));
		Race->RaceId = TEXT("HumanoidMedicColonies");
		Race->DisplayName = FText::FromString(TEXT("Medic Colonies"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 40.0f;
		Race->BaseDamage = 5.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::All };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::All, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Healing support unit — field medicine heals adjacent enemies for 5 HP/sec, keeping enemy groups in the fight."));
		Race->Weakness = FText::FromString(TEXT("Falls quickly to fast units and area damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidEngineerFactions"));
		Race->RaceId = TEXT("HumanoidEngineerFactions");
		Race->DisplayName = FText::FromString(TEXT("Engineer Factions"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 9.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Desert, EOLCRaceBiomeType::IndustrialWreckage };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Desert, 0.9f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::IndustrialWreckage, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Builds defensive structures during combat — Fortification constructs an automated turret at a location within 10 seconds."));
		Race->Weakness = FText::FromString(TEXT("Turret construction can be interrupted with damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_HumanoidMilitaryOutposts"));
		Race->RaceId = TEXT("HumanoidMilitaryOutposts");
		Race->DisplayName = FText::FromString(TEXT("Military Outposts"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 22.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::Jungle, EOLCRaceBiomeType::LightSnow };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Jungle, 0.9f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::LightSnow, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Organized troops with vehicles — calls in an armored vehicle reinforcement after 60 seconds of sustained combat."));
		Race->Weakness = FText::FromString(TEXT("Killing the officer unit causes remaining units to retreat for 30 seconds."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_HumanoidEliteCommanders"));
		Race->RaceId = TEXT("HumanoidEliteCommanders");
		Race->DisplayName = FText::FromString(TEXT("Elite Commanders"));
		Race->RaceFamily = EOLCRaceFamily::Humanoid;
		Race->BaseHP = 700.0f;
		Race->BaseDamage = 32.0f;
		Race->MovementSpeed = 250.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::All };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::All, 0.15f);
		Race->SpecialAbility = FText::FromString(TEXT("High-TIR leader — tactical aura grants +25% damage and +15% defense to all friendly units within 20m."));
		Race->Weakness = FText::FromString(TEXT("Most valuable target — removing the commander causes cascading enemy retreat."));
		Race->BossTier = EOLCBossTier::Fortress;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Tactical aura buffs all nearby allies; killing the commander breaks enemy morale across the encounter."));
		Race->FinalPhaseEnrageTimerSeconds = 60.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Aura active, basic ranged/melee attacks."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.3f, true, FText::FromString(TEXT("Summons Military Outposts and Punk Raiders adds, +30% damage."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.6f, true, FText::FromString(TEXT("Aura radius doubles, self-shield, elite Rustborn Clan add."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 2.0f, true, FText::FromString(TEXT("All buffs maxed; one-shot enrage if not defeated within 60s."))));
		RegisterRace(Race);
	}

	// ===== CRYSTALLOID FAMILY (6) — crystalline structures, elemental resistance/area-control doctrine =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_CrystalloidCrystalShards"));
		Race->RaceId = TEXT("CrystalloidCrystalShards");
		Race->DisplayName = FText::FromString(TEXT("Crystal Shards"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 25.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::Rocky, EOLCRaceBiomeType::CrystalCaves };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 1.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CrystalCaves, 1.2f);
		Race->SpecialAbility = FText::FromString(TEXT("Fragile but multiplies on death — each destroyed Shard splits into 3 smaller fragments; melee attackers take 2 damage in return."));
		Race->Weakness = FText::FromString(TEXT("Sound/sonic weapons deal +100% damage."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_CrystalloidObsidianGolems"));
		Race->RaceId = TEXT("CrystalloidObsidianGolems");
		Race->DisplayName = FText::FromString(TEXT("Obsidian Golems"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 240.0f;
		Race->BaseDamage = 36.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 1.0f);
		Race->SpecialAbility = FText::FromString(TEXT("Heavy melee construct of volcanic glass — Shatter explodes into razor shards on death, covering an 8m radius (50 damage)."));
		Race->Weakness = FText::FromString(TEXT("Temperature extremes crack its structure (-30% HP)."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_CrystalloidPrismGuardians"));
		Race->RaceId = TEXT("CrystalloidPrismGuardians");
		Race->DisplayName = FText::FromString(TEXT("Prism Guardians"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 110.0f;
		Race->BaseDamage = 28.0f;
		Race->MovementSpeed = 150.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::CrystalCaves, EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CrystalCaves, 1.2f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Light-reflection beam attacker — Refraction splits its beam into 3 when hitting crystal surfaces, hitting multiple targets."));
		Race->Weakness = FText::FromString(TEXT("Fog/smoke reduces beam accuracy by -70%."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_CrystalloidRadioactiveCores"));
		Race->RaceId = TEXT("CrystalloidRadioactiveCores");
		Race->DisplayName = FText::FromString(TEXT("Radioactive Cores"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 160.0f;
		Race->BaseDamage = 15.0f;
		Race->MovementSpeed = 60.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Swamp, EOLCRaceBiomeType::Rocky };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Swamp, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Rocky, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Floating energy sphere — radiation aura deals 1 dmg/sec and -20% accuracy to units within 15m."));
		Race->Weakness = FText::FromString(TEXT("Magnetic weapons pull it toward the source, disrupting the aura."));
		RegisterRace(Race);
	}
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_CrystalloidResonanceCrystals"));
		Race->RaceId = TEXT("CrystalloidResonanceCrystals");
		Race->DisplayName = FText::FromString(TEXT("Resonance Crystals"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 65.0f;
		Race->BaseDamage = 28.0f;
		Race->MovementSpeed = 0.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::LightSnow };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 1.0f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::LightSnow, 0.9f);
		Race->SpecialAbility = FText::FromString(TEXT("Sonic vibration attacker — each crystal plays a different note; multiple crystals create a destructive interference pattern that shatters armor."));
		Race->Weakness = FText::FromString(TEXT("Silence fields or dampening walls block sonic attacks entirely."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_CrystalloidDarkMatterShapers"));
		Race->RaceId = TEXT("CrystalloidDarkMatterShapers");
		Race->DisplayName = FText::FromString(TEXT("Dark Matter Shapers"));
		Race->RaceFamily = EOLCRaceFamily::Crystalloid;
		Race->BaseHP = 780.0f;
		Race->BaseDamage = 48.0f;
		Race->MovementSpeed = 200.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Ice, EOLCRaceBiomeType::CrystalCaves, EOLCRaceBiomeType::CenterGalaxy };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ice, 0.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CrystalCaves, 0.1f);
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CenterGalaxy, 0.05f);
		Race->SpecialAbility = FText::FromString(TEXT("Gravity-manipulation elite — warps space, pulling all units within 15m toward its center for 5 seconds before releasing a devastating pulse."));
		Race->Weakness = FText::FromString(TEXT("Light-based weapons disrupt the gravity field for +150% damage."));
		Race->BossTier = EOLCBossTier::Fortress;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Gravity well pulls units in for 5s then pulses; light-based weapons disrupt the field for +150% damage."));
		Race->FinalPhaseEnrageTimerSeconds = 60.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Basic gravity pulses, passive space-warp distortion."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.3f, true, FText::FromString(TEXT("Summons Crystal Shards and Resonance Crystals adds, +30% damage."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.6f, true, FText::FromString(TEXT("Gravity well radius doubles, self-repair, elite Obsidian Golem add."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 2.0f, true, FText::FromString(TEXT("Continuous gravity pulses; one-shot enrage if not defeated within 60s."))));
		RegisterRace(Race);
	}

	// ===== NEUTRAL RACES (2) — non-hostile; Ruins/All =====
	{
		UOLCRaceData* Race = NewObject<UOLCRaceData>(this, TEXT("RD_Race_NeutralAsgardLikeTraders"));
		Race->RaceId = TEXT("NeutralAsgardLikeTraders");
		Race->DisplayName = FText::FromString(TEXT("Asgard-like Traders"));
		Race->RaceFamily = EOLCRaceFamily::Neutral;
		Race->BaseHP = 50.0f;
		Race->BaseDamage = 0.0f;
		Race->MovementSpeed = 200.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::All };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::All, 0.3f);
		Race->SpecialAbility = FText::FromString(TEXT("Non-hostile merchant NPCs found at abandoned stations or neutral outposts — trade blueprints and resources for energy, minerals, and construction materials; inventory scales with player TIR; will escort valuable cargo if protected from pirates."));
		Race->Weakness = FText::FromString(TEXT("Unarmed — flees or relies on its escort if attacked."));
		RegisterRace(Race);
	}
	{
		UOLCBossRaceData* Race = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_NeutralAncientRuinKeepers"));
		Race->RaceId = TEXT("NeutralAncientRuinKeepers");
		Race->DisplayName = FText::FromString(TEXT("Ancient Ruin Keepers"));
		Race->RaceFamily = EOLCRaceFamily::Neutral;
		Race->BaseHP = 520.0f;
		Race->BaseDamage = 30.0f;
		Race->MovementSpeed = 100.0f;
		Race->PreferredBiomes = { EOLCRaceBiomeType::Ruins };
		Race->BiomeSpawnWeights.Add(EOLCRaceBiomeType::Ruins, 0.2f);
		Race->SpecialAbility = FText::FromString(TEXT("Non-hostile AI guardians of ancient structures — stand motionless until approached, then communicate or test the player; puzzle-based encounter that combat alone cannot defeat."));
		Race->Weakness = FText::FromString(TEXT("Solving the ruin's environmental puzzle disables its defenses and exposes it to damage."));
		Race->BossTier = EOLCBossTier::Large;
		Race->SpecialMechanicDescription = FText::FromString(TEXT("Puzzle-based boss — the environmental puzzle must be solved to lower its defenses before it can be damaged normally; rewards unique blueprints, permanent stat boosts, and key items."));
		Race->FinalPhaseEnrageTimerSeconds = 90.0f;
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Defenses up — puzzle must be solved before damage registers."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.2f, false, FText::FromString(TEXT("Defenses partially down, +20% damage, basic ranged attacks."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 1.5f, false, FText::FromString(TEXT("Area collapse hazards, self-repair."))));
		Race->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 1.8f, false, FText::FromString(TEXT("All defenses maxed; one-shot enrage if not defeated within 90s."))));
		RegisterRace(Race);
	}

	// ===== FACTION RACE BONUSES — all 4 playable factions, per Briefing/factions/README.md =====
	{
		UOLCFactionRaceBonusData* Bonus = NewObject<UOLCFactionRaceBonusData>(this, TEXT("RD_FactionBonus_DarkRealisticVsInsectoid"));
		Bonus->FactionId = TEXT("DarkRealistic");
		Bonus->TargetRaceFamily = EOLCRaceFamily::Insectoid;
		Bonus->DamageBonusPercent = 20.0f;
		Bonus->TooltipDescription = FText::FromString(TEXT("Dark Realistic: +20% damage vs Insectoid (organized swarm tactics match military discipline)."));
		RegisterFactionRaceBonus(Bonus);
	}
	{
		UOLCFactionRaceBonusData* Bonus = NewObject<UOLCFactionRaceBonusData>(this, TEXT("RD_FactionBonus_CartoonSciFiVsReptilian"));
		Bonus->FactionId = TEXT("CartoonSciFi");
		Bonus->TargetRaceFamily = EOLCRaceFamily::Reptilian;
		Bonus->SpeedBonusPercent = 25.0f;
		Bonus->TooltipDescription = FText::FromString(TEXT("Cartoon SciFi: +25% speed vs Reptilian (agility counters brute force)."));
		RegisterFactionRaceBonus(Bonus);
	}
	{
		UOLCFactionRaceBonusData* Bonus = NewObject<UOLCFactionRaceBonusData>(this, TEXT("RD_FactionBonus_BrightRealisticVsMolluskoid"));
		Bonus->FactionId = TEXT("BrightRealistic");
		Bonus->TargetRaceFamily = EOLCRaceFamily::Molluskoid;
		Bonus->LootBonusPercent = 30.0f;
		Bonus->TooltipDescription = FText::FromString(TEXT("Bright Realistic: +30% Survival resource yield vs Molluskoid (biome adaptation research)."));
		RegisterFactionRaceBonus(Bonus);
	}
	{
		// Legacy design notes credited this bonus to a "Mechanical" race family, which does not exist in
		// the canonical roster (Insectoid/Reptilian/Molluskoid/Humanoid/Crystalloid). Humanoid — with its
		// turret-building Engineer Factions and vehicle-driving Military Outposts — is the closest thematic
		// match to the "understands tech" rationale, so the bonus is repointed there.
		UOLCFactionRaceBonusData* Bonus = NewObject<UOLCFactionRaceBonusData>(this, TEXT("RD_FactionBonus_NeonPunkVsHumanoid"));
		Bonus->FactionId = TEXT("NeonPunk");
		Bonus->TargetRaceFamily = EOLCRaceFamily::Humanoid;
		Bonus->LootBonusPercent = 15.0f;
		Bonus->TooltipDescription = FText::FromString(TEXT("Neon Punk: +15% loot from Humanoid encounters (understands engineered tech)."));
		RegisterFactionRaceBonus(Bonus);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Races registered: %d total (%d bosses), faction bonuses: %d"),
		AllRaces.Num(), GetAllBosses().Num(), FactionRaceBonuses.Num());
}

// ---------------------------------------------------------------------------
// Endgame bosses (WP-125 Step 4)
// ---------------------------------------------------------------------------

void UOLCRaceSubsystem::RegisterEndgameBosses()
{
	// ===== CENTER GALAXY OVERLORD — final boss (EOLCBossTier::CentralGalaxy) =====
	{
		UOLCBossRaceData* Boss = NewObject<UOLCBossRaceData>(this, TEXT("RD_Race_CenterGalaxyOverlord"));
		Boss->RaceId = TEXT("CenterGalaxyOverlord");
		Boss->DisplayName = FText::FromString(TEXT("Center Galaxy Overlord"));
		Boss->RaceFamily = EOLCRaceFamily::Unknown; // Alien entity — no standard family match
		Boss->BaseHP = 5000.0f;
		Boss->BaseDamage = 80.0f;
		Boss->MovementSpeed = 120.0f;
		Boss->PreferredBiomes = { EOLCRaceBiomeType::CenterGalaxy };
		Boss->BiomeSpawnWeights.Add(EOLCRaceBiomeType::CenterGalaxy, 1.0f);
		Boss->SpecialAbility = FText::FromString(TEXT("Commands all alien forces in the center galaxy; adapts to player strategy each phase."));
		Boss->Weakness = FText::FromString(TEXT("Void damage bypasses its adaptive shielding during Desperate phase."));
		Boss->BossTier = EOLCBossTier::CentralGalaxy;
		Boss->SpecialMechanicDescription = FText::FromString(TEXT("Final assault: 4-phase encounter requiring full fleet preparation. Phase 1 summons Shadow minions, Phase 2 deploys Crystal defenses (energy weapons ineffective), Phase 3 creates gravity wells pulling units in, Phase 4 enrage with all damage types amplified."));
		Boss->FinalPhaseEnrageTimerSeconds = 90.0f;
		Boss->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Intro")), 100.0f, 1.0f, false, FText::FromString(TEXT("Summons Shadow family minions; player must clear adds to focus fire."))));
		Boss->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Enrage")), 70.0f, 1.5f, true, FText::FromString(TEXT("Deploys Crystal family defenses; energy weapons ineffective — use ballistic/kinetic."))));
		Boss->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Desperate")), 40.0f, 2.0f, true, FText::FromString(TEXT("Creates gravity wells that pull units in; formation changes required to avoid clumping."))));
		Boss->Phases.Add(FOLCBossPhaseData(FText::FromString(TEXT("Final")), 15.0f, 3.0f, true, FText::FromString(TEXT("Enrage — all damage types amplified; last stand with everything available. Enrage timer: 90s."))));
		RegisterRace(Boss);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Endgame bosses registered: %d total races (%d bosses)"),
		AllRaces.Num(), GetAllBosses().Num());
}
