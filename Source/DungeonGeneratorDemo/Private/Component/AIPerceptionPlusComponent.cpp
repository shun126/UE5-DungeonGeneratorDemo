
#include "AIPerceptionPlusComponent.h"

UAIPerceptionPlusComponent::UAIPerceptionPlusComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void UAIPerceptionPlusComponent::HandleExpiredStimulus(FAIStimulus& stimulusStore)
{
	OnExpiredStimulus.Broadcast(stimulusStore);
}
