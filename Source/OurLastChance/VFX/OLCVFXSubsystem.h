#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Ticker.h"
#include "Niagara/Classes/NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Combat/OLCCombatTypes.h"
#include "Core/OLCBuildingData.h"
#include "OLCVFXSubsystem.generated.h"

class UOLCVFXData;

/**
 * GameInstanceSubsystem that owns all VFX spawning and resolution (WP-126 Step 2).
 *
 * Provides:
 * - Core spawn primitives (one-shot, looping, attached) via UNiagaraFunctionLibrary.
 * - An FTSTicker reaper that prunes finished one-shot components from the tracking array.
 * - Semantic helpers that resolve the correct UNiagaraSystem from a registered
 *   UOLCVFXData asset (falling back to built-in defaults; null → warn + no-op).
 *
 * Tick note: UGameInstanceSubsystem has no virtual Tick in UE 5.8, so this class
 * self-registers an FTSTicker delegate in Initialize() and removes it in Deinitialize().
 */
UCLASS()
class OURLASTCHANCE_API UOLCVFXSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// -----------------------------------------------------------------------
	// Core spawn primitives
	// -----------------------------------------------------------------------

	/**
	 * Spawn a one-shot Niagara system at the given world location.
	 * The component auto-destroys when the system finishes (bAutoDestroy = true).
	 * Tracked by the internal reaper for cleanup on world teardown.
	 * @return The spawned NiagaraComponent, or null if System is null.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX")
	UNiagaraComponent* SpawnOneShotAtLocation(const UNiagaraSystem* System, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.0f));

	/**
	 * Spawn a looping Niagara system at the given world location.
	 * The component does NOT auto-destroy — the caller owns its lifetime.
	 * @return The spawned NiagaraComponent (as an actor handle), or null if System is null.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX")
	AActor* SpawnLoopingAtLocation(const UNiagaraSystem* System, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.0f));

	/**
	 * Create a looping Niagara component attached to the given scene component.
	 * The component does NOT auto-destroy — it dies with its parent.
	 * @return The attached NiagaraComponent, or null if System or AttachTo is null.
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX")
	UNiagaraComponent* AttachLoopingToComponent(const UNiagaraSystem* System, USceneComponent* AttachTo, FName AttachPointName = NAME_None);

	// -----------------------------------------------------------------------
	// Semantic helpers (resolve from UOLCVFXData; null → warn + no-op)
	// -----------------------------------------------------------------------

	/** Play the combat VFX for the given damage type at the specified location. */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Combat")
	void PlayCombatVFX(EOLCDamageType DamageType, FVector Location);

	/** Play an environmental/special VFX by named event key (e.g. "EMP", "DustStorm"). */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Environmental")
	void PlayEnvironmentalVFX(FName EventKey, FVector Location);

	/** Spawn the ambient biome particle system at the given location (looping). */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|BiomeAmbient")
	AActor* SpawnBiomeAmbient(EOLCBiomeType BiomeType, FVector Location);

	// --- Crash / Landing VFX --------------------------------------------------

	/** Play the one-shot crash impact explosion at the given location. */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Crash")
	void PlayCrashImpact(FVector Location);

	/** Play the expanding dust ring at the crash site. */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Crash")
	void PlayCrashDust(FVector Location);

	/** Spawn the dropship smoke trail (looping; caller manages lifetime). */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Crash")
	AActor* PlayCrashSmoke(FVector Location);

	// --- Building VFX ---------------------------------------------------------

	/** Attach the construction progress glow to a building component. */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Building")
	UNiagaraComponent* PlayBuildingConstructionGlow(USceneComponent* BuildingRoot);

	/** Play the one-shot building destruction debris burst at the given location. */
	UFUNCTION(BlueprintCallable, Category = "OLC|VFX|Building")
	void PlayBuildingDestruction(FVector Location);

	// -----------------------------------------------------------------------
	// Data access
	// -----------------------------------------------------------------------

	/** Returns the registered VFX data asset (may be null — built-in defaults apply). */
	const UOLCVFXData* GetVFXData() const { return VFXData; }

private:
	// --- Resolution helpers ---------------------------------------------------

	/** Resolve a Niagara system for the given damage type from data or defaults. Null if unavailable. */
	const UNiagaraSystem* ResolveCombatSystem(EOLCDamageType DamageType) const;

	/** Resolve a Niagara system for the given environmental event key. Null if unavailable. */
	const UNiagaraSystem* ResolveEnvironmentalSystem(FName EventKey) const;

	/** Resolve a Niagara system for the given biome ambient type. Null if unavailable. */
	const UNiagaraSystem* ResolveBiomeAmbientSystem(EOLCBiomeType BiomeType) const;

	// --- FTSTicker reaper -----------------------------------------------------

	/** Ticker callback: prunes destroyed/finished one-shot components from the tracking array. */
	bool HandleReaperTick(float DeltaTime);

	// --- State ----------------------------------------------------------------

	/** Registered VFX data asset (loaded in Initialize; null → built-in defaults). */
	UPROPERTY()
	TObjectPtr<UOLCVFXData> VFXData;

	/** FTSTicker handle for the one-shot reaper. */
	FTSTicker::FDelegateHandle ReaperTickHandle;

	/** Tracking array for spawned one-shot components (pruned by the reaper). */
	TArray<TWeakObjectPtr<UNiagaraComponent>> ActiveOneShots;
};
