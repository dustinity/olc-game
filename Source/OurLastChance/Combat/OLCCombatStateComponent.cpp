#include "Combat/OLCCombatStateComponent.h"

UOLCCombatStateComponent::UOLCCombatStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOLCCombatStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bPaused)
	{
		PhaseTime += DeltaTime;
	}
}

void UOLCCombatStateComponent::StartCombat()
{
	bPaused = false;
	SetPhase(EOLCCombatPhase::Engage);
}

void UOLCCombatStateComponent::AdvancePhase()
{
	switch (CurrentPhase)
	{
		case EOLCCombatPhase::Engage: SetPhase(EOLCCombatPhase::ExchangeFire); break;
		case EOLCCombatPhase::ExchangeFire: SetPhase(EOLCCombatPhase::Tactical); break;
		case EOLCCombatPhase::Tactical: SetPhase(EOLCCombatPhase::Resolution); break;
		case EOLCCombatPhase::Resolution: SetPhase(EOLCCombatPhase::Engage); break;
	}
}

void UOLCCombatStateComponent::SetPaused(bool bNewPaused)
{
	bPaused = bNewPaused;
}

void UOLCCombatStateComponent::SetPhase(EOLCCombatPhase NewPhase)
{
	const EOLCCombatPhase Previous = CurrentPhase;
	CurrentPhase = NewPhase;
	PhaseTime = 0.0f;
	OnPhaseChanged.Broadcast(Previous, CurrentPhase);
}
