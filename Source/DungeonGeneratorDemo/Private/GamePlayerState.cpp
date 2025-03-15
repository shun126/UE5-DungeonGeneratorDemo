#include "GamePlayerState.h"
#include "Net/UnrealNetwork.h"

AGamePlayerState::AGamePlayerState(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayerState, Life);
}

void AGamePlayerState::SetLife(const int32 newLife)
{
	Life = newLife;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, Life, this);
}

void AGamePlayerState::AddLife(const int32 life)
{
	Life += life;
	if (Life < 0)
		Life = 0;
	else if (Life > 100)
		Life = 100;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, Life, this);
}

void AGamePlayerState::OnRep_Life()
{
}

void AGamePlayerState::SetGameScore(const int32 gameScore)
{
	GameScore = gameScore;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, GameScore, this);
}

void AGamePlayerState::AddGameScore(const int32 gameScore)
{
	GameScore += gameScore;
	if (GameScore < 0)
		GameScore = 0;
	else if (GameScore > 100)
		GameScore = 100;
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, GameScore, this);
}

void AGamePlayerState::OnRep_GameScore()
{
}
