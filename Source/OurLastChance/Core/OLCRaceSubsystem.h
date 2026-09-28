#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCRaceData.h"
#include "Core/OLCBossRaceData.h"
#include "Core/OLCFactionRaceBonusData.h"
#include "OLCRaceSubsystem.generated.h"

/**
 * GameInstanceSubsystem that owns all 33 race definitions (WP-118), their
 * biome spawn weighting, the 6 boss encounters, and faction-vs-race combat
 * bonuses. Mirrors UOLCResearchSubsystem's pattern: races are constructed
 * as transient NewObject() instances in RegisterStarterRaces() rather than
 * as persistent DataAssets, so no MCP/editor round trip is required to
 * populate them.
 */
UCLASS()
class OURLASTCHANCE_API UOLCRaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// -----------------------------------------------------------------------
	// Race registration and lookup
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	void RegisterRace(UOLCRaceData* Race);

	/** Register the built-in 33-race roster (call once at game start). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	void RegisterStarterRaces();

	UFUNCTION(BlueprintPure, Category = "OLC|Race")
	UOLCRaceData* FindRaceById(const FString& RaceId) const;

	const TArray<TObjectPtr<UOLCRaceData>>& GetAllRaces() const { return AllRaces; }

	UFUNCTION(BlueprintPure, Category = "OLC|Race")
	TArray<UOLCRaceData*> GetRacesByFamily(EOLCRaceFamily Family) const;

	/** All races (boss and non-boss) whose biome weight for the given biome is > 0, sorted by weight descending. */
	UFUNCTION(BlueprintPure, Category = "OLC|Race")
	TArray<UOLCRaceData*> GetSpawnableRaces(EOLCRaceBiomeType Biome) const;

	UFUNCTION(BlueprintPure, Category = "OLC|Race")
	TArray<UOLCBossRaceData*> GetAllBosses() const;

	// -----------------------------------------------------------------------
	// Faction race bonuses
	// -----------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	void RegisterFactionRaceBonus(UOLCFactionRaceBonusData* Bonus);

	/** Returns the bonus data for FactionId vs Family, or nullptr if the faction has no bonus against that family. */
	UFUNCTION(BlueprintPure, Category = "OLC|Race")
	UOLCFactionRaceBonusData* FindFactionRaceBonus(const FString& FactionId, EOLCRaceFamily Family) const;

	// -----------------------------------------------------------------------
	// Endgame bosses (WP-125 Step 4)
	// -----------------------------------------------------------------------

	/** Register the Center Galaxy Overlord boss. Call after RegisterStarterRaces(). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Race")
	void RegisterEndgameBosses();

private:
	TArray<TObjectPtr<UOLCRaceData>> AllRaces;
	TArray<TObjectPtr<UOLCFactionRaceBonusData>> FactionRaceBonuses;
};
