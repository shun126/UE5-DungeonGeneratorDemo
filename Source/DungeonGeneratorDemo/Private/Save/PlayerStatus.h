/**
@author		Shun Moriya



プレイヤー能力
*/

#pragma once
#include "Ability.h"
#include "Equipment.h"
#include "Inventory.h"
#include "DataAsset/WeaponDataAsset.h"
#include "PlayerStatus.generated.h"

/**
プレイヤー情報
*/
USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

private:
	// 食料の初期値
	static constexpr int32 InitialFood = 200;

public:
	/*
	プレイヤーの状態（装備・所持金・食料・経験値・等々）と所持品をリセットします
	*/
	void Reset(const bool reset);

	/**
	アイテムを捨てる事ができるか？
	\param[in]	itemId		アイテムID
	\return		trueなら捨てれる
	*/
	bool CanDrop(const EItemDataId itemId) const;

	/**
	武器を捨てる事ができるか？
	\param[in]	weaponDataId		武器ID
	\return		trueなら捨てれる
	*/
	bool CanDrop(const EWeaponDataId weaponDataId) const;

	/**
	アイテムを捨てる
	\param[in]	itemId		アイテムID
	\return		trueなら捨てれた
	*/
	bool Drop(const EItemDataId itemId);

	/**
	武器を捨てる
	\param[in]	weaponDataId		武器ID
	\return		trueなら捨てれた
	*/
	bool Drop(const EWeaponDataId weaponDataId);

	/**
	アイテムを買う
	\param[in]	itemId		アイテムID
	\param[in]	price		購入金額
	\return		trueなら買えた
	*/
	bool Buy(const EItemDataId itemId, const int32 price);

	/**
	武器を買う
	\param[in]	weaponDataId	武器ID
	\param[in]	price			購入金額
	\return		trueなら買えた
	*/
	bool Buy(const EWeaponDataId weaponDataId, const int32 price);

	/**
	アイテムを売る
	\param[in]	itemId		アイテムID
	\param[in]	price		売却金額
	\return		trueなら買えた
	*/
	bool Sell(const EItemDataId itemId, const int32 price);

	/**
	武器を売る
	\param[in]	weaponDataId	武器ID
	\param[in]	price			売却金額
	\return		trueなら買えた
	*/
	bool Sell(const EWeaponDataId weaponDataId, const int32 price);

	/**
	武器を装備します
	*/
	void EquipWeapon(const EWeaponDataId weaponDataId);

	/**
	武器の装備を解除します
	*/
	void UnEquipWeapon();

	/**
	装備中の武器を取得します
	*/
	EWeaponDataId GetEquippedWeapon() const;

	/**
	装備中の武器データを取得します
	*/
	const FWeaponData& GetEquippedWeaponData() const;

	/**
	装備中の武器アクタークラスを取得します
	*/
	TSubclassOf<AActor> GetEquippedWeaponActorClass() const;

	/**
	能力を取得します
	*/
	FAbility& GetAbility();

	/**
	能力を取得します
	*/
	const FAbility& GetAbility() const;

	/**
	装備品を取得します
	*/
	FInventory& GetInventory();

	/**
	装備品を取得します
	*/
	const FInventory& GetInventory() const;

	/*
	売却可能なアイテム一覧を取得します
	*/
	TArray<EItemDataId> GetSalableItems() const;

	/*
	売却可能な武器一覧を取得します
	*/
	TArray<EWeaponDataId> GetSalableWeapons() const;

	/*
	アイテムの熟練度を百分率で取得します
	*/
	float GetItemSkillPercent(const EItemDataId itemDataId) const noexcept;

	/*
	武器の熟練度を百分率で取得します
	*/
	float GetWeaponSkillPercent(const EWeaponDataId weaponDataId) const noexcept;

	/**
	武器の熟練度に応じたコンボ数を取得します
	*/
	int32 GetWeaponComboCount() const;

	/**
	武器の攻撃力を取得します
	*/
	int32 GetWeaponAttackPower() const;

	/**
	武器の熟練値と経験値を加算します
	\param[in]	value	加算する値
	*/
	void AddWeaponExperience(const int32 value);

	/**
	武器の経験値を取得します
	\return		武器の経験値
	*/
	int32 GetWeaponExperience() const;

	/**
	武器のレベルを取得します
	\return		武器のレベル
	*/
	uint8 GetWeaponLevel() const;

	/*!
	称号の文字列テーブルキーを取得します
	*/
	FString GetHeroTitleStringTableKey() const noexcept;

	/**
	所持金を増減させます
	0未満にはなりません
	*/
	void AddGold(const int32 amount);

	/**
	金の量を取得します
	*/
	int32 GetGold() const noexcept;

	/**
	食事をします
	レベルに応じて食料を減らします	0未満にはなりません
	*/
	bool Eat();

	/**
	空腹状態か調べます
	*/
	bool Hungry() const;

	/**
	食料を増やします
	0未満にはなりません
	*/
	void AddFood(const int32 amount);

	/**
	食料の量を取得します
	*/
	int32 GetFood() const noexcept;

	/**
	鍵を増やします
	*/
	int32 AddKey(const FString& dungeonName, const int32 count);

	/**
	鍵を使います
	*/
	bool UseKey(const FString& dungeonName);

	/**
	ユニーク（主にボス）な鍵を増やします
	*/
	bool AddUniqueKey(const FString& dungeonName);

	/**
	ユニーク（主にボス）な鍵を使います
	*/
	bool UseUniqueKey(const FString& dungeonName);

	/**
	 * 最大体力
	 */
	int32 GetMaxHitPoint() const noexcept;

public:
	// 能力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAbility Ability;

	// 所持品
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInventory Inventory;

	// 装備品
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEquipment Equipment;

	// 所持金
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 Gold = 0;

	// 食料
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 Food = InitialFood;

	// 経験値
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 WeaponExperience = 0;

	// レベル
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 WeaponLevel = 0;
};
