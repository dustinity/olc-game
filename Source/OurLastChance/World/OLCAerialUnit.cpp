#include "OLCAerialUnit.h"

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

	UE_LOG(LogTemp, Log, TEXT("[OLC] Aerial unit '%s' online (altitude=%.0f)"),
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
	TargetAltitude = FMath::Clamp(NewAltitude, 50.0f, 2000.0f);
}
