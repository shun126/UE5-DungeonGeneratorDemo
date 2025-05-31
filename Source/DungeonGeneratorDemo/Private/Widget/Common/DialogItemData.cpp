/**
@author		Shun Moriya

* UDialogWidgetBaseとリストボックス登録用クラス
 * https://historia.co.jp/archives/10389/
 */

#include "DialogItemData.h"

UDialogItemData::UDialogItemData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDialogItemData* UDialogItemData::Set(const FName& stringTablePackageName, const FString& tableName, const int32 identifier)
{
	//Name = FText::FromStringTable(stringTablePackageName, tableName);
	Name = FText::FromString(tableName);
	Identifier = identifier;
	return const_cast<UDialogItemData*>(this);
}

const FText& UDialogItemData::GetName() const noexcept
{
	return Name;
}

int32 UDialogItemData::GetIdentifier() const noexcept
{
	return Identifier;
}
