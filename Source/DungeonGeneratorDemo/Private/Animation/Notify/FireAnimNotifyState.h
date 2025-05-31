/**
@author		Shun Moriya

攻撃アニメーション通知ヘッダーファイル
*/

#pragma once
/*
#include <AnimNotifies/AnimNotify_PlayMontageNotify.h>
UAnimNotify_PlayMontageNotifyWindow
*/
//#include "Battle/FireAndDamageReactionType.h"
#include <animation/AnimNotifies/AnimNotifyState.h>
#include "FireAnimNotifyState.generated.h"

/**
攻撃アニメーション通知クラス

UAvatarAnimInstance::IsFireing判定
AvatarのOnBeginFireとOnEndFireの通知
*/
UCLASS(EditInlineNew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "アバターへ攻撃の有効を通知"))
class UFireAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	explicit UFireAnimNotifyState(const FObjectInitializer& objectInitializer);
	virtual ~UFireAnimNotifyState() override = default;

	// UAnimNotifyState overrides
	virtual void NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration, const FAnimNotifyEventReference& eventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& eventReference) override;

#if WITH_EDITOR
	virtual bool CanBePlaced(UAnimSequenceBase* animation) const override;
#endif

protected:
	// Name of notify that is passed to ability.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	FName NotifyName;
#if 0
	// 攻撃側が求める防御側のリアクション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Notify")
	EFireReactionType FireReactionType = EFireReactionType::Hit;
#endif
};

