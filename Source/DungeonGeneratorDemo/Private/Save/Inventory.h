/**
 * @author		Shun Moriya
 */

#pragma once
#include "DataAsset/ItemDataId.h"
#include "DataAsset/WeaponDataId.h"
#include "Inventory.generated.h"

/**
 * Pairs of number of possessions and proficiency
 * 所有品の個数と熟練度のペア
 */
USTRUCT(BlueprintType)
struct FInventoryPair
{
	GENERATED_BODY()

	/**
	 * Number of
	 * 個数
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Count = 10;

	/**
	 * Proficiency
	 * 熟練度
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Skill = 0;

	static constexpr uint8 MaxCount = 255;
	static constexpr int32 MaxSkill = 1000;

	void Reset(const bool reset);
};

/**
 * Dungeon-specific possessions
 * ダンジョン固有の所有品
 */
USTRUCT(BlueprintType)
struct FDungeonInventory
{
	GENERATED_BODY()

public:
	/**
	 * general-purpose key
	 * 汎用鍵
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Key = 0;

	/**
	 * Unique key (boss key)
	 * ユニーク鍵（ボス鍵）
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UniqueKey = false;
};

/**
 * Inventory
 * 所持品
 */
USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

	/**
	 * Items in possession
	 * 所持しているアイテム
	 */
	FInventoryPair Item[ItemDataIdSize];

	/**
	 * Weapons in possession
	 * 所持している武器
	 */
	FInventoryPair Weapon[WeaponDataIdSize];

	/**
	 * Items per dungeon in your possession
	 * 所持しているダンジョン毎のアイテム
	 */
	FDungeonInventory Dungeons;

	/**
	 * Reset your possessions to their initial state
	 * 所持品を初期状態にリセットする
	 */
	void Reset(const bool reset);
};
