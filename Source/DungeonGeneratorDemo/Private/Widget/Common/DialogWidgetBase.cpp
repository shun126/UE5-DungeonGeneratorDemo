/**
@author		Shun Moriya

* ダイアログウィジットベースクラス
 */

#pragma once
#include "DialogWidgetBase.h"

UDialogWidgetBase::UDialogWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDialogWidgetBase::AddAnswers(const TArray<FString>& answers)
{
	for (int32 i = 0; i < answers.Num(); ++i)
	{
		const FString& answer = answers[i];
		AddAnswer(FText::FromString(answer), i);
	}
}

void UDialogWidgetBase::Decide(const int32 identifier)
{
	mDecideIdentifier = identifier;
	mDecided = true;
	OnDecided.Broadcast(identifier);

	if (AutoClose)
	{
		RemoveFromParent();
	}
}

bool UDialogWidgetBase::Decided() const noexcept
{
	return mDecided;
}

int32 UDialogWidgetBase::GetDecideIdentifier() const noexcept
{
	return mDecideIdentifier;
}
