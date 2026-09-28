#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/OLCColonyTypes.h"
#include "OLCColonyNetworkData.generated.h"

/**
 * Designer-tunable configuration for the colony network system (WP-124):
 * per-role bonus/eligibility table and per-method transport cost/speed/capacity.
 * Mirrors UOLCScanTierData (Core/OLCScanTierData.h). The consuming subsystem
 * falls back to GetDefaultRoleConfigs()/GetDefaultTransportConfigs() when no
 * asset is registered or an array is empty, so gameplay never depends on the
 * persistent asset existing.
 */
UCLASS()
class OURLASTCHANCE_API UOLCColonyNetworkData : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Per-role bonus/eligibility configuration (WP-124 Step 4). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FOLCColonyRoleConfig> RoleConfigs;

	/** Per-method speed/capacity/cost configuration (WP-124 Step 2). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	TArray<FOLCTransportMethodConfig> TransportConfigs;

	/** Recolonize cost (WP-124 Step 2): Construction Material deducted from the global counter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float RecolonizeCostConstructionMaterial = 150.0f;

	/** Recolonize cost (WP-124 Step 2): Minerals deducted from the global counter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Colony")
	float RecolonizeCostMinerals = 75.0f;

	/**
	 * Built-in defaults matching the WP-124 numbers exactly:
	 * Mining +25% (Minerals/ConstructionMaterial; Rocky/Dusty/Desert),
	 * Fuel +25% (Fuel; Desert/Swamp/Water), Military +20% training/+15% defense
	 * (Rocky/Ice), Research +15% research speed (Swamp/Ice/Water),
	 * Agricultural +25% Survival +10% crew capacity (Water/Jungle).
	 */
	static TArray<FOLCColonyRoleConfig> GetDefaultRoleConfigs();

	/**
	 * Built-in defaults matching the WP-124 numbers exactly:
	 * Tanker {10 s transit, 500 units/trip, 5 Energy}, Hauler {5 s, 200, 3},
	 * QuantumGate {instant, 50, free}.
	 */
	static TArray<FOLCTransportMethodConfig> GetDefaultTransportConfigs();
};
