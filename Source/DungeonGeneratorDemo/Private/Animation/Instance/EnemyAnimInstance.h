/**
@author		Shun Moriya

敵アニメーションインスタンス ヘッダーファイル
*/

#pragma once
#include "Actor/EnemyBase.h"
#include <Animation/AnimInstance.h>
//#include <PoseSearch/PoseSearchLibrary.h>
#include <atomic>
#include <functional>
#include <mutex>
#include "EnemyAnimInstance.generated.h"

// 前方宣言
class UCharacterTrajectoryComponent;
class UInterestAimComponent;

//! UpdateFootIkのデリゲート型
//DECLARE_DYNAMIC_DELEGATE_ThreeParams(FFootIkSignature, int32, index, ECoordinate, Coordinate, float, offset);

/**
 * 移動方式
 */
UENUM(BlueprintType)
enum class UEnemyAnimMovementMode : uint8
{
	OnGround,
	OnWall,
	InAir,
};

/**
 * 身構え方
 */
UENUM(BlueprintType)
enum class UEnemyAnimMovementStance : uint8
{
	Stand,
	Crouch,
	Climb
};

/**
 * 移動状態
 */
UENUM(BlueprintType)
enum class UEnemyAnimMovementState : uint8
{
	Idle,
	Moving
};

/**
 * 歩き方
 */
UENUM(BlueprintType)
enum class UEnemyAnimMovementGait : uint8
{
	Walk,
	Run,
	Sprint,
};



/**
FootIK計算パラメータ
*/
USTRUCT(Blueprintable, BlueprintType)
struct FFootIkParameter
{
	GENERATED_BODY()

	//! エフェクターソケット名
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName SocketName;
#if 0
	//! 変更する座標
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ECoordinate Coordinate = ECoordinate::Z;
#endif
	//! エフェクターオフセットのスケール値
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Scale = 1.f;
};

/**
ポーンアニメーションインスタンスクラス
このクラスは以下の情報を扱っています。
- 移動速度および移動方向
- 頭のエイム角度
- 体の角度
- 走っているか？
- 空中にいるか？
- 水中にいるか？
- 足のIK情報
*/

/**
キャラクターアニメーションインスタンスクラス
このクラスは以下の情報を扱っています。
- 加速中最後の移動速度および移動角度
- 加速中か？
- 盾などの防御と通常状態の割合
*/


/**
アバターアニメーションインスタンスクラス
このクラスは以下の情報を扱っています。
- コンボ攻撃の通知とコンボ中か？
- 移動の種類（通常、全力疾走、中腰移動）
- 掃射姿勢か？
- 攻撃中か？
*/
UCLASS(Blueprintable, BlueprintType)
class UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UEnemyAnimInstance(const FObjectInitializer& objectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UEnemyAnimInstance() override = default;

	/**
	オーナーアバターを取得
	*/
	UFUNCTION(BlueprintCallable)
	AEnemyBase* TryGetEnemyOwner() const;

	/*
	 *
	 */

	 /**
	  * 歩行開始
	  */
	void ChangeWalk() noexcept;

	/**
	 * 全力疾走開始
	 */
	void ChangeSprint() noexcept;

	/**
	 * 中腰移動
	 * @param[in]	withStartAnimation	trueならば開始アニメーションを再生している
	 */
	void BeginCrouch(const bool withStartAnimation) noexcept;

	/**
	 * 歩行移動中か？
	 * @return		trueならば歩行移動中
	 */
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsWalkMove() const noexcept;

	/**
	 * 全力疾走中か？
	 * \return		trueならば全力疾走中
	 */
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsSprintMove() const noexcept;

	/**
	 * 中腰移動中か？
	 * @param[in]	withStartAnimation	trueならば開始アニメーションを再生しているか？
	 * @return		trueならばしゃがみ移動中
	 */
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsCrouchMoveStart(const bool withStartAnimation = true) const noexcept;

	/**
	 * 中腰移動中か？
	 * @return		trueならばしゃがみ移動中
	 */
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsCrouchMove() const noexcept;

	/**
	 * 中腰移動が終了したか？
	 * @return		trueならばしゃがみ移動終了
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsCrouchMoveEnd() const noexcept;

	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsFalling() const noexcept;

	void SetFalling(const bool falling) noexcept;

	/*
	 * 
	 */

	/**
	 * 移動速度を取得します
	 * @return		移動速度（0.0～）
	 */
	float GetMovementSpeed() const noexcept;

	/**
	 * 移動速度の比率を取得します
	 * @return		0.0～1.0
	 */
	float GetMovementSpeedRatio() const;

	/**
	 * 移動角度を取得します
	 * @return		移動角度（-180.0～180.0）
	 */
	float GetMovementAngle() const noexcept;

	/**
	 * 加速中最後の移動速度
	 */
	float GetLastAcceleratingSpeed() const;

	/**
	 * 加速中最後の移動角度
	 */
	float GetLastAcceleratingAngle() const;

	/**
	 * 加速中か？
	 */
	bool Accelerating() const;

	// アニメーションブループリントのヘルパー関数
	// from JogForward
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogForwardToBrakeForward() const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogForwardToBrakeBackward() const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogForwardToJogBackward() const noexcept;

	// from JogBackward
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogBackwardToBrakeForward() const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogBackwardToBrakeBackward() const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionJogBackwardToJogForward() const noexcept;

	// from BrakeForward
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionBrakeForwardToJogForward(const float animTimeRemaining) const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionBrakeForwardToJogBackward(const float animTimeRemaining) const noexcept;

	// from BrakeBackward
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionBrakeBackwardToJogBackward(const float animTimeRemaining) const noexcept;

	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTransitionBrakeBackwardToJogForward(const float animTimeRemaining) const noexcept;

	/*
	待機から走り開始へ遷移可能？
	*/
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTranslationWaitToJog() const;

	/*
	走り開始から走りへ遷移可能？
	*/
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTranslationJogToWait(const float animTimeRemaining) const;


#if 0
	UFUNCTION(BlueprintPure, Category = "Legacy|Aiming", meta = (BlueprintThreadSafe))
	EPoseSearchInterruptMode GetMotionMatchingInterruptMode() const noexcept;
#endif

protected:
	/**
	アニメーション再生の為の最大移動速度を更新
	再生レート ＝ 移動速度 ÷ アニメーション再生の為の最大移動速度
	*/
	virtual bool UpdateMoveAnimationSpeedRatio(float& movementSpeed, float& movementSpeedRatio, float& movementDegree) const;

#if 0
	/**
	FootIKの為のエフェクターオフセットを計算します (deprecated)
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateFootIk(const TArray<FFootIkParameter>& footIKParameter, FFootIkSignature footIkDelegate);
#endif

	/*
	 * エイミング
	 */
public:
	/**
	頭のエイミング角度を取得します (deprecated)
	\return		頭の角度
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|Aiming", meta = (BlueprintThreadSafe))
	FRotator GetHeadAimRotation() const noexcept;

	/**
	頭のエイミング角度を設定します
	\param[in]	pitch	頭のピッチ角
	\param[in]	yaw		頭のヨー角
	*/
	void SetHeadAimRotation(const float pitch, const float yaw) noexcept;

protected:
	/**
	エフェクターのオフセットを計算します
	\param[in]	ownerActor		オーナーアクター
	\param[in]	socketX			ソケットのX座標
	\param[in]	socketY			ソケットのY座標
	\param[in]	fromLocationZ	アクターのルートZ座標
	\param[in]	toLocationZ		最も低いソケットのZ座標
	\return		ソケットのZ座標のオフセット
	*/
	float TryGetFootOffset(const AActor* ownerActor, const float socketX, const float socketY, const float fromLocationZ, const float toLocationZ) const;

public:
	/**
	 * 足IKは有効か？
	 */
	bool IsEnableFootIK() const noexcept;

	/**
	 * 足IKは有効か？
	 */
	void EnableFootIK(const bool enable) noexcept;






public:
	/**
	警戒状態か設定
	*/
	void Alert(const bool alert);

	/**
	警戒状態か設定
	*/
	bool IsInAlert() const noexcept;

	// 盾などで防御しているか？
	bool IsGuarding() const noexcept;

	// 盾などで防御する
	UFUNCTION(BlueprintCallable)
	void Guard(const bool guard) noexcept;

	// オーナーキャラクターを取得
	UFUNCTION(BlueprintCallable)
	ACharacter* TryGetCharacterOwner() const;
#if 0
	// ダメージリアクションを開始
	UFUNCTION(BlueprintCallable, Category = "Legacy|Reaction")
	void EnterDamageReaction(EDefenseReactionResult defenseReactionResult);

	// ダメージリアクションを終了
	UFUNCTION(BlueprintCallable, Category = "Legacy|Reaction")
	void ExitDamageReaction();
#endif


	/*
	通常待機から戦闘待機へ遷移可能？
	*/
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTranslationNormalToAlert() const;

	/*
	戦闘待機から通常待機へ遷移可能？
	*/
	UFUNCTION(BlueprintCallable, Category = "Legacy|Transition", meta = (BlueprintThreadSafe))
	bool CanTranslationAlertToNormal() const;

#if 0
	/**
	オーナーアバターの武器タイプを取得します
	*/
	EWeaponAnimationType GetWeaponAnimationType() const;

	/**
	オーナーアバターの武器タイプが等しいか判定します
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsWeaponAnimationType(const EWeaponAnimationType weaponType) const;

	/**
	オーナーアバターの武器タイプが等しくないか判定します
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsNotWeaponAnimationType(const EWeaponAnimationType weaponType) const;
#endif

	/**
	攻撃開始
	UAttackAnimNotifyState::NotifyBeginが呼び出します
	*/
	void RetainFireCount() noexcept;

	/**
	攻撃終了
	UAttackAnimNotifyState::NotifyEndが呼び出します
	*/
	void ReleaseFireCount() noexcept;

	/**
	UAttackAnimNotifyStateが示す攻撃期間中か？
	\return		trueならば攻撃中
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsAttacking() const noexcept;


	/**
	コンボ攻撃許可開始
	*/
	void RetainComboAcceptCount() noexcept;

	/**
	コンボ攻撃許可終了
	*/
	void ReleaseComboAcceptCount() noexcept;

	/**
	コンボ攻撃許可中か？
	\return		trueならばコンボ攻撃許可中
	*/
	UFUNCTION(BlueprintPure, Category = "Legacy|State", meta = (BlueprintThreadSafe))
	bool IsComboAccepting() const noexcept;

	/**
	コンボ攻撃許可開始コールバック
	*/
	void OnComboAcceptBegin(const std::function<void()>& func);

	/**
	コンボ攻撃許可終了コールバック
	*/
	void OnComboAcceptEnd(const std::function<void()>& func);

	/**
	コントロールリグの有効値を取得
	TODO:コントロールリグの実装を確認して、再度この関数が適切か検討してください
	*/
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	float GetEnemyControlRigBlendRatio() const;

#if WITH_EDITOR
	/**
	移動モードを取得します
	*/
	const FString& GetMovingType() const;

	// UEnemyAnimInstance overrides
	virtual void DebugInformation(TArray<FString>& output) const;
#endif

private:
#if 0
	/**
	オーナーアバターの武器タイプを設定します
	*/
	void SetWeaponAnimationType(const EWeaponAnimationType weaponType) noexcept;
#endif

protected:
	// UAnimInstance overrides
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float deltaSeconds) override;

protected:
#if 0
	//! 親のCharacterTrajectory Component
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectoryComponent;

	//! 頭のエイム情報
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInterestAimComponent> mInterestAimComponent;
#endif
	//! 移動速度 (X,Y方向の移動速度)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 0.f;

	//! 移動速度の比率 (X,Y方向の移動速度)
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeedRatio = 0.f;

	//! 移動角度
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Movement", meta = (AllowPrivateAccess = "true"))
	float MovementAngle = 0.f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	UEnemyAnimMovementMode MovementMode = UEnemyAnimMovementMode::OnGround;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	UEnemyAnimMovementStance MovementStance = UEnemyAnimMovementStance::Stand;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	UEnemyAnimMovementState MovementState = UEnemyAnimMovementState::Idle;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	UEnemyAnimMovementGait MovementGait = UEnemyAnimMovementGait::Walk;

	UEnemyAnimMovementMode LastMovementMode = UEnemyAnimMovementMode::OnGround;
	UEnemyAnimMovementStance LastMovementStance = UEnemyAnimMovementStance::Stand;
	UEnemyAnimMovementState LastMovementState = UEnemyAnimMovementState::Idle;
	UEnemyAnimMovementGait LastMovementGait = UEnemyAnimMovementGait::Walk;

	//! 頭のエイム角度
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Aiming", meta = (AllowPrivateAccess = "true"))
	float AimYaw = 0.f;

	//! 頭のエイム角度
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Aiming", meta = (AllowPrivateAccess = "true"))
	float AimPitch = 0.f;

	//! 体の角度
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Turn", meta = (AllowPrivateAccess = "true"))
	float TurnYaw = 0.f;

	//! 空中にいるか？
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	bool IsInAir = false;

	//! 水中にいるか？
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	bool IsInWater = false;

	//! 警戒中？
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	bool InAlert = false;

	//! 盾などの防御と通常状態の割合
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	bool Guarding = false;

	//! 加速中の移動速度（速度を補完した速度を保存）
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Movement", meta = (AllowPrivateAccess = "true"))
	float LastAcceleratingSpeed = 0.f;

	//! 加速中の移動角度（25%以上の速度で移動していた時の角度を保存）
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Movement", meta = (AllowPrivateAccess = "true"))
	float LastAcceleratingAngle = 0.f;

	//! 加速中？（MovementComponentのAccelerationが０より大きい）
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|State", meta = (AllowPrivateAccess = "true"))
	bool IsAccelerating = false;
#if 0
	// 武器のタイプ
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponAnimationType WeaponAnimationType = EWeaponAnimationType::Invalid;
#endif

#if 0
	// ダメージの状態
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Legacy|Reaction", meta = (AllowPrivateAccess = "true"))
	EDefenseReactionResult DefenseReactionResult = EDefenseReactionResult::None;
#endif

#if 0
	// 足のIK情報 (deprecated)
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	FFootIkSignature FootIkDelegate;
#endif


private:
	//! コンボ攻撃許可開始コールバック
	std::function<void()> mOnComboAcceptBegin;

	//! コンボ攻撃許可終了コールバック
	std::function<void()> mOnComboAcceptEnd;

	//! コンボ攻撃許可回数のミューテックス
	mutable std::mutex mComboAcceptCountMutex;

	//! 移動の種類
	enum struct MovingType : uint8_t
	{
		Walk,			//!< 通常
		Sprint,			//!< 全力疾走
		Crouch,			//!< 中腰移動
		QuickCrouch,	//!< 中腰移動（開始キャンセル）
	};

	//! 移動状態
	std::atomic_uint8_t mMovingType = { static_cast<uint8_t>(MovingType::Walk) };

	//! 攻撃中か？
	std::atomic_uint8_t mAttackCount = {0};

	//! 落下中か？
	std::atomic_bool mIsFalling = false;

	//! コンボ攻撃許可数
	uint8_t mComboAcceptCount = { 0 };

	// 足のIK
	bool mEnableFootIK = true;

	//friend void AEnemyBase::SetWeaponAnimationType(const EWeaponAnimationType weaponType, const bool force) noexcept;
};


inline float UEnemyAnimInstance::GetMovementSpeed() const noexcept
{
	return MovementSpeed;
}

inline float UEnemyAnimInstance::GetMovementSpeedRatio() const
{
	return MovementSpeedRatio;
}

inline float UEnemyAnimInstance::GetMovementAngle() const noexcept
{
	return MovementAngle;
}

inline void UEnemyAnimInstance::Alert(const bool alert)
{
	InAlert = alert;
}

inline bool UEnemyAnimInstance::IsInAlert() const noexcept
{
	return InAlert;
}


inline void UEnemyAnimInstance::Guard(const bool guard) noexcept
{
	Guarding = guard;
}

inline bool UEnemyAnimInstance::IsGuarding() const noexcept
{
	return Guarding;
}

inline bool UEnemyAnimInstance::IsEnableFootIK() const noexcept
{
	return mEnableFootIK;
}

inline void UEnemyAnimInstance::EnableFootIK(const bool enable) noexcept
{
	mEnableFootIK = enable;
}

#if 0
inline EWeaponAnimationType UEnemyAnimInstance::GetWeaponAnimationType() const
{
	return WeaponAnimationType;
}

inline bool UEnemyAnimInstance::IsWeaponAnimationType(const EWeaponAnimationType weaponType) const
{
	return GetWeaponAnimationType() == weaponType;
}

inline bool UEnemyAnimInstance::IsNotWeaponAnimationType(const EWeaponAnimationType weaponType) const
{
	return GetWeaponAnimationType() != weaponType;
}

inline void UEnemyAnimInstance::SetWeaponAnimationType(const EWeaponAnimationType weaponType) noexcept
{
	WeaponAnimationType = weaponType;
}
#endif

