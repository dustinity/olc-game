#pragma once

#include "CoreMinimal.h"
#include "Core/OLCTerrainTypes.h"
#include "GameFramework/GameModeBase.h"
#include "OLCMenuGameMode.generated.h"

class UOLCFactionData;
class UOLCCrashSequenceWidget;

// ---------------------------------------------------------------------------
// Game state enum
// ---------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOLCGameState : uint8
{
	Menu            UMETA(DisplayName = "Menu"),
	CrashSequence   UMETA(DisplayName = "Crash Sequence"),
	Gameplay        UMETA(DisplayName = "Gameplay"),
};

/**
 * Game mode that manages the opening experience:
 * Welcome screen → crash animation sequence → gameplay transition.
 */
UCLASS()
class OURLASTCHANCE_API AOLCMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOLCMenuGameMode();

	/** Get current game state. */
	UFUNCTION(BlueprintPure, Category = "OLC|GameState")
	EOLCGameState GetCurrentGameState() const { return CurrentGameState; }

	/** Start the crash sequence after welcome screen. Called from menu UI. */
	UFUNCTION(BlueprintCallable, Category = "OLC|GameState")
	void StartCrashSequence();

	/** Complete the crash sequence and transition to gameplay. */
	UFUNCTION(BlueprintCallable, Category = "OLC|GameState")
	void TransitionToGameplay();

	/** Get current phase of the crash animation. */
	UFUNCTION(BlueprintPure, Category = "OLC|GameState")
	int32 GetCurrentCrashPhase() const { return CurrentCrashPhase; }

protected:
	virtual void BeginPlay() override;

private:
	/** Advance to the next phase in the crash animation sequence. */
	void AdvanceCrashPhase();

	/** Spawn the gameplay world and initialize terrain after crash sequence. */
	void SpawnGameplayWorld();

	UOLCFactionData* LoadFactionDataById(const FString& FactionId) const;

	UPROPERTY(EditAnywhere, Category = "OLC|Gameplay")
	EOLCBiomeType GameplayBiome = EOLCBiomeType::Desert;

	/** Persisted campaign selection state from WP-100 / WP-101. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "OLC|Campaign")
	FString SelectedFactionId;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "OLC|Campaign")
	FString SelectedChampionId;

	UPROPERTY(EditAnywhere, Category = "OLC|Gameplay")
	int32 GameplaySeed = 184736;

public:
	UFUNCTION(BlueprintCallable, Category = "OLC|Campaign")
	void SetSelectedFactionId(const FString& InFactionId);

	UFUNCTION(BlueprintCallable, Category = "OLC|Campaign")
	FString GetSelectedFactionId() const;

	UFUNCTION(BlueprintCallable, Category = "OLC|Campaign")
	void SetSelectedChampionId(const FString& InChampionId);

	UFUNCTION(BlueprintCallable, Category = "OLC|Campaign")
	FString GetSelectedChampionId() const;

private:
	/** Timer callback for each crash animation phase duration. */
	void OnCrashPhaseTimer();

	void SetupSequenceCamera();
	void SetupRTSCamera();
	void StartPhaseTimer();

	/** WP-109: Switch player controller from Menu PC to Gameplay PC after crash sequence. */
	void TransitionPlayerControllerToGameplay();

	UPROPERTY(EditAnywhere, Category = "OLC|GameState")
	EOLCGameState CurrentGameState = EOLCGameState::Menu;

	/** Which phase of the crash animation is currently playing (0-4). */
	UPROPERTY(EditAnywhere, Category = "OLC|GameState")
	int32 CurrentCrashPhase = 0;

	FTimerHandle CrashPhaseTimer;

	/** Duration in seconds for each crash animation phase. */
	UPROPERTY(EditAnywhere, Category = "OLC|GameState")
	TArray<float> CrashPhaseDurations;

	/** Camera actor used during the crash sequence (side-view). */
	UPROPERTY()
	TObjectPtr<ACameraActor> SequenceCamera;

	UPROPERTY()
	TObjectPtr<UOLCCrashSequenceWidget> CrashSequenceWidget;

	/** Whether the welcome screen has been shown at least once. */
	UPROPERTY()
	bool bWelcomeShown = false;
};
