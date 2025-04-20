/**
@author		Shun Moriya
*/

#include "Controller/Task/BT_SetStimulusEnable.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>

EBTNodeResult::Type UBT_SetStimulusEnable::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    const auto blackboard = ownerComp.GetBlackboardComponent();
    if (IsValid(blackboard))
        blackboard->SetValueAsBool(GetSelectedBlackboardKey(), Enable);
    return EBTNodeResult::Succeeded;
}
