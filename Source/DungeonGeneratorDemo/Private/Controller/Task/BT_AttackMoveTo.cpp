/**
\author		Shun Moriya
\copyright	2023- Shun Moriya

*/

#include "Controller/Task/BT_AttackMoveTo.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>

UBT_AttackMoveTo::UBT_AttackMoveTo(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	NodeName = "Move and Attack";
	bNotifyTick = true;

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_AttackMoveTo, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBT_AttackMoveTo, BlackboardKey));
	TargetActorBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_AttackMoveTo, TargetActorBlackboardKey), AActor::StaticClass());
}

void UBT_AttackMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	mAccept = OnceAwayFromOpponent == false;

	if (AcceptableRadius > DistanceToCancelMovement.Min)
		AcceptableRadius = DistanceToCancelMovement.Min;
}

EBTNodeResult::Type UBT_AttackMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Reached(OwnerComp) == true)
	{
		return EBTNodeResult::Type::Succeeded;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBT_AttackMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	mAccept = OnceAwayFromOpponent == false;
	mFinished = false;

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBT_AttackMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 親クラスMoveToを呼び出す
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Reached(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

bool UBT_AttackMoveTo::Reached(UBehaviorTreeComponent& OwnerComp)
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

	// 移動対象がアクターの場合
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		const AActor* targetActor = Cast<AActor>(
			myBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
		if (targetActor)
		{
			// 攻撃対象のコリジョンを間合いとして取得
			distanceBetweenOpponents += targetActor->GetSimpleCollisionRadius();

			if (CompletionCheck(ownerPawnLocation, targetActor->GetActorLocation(), distanceBetweenOpponents) == true)
				return true;
		}
	}
	// 移動対象が位置の場合
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector& attackingTargetLocation =
			myBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		if (CompletionCheck(ownerPawnLocation, attackingTargetLocation, distanceBetweenOpponents) == true)
			return true;
	}

	// 攻撃対象との距離
	if (const auto targetObject = myBlackboard->GetValue<UBlackboardKeyType_Object>(TargetActorBlackboardKey.GetSelectedKeyID()))
	{
		if (const auto targetActor = Cast<AActor>(targetObject))
		{
			if (CompletionCheck(ownerPawnLocation, targetActor->GetActorLocation(), distanceBetweenOpponents) == true)
				return true;
		}
	}

	return false;
}

bool UBT_AttackMoveTo::CompletionCheck(const FVector& ownerPawnLocation, const FVector& attackingTargetLocation, const double distanceBetweenOpponents) noexcept
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
