#include "OLCAerialUnit.h"
#include "OurLastChance.h"

#include "Core/OLCUnitData.h"
#include "Logging/LogMacros.h"

AOLCAerialUnit::AOLCAerialUnit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCAerialUnit::BeginPlay()
{
	Super::BeginPlay();

	CurrentAltitude = TargetAltitude;

	if (Mesh)
	{
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, CurrentAltitude));
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] Aerial unit '%s' online (altitude=%.0f)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), CurrentAltitude);
}

void AOLCAerialUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;

	// Smooth altitude transition.
	float AltDiff = TargetAltitude - CurrentAltitude;
	CurrentAltitude += FMath::Sign(AltDiff) * FMath::Min(FMath::Abs(AltDiff), AltitudeSpeed * DeltaTime);

	if (Mesh)
	{
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, CurrentAltitude));
	}
}

void AOLCAerialUnit::SetTargetAltitude(float NewAltitude)
{
	TargetAltitude = FMath::Clamp(NewAltitude, 0.0f, 1000.0f);
	if (TargetAltitude < 200.0f) AltitudeBand = EOLCAltitudeBand::Low;
	else if (TargetAltitude <= 500.0f) AltitudeBand = EOLCAltitudeBand::Medium;
	else AltitudeBand = EOLCAltitudeBand::High;
}

void AOLCAerialUnit::SetAltitudeBand(EOLCAltitudeBand Band)
{
	AltitudeBand = Band;
	switch (Band)
	{
		case EOLCAltitudeBand::Low: SetTargetAltitude(120.0f); break;
		case EOLCAltitudeBand::Medium: SetTargetAltitude(360.0f); break;
		case EOLCAltitudeBand::High: SetTargetAltitude(800.0f); break;
	}
}

float AOLCAerialUnit::GetSpeedMultiplier() const
{
	switch (AltitudeBand)
	{
		case EOLCAltitudeBand::Low: return 0.85f;
		case EOLCAltitudeBand::Medium: return 1.0f;
		case EOLCAltitudeBand::High: return 1.25f;
		default: return 1.0f;
	}
}

float AOLCAerialUnit::GetDetectionProfile() const
{
	switch (AltitudeBand)
	{
		case EOLCAltitudeBand::Low: return 0.65f;
		case EOLCAltitudeBand::Medium: return 1.0f;
		case EOLCAltitudeBand::High: return 1.35f;
		default: return 1.0f;
	}
}

AOLCUnitBase* AOLCAerialUnit::DeployUnit(TSubclassOf<AOLCUnitBase> UnitClass)
{
	if (!GetWorld() || !UnitClass)
	{
		return nullptr;
	}
	const FVector DropLocation = GetActorLocation() + FVector(0.0f, 0.0f, 35.0f);
	return GetWorld()->SpawnActor<AOLCUnitBase>(UnitClass, DropLocation, GetActorRotation());
}
