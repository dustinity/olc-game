#include "OLCToastWidget.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "OLCToastWidget"

// ---------------------------------------------------------------------------
// Style helpers
// ---------------------------------------------------------------------------
namespace ToastStyle
{
	FLinearColor GetToastColor(EOLCColorRole Role)
	{
		switch (Role)
		{
			case EOLCColorRole::Primary:   return FLinearColor(0.910f, 0.522f, 0.165f, 1.0f); // orange
			case EOLCColorRole::Secondary: return FLinearColor(0.231f, 0.510f, 0.965f, 1.0f); // blue
			case EOLCColorRole::Success:   return FLinearColor(0.133f, 0.773f, 0.369f, 1.0f); // green
			case EOLCColorRole::Danger:    return FLinearColor(0.945f, 0.267f, 0.267f, 1.0f); // red
			case EOLCColorRole::Warning:   return FLinearColor(0.957f, 0.839f, 0.196f, 1.0f); // yellow
			default:                       return FLinearColor(0.920f, 0.950f, 0.980f, 1.0f); // white
		}
	}

	FLinearColor GetToastBG(EOLCColorRole Role)
	{
		switch (Role)
		{
			case EOLCColorRole::Primary:   return FLinearColor(0.910f, 0.522f, 0.165f, 0.15f);
			case EOLCColorRole::Secondary: return FLinearColor(0.231f, 0.510f, 0.965f, 0.15f);
			case EOLCColorRole::Success:   return FLinearColor(0.133f, 0.773f, 0.369f, 0.15f);
			case EOLCColorRole::Danger:    return FLinearColor(0.945f, 0.267f, 0.267f, 0.15f);
			case EOLCColorRole::Warning:   return FLinearColor(0.957f, 0.839f, 0.196f, 0.15f);
			default:                       return FLinearColor(0.580f, 0.620f, 0.680f, 0.15f);
		}
	}
}

// ===================================================================
// WBP_UI_Toast — Single toast notification
// ===================================================================
UOLCToastWidget::UOLCToastWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCToastWidget::ShowToast(const FOLCToastData& ToastData)
{
	CurrentToast = ToastData;
	bIsVisible = true;
	ElapsedTime = 0.0f;
	FadeInProgress = 0.0f;
	FadeOutProgress = 0.0f;
	bIsFadingOut = false;
	CurrentOpacity = 0.0f;
}

void UOLCToastWidget::HideToast()
{
	bIsVisible = false;
	CurrentOpacity = 0.0f;
}

TSharedRef<SWidget> UOLCToastWidget::RebuildWidget()
{
	return BuildToastContent();
}

void UOLCToastWidget::NativeTick(const FGeometry& InGeometry, float InDeltaTime)
{
	Super::NativeTick(InGeometry, InDeltaTime);

	if (!bIsVisible) return;

	ElapsedTime += InDeltaTime;

	if (bIsFadingOut)
	{
		// Fade-out phase: 0.3s
		FadeOutProgress = FMath::Clamp(ElapsedTime / FadeOutDuration, 0.0f, 1.0f);
		CurrentOpacity = 1.0f - FadeOutProgress;
		if (FadeOutProgress >= 1.0f)
		{
			bIsVisible = false;
			CurrentOpacity = 0.0f;
		}
	}
	else if (ElapsedTime < FadeInDuration)
	{
		// Fade-in phase: 0.2s
		FadeInProgress = ElapsedTime / FadeInDuration;
		CurrentOpacity = FadeInProgress;
	}
	else if (ElapsedTime < FadeInDuration + CurrentToast.DisplayDuration)
	{
		// Hold phase: 3s
		CurrentOpacity = 1.0f;
	}
	else
	{
		// Start fade-out
		bIsFadingOut = true;
		ElapsedTime = 0.0f;
	}
}

TSharedRef<SWidget> UOLCToastWidget::BuildToastContent()
{
	if (!bIsVisible || CurrentOpacity <= 0.0f)
	{
		return SNew(SBox);
	}

	FLinearColor AccentColor = ToastStyle::GetToastColor(CurrentToast.ColorRole);
	FLinearColor BGColor = ToastStyle::GetToastBG(CurrentToast.ColorRole);

	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.02f, 0.035f, 0.04f, 0.95f))
		.Padding(FMargin(12.0f, 8.0f))
		[
			SNew(SHorizontalBox)
			// Accent bar on left
			+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 6.0f, 0.0f)
			[
				SNew(SBox)
				.WidthOverride(3.0f)
				.HeightOverride(FMath::Max(40.0f, CurrentToast.Message.IsEmpty() ? 20.0f : 40.0f))
				[
					SNew(SBorder)
					.BorderBackgroundColor(AccentColor)
					.Padding(FMargin(0.0f))
				]
			]
			// Title + message
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(STextBlock)
					.Text(CurrentToast.Title)
					.ColorAndOpacity(AccentColor)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(2.0f, 2.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(CurrentToast.Message)
					.ColorAndOpacity(FLinearColor(0.75f, 0.78f, 0.82f, 1.0f))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
				]
			]
		];
}

// ===================================================================
// WBP_UI_ToastStack — Container for stacked toasts
// ===================================================================
UOLCToastStackWidget::UOLCToastStackWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCToastStackWidget::QueueToast(const FOLCToastData& ToastData)
{
	ToastQueue.Add(ToastData);
	UE_LOG(LogTemp, Log, TEXT("[OLC] Toast queued: %s — %s"), *ToastData.Title.ToString(), *ToastData.Message.ToString());
}

void UOLCToastStackWidget::ClearAllToasts()
{
	ToastQueue.Reset();
	ActiveToasts.Reset();
}

TSharedRef<SWidget> UOLCToastStackWidget::RebuildWidget()
{
	return BuildToastList();
}

void UOLCToastStackWidget::NativeTick(const FGeometry& InGeometry, float InDeltaTime)
{
	Super::NativeTick(InGeometry, InDeltaTime);

	TickTimer += InDeltaTime;

	// Process queue every 0.5s
	if (TickTimer >= 0.5f)
	{
		TickTimer = 0.0f;
		ProcessNextToast();
	}

	// Check for toasts that need removal (fade-out complete)
	TArray<int32> ToRemove;
	for (int32 i = 0; i < ActiveToasts.Num(); i++)
	{
		// Simple heuristic: if toast has been active longer than hold + fade, mark for removal
		// In a full implementation, each toast would track its own state
	}
	if (!ToRemove.IsEmpty())
	{
		for (int32 i = ToRemove.Num() - 1; i >= 0; i--)
		{
			ActiveToasts.RemoveAt(ToRemove[i]);
		}
	}
}

void UOLCToastStackWidget::ProcessNextToast()
{
	if (ActiveToasts.Num() < MaxVisibleToasts && !ToastQueue.IsEmpty())
	{
		ActiveToasts.Add(ToastQueue[0]);
		ToastQueue.RemoveAt(0);
	}
}

TSharedRef<SWidget> UOLCToastStackWidget::BuildToastList()
{
	TSharedRef<SScrollBox> ToastScroll = SNew(SScrollBox);

	for (const auto& Toast : ActiveToasts)
	{
		FLinearColor AccentColor = ToastStyle::GetToastColor(Toast.ColorRole);

		ToastScroll->AddSlot()
			.Padding(FMargin(0.0f, 8.0f, 0.0f, 0.0f))
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.02f, 0.035f, 0.04f, 0.95f))
				.Padding(FMargin(12.0f, 8.0f))
				[
					SNew(SHorizontalBox)
					// Accent bar
					+ SHorizontalBox::Slot().AutoWidth().Padding(0.0f, 0.0f, 8.0f, 0.0f)
					[
						SNew(SBox)
						.WidthOverride(3.0f)
						.HeightOverride(40.0f)
						[
							SNew(SBorder)
							.BorderBackgroundColor(AccentColor)
							.Padding(FMargin(0.0f))
						]
					]
					// Content
					+ SHorizontalBox::Slot().FillWidth(1.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight()
						[
							SNew(STextBlock)
							.Text(Toast.Title)
							.ColorAndOpacity(AccentColor)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(2.0f, 2.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(Toast.Message)
							.ColorAndOpacity(FLinearColor(0.75f, 0.78f, 0.82f, 1.0f))
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
						]
					]
				]
			];
	}

	return SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Right)
		.Padding(18.0f, 70.0f, 18.0f, 0.0f) // Top-right, below resource strip
		[
			SNew(SBox)
			.WidthOverride(320.0f)
			.HeightOverride(200.0f) // Max height for 3 toasts
			[ ToastScroll ]
		];
}

#undef LOCTEXT_NAMESPACE
