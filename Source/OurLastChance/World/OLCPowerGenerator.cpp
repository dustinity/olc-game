#include "OLCPowerGenerator.h"
#include "OurLastChance.h"

#include "Core/OLCUIDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"

AOLCPowerGenerator::AOLCPowerGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCPowerGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Cache power output (negative = produces, positive = consumes).
	CurrentPowerOutput = BuildingData.PowerConsumption;

	if (CurrentPowerOutput < 0.0f)
	{
		UE_LOG(LogOLC, Log, TEXT("[OLC] Power generator '%s' producing %.1f power"),
			*BuildingData.DisplayName.ToString(), FMath::Abs(CurrentPowerOutput));

	}
	else
	{
		UE_LOG(LogOLC, Log, TEXT("[OLC] Power consumer '%s' using %.1f power"),
			*BuildingData.DisplayName.ToString(), CurrentPowerOutput);
	}
}

void AOLCPowerGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only generate if producing (negative PowerConsumption) and powered.
	if (CurrentPowerOutput >= 0.0f || !IsPowered()) return;

	// Accumulate time toward next production tick.
	PowerAccumulator += DeltaTime;

	if (PowerAccumulator < ProductionInterval) return;
	PowerAccumulator = 0.0f;

	// Apply biome multiplier — uses CurrentBiome from OLCBuildingBase instead of hardcoded Desert.
	float Multiplier = GetBiomeMultiplier();

	float Output = FMath::Abs(CurrentPowerOutput) * Multiplier;

	// Feed energy to the subsystem resource counters.
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UOLCUIDataSubsystem* Data = GI->GetSubsystem<UOLCUIDataSubsystem>())
		{
			Data->AddResource(EOLCResourceType::Energy, Output);
		}
	}

	UE_LOG(LogOLC, Verbose, TEXT("[OLC] Generator '%s' produced %.1f energy (biome=%s, multiplier=%.2f)"),
		*BuildingData.DisplayName.ToString(), Output, *UEnum::GetValueAsString(CurrentBiome), Multiplier);
}

void AOLCPowerGenerator::SetPowered(bool bNewPowered)
{
	Super::SetPowered(bNewPowered);

	if (CurrentPowerOutput < 0.0f)
	{
		UE_LOG(LogOLC, Log, TEXT("[OLC] Generator '%s' %s power output"),
			*BuildingData.DisplayName.ToString(),
			bNewPowered ? TEXT("active") : TEXT("offline"));
	}
}
