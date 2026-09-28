// Copyright OLC Project. WP-128 Step 4 — building assembly/collapse sequence component.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OLCBuildingSequenceComponent.generated.h"

class USceneComponent;
class UNiagaraComponent;

/** Debug entry point for PIE verification (temporary debug path). */
UENUM(BlueprintType)
enum class EOLCBuildingSeqDebugMode : uint8
{
	None,
	/** BeginPlay: PlayAssemblySequence(DebugAssemblyDuration), then auto-collapse on completion. */
	AssemblyThenCollapse
};

/**
 * Staged construction/collapse sequences for buildings (WP-128 step 4).
 *
 * - PlayAssemblySequence(Duration): staged reveal — root scale 0→1 over the full
 *   duration (progress-faithful, so it can be tied to build time via
 *   SetAssemblyProgress), while child parts pop in one by one bottom-up with a
 *   per-part stagger. Duration <= 0 picks the tier-scaled duration from
 *   TierAssemblyDurations[BuildingTierIndex] (Default short … Elite longer).
 * - PlayCollapseSequence(): shake phase → staggered part scale-down from top/edges
 *   inward → final actor hide with a single destruction VFX burst.
 * - SetOperating(bool): idle motion for operating buildings — vertical oscillation
 *   on pump-arm style moving parts, continuous rotation on drill/rotor parts
 *   (name-filter based, e.g. Oil Pump PB-EX-02 / Mine PB-EX-01).
 *
 * VFX goes exclusively through UOLCVFXSubsystem; the one returned handle
 * (construction glow) is owned in a UPROPERTY and destroyed deterministically —
 * no MID or Niagara handles leak in either direction. All parameters are
 * EditAnywhere so DataAssets/BPs can tune without code changes.
 */
UCLASS(ClassGroup = (OLC), meta = (BlueprintSpawnableComponent))
class OURLASTCHANCE_API UOLCBuildingSequenceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOLCBuildingSequenceComponent();

	// --- Tunables (all EditAnywhere) ------------------------------------------

	/** Building-tier table: index = tier (0 = Default … last = Elite), value = assembly duration in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence")
	TArray<float> TierAssemblyDurations;

	/** This building's tier index into TierAssemblyDurations (clamped to range). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0"))
	int32 BuildingTierIndex = 0;

	/** Fraction of the assembly window consumed by the per-part stagger (0..1). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AssemblyStaggerFraction = 0.5f;

	/** Scale parts start from when they are revealed during assembly (0 = pop from nothing). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PartStartScale = 0.0f;

	/** Collapse shake phase duration (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0"))
	float CollapseShakeDuration = 0.8f;

	/** Collapse shake amplitude in world units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0"))
	float CollapseShakeAmplitude = 6.0f;

	/** Staggered scale-down phase duration after the shake (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.05"))
	float CollapseScaleDuration = 1.2f;

	/** Fraction of the collapse window consumed by the per-part stagger (0..1). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CollapseStaggerFraction = 0.5f;

	/** Pipe-separated name tokens identifying moving parts (case-insensitive), e.g. "Pump|Drill|Rotor|Arm". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence")
	FString MovingPartNameFilter = TEXT("Pump|Drill|Rotor|Arm");

	/** Vertical oscillation amplitude for pump-arm style moving parts (units). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0"))
	float OperatingOscillationAmplitudeZ = 8.0f;

	/** Continuous spin speed for drill/rotor style moving parts (degrees per second around Z). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.0"))
	float OperatingSpinDegreesPerSecond = 90.0f;

	/** Oscillation frequency for pump-arm style moving parts (Hz). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence", meta = (ClampMin = "0.01"))
	float OperatingOscillationFrequencyHz = 1.0f;

	/** Route VFX through UOLCVFXSubsystem (construction glow + destruction burst). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence")
	bool bEnableVFXHooks = true;

	// --- Debug path (temporary; used for PIE verification) ----------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence|Debug")
	EOLCBuildingSeqDebugMode DebugMode = EOLCBuildingSeqDebugMode::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence|Debug", meta = (ClampMin = "0.1"))
	float DebugAssemblyDuration = 3.0f;

	/** BeginPlay: also call SetOperating(true) when true. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|BuildingSequence|Debug")
	bool bDebugStartOperating = false;

	/** Deferred debug-sequence start (component BeginPlay runs before the actor has begun play). */
	bool bDebugSequencePending = false;

	// --- API ---------------------------------------------------------------------

	/**
	 * Play the staged assembly reveal. Duration <= 0 selects the tier-scaled
	 * duration from TierAssemblyDurations[BuildingTierIndex]. Replays cleanly
	 * after a collapse (unhides the actor and resets part scales).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|BuildingSequence")
	void PlayAssemblySequence(float Duration = -1.0f);

	/** Drive assembly progress externally (build-time tie-in). 0..1. */
	UFUNCTION(BlueprintCallable, Category = "OLC|BuildingSequence")
	void SetAssemblyProgress(float Progress);

	UFUNCTION(BlueprintPure, Category = "OLC|BuildingSequence")
	float GetAssemblyProgress() const { return AssemblyProgress; }

	UFUNCTION(BlueprintPure, Category = "OLC|BuildingSequence")
	bool IsAssembled() const { return AssemblyProgress >= 1.0f; }

	/** Play the collapse: shake → staggered top-down scale-down → hide + single VFX burst. */
	UFUNCTION(BlueprintCallable, Category = "OLC|BuildingSequence")
	void PlayCollapseSequence();

	UFUNCTION(BlueprintPure, Category = "OLC|BuildingSequence")
	bool IsCollapsed() const { return bCollapsed; }

	/** Operating-building idle motion on/off (pump arm oscillation / drill rotation). */
	UFUNCTION(BlueprintCallable, Category = "OLC|BuildingSequence")
	void SetOperating(bool bNewOperating);

	UFUNCTION(BlueprintPure, Category = "OLC|BuildingSequence")
	bool IsOperating() const { return bOperating; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	enum class ESeqPhase : uint8
	{
		Idle,
		Assembling,
		CollapsingShake,
		CollapsingScale
	};

	struct FPartInfo
	{
		USceneComponent* Comp = nullptr;
		FTransform BaseRelative;
		float HeightZ = 0.0f;          // relative Z — assembly order (bottom-up) / collapse order (top-down)
		bool bIsMovingPart = false;    // name matches MovingPartNameFilter
		bool bRotatingPart = false;    // drill/rotor style → continuous spin instead of vertical oscillation
		float SpinAngleDeg = 0.0f;     // accumulated operating spin
	};

	// --- Bookkeeping -------------------------------------------------------------
	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> RootComp;
	FTransform RootBaseRelative;
	TArray<FPartInfo> Parts;           // child scene components, sorted bottom → top

	void CollectParts();
	static bool NameMatchesFilter(const FString& Filter, const FString& Name);

	// --- Assembly ------------------------------------------------------------------
	float AssemblyProgress = 0.0f;
	float AssemblyDuration = 3.0f;
	float AssemblyClock = 0.0f;
	bool bExternallyDriven = false; // true while SetAssemblyProgress drives the reveal (build-time tie-in)
	bool bAssemblyCompletedForDebug = false;

	void ApplyAssemblyVisuals();
	void CompleteAssembly();
	void FireAssemblyVFXHook(bool bComplete);

	// --- Collapse --------------------------------------------------------------------
	ESeqPhase Phase = ESeqPhase::Idle;
	float CollapseClock = 0.0f;
	bool bCollapsed = false;

	void ApplyCollapseVisuals(float DeltaTime);
	void FinishCollapse();
	void FireCollapseVFXHook();

	// --- Operating idle motion ----------------------------------------------------------
	bool bOperating = false;
	float OperatingClock = 0.0f;

	void ApplyOperatingMotion(float DeltaTime);

	// --- VFX handle ownership (no leaks) -------------------------------------------------
	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> ConstructionGlow;

	static float EaseOutCubic(float X);
	static float EaseInQuad(float X);
};
