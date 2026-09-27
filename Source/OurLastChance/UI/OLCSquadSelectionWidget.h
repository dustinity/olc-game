#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "World/OLCUnitBase.h" // For AOLCUnitBase forward ref
#include "OLCSharedWidgets.h" // OLCStyleColors
#include "OLCSquadSelectionWidget.generated.h"

class AOLCUnitBase;

/** Delegate fired when squad is ready to deploy. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSquadReady);

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

	/** Remove unit from squad slot. */
	void RemoveFromSquad(int32 SlotIndex);

	/** Get total HP of squad. */
	float GetTotalHP() const;

	/** Get total damage of squad. */
	float GetTotalDamage() const;

	/** Get count of units in squad. */
	int32 GetSquadCount() const { return SquadSlots.Num(); }

	/** Whether squad has at least one unit (champion is auto-included). */
	bool bHasUnits = false;

	/** Available units from the game world (filtered for deployment). */
	TArray<AOLCUnitBase*> AvailableUnits;

	/** Squad slots — up to 16 units. First slot always champion. */
	TArray<AOLCUnitBase*> SquadSlots;

	/** Reference to available unit list for drag source. */
	AOLCUnitBase* DragSource = nullptr;

	/** Cached canvas size for layout. */
	FVector2D CanvasSize = FVector2D::ZeroVector;
};
