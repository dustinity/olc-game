#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AI/OLCFormationTypes.h"
#include "OLCFormationManager.generated.h"

class AOLCUnitBase;

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCFormationManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Formation")
	TArray<FVector> CalculateFormationPositions(const TArray<AOLCUnitBase*>& Units, const FVector& AnchorLocation, EOLCFormationType FormationType, float Spacing = 180.0f, bool bSnapToGrid = true) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Formation")
	FVector GetFormationOffset(int32 Index, int32 UnitCount, EOLCFormationType FormationType, float Spacing) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Formation")
	void ApplyFormation(const TArray<AOLCUnitBase*>& Units, const FVector& AnchorLocation, EOLCFormationType FormationType);

	UFUNCTION(BlueprintCallable, Category = "OLC|Formation")
	void RegisterFormation(const FOLCFormationConfig& Config);

private:
	FVector Snap(const FVector& Location, float GridSize) const;

	UPROPERTY()
	TArray<FOLCFormationConfig> RegisteredFormations;
};
