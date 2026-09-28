#include "OLCDropshipRadialMenu.h"

#include "Brushes/SlateDynamicImageBrush.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Player/OLCMenuPlayerController.h"

#include "UI/OLCSharedWidgets.h" // OLCStyleColors

#define LOCTEXT_NAMESPACE "OLCDropshipRadialMenu"

UOLCDropshipRadialMenu::UOLCDropshipRadialMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCDropshipRadialMenu::OpenAtPosition(const FVector2D& ScreenPosition)
{
	OpenPosition = ScreenPosition;
	InitializeActions();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		MenuPC = Cast<AOLCMenuPlayerController>(PC);

	SetAnchorsInViewport(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	SetAlignmentInViewport(FVector2D::ZeroVector);
	SetDesiredSizeInViewport(FVector2D::ZeroVector);
	AddToViewport(300); // Above other screens
}

void UOLCDropshipRadialMenu::CloseMenu()
{
	RemoveFromParent();
}

TSharedRef<SWidget> UOLCDropshipRadialMenu::RebuildWidget()
{
	TSharedRef<SOverlay> Root = SNew(SOverlay);
	TSharedRef<SConstraintCanvas> ButtonLayer = SNew(SConstraintCanvas);

	// Semi-transparent background to close menu on click.
	Root->AddSlot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f))
			.Padding(FMargin(0.0f))
			.OnMouseButtonDown_Lambda([this](const FGeometry&, const FPointerEvent&) -> FReply {
				CloseMenu();
				return FReply::Handled();
			})
		];

	// Radial buttons around center point.
	float Radius = 120.0f; // Distance from center
	for (int32 i = 0; i < Actions.Num(); i++)
	{
		const auto& Action = Actions[i];
		float Angle = (i * 2.0f * PI / Actions.Num()) - PI / 2.0f; // Start from top

		FVector2D ButtonPos(OpenPosition.X + FMath::Cos(Angle) * Radius,
		                     OpenPosition.Y + FMath::Sin(Angle) * Radius);

		ButtonLayer->AddSlot()
			.Offset(FMargin((ButtonPos - FVector2D(60.0f, 30.0f)).X, (ButtonPos - FVector2D(60.0f, 30.0f)).Y, 120.0f, 60.0f))
			[ BuildRadialButton(Action, Angle) ];
	}

	Root->AddSlot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[ ButtonLayer ];

	return Root;
}

TSharedRef<SWidget> UOLCDropshipRadialMenu::BuildRadialButton(const FOLCRadialAction& Action, float Angle)
{
	return SNew(SButton)
		.ButtonStyle(FCoreStyle::Get(), "NoBorder")
		.ContentPadding(FMargin(0.0f))
		.IsEnabled(Action.bEnabled)
		.OnClicked_Lambda([this, Action]() -> FReply {
			if (Action.ScreenIndex >= 0 && MenuPC)
			{
				// Convert int32 back to EOLCUIScreen enum.
				EOLCUIScreen Screen = static_cast<EOLCUIScreen>(Action.ScreenIndex);
				MenuPC->OpenUIScreen(Screen);
			}
			CloseMenu();
			return FReply::Handled();
		})
		[
			SNew(SBorder)
			.BorderBackgroundColor(Action.bEnabled ? OLCStyleColors::PrimaryOrange : OLCStyleColors::GunmetalBlack)
			.Padding(FMargin(16.0f, 10.0f))
			[
				SNew(STextBlock)
				.Text(Action.Label)
				.ColorAndOpacity(Action.bEnabled ? OLCStyleColors::TextWhite : OLCStyleColors::TextDim)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
		];
}

void UOLCDropshipRadialMenu::InitializeActions()
{
	Actions.Reset();

	FOLCRadialAction Action1;
	Action1.Label = LOCTEXT("Rad_Navigate", "NAVIGATE");
	Action1.ScreenIndex = static_cast<int32>(EOLCUIScreen::SolarSystem); // S11
	Action1.bEnabled = true;
	Actions.Add(Action1);

	FOLCRadialAction Action2;
	Action2.Label = LOCTEXT("Rad_Galaxy", "GALAXY MAP");
	Action2.ScreenIndex = static_cast<int32>(EOLCUIScreen::GalaxyMap); // S12
	Action2.bEnabled = true;
	Actions.Add(Action2);

	FOLCRadialAction Action3;
	Action3.Label = LOCTEXT("Rad_Repair", "REPAIR");
	Action3.ScreenIndex = static_cast<int32>(EOLCUIScreen::DropshipRepair);
	Action3.bEnabled = true;
	Actions.Add(Action3);
}

void UOLCDropshipRadialMenu::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

#undef LOCTEXT_NAMESPACE
