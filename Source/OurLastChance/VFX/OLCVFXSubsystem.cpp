#include "VFX/OLCVFXSubsystem.h"
#include "OurLastChance.h"

#include "NiagaraFunctionLibrary.h"
#include "VFX/OLCVFXData.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Logging/LogMacros.h"

#define LOCTEXT_NAMESPACE "OLCVFXSubsystem"

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UOLCVFXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load the designer-tunable VFX data asset (optional — built-in defaults apply when absent).
	VFXData = LoadObject<UOLCVFXData>(nullptr, TEXT("/Game/OurLastChance/Data/VFX/DA_VFX"));

	if (!VFXData)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] DA_VFX asset not found at /Game/OurLastChance/Data/VFX/DA_VFX — using built-in defaults (all null-tolerant)."));
	}

	// Register the FTSTicker reaper to prune finished one-shot components.
	ReaperTickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UOLCVFXSubsystem::HandleReaperTick), 0.5f);

	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] Subsystem initialized (data: %s)"),
		VFXData ? *VFXData->GetName() : TEXT("built-in defaults"));
}

void UOLCVFXSubsystem::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(ReaperTickHandle);

	// Destroy any remaining one-shot components.
	for (const TWeakObjectPtr<UNiagaraComponent>& WeakComp : ActiveOneShots)
	{
		if (UNiagaraComponent* Comp = WeakComp.Get())
		{
			Comp->DestroyComponent();
		}
	}
	ActiveOneShots.Empty();

	VFXData = nullptr;

	Super::Deinitialize();

	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] Subsystem deinitialized."));
}

// ---------------------------------------------------------------------------
// Core spawn primitives
// ---------------------------------------------------------------------------

UNiagaraComponent* UOLCVFXSubsystem::SpawnOneShotAtLocation(const UNiagaraSystem* System, FVector Location, FRotator Rotation, FVector Scale)
{
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] SpawnOneShotAtLocation: null system — no-op."));
		return nullptr;
	}

	UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	if (!World)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] SpawnOneShotAtLocation: no world — no-op."));
		return nullptr;
	}

	UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		const_cast<UNiagaraSystem*>(System),
		Location,
		Rotation,
		Scale,
		/*bAutoDestroy*/ true,
		/*bAutoActivate*/ true);

	if (Comp)
	{
		ActiveOneShots.Add(Comp);
	}

	return Comp;
}

AActor* UOLCVFXSubsystem::SpawnLoopingAtLocation(const UNiagaraSystem* System, FVector Location, FRotator Rotation, FVector Scale)
{
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] SpawnLoopingAtLocation: null system — no-op."));
		return nullptr;
	}

	UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	if (!World)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] SpawnLoopingAtLocation: no world — no-op."));
		return nullptr;
	}

	UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		const_cast<UNiagaraSystem*>(System),
		Location,
		Rotation,
		Scale,
		/*bAutoDestroy*/ false,
		/*bAutoActivate*/ true);

	return Comp ? Comp->GetOwner() : nullptr;
}

UNiagaraComponent* UOLCVFXSubsystem::AttachLoopingToComponent(const UNiagaraSystem* System, USceneComponent* AttachTo, FName AttachPointName)
{
	if (!System || !AttachTo)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] AttachLoopingToComponent: null system or attach target — no-op."));
		return nullptr;
	}

	UNiagaraComponent* Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		const_cast<UNiagaraSystem*>(System),
		AttachTo,
		AttachPointName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		/*bAutoDestroy*/ false,
		/*bAutoActivate*/ true);

	return Comp;
}

// ---------------------------------------------------------------------------
// Semantic helpers
// ---------------------------------------------------------------------------

void UOLCVFXSubsystem::PlayCombatVFX(EOLCDamageType DamageType, FVector Location)
{
	const UNiagaraSystem* System = ResolveCombatSystem(DamageType);
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayCombatVFX: no system resolved for damage type %d — no-op."), static_cast<int32>(DamageType));
		return;
	}

	UNiagaraComponent* Comp = SpawnOneShotAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-3 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayCombatVFX: damage type %d -> %s (component %s)"),
		static_cast<int32>(DamageType), *System->GetPathName(), Comp ? TEXT("spawned") : TEXT("FAILED"));
}

void UOLCVFXSubsystem::PlayEnvironmentalVFX(FName EventKey, FVector Location)
{
	const UNiagaraSystem* System = ResolveEnvironmentalSystem(EventKey);
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayEnvironmentalVFX: no system resolved for key %s — no-op."), *EventKey.ToString());
		return;
	}

	UNiagaraComponent* Comp = SpawnOneShotAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-4a verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayEnvironmentalVFX: key %s -> %s (component %s)"),
		*EventKey.ToString(), *System->GetPathName(), Comp ? TEXT("spawned") : TEXT("FAILED"));
}

AActor* UOLCVFXSubsystem::SpawnBiomeAmbient(EOLCBiomeType BiomeType, FVector Location)
{
	const UNiagaraSystem* System = ResolveBiomeAmbientSystem(BiomeType);
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] SpawnBiomeAmbient: no system resolved for biome %d — no-op."), static_cast<int32>(BiomeType));
		return nullptr;
	}

	AActor* Ambient = SpawnLoopingAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-6 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] SpawnBiomeAmbient: biome %d -> %s (looping actor %s)"),
		static_cast<int32>(BiomeType), *System->GetPathName(), Ambient ? TEXT("spawned") : TEXT("FAILED"));

	return Ambient;
}

void UOLCVFXSubsystem::PlayCrashImpact(FVector Location)
{
	TSoftObjectPtr<UNiagaraSystem> SoftRef;

	if (VFXData)
	{
		SoftRef = VFXData->CrashImpactExplosion;
	}

	UNiagaraSystem* System = SoftRef.LoadSynchronous();
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayCrashImpact: no crash impact system available — no-op."));
		return;
	}

	UNiagaraComponent* Comp = SpawnOneShotAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-5 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayCrashImpact -> %s (component %s)"),
		*System->GetPathName(), Comp ? TEXT("spawned") : TEXT("FAILED"));
}

void UOLCVFXSubsystem::PlayCrashDust(FVector Location)
{
	TSoftObjectPtr<UNiagaraSystem> SoftRef;

	if (VFXData)
	{
		SoftRef = VFXData->CrashLandingDust;
	}

	UNiagaraSystem* System = SoftRef.LoadSynchronous();
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayCrashDust: no crash dust system available — no-op."));
		return;
	}

	UNiagaraComponent* Comp = SpawnOneShotAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-5 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayCrashDust -> %s (component %s)"),
		*System->GetPathName(), Comp ? TEXT("spawned") : TEXT("FAILED"));
}

AActor* UOLCVFXSubsystem::PlayCrashSmoke(FVector Location)
{
	TSoftObjectPtr<UNiagaraSystem> SoftRef;

	if (VFXData)
	{
		SoftRef = VFXData->CrashSmokeTrail;
	}

	UNiagaraSystem* System = SoftRef.LoadSynchronous();
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayCrashSmoke: no crash smoke system available — no-op."));
		return nullptr;
	}

	AActor* Smoke = SpawnLoopingAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-5 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayCrashSmoke -> %s (looping actor %s)"),
		*System->GetPathName(), Smoke ? TEXT("spawned") : TEXT("FAILED"));

	return Smoke;
}

UNiagaraComponent* UOLCVFXSubsystem::PlayBuildingConstructionGlow(USceneComponent* BuildingRoot)
{
	if (!BuildingRoot)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayBuildingConstructionGlow: null building root — no-op."));
		return nullptr;
	}

	TSoftObjectPtr<UNiagaraSystem> SoftRef;

	if (VFXData)
	{
		SoftRef = VFXData->BuildingConstructionGlow;
	}

	UNiagaraSystem* System = SoftRef.LoadSynchronous();
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayBuildingConstructionGlow: no construction glow system available — no-op."));
		return nullptr;
	}

	UNiagaraComponent* Glow = AttachLoopingToComponent(System, BuildingRoot);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-5 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayBuildingConstructionGlow -> %s (component %s)"),
		*System->GetPathName(), Glow ? TEXT("attached") : TEXT("FAILED"));

	return Glow;
}

void UOLCVFXSubsystem::PlayBuildingDestruction(FVector Location)
{
	TSoftObjectPtr<UNiagaraSystem> SoftRef;

	if (VFXData)
	{
		SoftRef = VFXData->BuildingDestructionBurst;
	}

	UNiagaraSystem* System = SoftRef.LoadSynchronous();
	if (!System)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC VFX] PlayBuildingDestruction: no destruction burst system available — no-op."));
		return;
	}

	UNiagaraComponent* Comp = SpawnOneShotAtLocation(System, Location);

	// Diagnostic: make the resolution observable in PIE logs (WP-126 step-5 verification).
	UE_LOG(LogOLC, Log, TEXT("[OLC VFX] PlayBuildingDestruction -> %s (component %s)"),
		*System->GetPathName(), Comp ? TEXT("spawned") : TEXT("FAILED"));
}

// ---------------------------------------------------------------------------
// Resolution helpers
// ---------------------------------------------------------------------------

const UNiagaraSystem* UOLCVFXSubsystem::ResolveCombatSystem(EOLCDamageType DamageType) const
{
	const TArray<FOLCCombatVFXMapping>& Mappings = VFXData ? VFXData->CombatMappings : UOLCVFXData::GetDefaultCombatMappings();

	for (const FOLCCombatVFXMapping& Mapping : Mappings)
	{
		if (Mapping.DamageType == DamageType)
		{
			return Mapping.NiagaraSystem.LoadSynchronous();
		}
	}

	return nullptr;
}

const UNiagaraSystem* UOLCVFXSubsystem::ResolveEnvironmentalSystem(FName EventKey) const
{
	const TArray<FOLCEnvironmentalVFXMapping>& Mappings = VFXData ? VFXData->EnvironmentalMappings : UOLCVFXData::GetDefaultEnvironmentalMappings();

	for (const FOLCEnvironmentalVFXMapping& Mapping : Mappings)
	{
		if (Mapping.EventKey == EventKey)
		{
			return Mapping.NiagaraSystem.LoadSynchronous();
		}
	}

	return nullptr;
}

const UNiagaraSystem* UOLCVFXSubsystem::ResolveBiomeAmbientSystem(EOLCBiomeType BiomeType) const
{
	const TArray<FOLCBiomeAmbientMapping>& Mappings = VFXData ? VFXData->BiomeAmbientMappings : UOLCVFXData::GetDefaultBiomeAmbientMappings();

	for (const FOLCBiomeAmbientMapping& Mapping : Mappings)
	{
		if (Mapping.BiomeType == BiomeType)
		{
			return Mapping.NiagaraSystem.LoadSynchronous();
		}
	}

	return nullptr;
}

// ---------------------------------------------------------------------------
// FTSTicker reaper
// ---------------------------------------------------------------------------

bool UOLCVFXSubsystem::HandleReaperTick(float DeltaTime)
{
	// Prune any one-shot components that have been destroyed or are no longer valid.
	ActiveOneShots.RemoveAll([](const TWeakObjectPtr<UNiagaraComponent>& WeakComp)
	{
		return !WeakComp.IsValid();
	});

	// Continue ticking (return true to keep the delegate registered).
	return true;
}

#undef LOCTEXT_NAMESPACE
