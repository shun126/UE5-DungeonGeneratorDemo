/**
@author		Shun Moriya


*/

#pragma once
#include <Blueprint/UserWidget.h>
#include "PlayGameWidgetBase.generated.h"

/**
ゲームプレイ中のメインHUDベースクラス
*/
UCLASS(Abstract)
class UPlayGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UPlayGameWidgetBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UPlayGameWidgetBase() override = default;

	/**
	食料の量を取得
	*/
	UFUNCTION(BlueprintCallable)
	FText GetFoodValue() const noexcept;

	/**
	金の量を取得
	*/
	UFUNCTION(BlueprintCallable)
	FText GetGoldValue() const noexcept;

	/**
	称号を取得
	*/
	UFUNCTION(BlueprintCallable)
	FText GetHeroTitle() const noexcept;

	/**
	経験値（全ての経験値の総量）を取得
	*/
	UFUNCTION(BlueprintCallable)
	FText GetExperienceValue() const noexcept;

	/*
	装備中の武器の熟練度
	*/
	UFUNCTION(BlueprintCallable)
	FText GetSkillPercentOfEquippedWeapon() const noexcept;

	/*
	装備中の魔法の熟練度
	*/
	UFUNCTION(BlueprintCallable)
	FText GetSkillPercentOfEquippedMagic() const noexcept;

	/*
	装備中の鎧の熟練度
	*/
	UFUNCTION(BlueprintCallable)
	FText GetSkillPercentOfEquippedArmor() const noexcept;

	/*
	魔法を構えた時に呼ばれるイベント
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnHoldWeapon();

	/*
	魔法を下ろした時に呼ばれるイベント
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnLowerWeapon();

	// overrides
	virtual void BeginDestroy() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	bool mShowGunSight = false;
};



UCLASS(Blueprintable, BlueprintType)
class UPlayWidgetBase : public UPlayGameWidgetBase
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UPlayWidgetBase(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
	}

	/**
	デストラクタ
	*/
	virtual ~UPlayWidgetBase() = default;
};

