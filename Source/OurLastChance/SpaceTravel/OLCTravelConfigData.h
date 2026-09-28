#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OLCTravelConfigData.generated.h"

/**
 * Per-drive-tier travel profile (WP-122 step 3).
 *
 * Mirrors the six drive modules SM-DV-01..06 (Rocket -> Alien Warp):
 * higher tiers fly faster (lower TimeFactor) and cost less fuel/energy per distance unit.
 */
USTRUCT(BlueprintType)
struct FOLCDriveProfile
{
	GENERATED_BODY()

	/** Drive tier 1-6 (SM-DV-01..06). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	int32 DriveTier = 1;

	/** Display name of the drive module. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	FString DriveName = TEXT("Rocket");

	/** Multiplier applied to base transit time (lower = faster). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float TimeFactor = 1.0f;

	/** Fuel deducted at depart per distance unit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float FuelCostPerDistanceUnit = 4.0f;

	/** Energy deducted at depart per distance unit (drive spool-up / reactor load). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float EnergyCostPerDistanceUnit = 3.0f;
};

/**
 * Data-driven travel configuration (WP-122 step 3).
 *
 * Holds the phase timing plus one speed/fuel profile per drive tier. The constructor seeds
 * the six default drive profiles, so the class CDO doubles as the code-side fallback table:
 * UOLCTravelSubsystem reads an authored instance when one is assigned and otherwise falls
 * back to GetDefault<UOLCTravelConfigData>() — it runs without any authored asset.
 */
UCLASS(BlueprintType)
class OURLASTCHANCE_API UOLCTravelConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UOLCTravelConfigData();

	// -----------------------------------------------------------------------
	// Phase timing (promoted from the step-2 FOLCTravelConfig defaults)
	// -----------------------------------------------------------------------
	/** Depart phase duration in seconds (ship launch animation window). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float DepartDurationSeconds = 2.0f;

	/** Base transit duration in seconds at drive tier 1 per distance unit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float BaseTransitSecondsPerUnit = 8.0f;

	/** Route distance in units used until a later step wires real warp routes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float DefaultRouteDistanceUnits = 1.0f;

	/** EventCheck phase duration in seconds (event roll window). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float EventCheckDurationSeconds = 1.5f;

	/** Arrive phase duration in seconds (scan results applied / planet selection shown). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	float ArriveDurationSeconds = 2.0f;

	// -----------------------------------------------------------------------
	// Per-drive-tier profiles (SM-DV-01..06)
	// -----------------------------------------------------------------------
	/** One profile per drive tier (looked up by DriveTier value, order-independent). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Travel")
	TArray<FOLCDriveProfile> DriveProfiles;

	/**
	 * Look up the profile for a drive tier (clamped to 1-6, then nearest available entry).
	 * @return pointer into DriveProfiles; nullptr only if the table was emptied.
	 */
	const FOLCDriveProfile* FindDriveProfile(int32 InDriveTier) const;
};
