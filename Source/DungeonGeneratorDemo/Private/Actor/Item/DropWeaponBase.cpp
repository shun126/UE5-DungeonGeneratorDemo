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

FText ADropWeaponBase::GetLabel_Implementation() const
{
	if (WeaponDataId != EWeaponDataId::Invalid)
	{
		if (const auto gameInstance = UGameInstanceBase::Instance())
			return gameInstance->GetWeaponDataAsset().Get(WeaponDataId).GetName();
	}

	return FText();
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
				DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Cannot find player state to pick up weapon for actor '%s'"), *GetName());
			}
		}
		else
		{
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Specify the player who will pick up the weapon for actor '%s'"), *GetName());
		}
	}
	else
	{
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Set WeaponDataId for actor '%s'"), *GetName());
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
