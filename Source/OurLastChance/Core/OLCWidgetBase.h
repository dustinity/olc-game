#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCWidgetBase.generated.h"

/**
 * Base widget class for all OLC UI widgets.
 * Provides ApplyViewData, focus hooks, and animation preference.
 */
UCLASS()
class OURLASTCHANCE_API UOLCWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCWidgetBase(const FObjectInitializer& ObjectInitializer);

	/** Called to apply new view data from the subsystem. Override in Blueprint or C++. */
	UFUNCTION(BlueprintNativeEvent, Category = "OLC|UI")
	void ApplyViewData();

	/** Called when the widget receives focus. */
	UFUNCTION(BlueprintNativeEvent, Category = "OLC|UI")
	void OnWidgetFocusReceived();

	/** Called when the widget loses focus. */
	UFUNCTION(BlueprintNativeEvent, Category = "OLC|UI")
	void OnWidgetFocusLost();

	/** Get the owning player controller. */
	APlayerController* GetOwningPC() const;

protected:
	// Note: UUserWidget::AddToViewport/RemoveFromViewport are not virtual.
	// These are convenience wrappers — no override keyword.
	void AddToViewportSafe(int32 InZOrder);
	void RemoveFromViewportSafe();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI")
	bool bUseAnimations = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor PrimaryOrange = FLinearColor(0.91f, 0.52f, 0.16f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor HoverOrange = FLinearColor(0.96f, 0.62f, 0.25f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor TacticalBlue = FLinearColor(0.23f, 0.51f, 0.96f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor ValidGreen = FLinearColor(0.13f, 0.77f, 0.37f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor DangerRed = FLinearColor(0.94f, 0.27f, 0.27f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor GunmetalBlack = FLinearColor(0.10f, 0.11f, 0.13f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor DarkSteel = FLinearColor(0.14f, 0.16f, 0.19f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor CharcoalGray = FLinearColor(0.18f, 0.20f, 0.23f, 1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OLC|UI|Style")
	FLinearColor BorderGray = FLinearColor(0.24f, 0.26f, 0.30f, 1.0f);
};
