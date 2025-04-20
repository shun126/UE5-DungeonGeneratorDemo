/**
@author		Shun Moriya
*/

#include "Controller/Task/BT_ChangeStrafeMove.h"
#include "Actor/EnemyBase.h"
#include <AIController.h>
#include <BehaviorTree/BTTaskNode.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>

UBT_ChangeStrafeMove::UBT_ChangeStrafeMove()
{
    NodeName = "TaskChangeStrafeMove";

    BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_ChangeStrafeMove, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBT_ChangeStrafeMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    const auto blackboard = ownerComp.GetBlackboardComponent();
    if (IsValid(blackboard) == false)
        return EBTNodeResult::Failed;

    auto targetActor = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
    if (IsValid(targetActor) == false)
        return EBTNodeResult::Failed;

    auto controller = ownerComp.GetAIOwner();
    if (IsValid(controller) == false)
        return EBTNodeResult::Failed;

    auto ownerAvatar = Cast<AEnemyBase>(controller->GetPawn());
    if (IsValid(ownerAvatar) == false)
        return EBTNodeResult::Failed;


   // 防御を設定



    ownerAvatar->ChangeStrafeToActor(targetActor, false);
    ownerAvatar->Alert(Alert ? targetActor : nullptr);
    return EBTNodeResult::Succeeded;
}
