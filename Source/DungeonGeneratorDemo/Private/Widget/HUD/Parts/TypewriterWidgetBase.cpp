/**
@author		Shun Moriya



メッセージウィジットベースクラス
*/

#include "TypewriterWidgetBase.h"

UTypewriterWidgetBase::UTypewriterWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTypewriterWidgetBase::NativeTick(const FGeometry& myGeometry, float deltaTime)
{
	Super::NativeTick(myGeometry, deltaTime);

	if (mMessageFormatter.Update(deltaTime))
	{
		OnMessageChanged(mMessageFormatter.GetMessage());
	}
}

void UTypewriterWidgetBase::StartMessage(const FString& message, const float interval)
{
	mMessageFormatter.Start(message, interval);
}

void UTypewriterWidgetBase::SkipMessage()
{
	mMessageFormatter.Skip();

	OnMessageChanged(mMessageFormatter.GetMessage());
}

bool UTypewriterWidgetBase::IsEndMessage() const noexcept
{
	return mMessageFormatter.IsEnd();
}

const FString& UTypewriterWidgetBase::GetMessage() const noexcept
{
	return mMessageFormatter.GetMessage();
}
