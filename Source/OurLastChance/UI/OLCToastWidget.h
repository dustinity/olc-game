#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/OLCResourceTypes.h"
#include "OLCToastWidget.generated.h"

// ---------------------------------------------------------------------------
// Toast notification data
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOLCToastData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Toast")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Toast")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Toast")
	EOLCColorRole ColorRole = EOLCColorRole::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OLC|Toast")
	float DisplayDuration = 3.0f;

	FOLCToastData() {}

	FOLCToastData(const FText& InTitle, const FText& InMessage, EOLCColorRole InColor = EOLCColorRole::Default)
		: Title(InTitle), Message(InMessage), ColorRole(InColor), DisplayDuration(3.0f) {}
};

// ---------------------------------------------------------------------------
// WBP_UI_Toast — Single toast notification widget
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCToastWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCToastWidget(const FObjectInitializer& ObjectInitializer);

	/** Set the toast data and start fade-in animation. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Toast")
	void ShowToast(const FOLCToastData& ToastData);

	/** Hide the toast with fade-out animation. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Toast")
	void HideToast();

	/** Get whether this toast is currently visible. */
	bool IsVisible() const { return bIsVisible; }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;

private:
	TSharedRef<SWidget> BuildToastContent();

	/** Current toast data being displayed. */
	FOLCToastData CurrentToast;

	/** Whether the toast is currently visible (fade-in complete). */
	UPROPERTY()
	bool bIsVisible = false;

	/** Time elapsed since toast started displaying. */
	float ElapsedTime = 0.0f;

	/** Fade-in progress (0.0 to 1.0). */
	float FadeInProgress = 0.0f;

	/** Fade-out progress (0.0 to 1.0). */
	float FadeOutProgress = 0.0f;

	/** Whether the toast is in fade-out phase. */
	bool bIsFadingOut = false;

	/** Opacity multiplier for rendering. */
	float CurrentOpacity = 0.0f;

	static constexpr float FadeInDuration = 0.2f;
	static constexpr float FadeOutDuration = 0.3f;
};

// ---------------------------------------------------------------------------
// WBP_UI_ToastStack — Container for stacked toast notifications
// ---------------------------------------------------------------------------
UCLASS()
class OURLASTCHANCE_API UOLCToastStackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCToastStackWidget(const FObjectInitializer& ObjectInitializer);

	/** Queue a new toast notification. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Toast")
	void QueueToast(const FOLCToastData& ToastData);

	/** Clear all queued and visible toasts. */
	UFUNCTION(BlueprintCallable, Category = "OLC|Toast")
	void ClearAllToasts();

	/** Get the number of toasts currently in the queue. */
	UFUNCTION(BlueprintPure, Category = "OLC|Toast")
	int32 GetQueueCount() const { return ToastQueue.Num(); }

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;

private:
	TSharedRef<SWidget> BuildToastList();

	/** Process the next toast in the queue. */
	void ProcessNextToast();

	/** Remove the currently displayed toast and move to next. */
	void RemoveCurrentToast();

	/** Maximum number of visible toasts simultaneously. */
	static constexpr int32 MaxVisibleToasts = 3;

	/** Queue of pending toast notifications. */
	TArray<FOLCToastData> ToastQueue;

	/** Currently displayed toasts (up to MaxVisibleToasts). */
	TArray<FOLCToastData> ActiveToasts;

	/** Timer for tick updates. */
	float TickTimer = 0.0f;

	/** Fade-in duration in seconds. */
	static constexpr float FadeInDuration = 0.2f;

	/** Hold duration before fade-out. */
	static constexpr float HoldDuration = 3.0f;

	/** Fade-out duration in seconds. */
	static constexpr float FadeOutDuration = 0.3f;
};
