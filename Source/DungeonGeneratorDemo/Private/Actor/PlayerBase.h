/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include "PlayerBase.generated.h"

UCLASS()
class APlayerBase : public ACharacter
{
	GENERATED_BODY()

	//! �H���̊Ԋu
	static constexpr float EatIntervalTimer = 5.0f;

public:
	explicit APlayerBase(const FObjectInitializer& objectInitializer);
	virtual ~APlayerBase() override = default;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//! �H���̊Ԋu
	float mEatIntervalTimer = 0.f;
};
