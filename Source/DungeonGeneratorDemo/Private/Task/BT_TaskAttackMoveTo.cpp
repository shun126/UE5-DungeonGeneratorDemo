/**
\author		Shun Moriya
\copyright	2023- Shun Moriya
All Rights Reserved.
*/

#include "Task/BT_TaskAttackMoveTo.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include <Tasks/AITask_MoveTo.h>

UBT_TaskAttackMoveTo::UBT_TaskAttackMoveTo(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	NodeName = "Move and Attack";
	bNotifyTick = true;

	// accept only actors and vectors
	AttackingTargetBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_TaskAttackMoveTo, AttackingTargetBlackboardKey), AActor::StaticClass());
	AttackingTargetBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBT_TaskAttackMoveTo, AttackingTargetBlackboardKey));
}

void UBT_TaskAttackMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* blackboardAsset = GetBlackboardAsset())
	{
		AttackingTargetBlackboardKey.ResolveSelectedKey(*blackboardAsset);
	}

	mAccept = OnceAwayFromOpponent == false;
}

EBTNodeResult::Type UBT_TaskAttackMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Reached(OwnerComp) == true)
		return EBTNodeResult::Type::Succeeded;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBT_TaskAttackMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	mAccept = OnceAwayFromOpponent == false;
	mFinished = false;

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBT_TaskAttackMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 親クラスMoveToを呼び出す
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Reached(OwnerComp))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

bool UBT_TaskAttackMoveTo::Reached(UBehaviorTreeComponent& OwnerComp)
{
	// オーナーAIコントローラーを取得
	const AAIController* ownerController = OwnerComp.GetAIOwner();
	if (ownerController == nullptr)
		return false;

	// オーナーポーンを取得
	const APawn* ownerPawn = ownerController->GetPawn();
	if (ownerPawn == nullptr)
		return true;

	// オーナーポーンの位置を取得
	const FVector& ownerPawnLocation = ownerPawn->GetActorLocation();

	// オーナーポーンのコリジョンを間合いとして取得
	float distanceBetweenOpponents = ownerPawn->GetSimpleCollisionRadius();

	// ブラックボードを取得
	const UBlackboardComponent* myBlackboard = OwnerComp.GetBlackboardComponent();
	if (myBlackboard == nullptr)
		return true;

	// 攻撃対象がアクターの場合
	if (AttackingTargetBlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		const AActor* targetActor = Cast<AActor>(
			myBlackboard->GetValue<UBlackboardKeyType_Object>(AttackingTargetBlackboardKey.GetSelectedKeyID()));
		if (targetActor)
		{
			// 攻撃対象のコリジョンを間合いとして取得
			distanceBetweenOpponents += targetActor->GetSimpleCollisionRadius();

			if (CompletionCheck(ownerPawnLocation, targetActor->GetActorLocation(), distanceBetweenOpponents) == true)
				return true;
		}
	}
	// 攻撃対象が位置の場合
	else if (AttackingTargetBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector& attackingTargetLocation =
			myBlackboard->GetValue<UBlackboardKeyType_Vector>(AttackingTargetBlackboardKey.GetSelectedKeyID());
		if (CompletionCheck(ownerPawnLocation, attackingTargetLocation, distanceBetweenOpponents) == true)
			return true;
	}

	return false;
}

bool UBT_TaskAttackMoveTo::CompletionCheck(const FVector& ownerPawnLocation, const FVector& attackingTargetLocation, const double distanceBetweenOpponents) noexcept
{
	double distance = FVector::Distance(ownerPawnLocation, attackingTargetLocation);

	distance -= distanceBetweenOpponents;

	if (mAccept)
	{
		mFinished = DistanceToCancelMovement.Min <= distance && distance <= DistanceToCancelMovement.Max;
		return mFinished;
	}

	if (distance > DistanceToCancelMovement.Max)
		mAccept = true;

	return false;
}
