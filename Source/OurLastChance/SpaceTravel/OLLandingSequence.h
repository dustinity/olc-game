#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Core/OLCBuildingData.h"
#include "OLLandingSequence.generated.h"

/**
 * Landing sequence phases (WP-122 step 8).
 * Order: Approach -> SafetyAssessment -> Descend -> Touchdown.
 */
UENUM(BlueprintType)
enum class EOLCLandingPhase : uint8
{
	Approach         UMETA(DisplayName = "Approach"),
	SafetyAssessment UMETA(DisplayName = "Safety Assessment"),
	Descend          UMETA(DisplayName = "Descend"),
	Touchdown        UMETA(DisplayName = "Touchdown")
};

/**
 * Per-biome landing constraint row (SYS-PG-01 biome rules, code-side defaults).
 * bRequiresCoastalTerrain gates the safety assessment (e.g. Water: "Landing needs
 * coastal terrain"); LandingRiskLevel is informational for the UI (Ice = high-risk cold).
 */
USTRUCT(BlueprintType)
struct FOLCBiomeLandingConstraint
{
	GENERATED_BODY()

	/** Biome this row applies to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	EOLCBiomeType Biome = EOLCBiomeType::Desert;

	/** When true, Touchdown is blocked unless coastal terrain is available at the destination. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	bool bRequiresCoastalTerrain = false;

	/** Informational landing risk 0-1 (e.g. Ice high-risk cold); does not block by itself. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	float LandingRiskLevel = 0.0f;

	/** Human-readable constraint note for toasts/UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	FString ConstraintNote;
};

/** Fired on every landing phase transition (old, new) — animation/UI hook. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCLandingPhaseChanged, EOLCLandingPhase, OldPhase, EOLCLandingPhase, NewPhase);

/**
 * Final touchdown -> planet-surface handoff.
 * Payload: destination planet id + informational landing risk level of the biome.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCLandingComplete, const FString&, DestinationPlanetId, float, LandingRiskLevel);

/** Fired when the safety assessment blocks Touchdown (constraint unmet). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOLCLandingBlocked, const FString&, DestinationPlanetId, const FString&, BlockReason);

/**
 * Phased landing sequence driven by UOLCTravelSubsystem while in the Land state
 * (WP-122 step 8): Approach -> SafetyAssessment -> Descend -> Touchdown.
 *
 * The SafetyAssessment phase checks the destination biome's landing constraint
 * (SYS-PG-01 rules, code-side defaults seeded in the constructor) and only allows
 * Touchdown when the constraint is met — e.g. Water requires coastal terrain.
 * A blocked landing stops the sequence at SafetyAssessment and broadcasts
 * OnLandingBlocked instead of handing off to the planet surface.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLLandingSequence : public UObject
{
	GENERATED_BODY()

public:
	UOLLandingSequence();

	// -----------------------------------------------------------------------
	// Phase durations (code-side defaults, seconds)
	// -----------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	float ApproachDurationSeconds = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	float SafetyAssessmentDurationSeconds = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	float DescendDurationSeconds = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	float TouchdownDurationSeconds = 1.0f;

	/** Biome landing constraints (code-side defaults for every EOLCBiomeType). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Landing")
	TArray<FOLCBiomeLandingConstraint> BiomeConstraints;

	/** @return pointer into BiomeConstraints, or nullptr if the biome has no row. */
	const FOLCBiomeLandingConstraint* FindConstraint(EOLCBiomeType InBiome) const;

	// -----------------------------------------------------------------------
	// Driving API (called by UOLCTravelSubsystem)
	// -----------------------------------------------------------------------
	/**
	 * Start the sequence at Approach.
	 * @param bInCoastalTerrainAvailable  Whether coastal terrain exists at the destination
	 *                                    (provided by the caller/terrain system; defaults to true).
	 */
	UFUNCTION(BlueprintCallable, Category = "OLC|Landing")
	void StartLanding(const FString& InDestinationPlanetId, EOLCBiomeType InDestinationBiome, bool bInCoastalTerrainAvailable = true);

	/** Advance the sequence by DeltaTime. @return false once finished (touchdown or blocked). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Landing")
	bool TickLanding(float DeltaTime);

	// -----------------------------------------------------------------------
	// State queries
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = "OLC|Landing")
	bool IsRunning() const { return bRunning; }

	/** True once the sequence has ended (touchdown or blocked). */
	UFUNCTION(BlueprintPure, Category = "OLC|Landing")
	bool HasFinished() const { return bFinished; }

	UFUNCTION(BlueprintPure, Category = "OLC|Landing")
	EOLCLandingPhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "OLC|Landing")
	FString GetDestinationPlanetId() const { return DestinationPlanetId; }

	/** Informational biome landing risk recorded during SafetyAssessment (0-1). */
	UFUNCTION(BlueprintPure, Category = "OLC|Landing")
	float GetLandingRiskLevel() const { return LandingRiskLevel; }

	// -----------------------------------------------------------------------
	// Events (for animation/UI binding)
	// -----------------------------------------------------------------------
	/** Fired on every phase transition. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Landing")
	FOnOLCLandingPhaseChanged OnLandingPhaseChanged;

	/** Final touchdown -> planet-surface handoff. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Landing")
	FOnOLCLandingComplete OnLandingComplete;

	/** Safety assessment blocked Touchdown (constraint unmet). */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Landing")
	FOnOLCLandingBlocked OnLandingBlocked;

private:
	void ChangePhase(EOLCLandingPhase NewPhase);
	void RunSafetyAssessment();
	float GetPhaseDuration(EOLCLandingPhase Phase) const;

	EOLCLandingPhase CurrentPhase = EOLCLandingPhase::Approach;
	float PhaseElapsedSeconds = 0.0f;
	bool bRunning = false;
	bool bFinished = false;
	bool bAssessmentSafe = true;

	FString DestinationPlanetId;
	EOLCBiomeType DestinationBiome = EOLCBiomeType::Desert;
	bool bCoastalTerrainAvailable = true;
	float LandingRiskLevel = 0.0f;
	FString BlockReason;
};
