/**
@author		Shun Moriya



ベースクラス
*/

#include "MainWidgetBase.h"
#include <Components/WidgetSwitcher.h>

UMainWidgetBase::UMainWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UMainWidgetBase::ClampPage(UWidgetSwitcher* widgetSwitcher, const int32 page, const bool loop)
{
	check(IsValid(widgetSwitcher));
	if (loop)
	{
		return page % widgetSwitcher->GetNumWidgets();
	}
	else
	{
		return std::max(0, std::min(page, widgetSwitcher->GetNumWidgets()));
	}
}

void UMainWidgetBase::ChangePage(UWidgetSwitcher* widgetSwitcher, const int32 page, const bool loop)
{
	if (IsValid(widgetSwitcher))
	{
		widgetSwitcher->SetActiveWidgetIndex(ClampPage(widgetSwitcher, page, loop));
	}
}

int32 UMainWidgetBase::ChangeLeftPage(UWidgetSwitcher* widgetSwitcher, const bool loop)
{
	if (IsValid(widgetSwitcher))
	{
		const int32 page = ClampPage(widgetSwitcher, widgetSwitcher->GetActiveWidgetIndex() - 1, loop);
		widgetSwitcher->SetActiveWidgetIndex(page);
		return page;
	}
	return 0;
}

int32 UMainWidgetBase::ChangeRightPage(UWidgetSwitcher* widgetSwitcher, const bool loop)
{
	if (IsValid(widgetSwitcher))
	{
		const int32 page = ClampPage(widgetSwitcher, widgetSwitcher->GetActiveWidgetIndex() + 1, loop);
		widgetSwitcher->SetActiveWidgetIndex(page);
		return page;
	}
	return 0;
}

bool UMainWidgetBase::CanChangeLeftPage(UWidgetSwitcher* widgetSwitcher)
{
	return IsValid(widgetSwitcher) && widgetSwitcher->GetActiveWidgetIndex() > 0;
}

bool UMainWidgetBase::CanChangeRightPage(UWidgetSwitcher* widgetSwitcher)
{
	return IsValid(widgetSwitcher) && widgetSwitcher->GetActiveWidgetIndex() < widgetSwitcher->GetNumWidgets() - 1;
}
