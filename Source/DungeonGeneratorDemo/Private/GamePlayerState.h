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

	UFUNCTION(BlueprintCallable)
	void SetLife(const int32 newLife);

	UFUNCTION(BlueprintCallable)
	void AddLife(const int32 life);

	UFUNCTION()
	virtual void OnRep_Life();

	UFUNCTION(BlueprintGetter)
	int32 GetGameScore() const
	{
		return GameScore;
	}

	UFUNCTION(BlueprintCallable)
	void SetGameScore(const int32 gameScore);

	UFUNCTION(BlueprintCallable)
	void AddGameScore(const int32 gameScore);

	UFUNCTION()
	virtual void OnRep_GameScore();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Life, Category = PlayerState, BlueprintGetter = GetLife)
	int32 Life = 100;

	UPROPERTY(ReplicatedUsing = OnRep_GameScore, Category = PlayerState, BlueprintGetter = GetGameScore)
	int32 GameScore = 0;
};
