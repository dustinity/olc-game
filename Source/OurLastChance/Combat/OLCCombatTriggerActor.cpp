#include "Combat/OLCCombatTriggerActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "UI/OLCCombatResultsWidget.h"
#include "UI/OLCTacticalCombatWidget.h"

AOLCCombatTriggerActor::AOLCCombatTriggerActor()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(320.0f, 320.0f, 180.0f));
	PrimaryActorTick.bCanEverTick = false;
}

void AOLCCombatTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AOLCCombatTriggerActor::OnTriggerOverlap);
	if (!TacticalCombatWidgetClass)
	{
		TacticalCombatWidgetClass = UOLCTacticalCombatWidget::StaticClass();
	}
	if (!CombatResultsWidgetClass)
	{
		CombatResultsWidgetClass = UOLCCombatResultsWidget::StaticClass();
	}
}

void AOLCCombatTriggerActor::OnTriggerOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (bTriggered || !OtherActor || !OtherActor->IsA<APawn>() || !GetWorld())
	{
		return;
	}

	bTriggered = true;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ActiveCombatWidget = CreateWidget<UOLCTacticalCombatWidget>(PC, TacticalCombatWidgetClass);
	if (ActiveCombatWidget)
	{
		ActiveCombatWidget->OnCombatEnded.AddDynamic(this, &AOLCCombatTriggerActor::OnCombatEnded);
		ActiveCombatWidget->AddToViewport(300);
	}
}

void AOLCCombatTriggerActor::OnCombatEnded(bool bVictory)
{
	if (ActiveCombatWidget)
	{
		ActiveCombatWidget->RemoveFromParent();
		ActiveCombatWidget = nullptr;
	}

	if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		if (UOLCCombatResultsWidget* Results = CreateWidget<UOLCCombatResultsWidget>(PC, CombatResultsWidgetClass))
		{
			Results->AddToViewport(300);
		}
	}
}
