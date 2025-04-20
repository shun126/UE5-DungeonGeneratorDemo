/**
@author		Shun Moriya



所持品
*/

#pragma once
#include "DataAsset/ItemDataId.h"
#include "DataAsset/WeaponDataId.h"
#include "Inventory.generated.h"

/**
所持情報のペア
*/
USTRUCT(BlueprintType)
struct FInventoryPair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Count = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Skill = 0;

	static constexpr uint8 MaxCount = 255;
	static constexpr int32 MaxSkill = 1000;

public:
	void Reset(const bool reset);
};

/*
 * ダンジョン固有の所持品
 */
USTRUCT(BlueprintType)
struct FDungeonInventory
{
	GENERATED_BODY()

public:
	// 汎用鍵
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Key = 0;

	// ユニーク鍵（ボス鍵）
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UniqueKey = false;
};

/**
所持品
*/
USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	// 所持しているアイテム
	FInventoryPair Item[ItemDataIdSize];

	// 所持している武器
	FInventoryPair Weapon[WeaponDataIdSize];

	// 所持しているダンジョン毎のアイテム
	FDungeonInventory Dungeons;

	/*
	所持品を初期状態にリセットする
	*/
	void Reset(const bool reset);
};
