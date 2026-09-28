#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AI/OLCThreatWeights.h"
#include "OLCCombatAIController.generated.h"

class AOLCUnitBase;

UENUM(BlueprintType)
enum class EOLCCombatDecision : uint8
{
	Hold UMETA(DisplayName = "Hold"),
	Engage UMETA(DisplayName = "Engage"),
	Retreat UMETA(DisplayName = "Retreat")
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCCombatAIController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	TArray<AOLCUnitBase*> ScanForTargets(AOLCUnitBase* Unit, float SensorRange) const;

	UFUNCTION(BlueprintPure, Category = "OLC|AI")
	float CalculateThreatLevel(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target, const FOLCThreatWeights& Weights) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	AOLCUnitBase* SelectBestTarget(AOLCUnitBase* ControlledUnit, const TArray<AOLCUnitBase*>& Targets, const FOLCThreatWeights& Weights) const;

	UFUNCTION(BlueprintPure, Category = "OLC|AI")
	EOLCCombatDecision EvaluateEngageRetreat(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	void IssueAttackCommand(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Target) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	void IssueRetreatCommand(AOLCUnitBase* ControlledUnit, AOLCUnitBase* Threat, float RetreatDistance = 600.0f) const;
};
