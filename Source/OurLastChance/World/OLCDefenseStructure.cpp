#include "OLCDefenseStructure.h"

#include "Logging/LogMacros.h"

AOLCDefenseStructure::AOLCDefenseStructure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOLCDefenseStructure::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	UE_LOG(LogTemp, Log, TEXT("[OLC] Defense structure '%s' online: HP=%.0f, range=%.0f, dmg=%.0f"),
		*BuildingData.DisplayName.ToString(),
		MaxHP,
		AttackRange,
		DamagePerTick);
}

void AOLCDefenseStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPowered() || CurrentHP <= 0.0f) return;

	// Accumulate toward next attack tick.
	AttackAccumulator += DeltaTime;

	if (AttackAccumulator >= AttackInterval)
	{
		AttackAccumulator = 0.0f;

		// In a full implementation, find nearest enemy within AttackRange and deal DamagePerTick.
		// Placeholder: log attack fire.
		UE_LOG(LogTemp, Verbose, TEXT("[OLC] Defense '%s' fired (range=%.0f, dmg=%.0f)"),
			*BuildingData.DisplayName.ToString(),
			AttackRange,
			DamagePerTick);
	}
}
