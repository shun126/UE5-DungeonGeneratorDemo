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

void AGamePlayerState::SetLife(const float newLife)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AGamePlayerState, Life, this);
	Life = newLife;
}

void AGamePlayerState::OnRep_Life()
{
}
