#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCNavigationTypes.h"
#include "OLCScanTierData.generated.h"

/** Designer-editable table of scan tier configurations (cost, duration, reveal depth). */
UCLASS()
class OURLASTCHANCE_API UOLCScanTierData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Scan")
	TArray<FOLCScanTierConfig> ScanTiers;
};
