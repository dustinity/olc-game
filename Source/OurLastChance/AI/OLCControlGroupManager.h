#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AI/OLCFormationTypes.h"
#include "OLCControlGroupManager.generated.h"

class AOLCUnitBase;

USTRUCT(BlueprintType)
struct FOLCControlGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AOLCUnitBase>> Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Control")
	EOLCFormationType LastFormation = EOLCFormationType::Line;
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCControlGroupManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Control")
	void AssignGroup(int32 GroupNumber, const TArray<AOLCUnitBase*>& Units);

	UFUNCTION(BlueprintCallable, Category = "OLC|Control")
	TArray<AOLCUnitBase*> SelectGroup(int32 GroupNumber) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Control")
	void RecallFormation(int32 GroupNumber, const FVector& AnchorLocation);

	UFUNCTION(BlueprintCallable, Category = "OLC|Control")
	void SetChampion(AOLCUnitBase* ChampionUnit);

	UFUNCTION(BlueprintPure, Category = "OLC|Control")
	bool IsChampionIncluded(int32 GroupNumber) const;

private:
	UPROPERTY()
	TMap<int32, FOLCControlGroup> Groups;

	UPROPERTY()
	TObjectPtr<AOLCUnitBase> Champion;
};
