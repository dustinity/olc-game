#include "Combat/OLCCombatLogSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UOLCCombatLogSubsystem::AddCombatLogEntry(const FText& Message, EOLCCombatLogType Type)
{
	FOLCCombatLogEntry Entry;
	Entry.Message = Message;
	Entry.Type = Type;
	Entry.TimeSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	Entries.Add(Entry);
	if (Entries.Num() > MaxEntries)
	{
		Entries.RemoveAt(0, Entries.Num() - MaxEntries);
	}
}

void UOLCCombatLogSubsystem::ClearCombatLog()
{
	Entries.Reset();
}
