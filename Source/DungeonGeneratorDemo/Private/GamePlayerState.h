#pragma once
#include <GameFramework/PlayerState.h>
#include "GamePlayerState.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	explicit AGamePlayerState(const FObjectInitializer& objectInitializer);
	virtual ~AGamePlayerState() override = default;


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintGetter)
	int32 GetLife() const
	{
		return Life;
	}

	void SetLife(const float newLife);

	/** Replication Notification Callbacks */
	UFUNCTION()
	virtual void OnRep_Life();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Life, Category = PlayerState, BlueprintGetter = GetLife)
	int32 Life = 100;
};
