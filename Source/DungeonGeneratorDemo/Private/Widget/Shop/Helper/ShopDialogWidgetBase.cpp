/**
@author		Shun Moriya

店の行動ウィジットベースクラス
*/

#include "ShopDialogWidgetBase.h"

UShopDialogWidgetBase::UShopDialogWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShopDialogWidgetBase::Decide(const EShopDialogResult shopActionType)
{
	OnDecided.Broadcast(Index, shopActionType);
}
