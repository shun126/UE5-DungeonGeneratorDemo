/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "BT_ChangeStrafeMove.generated.h"

UCLASS()
class UBT_ChangeStrafeMove : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBT_ChangeStrafeMove();
    virtual ~UBT_ChangeStrafeMove() override = default;

    // override
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Alert = false;
};