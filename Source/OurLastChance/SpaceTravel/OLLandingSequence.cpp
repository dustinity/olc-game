#include "SpaceTravel/OLLandingSequence.h"
#include "OurLastChance.h"

#include "Logging/LogMacros.h"

UOLLandingSequence::UOLLandingSequence()
{
	// Code-side biome landing constraints (SYS-PG-01 rules). Seeded for every
	// EOLCBiomeType so the safety gate has a row to check regardless of destination.

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Desert;
		Row.LandingRiskLevel = 0.1f;
		Row.ConstraintNote = TEXT("Open flat terrain — no constraints");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Dusty;
		Row.LandingRiskLevel = 0.2f;
		Row.ConstraintNote = TEXT("Dust reduces visibility on approach");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Rocky;
		Row.LandingRiskLevel = 0.3f;
		Row.ConstraintNote = TEXT("Uneven ground — careful touchdown");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Water;
		Row.bRequiresCoastalTerrain = true;
		Row.LandingRiskLevel = 0.5f;
		Row.ConstraintNote = TEXT("Landing needs coastal terrain");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Swamp;
		Row.LandingRiskLevel = 0.4f;
		Row.ConstraintNote = TEXT("Soft ground — limited landing zones");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Jungle;
		Row.LandingRiskLevel = 0.3f;
		Row.ConstraintNote = TEXT("Dense canopy — careful approach");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::LightSnow;
		Row.LandingRiskLevel = 0.4f;
		Row.ConstraintNote = TEXT("Cold conditions — reduced grip");
		BiomeConstraints.Add(Row);
	}

	{
		FOLCBiomeLandingConstraint Row;
		Row.Biome = EOLCBiomeType::Ice;
		Row.LandingRiskLevel = 0.8f;
		Row.ConstraintNote = TEXT("High-risk cold — extreme temperatures");
		BiomeConstraints.Add(Row);
	}
}

const FOLCBiomeLandingConstraint* UOLLandingSequence::FindConstraint(EOLCBiomeType InBiome) const
{
	for (const FOLCBiomeLandingConstraint& Constraint : BiomeConstraints)
	{
		if (Constraint.Biome == InBiome)
		{
			return &Constraint;
		}
	}
	return nullptr;
}

void UOLLandingSequence::StartLanding(const FString& InDestinationPlanetId, EOLCBiomeType InDestinationBiome, bool bInCoastalTerrainAvailable)
{
	DestinationPlanetId = InDestinationPlanetId;
	DestinationBiome = InDestinationBiome;
	bCoastalTerrainAvailable = bInCoastalTerrainAvailable;
	LandingRiskLevel = 0.0f;
	BlockReason.Empty();
	bAssessmentSafe = true;
	bFinished = false;
	PhaseElapsedSeconds = 0.0f;

	CurrentPhase = EOLCLandingPhase::Approach;
	bRunning = true;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Landing sequence started: dest=%s biome=%d coastal=%d"),
		*DestinationPlanetId, static_cast<int32>(DestinationBiome), bCoastalTerrainAvailable ? 1 : 0);
}

bool UOLLandingSequence::TickLanding(float DeltaTime)
{
	if (!bRunning || bFinished)
	{
		return false;
	}

	PhaseElapsedSeconds += DeltaTime;
	if (PhaseElapsedSeconds < GetPhaseDuration(CurrentPhase))
	{
		return true; // phase still running
	}

	switch (CurrentPhase)
	{
	case EOLCLandingPhase::Approach:
		ChangePhase(EOLCLandingPhase::SafetyAssessment);
		RunSafetyAssessment(); // evaluate at the start of the assessment window
		break;

	case EOLCLandingPhase::SafetyAssessment:
		if (!bAssessmentSafe)
		{
			bRunning = false;
			bFinished = true;
			UE_LOG(LogOLC, Warning, TEXT("[OLC] Landing blocked at SafetyAssessment: %s"), *BlockReason);
			OnLandingBlocked.Broadcast(DestinationPlanetId, BlockReason);
			return false;
		}
		ChangePhase(EOLCLandingPhase::Descend);
		break;

	case EOLCLandingPhase::Descend:
		ChangePhase(EOLCLandingPhase::Touchdown);
		break;

	case EOLCLandingPhase::Touchdown:
		bRunning = false;
		bFinished = true;
		UE_LOG(LogOLC, Display, TEXT("[OLC] Touchdown complete on %s (biome risk %.1f) — planet-surface handoff"),
			*DestinationPlanetId, LandingRiskLevel);
		OnLandingComplete.Broadcast(DestinationPlanetId, LandingRiskLevel);
		return false;

	default:
		bRunning = false;
		bFinished = true;
		return false;
	}

	return true; // transitioned to the next phase, still running
}

void UOLLandingSequence::ChangePhase(EOLCLandingPhase NewPhase)
{
	if (NewPhase == CurrentPhase)
	{
		return;
	}

	const EOLCLandingPhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;
	PhaseElapsedSeconds = 0.0f;

	UE_LOG(LogOLC, Display, TEXT("[OLC] Landing phase: %d -> %d (dest=%s)"),
		static_cast<int32>(OldPhase), static_cast<int32>(NewPhase), *DestinationPlanetId);

	OnLandingPhaseChanged.Broadcast(OldPhase, NewPhase);
}

void UOLLandingSequence::RunSafetyAssessment()
{
	bAssessmentSafe = true;
	BlockReason.Empty();

	const FOLCBiomeLandingConstraint* Constraint = FindConstraint(DestinationBiome);
	if (!Constraint)
	{
		// Unknown biome: allow touchdown, flag it for the log.
		UE_LOG(LogOLC, Warning, TEXT("[OLC] SafetyAssessment: no constraint row for biome %d — allowing touchdown"),
			static_cast<int32>(DestinationBiome));
		return;
	}

	LandingRiskLevel = Constraint->LandingRiskLevel;

	if (Constraint->bRequiresCoastalTerrain && !bCoastalTerrainAvailable)
	{
		bAssessmentSafe = false;
		BlockReason = FString::Printf(TEXT("Landing blocked (%s): no coastal terrain available at destination"),
			*Constraint->ConstraintNote);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] SafetyAssessment: biome=%d risk=%.1f safe=%d"),
		static_cast<int32>(DestinationBiome), LandingRiskLevel, bAssessmentSafe ? 1 : 0);
}

float UOLLandingSequence::GetPhaseDuration(EOLCLandingPhase Phase) const
{
	switch (Phase)
	{
	case EOLCLandingPhase::Approach:         return ApproachDurationSeconds;
	case EOLCLandingPhase::SafetyAssessment: return SafetyAssessmentDurationSeconds;
	case EOLCLandingPhase::Descend:          return DescendDurationSeconds;
	case EOLCLandingPhase::Touchdown:        return TouchdownDurationSeconds;
	default:                                 return 0.0f;
	}
}
