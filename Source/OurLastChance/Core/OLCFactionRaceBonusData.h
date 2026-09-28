#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCRaceFamily.h"
#include "OLCFactionRaceBonusData.generated.h"

/**
 * One faction's combat modifier against a specific race family — e.g. Neon
 * Punk gets +20% damage vs Mechanical. Applied in combat AI damage
 * calculation (WP-113) and shown as a tooltip when selecting a target unit.
 */
UCLASS()
class OURLASTCHANCE_API UOLCFactionRaceBonusData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOLCFactionRaceBonusData();

	/** e.g. "NeonPunk", "DarkRealistic" — matches UOLCFactionData::FactionId. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	FString FactionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	EOLCRaceFamily TargetRaceFamily = EOLCRaceFamily::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	float DamageBonusPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	float DefenseBonusPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	float SpeedBonusPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	float LootBonusPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	float HealingBonusPercent = 0.0f;

	/** Tooltip text shown when selecting a target unit of the bonus family. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|FactionRaceBonus")
	FText TooltipDescription;
};
