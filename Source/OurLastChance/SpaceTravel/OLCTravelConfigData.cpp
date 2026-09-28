#include "SpaceTravel/OLCTravelConfigData.h"

UOLCTravelConfigData::UOLCTravelConfigData()
{
	// SM-DV-01..06 (Rocket -> Alien Warp): each tier flies faster and burns less
	// fuel/energy per distance unit. These constructor defaults seed the class CDO,
	// which UOLCTravelSubsystem uses as its code-side fallback when no authored
	// DataAsset instance is assigned.
	const struct FDefaultRow
	{
		int32 Tier;
		const TCHAR* Name;
		float TimeFactor;
		float FuelPerUnit;
		float EnergyPerUnit;
	}
	Rows[] = {
		{ 1, TEXT("Rocket"),         1.00f, 4.0f, 3.0f }, // SM-DV-01
		{ 2, TEXT("Atomic Reactor"), 0.85f, 3.2f, 2.4f }, // SM-DV-02
		{ 3, TEXT("Ionic Reactor"),  0.70f, 2.6f, 1.9f }, // SM-DV-03
		{ 4, TEXT("Energy Stream"),  0.55f, 2.0f, 1.5f }, // SM-DV-04
		{ 5, TEXT("Void Warp"),      0.40f, 1.4f, 1.0f }, // SM-DV-05
		{ 6, TEXT("Alien Warp"),     0.20f, 0.8f, 0.5f }, // SM-DV-06
	};

	for (const FDefaultRow& Row : Rows)
	{
		FOLCDriveProfile Profile;
		Profile.DriveTier = Row.Tier;
		Profile.DriveName = Row.Name;
		Profile.TimeFactor = Row.TimeFactor;
		Profile.FuelCostPerDistanceUnit = Row.FuelPerUnit;
		Profile.EnergyCostPerDistanceUnit = Row.EnergyPerUnit;
		DriveProfiles.Add(Profile);
	}
}

const FOLCDriveProfile* UOLCTravelConfigData::FindDriveProfile(int32 InDriveTier) const
{
	if (DriveProfiles.Num() == 0)
	{
		return nullptr;
	}

	const int32 ClampedTier = FMath::Clamp(InDriveTier, 1, 6);

	for (const FOLCDriveProfile& Profile : DriveProfiles)
	{
		if (Profile.DriveTier == ClampedTier)
		{
			return &Profile;
		}
	}

	// Table was partially authored without the requested tier — use the closest
	// available entry rather than failing the trip.
	int32 BestIndex = 0;
	int32 BestDelta = MAX_int32;
	for (int32 i = 0; i < DriveProfiles.Num(); ++i)
	{
		const int32 Delta = FMath::Abs(DriveProfiles[i].DriveTier - ClampedTier);
		if (Delta < BestDelta)
		{
			BestDelta = Delta;
			BestIndex = i;
		}
	}
	return &DriveProfiles[BestIndex];
}
