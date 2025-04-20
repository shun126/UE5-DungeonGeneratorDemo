/**
ドロップ食料基底クラス
*/

#include "DropFoodBase.h"
#include "Actor/GamePlayerState.h"
#include "Actor/PlayerBase.h"
#include "Helper/Log.h"
#include "GameInstanceBase.h"

#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Internationalization/StringTable.h"

ADropFoodBase::ADropFoodBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FString ADropFoodBase::GetStringTableKey_Implementation() const
{
	if (ItemDataId != EItemDataId::Invalid)
	{
		if (const auto gameInstance = UGameInstanceBase::Instance())
			return gameInstance->GetItemDataAsset().Get(ItemDataId).Key();
	}

	return TEXT("");
}

void ADropFoodBase::PickUp(const APawn* finderPawn)
{
	if (IsValid(finderPawn))
	{
		if (auto playerState = finderPawn->GetPlayerState<AGamePlayerState>())
		{
			playerState->GetPlayerStatus().AddFood(Amount);
			Destroy();
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アイテムを拾うプレイヤーステートが見つかりません"));
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アイテムを拾うプレイヤーを指定して下さい"));
	}
}

void ADropFoodBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADropFoodBase::OnBeginOverlap);
	}
}

void ADropFoodBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADropFoodBase::OnBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void ADropFoodBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(Cast<APlayerBase>(OtherActor));
}
