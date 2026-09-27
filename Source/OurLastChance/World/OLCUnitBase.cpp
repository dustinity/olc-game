#include "OLCUnitBase.h"

#include "Core/OLCUnitData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/DecalComponent.h"
#include "Logging/LogMacros.h"

AOLCUnitBase::AOLCUnitBase()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	SelectionHighlight = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionHighlight"));
	SelectionHighlight->SetupAttachment(RootComponent);
	SelectionHighlight->DecalSize = FVector(80.0f, 80.0f, 80.0f);
	SelectionHighlight->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	SelectionHighlight->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = false;
}

void AOLCUnitBase::BeginPlay()
{
	Super::BeginPlay();

	if (UnitData)
	{
		CurrentHP = MaxHP = UnitData->MaxHP;
	}

	UE_LOG(LogTemp, Log, TEXT("[OLC] Unit '%s' spawned (%.0f HP)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), MaxHP);

	// Start combat AI if this unit has attack damage defined.
	if (UnitData && UnitData->AttackDamage > 0.0f)
	{
		StartCombatAI();
	}
}

void AOLCUnitBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Cancel combat timer on destruction.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
	}

	if (CurrentHP <= 0.0f && EndPlayReason == EEndPlayReason::Destroyed)
	{
		UE_LOG(LogTemp, Log, TEXT("[OLC] Unit '%s' destroyed"), UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"));
	}
}

void AOLCUnitBase::SetUnitData(UOLCUnitData* InData)
{
	UnitData = InData;
	if (UnitData)
	{
		MaxHP = UnitData->MaxHP;
		if (CurrentHP > MaxHP) CurrentHP = MaxHP;
	}
}

void AOLCUnitBase::SetCurrentHP(float NewHP)
{
	CurrentHP = FMath::Clamp(NewHP, 0.0f, MaxHP);
}

void AOLCUnitBase::TakeDamage(float DamageAmount)
{
	if (IsDead()) return;

	SetCurrentHP(CurrentHP - DamageAmount);

	UE_LOG(LogTemp, Verbose, TEXT("[OLC] Unit '%s' took %.0f damage (%.0f/%.0f HP remaining)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), DamageAmount, CurrentHP, MaxHP);

	if (IsDead())
	{
		Destroy();
	}
}

void AOLCUnitBase::SetSelected(bool bNewSelected)
{
	bIsSelected = bNewSelected;

	if (SelectionHighlight)
	{
		SelectionHighlight->SetVisibility(bNewSelected);
	}
}

// ---------------------------------------------------------------------------
// Combat AI
// ---------------------------------------------------------------------------

void AOLCUnitBase::StartCombatAI()
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(CombatTimer, this,
		&AOLCUnitBase::CheckForTarget, CombatCheckInterval, true);

	UE_LOG(LogTemp, Verbose, TEXT("[OLC] Unit '%s' started combat AI (interval=%.1fs)"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"), CombatCheckInterval);
}

void AOLCUnitBase::StopCombatAI()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
	}

	UE_LOG(LogTemp, Verbose, TEXT("[OLC] Unit '%s' stopped combat AI"),
		UnitData ? *UnitData->DisplayName.ToString() : TEXT("Unknown"));
}

void AOLCUnitBase::CheckForTarget()
{
	if (IsDead() || !UnitData) return;

	const float CurrentRange = UnitData->Range;
	const FVector MyLocation = GetActorLocation();
	float ClosestDist = MAX_FLT;
	AActor* ClosestEnemy = nullptr;

	// Scan all actors of class AOLCUnitBase.
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOLCUnitBase::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		if (Actor == this) continue; // Skip self.
		if (!Actor->IsValidLowLevel()) continue;

		AOLCUnitBase* Other = Cast<AOLCUnitBase>(Actor);
		if (!Other || Other->IsDead()) continue;

		const float Dist = FVector::Dist(MyLocation, Other->GetActorLocation());
		if (Dist <= CurrentRange && Dist < ClosestDist)
		{
			ClosestDist = Dist;
			ClosestEnemy = Other;
		}
	}

	if (ClosestEnemy)
	{
		const float Now = GetWorld()->GetTimeSeconds();
		if (Now - LastAttackTime >= AttackCooldown)
		{
			Attack(ClosestEnemy);
			LastAttackTime = Now;
		}
	}
}

void AOLCUnitBase::Attack(AActor* Target)
{
	if (!Target || !UnitData) return;

	if (AOLCUnitBase* Enemy = Cast<AOLCUnitBase>(Target))
	{
		Enemy->TakeDamage(UnitData->AttackDamage);

		UE_LOG(LogTemp, Verbose, TEXT("[OLC] Unit '%s' attacked '%s' for %.0f damage (range=%.0f)"),
			*UnitData->DisplayName.ToString(),
			Enemy->GetUnitData() ? *Enemy->GetUnitData()->DisplayName.ToString() : TEXT("Unknown"),
			UnitData->AttackDamage,
			UnitData->Range);
	}
}

#undef LOCTEXT_NAMESPACE
