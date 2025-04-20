/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <BehaviorTree/Tasks/BTTask_BlackboardBase.h>
#include "BT_SetStimulusEnable.generated.h"

UCLASS()
class UBT_SetStimulusEnable : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBT_SetStimulusEnable() = default;
    virtual ~UBT_SetStimulusEnable() override = default;

    // override
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Enable = true;
};