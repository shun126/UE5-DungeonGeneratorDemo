/**
@author		Shun Moriya

店アイコンベースクラス
*/

#include "ShopIconWidgetBase.h"
#include "ShopIconEntryBase.h"

UShopIconWidgetBase::UShopIconWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShopIconWidgetBase::Set(const UShopIconEntryBase* shopIconEntryBase)
{
	if (shopIconEntryBase)
	{
		Name = shopIconEntryBase->GetName();
		Description = shopIconEntryBase->GetDescription();
		Inventory = shopIconEntryBase->GetInventory();
		Price = shopIconEntryBase->GetPrice();
		Performance = shopIconEntryBase->GetPerformance();
		Skill = shopIconEntryBase->GetSkill();
		Index = shopIconEntryBase->GetIndex();
	}
}
