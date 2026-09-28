#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCWeaponMountData.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCWeaponMountConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	TArray<FOLCWeaponMountData> Mounts;
};
