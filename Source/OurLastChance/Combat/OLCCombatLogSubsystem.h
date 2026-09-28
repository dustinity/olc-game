#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Combat/OLCCombatTypes.h"
#include "OLCCombatLogSubsystem.generated.h"

UCLASS()
class OURLASTCHANCE_API UOLCCombatLogSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void AddCombatLogEntry(const FText& Message, EOLCCombatLogType Type);

	UFUNCTION(BlueprintCallable, Category = "OLC|Combat")
	void ClearCombatLog();

	UFUNCTION(BlueprintPure, Category = "OLC|Combat")
	const TArray<FOLCCombatLogEntry>& GetCombatLogEntries() const { return Entries; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Combat")
	int32 MaxEntries = 100;

private:
	UPROPERTY()
	TArray<FOLCCombatLogEntry> Entries;
};
