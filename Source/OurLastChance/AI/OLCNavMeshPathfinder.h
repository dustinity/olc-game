#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OLCNavMeshPathfinder.generated.h"

USTRUCT(BlueprintType)
struct FOLCPathResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OLC|AI")
	TArray<FVector> Points;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|AI")
	bool bValid = false;

	UPROPERTY(BlueprintReadOnly, Category = "OLC|AI")
	float Cost = 0.0f;
};

UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCNavMeshPathfinder : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	FOLCPathResult FindPath(UWorld* World, const FVector& Start, const FVector& Goal, float AgentRadius = 45.0f) const;

	UFUNCTION(BlueprintCallable, Category = "OLC|AI")
	TArray<FVector> SmoothPath(const TArray<FVector>& RawPath) const;
};
