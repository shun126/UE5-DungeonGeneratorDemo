/**
@author		Shun Moriya

バルーン（吹き出し）ウィジットクラス
*/

#include "BalloonWidgetBase.h"
#include "Helper/Math.h"

UBalloonWidgetBase::UBalloonWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	SetLabel(FText::FromString(TEXT("TEXT")));
#endif
}

const FText& UBalloonWidgetBase::GetLabel() const
{
	return Label;
}

void UBalloonWidgetBase::SetLabel(const FText& label)
{
	Label = label;
}

void UBalloonWidgetBase::Update(const AActor* ownerActor)
{
	ESlateVisibility visible = ESlateVisibility::Hidden;

	// ターゲットアクターの位置に更新
	if (ownerActor)
	{
		if (const auto* playerController = GetValid(GetOwningPlayer()))
		{
			FVector2D screenLocation;
			if (playerController->ProjectWorldLocationToScreen(ownerActor->GetActorLocation(), screenLocation, true))
			{
				SetPositionInViewport(screenLocation, true);
			}

			if (const auto playerPawn = playerController->GetPawn())
			{
				const double distance = FVector::DistSquared(ownerActor->GetActorLocation(), playerPawn->GetActorLocation());
				if (distance <= math::Square(VisibleDistance))
					visible = ESlateVisibility::HitTestInvisible;
			}
		}
	}

	// 可視性を更新
	if (GetVisibility() != visible)
	{
		SetVisibility(visible);
	}
}
