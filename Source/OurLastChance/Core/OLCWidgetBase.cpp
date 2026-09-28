#include "OLCWidgetBase.h"

UOLCWidgetBase::UOLCWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCWidgetBase::ApplyViewData_Implementation()
{
	// Default no-op; override in subclasses or Blueprint.
}

void UOLCWidgetBase::OnWidgetFocusReceived_Implementation()
{
}

void UOLCWidgetBase::OnWidgetFocusLost_Implementation()
{
}

APlayerController* UOLCWidgetBase::GetOwningPC() const
{
	return GetOwningPlayer();
}

void UOLCWidgetBase::AddToViewportSafe(int32 InZOrder)
{
	AddToViewport(InZOrder);
	if (IsValid(this))
	{
		OnWidgetFocusReceived();
	}
}

void UOLCWidgetBase::RemoveFromViewportSafe()
{
	if (IsValid(this))
	{
		OnWidgetFocusLost();
	}
	RemoveFromViewport();
}
