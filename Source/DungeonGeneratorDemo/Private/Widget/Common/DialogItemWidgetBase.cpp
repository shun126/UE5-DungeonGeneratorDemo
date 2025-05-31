/**
@author		Shun Moriya

* ダイアログ リストアイテム ウィジットクラス
 */

#pragma once
#include "DialogItemWidgetBase.h"

UDialogItemWidgetBase::UDialogItemWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDialogItemWidgetBase::Set(const UDialogItemData* dialogEntryBase)
{
	if (dialogEntryBase)
	{
		Name = dialogEntryBase->GetName();
		Identifier = dialogEntryBase->GetIdentifier();
	}
}
