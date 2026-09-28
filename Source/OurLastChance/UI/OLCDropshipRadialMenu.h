#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCDropshipRadialMenu.generated.h"

class AOLCMenuPlayerController;

/** A radial menu action item. */
USTRUCT()
struct FOLCRadialAction
{
	GENERATED_BODY()

	UPROPERTY()
	FText Label;

	/** Index of the screen to open (EOLCUIScreen enum value). */
	UPROPERTY()
	int32 ScreenIndex = 0;

	/** Whether this action is currently available. */
	UPROPERTY()
	bool bEnabled = true;

	FOLCRadialAction() {}
};

/**
 * Radial menu that appears when clicking the dropship on S11/S12.
 * Circular button layout around click point with actions:
 * Navigate to Planet (S11), Zoom Out to Galaxy (S12), Repair/Customize.
 */
UCLASS()
class OURLASTCHANCE_API UOLCDropshipRadialMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCDropshipRadialMenu(const FObjectInitializer& ObjectInitializer);

	/** Open the radial menu at a specific screen position. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void OpenAtPosition(const FVector2D& ScreenPosition);

	/** Close the radial menu. */
	UFUNCTION(BlueprintCallable, Category = "OLC|UI")
	void CloseMenu();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	/** Build a single radial action button. */
	TSharedRef<SWidget> BuildRadialButton(const FOLCRadialAction& Action, float Angle);

	/** Initialize the default actions from Briefing specs. */
	void InitializeActions();

	TArray<FOLCRadialAction> Actions;

	/** Screen position where menu was opened. */
	FVector2D OpenPosition = FVector2D::ZeroVector;

	UPROPERTY()
	TObjectPtr<AOLCMenuPlayerController> MenuPC;
};
