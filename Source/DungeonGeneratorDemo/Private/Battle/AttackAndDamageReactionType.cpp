/**
@author		Shun Moriya

攻撃とダメージのリアクションタイプに関するソースファイル
*/

#include "AttackAndDamageReactionType.h"
//#include "../../LegacyGameSingletonBase.h"
#include <array>

EDefenseReactionResult CAttackAndDamageReactionType::Evaluate(const EAttackReactionType attackReactionType, const bool guard)
{
	// EAttackReactionTypeとEDefenseReactionTypeが対応している事を期待しています
	static constexpr std::array<EDefenseReactionResult, 3> attackToHitMap = {
		{
			EDefenseReactionResult::KnockedDown,	// EAttackReactionResult::Down
			EDefenseReactionResult::Hit,			// EAttackReactionResult::Hit
			EDefenseReactionResult::None,			// EAttackReactionResult::Invalid
		}
	};
	static constexpr std::array<EDefenseReactionResult, 3> attackToGuardMap = {
		{
			EDefenseReactionResult::GuardBreak,		// EAttackReactionResult::Down
			EDefenseReactionResult::Guard,			// EAttackReactionResult::Hit
			EDefenseReactionResult::Guard,			// EAttackReactionResult::Invalid
		}
	};

	if (guard)
	{
		// 防御成功
		return attackToGuardMap.at(static_cast<uint8>(attackReactionType));
	}
	else
	{
		EDefenseReactionResult defenseReactionResult = attackToHitMap.at(static_cast<uint8>(attackReactionType));
#if 0
		// 防御失敗の時、装備品によるリアクション修正
		ULegacyGameSingletonBase* instance = ULegacyGameSingletonBase::Instance();
		check(instance != nullptr);

		const UArmorDataAsset& armorDataAsset = instance->GetArmorDataAsset();
		const FArmorData& armorData = armorDataAsset.Get(armorDataId);

		// 防御側の装備に従ってリアクションを補正
		if (armorData.DefenseReactionType == EDefenseReactionType::Hard)
		{
			int16_t value = static_cast<int16_t>(defenseReactionResult) - 1;
			if (value < 0)
				value = 0;
			defenseReactionResult = static_cast<EDefenseReactionResult>(value);
		}
		else if (armorData.DefenseReactionType == EDefenseReactionType::Soft)
		{
			int16_t value = static_cast<int16_t>(defenseReactionResult) + 1;
			if (value >= attackToHitMap.size())
				value = attackToHitMap.size() - 1;
			defenseReactionResult = static_cast<EDefenseReactionResult>(value);
		}
#endif
		return defenseReactionResult;
	}
}
