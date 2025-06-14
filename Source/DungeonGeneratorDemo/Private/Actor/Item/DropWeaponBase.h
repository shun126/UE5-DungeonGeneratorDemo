/**
 * ドロップ武器基底クラス
 */

#pragma once
#include "Stockable.h"
#include "DataAsset/WeaponDataId.h"
#include "DropWeaponBase.generated.h"

/**
 * 武器基底クラス
 */
UCLASS(Abstract)
class ADropWeaponBase : public AStockable
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit ADropWeaponBase(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~ADropWeaponBase() override = default;

	/**
	武器IDを取得します
	*/
	EWeaponDataId GetId() const;

	/**
	武器IDを設定します
	*/
	void SetId(const EWeaponDataId weaponDataId);

	/**
	この武器を拾って所持品に加える
	\param[in]	finderPawn		拾ったポーン
	*/
	UFUNCTION()
	void PickUp(const APawn* finderPawn);

	/**
	攻撃コリジョンのプロファイルを切り替えます
	\param[in]	playerAttack	trueならばプレイヤーの攻撃、falseなら敵の攻撃
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeAttackCollisionProfile(const bool playerAttack);

	/**
	攻撃コリジョンの有効化
	\param[in]	enable		trueならば有効化、falseなら無効化
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAttackCollisionEnabled(const bool enable);

	/**
	ダメージコリジョンのプロファイルを切り替えます
	\param[in]	playerDamage	trueならばプレイヤーのダメージ、falseなら敵のダメージ
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChangeDamageCollisionProfile(const bool playerDamage);

	/**
	ダメージコリジョンの有効化
	\param[in]	enable		trueならば有効化、falseなら無効化
	*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageCollisionEnabled(const bool enable);

	// UAvatarComponent overrides
	virtual FText GetLabel_Implementation() const override;

	// AStockable overrides
	virtual void Equip_Implementation(const bool playerAttack) override;
	virtual void UnEquip_Implementation() override;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponDataId WeaponDataId = EWeaponDataId::Invalid;
};
