/**
@author		Shun Moriya

能力
*/

#pragma once
#include "Ability.generated.h"

/**
能力
*/
USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_BODY()

public:
	// 強さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Strength = 0;

	// 知識
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Wisdom = 0;

	// カリスマ性
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Charisma = 0;

	static constexpr uint8 Max = 200;

public:
	/*
	能力をリセットします
	*/
	void Reset();
};
