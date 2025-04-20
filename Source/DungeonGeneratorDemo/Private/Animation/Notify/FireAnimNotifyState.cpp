/**
@author		Shun Moriya

攻撃アニメーション通知ソースファイル
*/

#include "animation/Notify/FireAnimNotifyState.h"
#include "Actor/EnemyBase.h"
#include "animation/Instance/EnemyAnimInstance.h"
#include <animation/AnimMontage.h>

UFireAnimNotifyState::UFireAnimNotifyState(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void UFireAnimNotifyState::NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration, const FAnimNotifyEventReference& eventReference)
{
	// 親クラスを呼び出す
	Super::NotifyBegin(meshComp, animation, totalDuration, eventReference);

	// 攻撃開始
	if (IsValid(meshComp))
	{
		if (auto* animInstance = GetValid(Cast<UEnemyAnimInstance>(meshComp->GetAnimInstance())))
		{
			animInstance->RetainFireCount();
		}

		if (auto* enemy = GetValid(Cast<AEnemyBase>(meshComp->GetOwner())))
		{
			enemy->NotifyBeginFire(NotifyName);
		}
	}
}

void UFireAnimNotifyState::NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& eventReference)
{
	// 攻撃終了
	if (IsValid(meshComp))
	{
		if (auto* animInstance = GetValid(Cast<UEnemyAnimInstance>(meshComp->GetAnimInstance())))
		{
			animInstance->ReleaseFireCount();
		}

		if (auto* enemy = GetValid(Cast<AEnemyBase>(meshComp->GetOwner())))
		{
			enemy->NotifyEndFire(NotifyName);
		}
	}

	// 親クラスを呼び出す
	Super::NotifyEnd(meshComp, animation, eventReference);
}

#if WITH_EDITOR
bool UFireAnimNotifyState::CanBePlaced(UAnimSequenceBase* animation) const
{
	if (animation)
	{
		if (
			animation->IsA(UAnimSequence::StaticClass()) ||
			animation->IsA(UAnimMontage::StaticClass()))
		{
			return true;
		}
	}

	return false;
}
#endif // WITH_EDITOR
