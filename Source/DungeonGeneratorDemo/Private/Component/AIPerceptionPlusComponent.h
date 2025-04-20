#pragma once
#include <Perception/AIPerceptionComponent.h>
#include "AIPerceptionPlusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorPerceptionPlusExpiredStimulusDelegate, FAIStimulus, Stimulus);

UCLASS(ClassGroup = AI, meta = (BlueprintSpawnableComponent))
class UAIPerceptionPlusComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	explicit UAIPerceptionPlusComponent(const FObjectInitializer& objectInitializer);
	virtual ~UAIPerceptionPlusComponent() override = default;

protected:
	virtual void HandleExpiredStimulus(FAIStimulus& stimulusStore) override;

public:
	UPROPERTY(BlueprintAssignable)
	FActorPerceptionPlusExpiredStimulusDelegate OnExpiredStimulus;
};
