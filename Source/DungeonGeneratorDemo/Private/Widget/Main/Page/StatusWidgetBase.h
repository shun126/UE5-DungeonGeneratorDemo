/**
@author		Shun Moriya


*/

#pragma once
#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "StatusWidgetBase.generated.h"

/**
ステータスベースクラス
*/
UCLASS(Abstract)
class UStatusWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FText GetHeroTitle() const noexcept;

	UFUNCTION(BlueprintPure)
	FText GetEquippedWeaponName() const noexcept;

	// 強さ
	UFUNCTION(BlueprintPure)
	int32 GetStrength() const noexcept;

	// 知識
	UFUNCTION(BlueprintPure)
	int32 GetWisdom() const noexcept;

	// カリスマ性
	UFUNCTION(BlueprintPure)
	int32 GetCharisma() const noexcept;

	UFUNCTION(BlueprintPure)
	int32 GetKeyCount() const noexcept;

	// ユニーク鍵（ボス鍵）
	UFUNCTION(BlueprintPure)
	bool HasUniqueKey() const noexcept;

	UFUNCTION(BlueprintPure)
	int32 GetGold() const noexcept;

	UFUNCTION(BlueprintPure)
	int32 GetFood() const noexcept;

	UFUNCTION(BlueprintPure)
	int32 GetExperience() const noexcept;

	// レベル
	UFUNCTION(BlueprintPure)
	int32 GetLevel() const noexcept;


	UFUNCTION(BlueprintPure)
	int32 GetHitPoint() const noexcept;

	UFUNCTION(BlueprintPure)
	int32 GetMaxHitPoint() const noexcept;
};
