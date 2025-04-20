/**
ドロップ武器基底クラス
*/

#include "DropWeaponBase.h"
#include "Actor/GamePlayerState.h"
#include "Actor/PlayerBase.h"
#include "Helper/Log.h"
#include "../../GameInstanceBase.h"

ADropWeaponBase::ADropWeaponBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EWeaponDataId ADropWeaponBase::GetId() const
{
	return WeaponDataId;
}

void ADropWeaponBase::SetId(const EWeaponDataId weaponDataId)
{
	WeaponDataId = weaponDataId;
}

FString ADropWeaponBase::GetStringTableKey_Implementation() const
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		if (const auto gameInstance = UGameInstanceBase::Instance())
			return gameInstance->GetWeaponDataAsset().Get(WeaponDataId).Key();
	}

	return TEXT("");
}

void ADropWeaponBase::PickUp(const APawn* finderPawn)
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		if (IsValid(finderPawn))
		{
			if (auto* playerState = finderPawn->GetPlayerState<AGamePlayerState>())
			{
				// TODO: FPlayerStatus内で操作してください
				FInventory& inventory = playerState->GetPlayerStatus().GetInventory();
				inventory.Weapon[static_cast<size_t>(WeaponDataId)].Count += 1;
				Destroy();
			}
			else
			{
				DUNGEON_GENERATOR_DEMO_ERROR(TEXT("武器を拾うプレイヤーステートが見つかりません"));
			}
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("武器を拾うプレイヤーを指定して下さい"));
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("WeaponDataIdを設定して下さい"));
	}
}

void ADropWeaponBase::Equip_Implementation(const bool playerAttack)
{
	Super::Equip_Implementation(playerAttack);
	ChangeAttackCollisionProfile(playerAttack);
}

void ADropWeaponBase::UnEquip_Implementation()
{
	Super::UnEquip_Implementation();
}

void ADropWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADropWeaponBase::OnBeginOverlap);
	}
}

void ADropWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ADropWeaponBase::OnBeginOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void ADropWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickUp(Cast<APlayerBase>(OtherActor));
}
