#include "GamePlayerState.h"
#include "Net/UnrealNetwork.h"

//#define ENABLE_PUSH_MODEL

AGamePlayerState::AGamePlayerState(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

#if ENABLE_PUSH_MODEL
	// PushModel型Replicationが有効ならばそれを使い、無効ならばいつも通りのReplicationを行う
	FDoRepLifetimeParams params;
	params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(AGamePlayerState, Life, params);
	DOREPLIFETIME_WITH_PARAMS_FAST(AGamePlayerState, GameScore, params);
#else
	DOREPLIFETIME(AGamePlayerState, Life);
	DOREPLIFETIME(AGamePlayerState, GameScore);
#endif
}

void AGamePlayerState::SetLife(const int32 newLife)
{
	Life = newLife;
#if defined(ENABLE_PUSH_MODEL)
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, Life, this);
#endif
}
	
void AGamePlayerState::AddLife(const int32 life)
{
	Life += life;
	if (Life < 0)
		Life = 0;
	else if (Life > 100)
		Life = 100;
#if defined(ENABLE_PUSH_MODEL)
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, Life, this);
#endif
}

void AGamePlayerState::OnRep_Life()
{
}

void AGamePlayerState::SetGameScore(const int32 gameScore)
{
	GameScore = gameScore;
#if defined(ENABLE_PUSH_MODEL)
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, GameScore, this);
#endif
}

void AGamePlayerState::AddGameScore(const int32 gameScore)
{
	GameScore += gameScore;
	if (GameScore < 0)
		GameScore = 0;
#if defined(ENABLE_PUSH_MODEL)
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, GameScore, this);
#endif
}

void AGamePlayerState::OnRep_GameScore()
{
}
