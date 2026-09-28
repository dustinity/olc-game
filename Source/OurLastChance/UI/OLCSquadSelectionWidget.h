#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/DragAndDrop.h"
#include "Widgets/Views/SListView.h"
#include "World/OLCUnitBase.h" // For AOLCUnitBase forward ref
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "Core/OLCUIDataSubsystem.h" // FOLCSquadDeploymentData
#include "Core/OLCRaceFamily.h" // EOLCRaceFamily
#include "OLCSquadSelectionWidget.generated.h"

class AOLCUnitBase;
class AOLCProductionFacility;

/** Delegate fired when squad is ready to deploy. Carries the built deployment payload. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadReady, const FOLCSquadDeploymentData&, SquadData);

/**
 * Drag-and-drop operator that carries an AOLCUnitBase pointer and the source
 * slot index from the roster or an existing squad slot. Subclasses FDragDropOperator
 * so it can be returned from OnDragDetected and inspected in OnDragDropped.
 */
class ODragDrop : public FDragDropOperation
{
public:
	DRAG_DROP_OPERATOR_TYPE(ODragDrop, FDragDropOperation)

	static TSharedRef<ODragDrop> New(AOLCUnitBase* InUnit, int32 InSourceSlot)
	{
		TSharedRef<ODragDrop> Operation = MakeShared<ODragDrop>();
		Operation->DraggedUnit = InUnit;
		Operation->SourceSlotIndex = InSourceSlot;
		Operation->Construct();
		return Operation;
	}

	/** Set the unit that is being dragged. */
	void SetDragSource(AOLCUnitBase* InUnit)
	{
		DraggedUnit = InUnit;
	}

	/** Get the unit that was dragged. */
	AOLCUnitBase* GetDragSource() const
	{
		return DraggedUnit;
	}

	/** Set the source slot index (-1 if from roster). */
	void SetSourceSlot(int32 InIndex)
	{
		SourceSlotIndex = InIndex;
	}

	/** Get the source slot index (-1 if from roster). */
	int32 GetSourceSlot() const
	{
		return SourceSlotIndex;
	}

protected:
	/** The unit being dragged from the roster or a squad slot. */
	AOLCUnitBase* DraggedUnit = nullptr;

	/** The source slot index (-1 if dragged from the roster). */
	int32 SourceSlotIndex = -1;
};

/**
 * S09 Squad Selection screen — unit roster grid, drag-to-squad slots (max 16),
 * champion auto-included, total stats summary.
 */
UCLASS()
class OURLASTCHANCE_API UOLCSquadSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCSquadSelectionWidget(const FObjectInitializer& ObjectInitializer);

	/** Fired when player clicks Ready and squad has units. */
	UPROPERTY(BlueprintAssignable, Category = "OLC|Squad")
	FOnSquadReady OnSquadReady;

	/**
	 * Race family the squad is being deployed against, if known (e.g. set from the
	 * expedition/dungeon-entry flow before this screen opens). Unknown by default —
	 * no upstream screen currently feeds this, so the "Effective vs Target Race" stat
	 * line only appears once a caller sets it.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "OLC|Squad")
	EOLCRaceFamily TargetRaceFamily = EOLCRaceFamily::Unknown;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/** Build the top bar with title and back button. */
	TSharedRef<SWidget> BuildTopBar();

	/** Build the unit roster grid (left panel). */
	TSharedRef<SWidget> BuildUnitRoster();

	/** Build a single roster entry for an available unit. */
	TSharedRef<SWidget> BuildRosterEntry(AOLCUnitBase* Unit);

	/** Build the squad slots area (center panel). */
	TSharedRef<SWidget> BuildSquadSlots();

	/** Build a single squad slot. */
	TSharedRef<SWidget> BuildSquadSlot(int32 Index, AOLCUnitBase* UnitInSlot);

	/** Build the stats summary panel (right side). */
	TSharedRef<SWidget> BuildStatsSummary();

	/** Refresh roster display from available units. */
	void RefreshRoster();

	/** Add unit to first empty squad slot. */
	void AddToSquad(AOLCUnitBase* Unit);

	/** Move a unit from one slot to another (reordering). */
	void MoveUnitToSlot(int32 FromIndex, int32 ToIndex);

	/** Ensure the champion unit is always placed in slot 1 when available. */
	void EnsureChampionSlot();

	/** Remove unit from squad slot. */
	void RemoveFromSquad(int32 SlotIndex);

	/** Get total HP of squad. */
	float GetTotalHP() const;

	/** Get total damage of squad. */
	float GetTotalDamage() const;

	/** Get total speed of squad. */
	float GetTotalSpeed() const;

	/** Get count of units in squad. */
	int32 GetSquadCount() const { return SquadSlots.Num(); }

	/** Count squad members of a given unit type, for the stats-summary breakdown. */
	int32 GetUnitTypeCount(EOLCUnitType Type) const;

	/** Whether a drop of the dragged unit onto TargetSlot would be accepted (used for drag-hover highlighting). */
	bool IsValidDropTarget(AOLCUnitBase* DraggedUnit, int32 SourceSlot, int32 TargetSlot) const;

	/** Build the deployment payload (unit ids, live actor refs, champion slot) from the current SquadSlots. */
	FOLCSquadDeploymentData BuildDeploymentPayload() const;

	/** Whether squad has at least one unit (champion is auto-included). */
	bool bHasUnits = false;

	/** Available units from the game world (filtered for deployment). */
	TArray<AOLCUnitBase*> AvailableUnits;

	/** Squad slots — up to 16 units. Slot 0 is always the locked champion. */
	TArray<AOLCUnitBase*> SquadSlots;

	/** The champion unit locked in slot 0 (auto-placed by EnsureChampionSlot). */
	AOLCUnitBase* ChampionUnit = nullptr;

	/** Reference to available unit list for drag source. */
	AOLCUnitBase* DragSource = nullptr;

	/** Cached canvas size for layout. */
	FVector2D CanvasSize = FVector2D::ZeroVector;

	/**
	 * Snapshot of production facilities' in-progress queue entries, refreshed alongside
	 * RefreshRoster() so the roster can show grayed-out "In Production" rows for units
	 * that aren't AOLCUnitBase actors yet.
	 */
	TArray<AOLCProductionFacility*> ProductionFacilities;
};
