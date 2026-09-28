// Copyright OLC Project. WP-128 Step 4 — building assembly/collapse sequence component.

#include "Animation/OLCBuildingSequenceComponent.h"
#include "OurLastChance.h"

#include "Components/SceneComponent.h"
#include "Engine/GameInstance.h"
#include "NiagaraComponent.h"
#include "VFX/OLCVFXSubsystem.h"

namespace OLCBldSeq
{
	static constexpr float TAU = 6.2831853f;
	/** Degenerate-scale guard so transforms never hit exactly zero. */
	static constexpr float MinScale = 0.0001f;
}

UOLCBuildingSequenceComponent::UOLCBuildingSequenceComponent()
{
	// Ticks at the owning actor's tick (visual-only, local to the instance).
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// Default building-tier table: index 0 = Default (short) … last = Elite (longer).
	TierAssemblyDurations = { 2.0f, 3.0f, 4.5f };
}

void UOLCBuildingSequenceComponent::BeginPlay()
{
	Super::BeginPlay();

	CollectParts();

	if (bDebugStartOperating)
	{
		SetOperating(true);
	}

	// Temporary debug path for PIE verification: self-run assembly, then auto-collapse.
	// Deferred to the first tick: component BeginPlay runs while the actor is still in
	// BeginningPlay state, so HasActorBegunPlay() is false there and PlayAssemblySequence would reject.
	if (DebugMode == EOLCBuildingSeqDebugMode::AssemblyThenCollapse)
	{
		bDebugSequencePending = true;
	}
}

void UOLCBuildingSequenceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Deterministic cleanup: release the VFX handle, restore transforms and visibility.
	if (ConstructionGlow)
	{
		ConstructionGlow->DestroyComponent();
		ConstructionGlow = nullptr;
	}

	if (AActor* Owner = GetOwner())
	{
		Owner->SetActorHiddenInGame(false);
	}
	if (RootComp)
	{
		RootComp->SetRelativeTransform(RootBaseRelative);
	}
	for (FPartInfo& Part : Parts)
	{
		if (Part.Comp)
		{
			Part.Comp->SetRelativeTransform(Part.BaseRelative);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UOLCBuildingSequenceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner() || !RootComp)
	{
		return;
	}

	// Deferred debug-sequence start (see BeginPlay comment).
	if (bDebugSequencePending && GetOwner()->HasActorBegunPlay())
	{
		bDebugSequencePending = false;
		PlayAssemblySequence(DebugAssemblyDuration);
	}

	switch (Phase)
	{
	case ESeqPhase::Assembling:
		if (!bExternallyDriven)
		{
			AssemblyClock += DeltaTime;
			AssemblyProgress = FMath::Clamp(AssemblyClock / AssemblyDuration, 0.0f, 1.0f);
			ApplyAssemblyVisuals();
			if (AssemblyProgress >= 1.0f)
			{
				CompleteAssembly();
			}
		}
		break;

	case ESeqPhase::CollapsingShake:
	case ESeqPhase::CollapsingScale:
		CollapseClock += DeltaTime;
		ApplyCollapseVisuals(DeltaTime);
		break;

	case ESeqPhase::Idle:
	default:
		if (bOperating && !bCollapsed)
		{
			ApplyOperatingMotion(DeltaTime);
		}
		break;
	}
}

void UOLCBuildingSequenceComponent::CollectParts()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	RootComp = Owner->GetRootComponent();
	if (RootComp)
	{
		// The sequence moves this component every tick; Stationary components reject transform updates.
		if (RootComp->Mobility != EComponentMobility::Movable)
		{
			RootComp->SetMobility(EComponentMobility::Movable);
		}
		RootBaseRelative = RootComp->GetRelativeTransform();
	}

	TArray<USceneComponent*> All;
	Owner->GetComponents<USceneComponent>(All);
	for (USceneComponent* Comp : All)
	{
		if (!Comp || Comp == RootComp)
		{
			continue;
		}

		FPartInfo Info;
		Info.Comp = Comp;
		if (Comp->Mobility != EComponentMobility::Movable)
		{
			Comp->SetMobility(EComponentMobility::Movable);
		}
		Info.BaseRelative = Comp->GetRelativeTransform();
		Info.HeightZ = Info.BaseRelative.GetLocation().Z;

		const FString Name = Comp->GetName();
		Info.bIsMovingPart = NameMatchesFilter(MovingPartNameFilter, Name);
		// Drill/rotor style parts spin (Mine PB-EX-01); other moving parts oscillate vertically (Oil Pump PB-EX-02).
		Info.bRotatingPart = Info.bIsMovingPart &&
			(Name.Contains(TEXT("Drill")) || Name.Contains(TEXT("Rotor")));

		Parts.Add(Info);
	}

	// Assembly order: bottom → top (foundation first, roof last).
	Parts.Sort([](const FPartInfo& A, const FPartInfo& B) { return A.HeightZ < B.HeightZ; });
}

bool UOLCBuildingSequenceComponent::NameMatchesFilter(const FString& Filter, const FString& Name)
{
	TArray<FString> Tokens;
	Filter.ParseIntoArray(Tokens, TEXT("|"), true);
	for (const FString& Token : Tokens)
	{
		if (!Token.IsEmpty() && Name.Contains(Token, ESearchCase::IgnoreCase))
		{
			return true;
		}
	}
	return false;
}

void UOLCBuildingSequenceComponent::PlayAssemblySequence(float Duration)
{
	AActor* Owner = GetOwner();
	if (!Owner || !Owner->HasActorBegunPlay())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildingSequence: PlayAssemblySequence before BeginPlay — ignored (%s)"), *GetNameSafe(Owner));
		return;
	}

	// Duration <= 0 → tier-scaled duration from the building-tier table (Default short … Elite longer).
	if (Duration <= 0.0f)
	{
		const int32 Tier = FMath::Clamp(BuildingTierIndex, 0, TierAssemblyDurations.Num() - 1);
		Duration = TierAssemblyDurations.IsValidIndex(Tier) ? TierAssemblyDurations[Tier] : 3.0f;
	}
	AssemblyDuration = FMath::Max(0.1f, Duration);

	// Replay from a collapsed state: unhide and reset parts to the start of the reveal.
	Owner->SetActorHiddenInGame(false);
	bCollapsed = false;
	bExternallyDriven = false;
	Phase = ESeqPhase::Assembling;
	AssemblyClock = 0.0f;
	AssemblyProgress = 0.0f;
	bAssemblyCompletedForDebug = false;

	FireAssemblyVFXHook(/*bComplete=*/false);
	ApplyAssemblyVisuals();

	UE_LOG(LogOLC, Log, TEXT("[OLC] BuildingSequence: %s assembly start (duration=%.2fs tier=%d parts=%d)"),
		*GetNameSafe(Owner), AssemblyDuration, BuildingTierIndex, Parts.Num());
}

void UOLCBuildingSequenceComponent::SetAssemblyProgress(float Progress)
{
	AActor* Owner = GetOwner();
	if (!Owner || !Owner->HasActorBegunPlay())
	{
		return;
	}

	// External drive (build-time tie-in): cancel any running collapse, take over the clock.
	bExternallyDriven = true;
	if (Phase == ESeqPhase::CollapsingShake || Phase == ESeqPhase::CollapsingScale)
	{
		Owner->SetActorHiddenInGame(false);
		bCollapsed = false;
	}
	Phase = ESeqPhase::Assembling;
	AssemblyProgress = FMath::Clamp(Progress, 0.0f, 1.0f);
	ApplyAssemblyVisuals();

	if (AssemblyProgress >= 1.0f)
	{
		CompleteAssembly();
	}
}

void UOLCBuildingSequenceComponent::PlayCollapseSequence()
{
	AActor* Owner = GetOwner();
	if (!Owner || !Owner->HasActorBegunPlay())
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] BuildingSequence: PlayCollapseSequence before BeginPlay — ignored (%s)"), *GetNameSafe(Owner));
		return;
	}
	if (bCollapsed)
	{
		return; // Already hidden — replay via PlayAssemblySequence first.
	}

	// Ensure the building is fully revealed before it collapses.
	bExternallyDriven = false;
	AssemblyProgress = 1.0f;
	ApplyAssemblyVisuals();

	Phase = ESeqPhase::CollapsingShake;
	CollapseClock = 0.0f;

	UE_LOG(LogOLC, Log, TEXT("[OLC] BuildingSequence: %s collapse start (shake=%.2fs scale=%.2fs parts=%d)"),
		*GetNameSafe(Owner), CollapseShakeDuration, CollapseScaleDuration, Parts.Num());
}

void UOLCBuildingSequenceComponent::SetOperating(bool bNewOperating)
{
	bOperating = bNewOperating;
	OperatingClock = 0.0f;
	for (FPartInfo& Part : Parts)
	{
		Part.SpinAngleDeg = 0.0f;
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] BuildingSequence: %s operating=%d"), *GetNameSafe(GetOwner()), bNewOperating ? 1 : 0);
}

void UOLCBuildingSequenceComponent::ApplyAssemblyVisuals()
{
	const float P = AssemblyProgress;

	if (RootComp)
	{
		// Root scales 0→1 linearly with progress (progress-faithful for build-time tie-in).
		const float RootScale = FMath::Max(OLCBldSeq::MinScale, P);
		RootComp->SetRelativeLocation(RootBaseRelative.GetLocation());
		RootComp->SetRelativeRotation(RootBaseRelative.GetRotation().Rotator());
		RootComp->SetRelativeScale3D(FVector(RootScale, RootScale, RootScale));
	}

	const int32 N = Parts.Num();
	for (int32 i = 0; i < N; ++i)
	{
		FPartInfo& Part = Parts[i];
		if (!Part.Comp)
		{
			continue;
		}

		// Staggered reveal, bottom → top: each part gets its own window inside the total duration.
		const float StaggerStart = (N > 1) ? (AssemblyStaggerFraction * static_cast<float>(i) / static_cast<float>(N - 1)) : 0.0f;
		const float Window = FMath::Max(0.05f, 1.0f - AssemblyStaggerFraction);
		const float LocalP = FMath::Clamp((P - StaggerStart) / Window, 0.0f, 1.0f);
		const float S = FMath::Max(OLCBldSeq::MinScale, FMath::Lerp(PartStartScale, 1.0f, EaseOutCubic(LocalP)));

		Part.Comp->SetRelativeLocation(Part.BaseRelative.GetLocation());
		Part.Comp->SetRelativeRotation(Part.BaseRelative.GetRotation().Rotator());
		Part.Comp->SetRelativeScale3D(FVector(S, S, S));
	}
}

void UOLCBuildingSequenceComponent::CompleteAssembly()
{
	Phase = ESeqPhase::Idle;
	FireAssemblyVFXHook(/*bComplete=*/true);

	UE_LOG(LogOLC, Log, TEXT("[OLC] BuildingSequence: %s assembly complete (full scale)"), *GetNameSafe(GetOwner()));

	if (DebugMode == EOLCBuildingSeqDebugMode::AssemblyThenCollapse && !bAssemblyCompletedForDebug)
	{
		bAssemblyCompletedForDebug = true;
		PlayCollapseSequence();
	}
}

void UOLCBuildingSequenceComponent::ApplyCollapseVisuals(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner || !RootComp)
	{
		return;
	}

	if (Phase == ESeqPhase::CollapsingShake)
	{
		// Structural shake: jitter the root location with a slight rotation wobble.
		const FVector Jitter =
			FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-0.5f, 0.5f)) * CollapseShakeAmplitude;
		const FRotator BaseRot = RootBaseRelative.GetRotation().Rotator();
		RootComp->SetRelativeLocation(RootBaseRelative.GetLocation() + Jitter);
		RootComp->SetRelativeRotation(FRotator(FMath::FRandRange(-2.0f, 2.0f), FMath::FRandRange(-2.0f, 2.0f), BaseRot.Roll));

		if (CollapseClock >= CollapseShakeDuration)
		{
			Phase = ESeqPhase::CollapsingScale;
			CollapseClock = 0.0f;
		}
		return;
	}

	// Staggered scale-down, top → bottom ("from top/edges inward").
	const float P = FMath::Clamp(CollapseClock / CollapseScaleDuration, 0.0f, 1.0f);
	const int32 N = Parts.Num();

	if (N == 0)
	{
		// No child parts: the root itself is the collapsing body.
		const float S = FMath::Max(OLCBldSeq::MinScale, 1.0f - EaseInQuad(P));
		RootComp->SetRelativeScale3D(FVector(S, S, S));
	}
	else
	{
		for (int32 i = N - 1; i >= 0; --i) // topmost first
		{
			FPartInfo& Part = Parts[i];
			if (!Part.Comp)
			{
				continue;
			}
			const int32 Rank = N - 1 - i; // 0 = topmost
			const float StaggerStart = (N > 1) ? (CollapseStaggerFraction * static_cast<float>(Rank) / static_cast<float>(N - 1)) : 0.0f;
			const float Window = FMath::Max(0.05f, 1.0f - CollapseStaggerFraction);
			const float LocalP = FMath::Clamp((P - StaggerStart) / Window, 0.0f, 1.0f);
			const float S = FMath::Max(OLCBldSeq::MinScale, 1.0f - EaseInQuad(LocalP));
			Part.Comp->SetRelativeScale3D(FVector(S, S, S));
		}
	}

	if (P >= 1.0f)
	{
		FinishCollapse();
	}
}

void UOLCBuildingSequenceComponent::FinishCollapse()
{
	AActor* Owner = GetOwner();
	bCollapsed = true;
	Phase = ESeqPhase::Idle;

	FireCollapseVFXHook(); // single VFX burst hook at the moment of final hide
	if (Owner)
	{
		Owner->SetActorHiddenInGame(true);
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] BuildingSequence: %s collapse complete — actor hidden"), *GetNameSafe(Owner));
}

void UOLCBuildingSequenceComponent::ApplyOperatingMotion(float DeltaTime)
{
	OperatingClock += DeltaTime;

	for (FPartInfo& Part : Parts)
	{
		if (!Part.Comp || !Part.bIsMovingPart)
		{
			continue;
		}

		if (Part.bRotatingPart)
		{
			// Drill/rotor: continuous spin around Z (Mine PB-EX-01).
			Part.SpinAngleDeg += OperatingSpinDegreesPerSecond * DeltaTime;
			const FRotator Base = Part.BaseRelative.GetRotation().Rotator();
			Part.Comp->SetRelativeRotation(FRotator(Base.Pitch, Base.Yaw, Base.Roll + Part.SpinAngleDeg));
		}
		else
		{
			// Pump arm: vertical oscillation (Oil Pump PB-EX-02).
			const float Offset = OperatingOscillationAmplitudeZ *
				FMath::Sin(OLCBldSeq::TAU * OperatingOscillationFrequencyHz * OperatingClock);
			Part.Comp->SetRelativeLocation(Part.BaseRelative.GetLocation() + FVector(0.0f, 0.0f, Offset));
		}
	}
}

void UOLCBuildingSequenceComponent::FireAssemblyVFXHook(bool bComplete)
{
	if (!bEnableVFXHooks || !GetOwner())
	{
		return;
	}
	UGameInstance* GameInstance = GetOwner()->GetGameInstance();
	UOLCVFXSubsystem* VFX = GameInstance ? GameInstance->GetSubsystem<UOLCVFXSubsystem>() : nullptr;
	if (!VFX)
	{
		return;
	}

	// The construction glow is the one VFX handle we own — always released before re-acquiring.
	if (ConstructionGlow)
	{
		ConstructionGlow->DestroyComponent();
		ConstructionGlow = nullptr;
	}

	if (!bComplete && RootComp)
	{
		// Null-tolerant: returns null when no construction-glow system is registered in DA_VFX.
		ConstructionGlow = VFX->PlayBuildingConstructionGlow(RootComp);
	}
}

void UOLCBuildingSequenceComponent::FireCollapseVFXHook()
{
	if (!bEnableVFXHooks || !GetOwner())
	{
		return;
	}
	UGameInstance* GameInstance = GetOwner()->GetGameInstance();
	UOLCVFXSubsystem* VFX = GameInstance ? GameInstance->GetSubsystem<UOLCVFXSubsystem>() : nullptr;
	if (!VFX)
	{
		return;
	}

	// One-shot fire-and-forget burst — the subsystem owns its emitter.
	VFX->PlayBuildingDestruction(GetOwner()->GetActorLocation());
}

float UOLCBuildingSequenceComponent::EaseOutCubic(float X)
{
	return 1.0f - FMath::Pow(1.0f - X, 3.0f);
}

float UOLCBuildingSequenceComponent::EaseInQuad(float X)
{
	return X * X;
}
