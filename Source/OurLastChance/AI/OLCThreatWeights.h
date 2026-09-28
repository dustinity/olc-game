#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCThreatWeights.generated.h"

USTRUCT(BlueprintType)
struct FOLCThreatWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	float DamageWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	float DistanceWeight = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	float VehicleWeight = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	float AerialWeight = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	float SniperWeight = 1.4f;
};

UCLASS()
class OURLASTCHANCE_API UOLCThreatWeightData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|AI")
	FOLCThreatWeights Weights;
};
