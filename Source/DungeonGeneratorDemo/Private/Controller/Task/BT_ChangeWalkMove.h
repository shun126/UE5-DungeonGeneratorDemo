/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "BT_ChangeWalkMove.generated.h"

UCLASS()
class UBT_ChangeWalkMove : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBT_ChangeWalkMove() = default;
    virtual ~UBT_ChangeWalkMove() override = default;

    // override
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
};