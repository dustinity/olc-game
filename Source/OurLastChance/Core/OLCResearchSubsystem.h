#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/OLCTechData.h"
#include "Core/OLCResourceTypes.h"
#include "OLCResearchSubsystem.generated.h"

/** Delegate fired when a tech completes research. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechComplete, UOLCTechData*, CompletedTech);

/** Delegate fired when research starts on a new tech. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResearchStarted, UOLCTechData*, NewTech);

/**
 * GameInstanceSubsystem that manages the single-slot research queue.
 * Tracks completed/available/researching techs, advances progress via Tick,
 * and fires completion delegates to unlock effects (build cards, etc.).
 */
UCLASS()
class OURLASTCHANCE_API UOLCResearchSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void Tick(float DeltaTime);

	// -----------------------------------------------------------------------
	// Tech data management
	// -----------------------------------------------------------------------
	/** Register a tech DataAsset so the subsystem knows about it. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	void RegisterTech(UOLCTechData* Tech);

	/** Get all registered techs. */
	const TArray<TObjectPtr<UOLCTechData>>& GetAllTechs() const { return AllTechs; }

	/** Get a tech by display name (case-insensitive). */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	UOLCTechData* FindTechByName(const FString& Name) const;

	// -----------------------------------------------------------------------
	// Research state queries
	// -----------------------------------------------------------------------
	/** Get the currently researching tech (null if idle). */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	UOLCTechData* GetCurrentResearch() const { return CurrentResearch; }

	/** Get research progress as a 0.0–1.0 fraction. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	float GetResearchProgress() const;

	/** Get current progress in seconds. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	float GetCurrentProgressSeconds() const { return CurrentProgressSeconds; }

	/** Get total research time for the current tech. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	float GetTotalResearchTime() const;

	/** Get all completed techs. */
	const TArray<TObjectPtr<UOLCTechData>>& GetCompletedTechs() const { return CompletedTechs; }

	/** Get all available (but not yet started) researchable techs. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	TArray<UOLCTechData*> GetAvailableTechs() const;

	/** Check if a specific tech is completed. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	bool IsTechCompleted(UOLCTechData* Tech) const;

	// -----------------------------------------------------------------------
	// Research actions
	// -----------------------------------------------------------------------
	/** Start researching a tech (if available). Deducts material costs. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	bool StartResearch(UOLCTechData* Tech);

	/** Cancel current research (no refund). */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	void CancelResearch();

	// -----------------------------------------------------------------------
	// Unlock management — bridges research completion to build cards
	// -----------------------------------------------------------------------
	/** Register a tech that unlocks a specific build card by name. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	void RegisterBuildCardUnlock(UOLCTechData* Tech, const FString& BuildCardName);

	/** Check if a build card is unlocked by research completion. */
	UFUNCTION(BlueprintPure, Category = "OLC|Research")
	bool IsBuildCardUnlocked(const FString& BuildCardName) const;

	// -----------------------------------------------------------------------
	// Core ring auto-unlock — call at game start
	// -----------------------------------------------------------------------
	/** Auto-complete all Core ring + bAutoUnlock techs. Called when gameplay begins. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	void AutoCompleteCoreTechs();

	/** Register the starter tech topics (built-in defaults). Call once at game start. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Research")
	void RegisterStarterTechs();

	// -----------------------------------------------------------------------
	// Events (for Blueprint binding)
	// -----------------------------------------------------------------------
	UPROPERTY(BlueprintAssignable, Category = "OLC|Research")
	FOnTechComplete OnTechCompleted;

	UPROPERTY(BlueprintAssignable, Category = "OLC|Research")
	FOnResearchStarted OnResearchStarted;

private:
	/** Try to deduct material costs from resource counters. Returns true if successful. */
	bool DeductMaterialCosts(const TArray<FOLCResourceAmount>& Costs);

	/** Apply the completion effect of a tech (unlock build cards, etc.). */
	void ApplyTechEffect(UOLCTechData* Tech);

	/** Mark a tech complete, apply effects, and broadcast completion. */
	void CompleteResearch(UOLCTechData* Tech);

	TArray<TObjectPtr<UOLCTechData>> AllTechs;
	TArray<TObjectPtr<UOLCTechData>> CompletedTechs;

	UPROPERTY()
	TObjectPtr<UOLCTechData> CurrentResearch;

	float CurrentProgressSeconds = 0.0f;

	/** Maps build card names to the tech that unlocks them. */
	TMap<FString, TObjectPtr<UOLCTechData>> BuildCardUnlocks;

	/** Set of already-unlocked build card names (for fast lookup). */
	TSet<FString> UnlockedBuildCards;
};
