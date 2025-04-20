/**
@author		Shun Moriya

装備品
*/

#pragma once
#include "DataAsset/WeaponDataId.h"
#include "Equipment.generated.h"

/**
装備品
*/
USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_BODY()

	// 装備している武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponDataId Weapon = EWeaponDataId::Invalid;

	/*
	装備をリセットします
	*/
	void Reset(const bool reset);
};
