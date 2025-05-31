/**
@author		Shun Moriya
*/

#include "Controller/Task/BT_ChangeWalkMove.h"
#include "Actor/EnemyBase.h"
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>

EBTNodeResult::Type UBT_ChangeWalkMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    auto controller = ownerComp.GetAIOwner();
    if (IsValid(controller) == false)
        return EBTNodeResult::Failed;

    auto ownerAvatar = Cast<AEnemyBase>(controller->GetPawn());
    if (IsValid(ownerAvatar) == false)
        return EBTNodeResult::Failed;

    // 防御を設定


    ownerAvatar->ChangeWalk();
    ownerAvatar->Alert(false);
    return EBTNodeResult::Succeeded;
}
