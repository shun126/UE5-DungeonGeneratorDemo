/**
@author		Shun Moriya
*/

#pragma once
#include "DataAsset/WeaponDataId.h"
#include <CoreMinimal.h>

/**
@addtogroup Battle
@{
*/

/*
攻撃側が求める防御側のリアクション

編集した場合はCAttackAndDamageReactionType::Evaluateの判定を修正して下さい
*/
UENUM(BlueprintType)
enum class EAttackReactionType : uint8
{
	Down		UMETA(DisplayName = "ダウン攻撃"),
	Hit			UMETA(DisplayName = "通常攻撃"),
	Invalid		UMETA(DisplayName = "攻撃無効"),
};

/**
攻撃側が受け入れる結果
*/
UENUM(BlueprintType)
enum class EAttackReactionResult : uint8
{
	Success		UMETA(DisplayName = "攻撃成功"),
	Knockback	UMETA(DisplayName = "弾かれる"),
};

/*
攻撃を受けた時の防御側のリアクション

編集した場合はCAttackAndDamageReactionType::Evaluateの判定を修正して下さい
*/
UENUM(BlueprintType)
enum class EDefenseReactionType : uint8
{
	Hard		UMETA(DisplayName = "固い"),
	Normal		UMETA(DisplayName = "普通"),
	Soft		UMETA(DisplayName = "柔らかい"),
};

/**
防御側が受け入れる結果
上に行くほど防御に優れる

編集した場合はCAttackAndDamageReactionType::Evaluateの判定を修正して下さい
*/
UENUM(BlueprintType)
enum class EDefenseReactionResult : uint8
{
	None		UMETA(DisplayName = "何もしない"),
	Guard		UMETA(DisplayName = "防御に当たる"),
	GuardBreak	UMETA(DisplayName = "防御が崩れる"),
	Hit			UMETA(DisplayName = "当たる"),
	KnockedDown	UMETA(DisplayName = "ノックダウン"),
};


class CAttackAndDamageReactionType
{
public:
	static EDefenseReactionResult Evaluate(const EAttackReactionType attackReactionType, const bool guard);
};

/**
@}
*/
