// Copyright OLC Project. WP-128 Step 5 — append-only animation auto-attach subsystem.

#include "Animation/OLCAnimationSubsystem.h"
#include "OurLastChance.h"

#include "Animation/OLCAnimationPlayerComponent.h"
#include "Animation/OLCBuildingSequenceComponent.h"
#include "AssetRegistry/ARFilter.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/OLCUnitAnimationData.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "World/OLCBuildingBase.h"
#include "World/OLCUnitBase.h"

namespace OLCAnimSub
{
	/** Folder scanned for UOLCUnitAnimationSet assets (step-1 findings folder convention). */
	static const TCHAR* AnimationFolder = TEXT("/Game/OurLastChance/Animation");
}

void UOLCAnimationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Immediate first pass, then a 0.5 s loop so late-spawned actors attach within ~1 s.
	RescanWorld();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ScanTimerHandle, this, &UOLCAnimationSubsystem::RescanWorld, ScanIntervalSeconds, true);
	}
}

void UOLCAnimationSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ScanTimerHandle);
	}

	CachedSets.Reset();
	ProcessedActors.Reset();

	Super::Deinitialize();
}

bool UOLCAnimationSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UOLCAnimationSubsystem::RescanNow()
{
	RescanWorld();
}

void UOLCAnimationSubsystem::RefreshCachedSets()
{
	CachedSets.Reset();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// UE 5.8: the query API is IAssetRegistry::EnumerateAssets with a FARFilter
	// (the old FARQueryQuery/AssetsMatch pair is no longer part of the public API).
	FARFilter Filter;
	Filter.ClassPaths.Add(UOLCUnitAnimationSet::StaticClass()->GetClassPathName());
	Filter.PackagePaths.Add(FName(OLCAnimSub::AnimationFolder));
	Filter.bRecursivePaths = true;

	AssetRegistry.EnumerateAssets(Filter, [this](const FAssetData& Data)
	{
		if (UOLCUnitAnimationSet* Set = Cast<UOLCUnitAnimationSet>(Data.GetAsset()))
		{
			CachedSets.Add(Set);
		}
		return true; // keep enumerating
	});
}

void UOLCAnimationSubsystem::RescanWorld()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Prune destroyed actors (weak pointers expire automatically).
	for (auto It = ProcessedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	// Refresh the asset cache each scan so newly created/edited sets are picked up within one interval.
	RefreshCachedSets();
	if (CachedSets.Num() == 0)
	{
		return; // No-op case: no animation sets present → nothing to match, no attachments, no errors.
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor))
		{
			continue; // null / unreachable / pending-kill (single check in UE 5.8)
		}
		if (ProcessedActors.Contains(Actor))
		{
			continue; // Exactly-once: already attached.
		}

		UOLCUnitAnimationSet* Match = FindMatchingSet(*Actor);
		if (!Match)
		{
			continue;
		}

		AttachComponentsFor(Actor, Match);
		ProcessedActors.Add(Actor);
	}
}

UOLCUnitAnimationSet* UOLCAnimationSubsystem::FindMatchingSet(const AActor& Actor) const
{
	for (const TObjectPtr<UOLCUnitAnimationSet>& SetPtr : CachedSets)
	{
		UOLCUnitAnimationSet* Set = SetPtr.Get();
		if (!Set)
		{
			continue;
		}

		// 1) Explicit class targeting (strict).
		if (Set->TargetClass && Actor.GetClass()->IsChildOf(Set->TargetClass))
		{
			return Set;
		}

		// 2) UnitTypeId matching.
		if (!Set->UnitTypeId.IsEmpty())
		{
			// Typed unit config id (AOLCUnitBase::UnitData->UnitId).
			if (const AOLCUnitBase* Unit = Cast<AOLCUnitBase>(&Actor))
			{
				const UOLCUnitData* UnitData = Unit->GetUnitData();
				if (UnitData && UnitData->UnitId.Equals(Set->UnitTypeId, ESearchCase::IgnoreCase))
				{
					return Set;
				}
			}

			// Typed building config id (AOLCBuildingBase::GetBuildingData().BuildingId).
			if (const AOLCBuildingBase* Building = Cast<AOLCBuildingBase>(&Actor))
			{
				const FString& BuildingId = Building->GetBuildingData().BuildingId;
				if (!BuildingId.IsEmpty() && BuildingId.Equals(Set->UnitTypeId, ESearchCase::IgnoreCase))
				{
					return Set;
				}
			}

			// Generic fallback: actor class name or actor name contains the unit type id.
			if (Actor.GetClass()->GetName().Contains(Set->UnitTypeId, ESearchCase::IgnoreCase) ||
				Actor.GetName().Contains(Set->UnitTypeId, ESearchCase::IgnoreCase))
			{
				return Set;
			}
		}
	}

	return nullptr;
}

bool UOLCAnimationSubsystem::IsBuildingCategorySet(const UOLCUnitAnimationSet* Set)
{
	if (!Set)
	{
		return false;
	}

	// Explicit building class target.
	if (Set->TargetClass && Set->TargetClass->IsChildOf(AOLCBuildingBase::StaticClass()))
	{
		return true;
	}

	// Canonical building-id convention (PB-*).
	if (Set->UnitTypeId.StartsWith(TEXT("PB-"), ESearchCase::IgnoreCase))
	{
		return true;
	}

	// Building-only animation slots.
	for (const FOLCAnimationEntry& Entry : Set->Entries)
	{
		if (Entry.Slot == EOLCAnimSlot::Assemble || Entry.Slot == EOLCAnimSlot::Collapse)
		{
			return true;
		}
	}

	return false;
}

void UOLCAnimationSubsystem::AttachComponentsFor(AActor* Actor, UOLCUnitAnimationSet* Set)
{
	if (!Actor || !Set)
	{
		return;
	}

	// Animation player: reuse an existing one (idempotent), otherwise create + register.
	UOLCAnimationPlayerComponent* Player = Actor->FindComponentByClass<UOLCAnimationPlayerComponent>();
	if (!Player)
	{
		Player = NewObject<UOLCAnimationPlayerComponent>(Actor);
		Player->RegisterComponent();
	}
	Player->AnimationSet = Set;

	// Building-category sets additionally get the assembly/collapse sequence component.
	const bool bBuilding = IsBuildingCategorySet(Set);
	if (bBuilding)
	{
		UOLCBuildingSequenceComponent* Sequence = Actor->FindComponentByClass<UOLCBuildingSequenceComponent>();
		if (!Sequence)
		{
			Sequence = NewObject<UOLCBuildingSequenceComponent>(Actor);
			Sequence->RegisterComponent();
		}
	}

	UE_LOG(LogOLC, Log, TEXT("[OLC] AnimationSubsystem: attached animation components to %s (set=%s%s)"),
		*Actor->GetName(), *Set->GetName(), bBuilding ? TEXT(" +building-sequence") : TEXT(""));
}
