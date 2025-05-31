#include "DungeonRoomSensor.h"


ADungeonRoomSensor::ADungeonRoomSensor(const FObjectInitializer& initializer)
    : Super(initializer)
{
}

void ADungeonRoomSensor::OnNativeInitialize()
{
    Super::OnNativeInitialize();

    if (IsValid(EnemyAppearance))
    {
	    
    }
}
