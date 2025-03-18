/**
\author		Shun Moriya
\copyright	2023- Shun Moriya
All Rights Reserved.
*/

#pragma once
#include "AITypes.h"
#include <BehaviorTree/Tasks/BTTask_MoveTo.h>
#include "BT_TaskAttackMoveTo.generated.h"

/*
MoveToの機能に攻撃対象までの距離が指定距離以内に近づいた。
または指定距離以上離れた時に移動を終了する。
*/
UCLASS()
class UBT_TaskAttackMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
    explicit UBT_TaskAttackMoveTo(const FObjectInitializer& objectInitializer);
    virtual ~UBT_TaskAttackMoveTo() override = default;

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// 移動をキャンセルする攻撃対象のアクターとの距離
	UPROPERTY(EditAnywhere, Category = "Blackboard|AttackMoveTo")
	FFloatInterval DistanceToCancelMovement = { 100.f, 200.f };

	// 一度遠のいてから判定する
	UPROPERTY(EditAnywhere, Category = "Blackboard|AttackMoveTo")
	bool OnceAwayFromOpponent = false;

private:
	bool Reached(UBehaviorTreeComponent& OwnerComp);
	bool CompletionCheck(const FVector& ownerPawnLocation, const FVector& attackingTargetLocation, const double distanceBetweenOpponents) noexcept;

private:
	bool mAccept = false;
	bool mFinished = false;
};
