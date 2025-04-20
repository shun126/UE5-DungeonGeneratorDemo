#pragma once
#include "DataAsset/EnemyAppearanceAsset.h"
#include <SubActor/DungeonRoomSensorBase.h>
#include "DungeonRoomSensor.generated.h"

UCLASS()
class ADungeonRoomSensor : public ADungeonRoomSensorBase
{
	GENERATED_BODY()

public:
	explicit ADungeonRoomSensor(const FObjectInitializer& initializer);
	virtual ~ADungeonRoomSensor() override = default;

protected:
	virtual void OnNativeInitialize() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnemyAppearanceAsset> EnemyAppearance;
};
