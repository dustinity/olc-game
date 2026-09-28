#include "Combat/OLCMinigameManager.h"
#include "OurLastChance.h"

#include "Logging/LogMacros.h"

void UOLCMinigameManager::StartMinigame(EOLCMinigameType Type, float DurationSeconds)
{
	CurrentType = Type;
	bActive = true;
	Score = 0;
	Progress = 0.0f;
	SequenceIndex = 0;
	RequiredSequence.Reset();

	switch (Type)
	{
		case EOLCMinigameType::WireRepair: RequiredSequence = { TEXT("R"), TEXT("G"), TEXT("B"), TEXT("Y") }; break;
		case EOLCMinigameType::AsteroidEvasion: RequiredSequence = { TEXT("CLICK"), TEXT("CLICK"), TEXT("CLICK") }; break;
		case EOLCMinigameType::SignalDecoding: RequiredSequence = { TEXT("A"), TEXT("B"), TEXT("A"), TEXT("C") }; break;
		case EOLCMinigameType::WarpNavigation: RequiredSequence = { TEXT("LEFT"), TEXT("RIGHT"), TEXT("UP"), TEXT("DOWN") }; break;
	}
}

void UOLCMinigameManager::RegisterInput(const FString& InputToken)
{
	if (!bActive || RequiredSequence.Num() == 0)
	{
		return;
	}

	if (RequiredSequence.IsValidIndex(SequenceIndex) && RequiredSequence[SequenceIndex].Equals(InputToken, ESearchCase::IgnoreCase))
	{
		Score += 25;
		SequenceIndex++;
		Progress = static_cast<float>(SequenceIndex) / static_cast<float>(RequiredSequence.Num());
	}
	else
	{
		Score = FMath::Max(0, Score - 10);
	}
}

bool UOLCMinigameManager::CompleteMinigame()
{
	const bool bSuccess = bActive && Progress >= 1.0f;
	bActive = false;
	return bSuccess;
}

void UOLCMinigameManager::StartMinigameFromData(const UOLCMinigameData* InData, EOLCMinigameType InType)
{
	const FOLCMinigameDefinition* Def = InData ? InData->FindData(InType) : nullptr;
	if (!Def || Def->InputSequence.Num() == 0)
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Minigame start from data rejected — no row/sequence for type %d"),
			static_cast<int32>(InType));
		return;
	}

	// Same configuration as the legacy StartMinigame, sourced from the DataAsset row.
	CurrentType = Def->MinigameType;
	bActive = true;
	Score = 0;
	Progress = 0.0f;
	SequenceIndex = 0;
	RequiredSequence.Reset();
	for (const FString& Token : Def->InputSequence)
	{
		RequiredSequence.Add(Token);
	}

	UE_LOG(LogOLC, Display, TEXT("[OLC] Minigame started from data: %s (%d tokens, %.0fs limit)"),
		*Def->DisplayName, Def->InputSequence.Num(), Def->DurationSeconds);
}
