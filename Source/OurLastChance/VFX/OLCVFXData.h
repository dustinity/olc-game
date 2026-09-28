#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "Combat/OLCCombatTypes.h"
#include "Core/OLCBuildingData.h"
#include "OLCVFXData.generated.h"

// ---------------------------------------------------------------------------
// FOLCCombatVFXMapping — maps a damage type to its Niagara VFX system
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCCombatVFXMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	EOLCDamageType DamageType = EOLCDamageType::Kinetic;

	/** Soft reference to the Niagara particle system for this damage type. Null = no VFX (null-tolerant). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;
};

// ---------------------------------------------------------------------------
// FOLCEnvironmentalVFXMapping — maps a named environmental event key to its Niagara system
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCEnvironmentalVFXMapping
{
	GENERATED_BODY()

	/** Named key identifying the environmental effect (e.g. "EMP", "DustStorm", "Lightning"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	FName EventKey = NAME_None;

	/** Soft reference to the Niagara particle system. Null = no VFX (null-tolerant). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;
};

// ---------------------------------------------------------------------------
// FOLCBiomeAmbientMapping — maps a biome type to its ambient particle system
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCBiomeAmbientMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	EOLCBiomeType BiomeType = EOLCBiomeType::Desert;

	/** Soft reference to the ambient Niagara particle system. Null = no VFX (null-tolerant). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;
};

// ---------------------------------------------------------------------------
// UOLCVFXData — designer-tunable VFX mapping table (WP-126)
// ---------------------------------------------------------------------------
/**
 * Central VFX configuration data asset for all visual effects in the game.
 * Covers combat VFX (per damage type), environmental/special effects (by named key),
 * biome ambient particles (per biome), and named crash/landing + building effects.
 *
 * Ships built-in engine-stock defaults via static GetDefault*() methods so gameplay
 * never depends on the persistent asset existing (mirrors UOLCColonyNetworkData).
 * All soft references are null-tolerant: consuming code must check validity before spawn.
 */
UCLASS()
class OURLASTCHANCE_API UOLCVFXData : public UDataAsset
{
	GENERATED_BODY()

public:
	// --- Combat VFX (per damage type) ----------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Combat")
	TArray<FOLCCombatVFXMapping> CombatMappings;

	// --- Environmental / Special VFX (by named event key) --------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Environmental")
	TArray<FOLCEnvironmentalVFXMapping> EnvironmentalMappings;

	// --- Biome Ambient Particles (per biome) ---------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|BiomeAmbient")
	TArray<FOLCBiomeAmbientMapping> BiomeAmbientMappings;

	// --- Crash / Landing VFX --------------------------------------------------

	/** One-shot crash impact explosion (fire sphere → smoke column → debris → embers). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Crash")
	TSoftObjectPtr<UNiagaraSystem> CrashImpactExplosion;

	/** Expanding dust ring at crash site edges. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Crash")
	TSoftObjectPtr<UNiagaraSystem> CrashLandingDust;

	/** Dropship smoke columns (fade over 30 s). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Crash")
	TSoftObjectPtr<UNiagaraSystem> CrashSmokeTrail;

	// --- Building VFX ---------------------------------------------------------

	/** Construction progress glow (green shimmer, intensity scales with build %). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Building")
	TSoftObjectPtr<UNiagaraSystem> BuildingConstructionGlow;

	/** Building destruction debris burst. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|VFX|Building")
	TSoftObjectPtr<UNiagaraSystem> BuildingDestructionBurst;

	// --- Built-in defaults (null-tolerant: all NiagaraSystem pointers are null) --

	/**
	 * Returns one row per EOLCDamageType (Kinetic, Energy, Explosive, Void).
	 * All NiagaraSystem pointers are null by default — the consuming code
	 * must fall back to a generic effect or skip when null.
	 */
	static TArray<FOLCCombatVFXMapping> GetDefaultCombatMappings();

	/**
	 * Returns rows for the standard environmental event keys:
	 * EMP, DustStorm, Lightning, SporeCloud, CrystalResonance.
	 * All NiagaraSystem pointers are null by default.
	 */
	static TArray<FOLCEnvironmentalVFXMapping> GetDefaultEnvironmentalMappings();

	/**
	 * Returns one row per EOLCBiomeType (all 8 biomes).
	 * All NiagaraSystem pointers are null by default.
	 */
	static TArray<FOLCBiomeAmbientMapping> GetDefaultBiomeAmbientMappings();
};
