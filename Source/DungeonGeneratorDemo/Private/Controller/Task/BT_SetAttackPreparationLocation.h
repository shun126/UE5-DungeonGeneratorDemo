/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "BT_SetAttackPreparationLocation.generated.h"

UCLASS()
class UBT_SetAttackPreparationLocation : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBT_SetAttackPreparationLocation();
    virtual ~UBT_SetAttackPreparationLocation() override = default;

    // override
    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, Category = Blackboard)
    FBlackboardKeySelector TargetActorKey;
};