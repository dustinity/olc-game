// Copyright OLC Project. WP-128 Step 3 — state-slot animation player component.

#include "Animation/OLCAnimationPlayerComponent.h"
#include "OurLastChance.h"

#include "Animation/AnimSequence.h"
#include "Combat/OLCCombatTypes.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/Crc.h"
#include "VFX/OLCVFXData.h"
#include "VFX/OLCVFXSubsystem.h"

namespace OLCAnim
{
	static constexpr float TAU = 6.2831853f;

	// Procedural slot durations (seconds).
	static constexpr float CrashDuration = 1.2f;
	static constexpr float CollapseDuration = 1.5f;
	static constexpr float RampDuration = 2.0f;     // Takeoff / Descent
	static constexpr float AssembleDuration = 2.0f;
	static constexpr float ShipAltitude = 300.0f;   // sustained cruise altitude offset
}

UOLCAnimationPlayerComponent::UOLCAnimationPlayerComponent()
{
	// Ticks at the owning actor's tick (visual-only, local to the instance).
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOLCAnimationPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	ResolveVisualComponent();
	if (VisualComponent)
	{
		BaseVisualTransform = VisualComponent->GetRelativeTransform();
	}

	SetSlot(InitialSlot, 0.0f);
}

void UOLCAnimationPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearPulseMaterial();

	if (VisualComponent)
	{
		VisualComponent->SetRelativeTransform(BaseVisualTransform);
		VisualComponent->SetVisibility(true, true);
	}
	bSlotHidden = false;

	Super::EndPlay(EndPlayReason);
}

void UOLCAnimationPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!VisualComponent || !GetOwner())
	{
		return;
	}

	// Advance slot clock, attack pulse clock and blend.
	SlotClock += DeltaTime;
	AttackPulseClock += DeltaTime;
	if (BlendTimeRemaining > 0.0f)
	{
		BlendTimeRemaining = FMath::Max(0.0f, BlendTimeRemaining - DeltaTime);
	}

	// Compute the target offset for the current slot (identity when asset-backed).
	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;
	FVector TargetScale = FVector::OneVector;
	if (!bAssetBackedActive)
	{
		ComputeSlotOffset(TargetLocation, TargetRotation, TargetScale);

		// Hide-on-completion (Crash / Collapse).
		const bool bReachedEnd =
			(CurrentSlot == EOLCAnimSlot::Crash && SlotClock >= OLCAnim::CrashDuration) ||
			(CurrentSlot == EOLCAnimSlot::Collapse && SlotClock >= OLCAnim::CollapseDuration);
		if (bReachedEnd && !bSlotHidden)
		{
			bSlotHidden = true;
			VisualComponent->SetVisibility(false, true);
		}
	}

	// Blend the applied transform from the blend-start point toward the target.
	const float BlendWeight = (BlendDuration > 0.0f) ? (1.0f - BlendTimeRemaining / BlendDuration) : 1.0f;
	AppliedLocationOffset = FMath::Lerp(BlendStartLocation, TargetLocation, BlendWeight);
	AppliedRotationOffset = FRotator(
		FMath::Lerp(BlendStartRotation.Pitch, TargetRotation.Pitch, BlendWeight),
		FMath::Lerp(BlendStartRotation.Yaw, TargetRotation.Yaw, BlendWeight),
		FMath::Lerp(BlendStartRotation.Roll, TargetRotation.Roll, BlendWeight));
	AppliedScale = FMath::Lerp(BlendStartScale, TargetScale, BlendWeight);

	if (!bSlotHidden)
	{
		VisualComponent->SetRelativeLocation(BaseVisualTransform.GetLocation() + AppliedLocationOffset);
		VisualComponent->SetRelativeRotation((BaseVisualTransform.GetRotation() * AppliedRotationOffset.Quaternion()).Rotator());
		VisualComponent->SetRelativeScale3D(BaseVisualTransform.GetScale3D() * AppliedScale);
	}

	// Emissive pulse (Heal / Damage).
	if (PulseMID)
	{
		UpdatePulseMaterial(DeltaTime);
	}
}

void UOLCAnimationPlayerComponent::SetSlot(EOLCAnimSlot NewSlot, float BlendTime)
{
	// Idempotent: same settled slot → no-op (no restart flicker).
	if (NewSlot == CurrentSlot && BlendTimeRemaining <= 0.0f && !bSlotHidden)
	{
		return;
	}

	const bool bReady = GetOwner() && GetOwner()->HasActorBegunPlay();

	// Leaving a hidden slot: restore visibility.
	if (bSlotHidden && VisualComponent)
	{
		VisualComponent->SetVisibility(true, true);
		bSlotHidden = false;
	}

	CurrentSlot = NewSlot;
	SlotClock = 0.0f;
	AttackPulseClock = 0.0f;
	BlendDuration = FMath::Max(0.0f, BlendTime);
	BlendTimeRemaining = BlendDuration;

	if (!bReady)
	{
		// Pre-BeginPlay configuration: BeginPlay applies the slot once components exist.
		bAssetBackedActive = false;
		return;
	}

	// Capture the current applied transform as the blend start point.
	BlendStartLocation = AppliedLocationOffset;
	BlendStartRotation = AppliedRotationOffset;
	BlendStartScale = AppliedScale;

	// Asset-backed playback attempt (procedural fallback when it fails).
	bAssetBackedActive = false;
	if (AnimationSet)
	{
		FOLCAnimationEntry Entry;
		if (AnimationSet->FindEntry(NewSlot, Entry))
		{
			bAssetBackedActive = TryPlayAssetBacked(Entry);
		}
	}

	if (!bAssetBackedActive && (NewSlot == EOLCAnimSlot::Heal || NewSlot == EOLCAnimSlot::Damage))
	{
		SetupPulseMaterial(NewSlot);
	}
	else
	{
		ClearPulseMaterial();
	}

	if (const UEnum* SlotEnum = StaticEnum<EOLCAnimSlot>())
	{
		UE_LOG(LogOLC, Log, TEXT("[OLC] AnimationPlayer: %s -> slot=%s assetBacked=%d blend=%.2fs"),
			*GetNameSafe(GetOwner()),
			*SlotEnum->GetNameStringByValue(static_cast<int64>(CurrentSlot)),
			bAssetBackedActive ? 1 : 0,
			BlendDuration);
	}
}

void UOLCAnimationPlayerComponent::ResolveVisualComponent()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	USceneComponent* Resolved = nullptr;

	// Prefer a non-root skeletal mesh with an assigned mesh (units).
	TArray<USkeletalMeshComponent*> SkeletalMeshes;
	Owner->GetComponents<USkeletalMeshComponent>(SkeletalMeshes);
	for (USkeletalMeshComponent* Mesh : SkeletalMeshes)
	{
		if (Mesh && Mesh != Owner->GetRootComponent() && Mesh->GetSkeletalMeshAsset())
		{
			Resolved = Mesh;
			break;
		}
	}

	// Then a non-root static mesh with an assigned mesh (buildings / test actors).
	if (!Resolved)
	{
		TArray<UStaticMeshComponent*> StaticMeshes;
		Owner->GetComponents<UStaticMeshComponent>(StaticMeshes);
		for (UStaticMeshComponent* Mesh : StaticMeshes)
		{
			if (Mesh && Mesh != Owner->GetRootComponent() && Mesh->GetStaticMesh())
			{
				Resolved = Mesh;
				break;
			}
		}
	}

	// Fall back to any mesh component, then the root scene component.
	if (!Resolved)
	{
		for (USkeletalMeshComponent* Mesh : SkeletalMeshes)
		{
			if (Mesh)
			{
				Resolved = Mesh;
				break;
			}
		}
	}
	if (!Resolved)
	{
		TArray<UStaticMeshComponent*> StaticMeshes;
		Owner->GetComponents<UStaticMeshComponent>(StaticMeshes);
		for (UStaticMeshComponent* Mesh : StaticMeshes)
		{
			if (Mesh)
			{
				Resolved = Mesh;
				break;
			}
		}
	}
	if (!Resolved)
	{
		Resolved = Owner->GetRootComponent();
	}

	VisualComponent = Resolved;

	// The animation player moves this component every tick; Stationary/Static
	// components reject transform updates, so make it movable up front.
	if (VisualComponent && VisualComponent->Mobility != EComponentMobility::Movable)
	{
		VisualComponent->SetMobility(EComponentMobility::Movable);
	}
}

UStaticMeshComponent* UOLCAnimationPlayerComponent::FindPrimaryStaticMesh() const
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}

	TArray<UStaticMeshComponent*> StaticMeshes;
	Owner->GetComponents<UStaticMeshComponent>(StaticMeshes);

	UStaticMeshComponent* Fallback = nullptr;
	for (UStaticMeshComponent* Mesh : StaticMeshes)
	{
		if (!Mesh || !Mesh->GetStaticMesh())
		{
			continue;
		}
		if (Mesh != Owner->GetRootComponent())
		{
			return Mesh;
		}
		if (!Fallback)
		{
			Fallback = Mesh;
		}
	}
	return Fallback;
}

bool UOLCAnimationPlayerComponent::TryPlayAssetBacked(const FOLCAnimationEntry& Entry) const
{
	if (!Entry.AnimationAsset)
	{
		return false;
	}

	// Skeletal path (units): play the sequence on the owner's skeletal mesh.
	if (const UAnimSequence* Sequence = Cast<UAnimSequence>(Entry.AnimationAsset))
	{
		AActor* Owner = GetOwner();
		if (!Owner)
		{
			return false;
		}

		TArray<USkeletalMeshComponent*> Meshes;
		Owner->GetComponents<USkeletalMeshComponent>(Meshes);
		for (USkeletalMeshComponent* Mesh : Meshes)
		{
			if (Mesh && Mesh->IsRegistered())
			{
				Mesh->PlayAnimation(const_cast<UAnimSequence*>(Sequence), Entry.bLoop);
				Mesh->SetPlayRate(Entry.PlayRate);
				return true;
			}
		}
		return false; // No skeletal mesh on the owner -> procedural fallback.
	}

	// Sprite/material path (buildings, billboards): apply to the primary static mesh.
	if (const UMaterialInterface* Material = Cast<UMaterialInterface>(Entry.AnimationAsset))
	{
		if (UStaticMeshComponent* Mesh = FindPrimaryStaticMesh())
		{
			Mesh->SetMaterial(0, const_cast<UMaterialInterface*>(Material));
			return true;
		}
		return false;
	}

	return false; // Unsupported asset type -> procedural fallback.
}

void UOLCAnimationPlayerComponent::ComputeSlotOffset(FVector& OutLocation, FRotator& OutRotation, FVector& OutScale) const
{
	const float T = SlotClock;
	const float Speed = GetOwner() ? GetOwner()->GetVelocity().Size() : 0.0f;

	OutLocation = FVector::ZeroVector;
	OutRotation = FRotator::ZeroRotator;
	OutScale = FVector::OneVector;

	switch (CurrentSlot)
	{
	case EOLCAnimSlot::Idle:
		// Gentle vertical bob (~0.42 Hz).
		OutLocation.Z = 3.0f * FMath::Sin(OLCAnim::TAU * T / 2.4f);
		break;

	case EOLCAnimSlot::Walk:
	case EOLCAnimSlot::Move:
	{
		// Speed-scaled forward lean + bob (slower tread rhythm for Move).
		const float Lean = FMath::Clamp(Speed / 350.0f, 0.0f, 1.0f) * 8.0f;
		const float Freq = (CurrentSlot == EOLCAnimSlot::Walk ? 1.2f : 1.0f) + Speed / 300.0f;
		const float Amp = (CurrentSlot == EOLCAnimSlot::Walk ? 4.0f : 3.0f);
		OutLocation.Z = Amp * FMath::Sin(OLCAnim::TAU * Freq * T);
		OutRotation.Pitch = -Lean; // lean forward (toward +X)
		break;
	}

	case EOLCAnimSlot::Run:
	{
		const float Lean = FMath::Clamp(Speed / 350.0f, 0.0f, 1.0f) * 16.0f;
		OutLocation.Z = 7.0f * FMath::Sin(OLCAnim::TAU * (2.2f + Speed / 250.0f) * T);
		OutRotation.Pitch = -Lean;
		break;
	}

	case EOLCAnimSlot::Attack:
	case EOLCAnimSlot::Fire:
	{
		// Quick recoil scale/rotation pulse, repeating at the ~0.5 s shot cadence.
		constexpr float Cadence = 0.5f;
		constexpr float PulseLen = 0.12f;
		const float PhaseIn = FMath::Fmod(AttackPulseClock, Cadence);
		if (PhaseIn < PulseLen)
		{
			const float Shape = FMath::Sin(PI * (PhaseIn / PulseLen)); // 0 -> 1 -> 0
			OutScale = FVector(1.0f - 0.06f * Shape, 1.0f - 0.06f * Shape, 1.0f + 0.04f * Shape);
			OutRotation.Pitch = 5.0f * Shape; // recoil kick back
		}
		break;
	}

	case EOLCAnimSlot::Heal:
		// Slight crouch; the emissive green pulse is handled by the MID.
		OutLocation.Z = -6.0f;
		break;

	case EOLCAnimSlot::Hover:
		// Sine altitude oscillation (~0.5 Hz).
		OutLocation.Z = 12.0f * FMath::Sin(OLCAnim::TAU * T / 2.0f);
		break;

	case EOLCAnimSlot::Fly:
	{
		// Sustained hover with directional (pitch) tilt by speed.
		const float Lean = FMath::Clamp(Speed / 400.0f, 0.0f, 1.0f) * 14.0f;
		OutLocation.Z = 3.0f * FMath::Sin(OLCAnim::TAU * T / 1.6f);
		OutRotation.Pitch = -Lean;
		break;
	}

	case EOLCAnimSlot::Damage:
		// Shake (random jitter) + red flash pulse via MID.
		OutLocation = FVector(FMath::FRandRange(-4.0f, 4.0f), FMath::FRandRange(-4.0f, 4.0f), FMath::FRandRange(-3.0f, 3.0f));
		break;

	case EOLCAnimSlot::Crash:
	{
		// Accelerating fall (v0*t + 1/2*g*t^2).
		const float Fall = 120.0f * T + 0.5f * 900.0f * T * T;
		OutLocation.Z = -FMath::Min(Fall, 8000.0f);
		OutRotation.Pitch = -12.0f;
		break;
	}

	case EOLCAnimSlot::Takeoff:
	{
		const float P = FMath::Clamp(T / OLCAnim::RampDuration, 0.0f, 1.0f);
		OutLocation.Z = OLCAnim::ShipAltitude * EaseOutCubic(P);
		OutRotation.Pitch = 6.0f * (1.0f - P); // nose up while climbing
		break;
	}

	case EOLCAnimSlot::Cruise:
	{
		const float Lean = FMath::Clamp(Speed / 400.0f, 0.0f, 1.0f) * 8.0f;
		OutLocation.Z = OLCAnim::ShipAltitude + 4.0f * FMath::Sin(OLCAnim::TAU * T / 2.5f);
		OutRotation.Pitch = -Lean;
		break;
	}

	case EOLCAnimSlot::Descent:
	{
		const float P = FMath::Clamp(T / OLCAnim::RampDuration, 0.0f, 1.0f);
		OutLocation.Z = OLCAnim::ShipAltitude * (1.0f - EaseInCubic(P));
		OutRotation.Pitch = -6.0f * P; // nose down while descending
		break;
	}

	case EOLCAnimSlot::Touchdown:
	{
		// Damped settle wobble.
		const float Damp = FMath::Exp(-2.0f * T);
		OutLocation.Z = 8.0f * Damp * FMath::Sin(OLCAnim::TAU * T / 0.6f);
		break;
	}

	case EOLCAnimSlot::Assemble:
	{
		// Framework rises: scale in from 20 % over AssembleDuration.
		const float P = FMath::Clamp(T / OLCAnim::AssembleDuration, 0.0f, 1.0f);
		const float S = 0.2f + 0.8f * EaseOutCubic(P);
		OutScale = FVector(S, S, S);
		break;
	}

	case EOLCAnimSlot::Collapse:
	{
		// Jitter + scale down to zero over CollapseDuration.
		const float P = FMath::Clamp(T / OLCAnim::CollapseDuration, 0.0f, 1.0f);
		const float S = FMath::Max(0.0f, 1.0f - EaseInQuad(P));
		OutScale = FVector(S, S, S);
		OutLocation = FVector(FMath::FRandRange(-3.0f, 3.0f), FMath::FRandRange(-3.0f, 3.0f), 0.0f);
		break;
	}
	}
}

float UOLCAnimationPlayerComponent::EaseOutCubic(float X)
{
	return 1.0f - FMath::Pow(1.0f - X, 3.0f);
}

float UOLCAnimationPlayerComponent::EaseInCubic(float X)
{
	return X * X * X;
}

float UOLCAnimationPlayerComponent::EaseInQuad(float X)
{
	return X * X;
}

void UOLCAnimationPlayerComponent::SetupPulseMaterial(EOLCAnimSlot Slot)
{
	ClearPulseMaterial();

	UStaticMeshComponent* Mesh = FindPrimaryStaticMesh();
	if (!Mesh || !Mesh->GetMaterial(PulseMaterialSlot))
	{
		return; // No material to pulse (placeholder content) — transform fallback still runs.
	}

	PulseBaseMaterial = Mesh->GetMaterial(PulseMaterialSlot);
	PulseMID = UMaterialInstanceDynamic::Create(PulseBaseMaterial, this);
	Mesh->SetMaterial(PulseMaterialSlot, PulseMID);

	PulsePhase = 0.0f;
	if (Slot == EOLCAnimSlot::Heal)
	{
		PulseFrequencyHz = 2.0f; // calm green pulse (same 2 Hz cadence as the resource marker)
		PulseMID->SetVectorParameterValue(TEXT("PulseColor"), FLinearColor(0.2f, 1.0f, 0.4f));
	}
	else // Damage
	{
		PulseFrequencyHz = 3.0f; // faster red alarm pulse
		PulseMID->SetVectorParameterValue(TEXT("PulseColor"), FLinearColor(1.0f, 0.15f, 0.1f));
	}
}

void UOLCAnimationPlayerComponent::ClearPulseMaterial()
{
	if (PulseMID && PulseBaseMaterial)
	{
		if (UStaticMeshComponent* Mesh = FindPrimaryStaticMesh())
		{
			Mesh->SetMaterial(PulseMaterialSlot, PulseBaseMaterial);
		}
	}
	PulseMID = nullptr;
	PulseBaseMaterial = nullptr;
}

void UOLCAnimationPlayerComponent::UpdatePulseMaterial(float DeltaTime)
{
	// Same MID pattern as the resource marker pulse: phase -> sine 0..1 -> "Pulse" scalar.
	PulsePhase += DeltaTime * OLCAnim::TAU * PulseFrequencyHz;
	const float PulseValue = (FMath::Sin(PulsePhase) + 1.0f) * 0.5f;
	PulseMID->SetScalarParameterValue(TEXT("Pulse"), PulseValue);
}

void UOLCAnimationPlayerComponent::PlayAbilityAnimation(AActor* Target, const FOLCChampionAbilityAnim& Anim)
{
	if (!Target)
	{
		return;
	}

	// 1) Asset-backed playback.
	if (Anim.AnimationAsset)
	{
		if (const UAnimSequence* Sequence = Cast<UAnimSequence>(Anim.AnimationAsset))
		{
			TArray<USkeletalMeshComponent*> Meshes;
			Target->GetComponents<USkeletalMeshComponent>(Meshes);
			for (USkeletalMeshComponent* Mesh : Meshes)
			{
				if (Mesh && Mesh->IsRegistered())
				{
					Mesh->PlayAnimation(const_cast<UAnimSequence*>(Sequence), Anim.bLoop);
					Mesh->SetPlayRate(Anim.PlayRate);
					return;
				}
			}
		}
		else if (const UMaterialInterface* Material = Cast<UMaterialInterface>(Anim.AnimationAsset))
		{
			TArray<UStaticMeshComponent*> Meshes;
			Target->GetComponents<UStaticMeshComponent>(Meshes);
			if (Meshes.Num() > 0 && Meshes[0])
			{
				Meshes[0]->SetMaterial(0, const_cast<UMaterialInterface*>(Material));
				return;
			}
		}
	}

	// 2) VFX fallback via UOLCVFXSubsystem.
	UGameInstance* GameInstance = Target->GetGameInstance();
	if (!GameInstance)
	{
		return;
	}
	UOLCVFXSubsystem* VFX = GameInstance->GetSubsystem<UOLCVFXSubsystem>();
	if (!VFX)
	{
		return;
	}

	const FVector Location = Target->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);

	// Ability-keyed environmental system (designers can register per-ability
	// systems under the AbilityId key in DA_VFX).
	if (!Anim.AbilityId.IsEmpty())
	{
		if (const UOLCVFXData* VFXData = VFX->GetVFXData())
		{
			for (const FOLCEnvironmentalVFXMapping& Mapping : VFXData->EnvironmentalMappings)
			{
				if (Mapping.EventKey == *Anim.AbilityId && Mapping.NiagaraSystem.IsValid())
				{
					VFX->PlayEnvironmentalVFX(Mapping.EventKey, Location);
					return;
				}
			}
		}
	}

	// Deterministic ability-keyed combat flash. FOLCChampionAbilityAnim carries no
	// VFXTag field (step 2), so the flash is keyed off a stable hash of the
	// AbilityId over the four damage types (see OPEN.md, step-3 note).
	static const EOLCDamageType FlashKeys[] = {
		EOLCDamageType::Kinetic, EOLCDamageType::Energy, EOLCDamageType::Explosive, EOLCDamageType::Void };
	const uint32 Hash = FCrc::StrCrc32(*Anim.AbilityId);
	VFX->PlayCombatVFX(FlashKeys[Hash % UE_ARRAY_COUNT(FlashKeys)], Location);
}
