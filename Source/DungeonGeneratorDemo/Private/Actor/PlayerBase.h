/**
@author		Shun Moriya
*/

#pragma once
#include "Helper/DamageHistory.h"
#include <GameFramework/Character.h>
#include "PlayerBase.generated.h"

UCLASS()
class APlayerBase : public ACharacter
{
	GENERATED_BODY()

	//! êHéñÇÃä‘äu
	static constexpr float EatIntervalTimer = 5.0f;

public:
	explicit APlayerBase(const FObjectInitializer& objectInitializer);
	virtual ~APlayerBase() override = default;

public:
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Result"))
	void RegisterDamageHistory(EDamageHistoryRegisterResult& Result, const AActor* actor);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDied();

	UPROPERTY(EditAnywhere)
	FDamageHistory DamageHistory;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//! êHéñÇÃä‘äu
	float mEatIntervalTimer = 0.f;

	bool mIsDead = false;
};
