/**
@author		Shun Moriya

敵の移動コンポーネントクラス ヘッダーファイル

`MovementComp->UseAccelerationForPathFollowing()`または
`Movement::bUseAccelerationForPaths`を有効にすると移動に慣性が加味される
*/

#pragma once
#include <GameFramework/CharacterMovementComponent.h>
#include <bitset>
#include "EnemyMovementComponent.generated.h"

/**
移動モード
*/
UENUM(BlueprintType)
enum class EEnemyMovementMode : uint8
{
	Walk,
	Sprint,
	Strafe,
	Crouch,
};
static constexpr size_t EAvatarMovementModeSize = 4;

/**
アバターの移動パラメータ―
*/
USTRUCT(BlueprintType)
struct FEnemyMovementParameter
{
	GENERATED_BODY()

	// 移動の最大速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float MoveMaxSpeed;

	// 回転速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationRate;

	// コンストラクタ
	FEnemyMovementParameter() = default;
	FEnemyMovementParameter(const float moveMaxSpeed, const FRotator& rotationRate);
	
	// 設定
	void Set(const float moveMaxSpeed, const FRotator& rotationRate);
};

/**
アバターの移動コンポーネントクラス
歩きや疾走などモードにあわせてCharacterMovementComponentのパラメータを切り替えます
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UEnemyMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/**
	コンストラクタ
	*/
	explicit UEnemyMovementComponent(const FObjectInitializer& ObjectInitializer);

	/**
	デストラクタ
	*/
	virtual ~UEnemyMovementComponent() override = default;

public:
	/**
	歩行開始
	@return		trueならばモード、Movementのパラメータを切り替えた
	*/
	virtual bool ChangeWalk();

	/**
	全力疾走開始
	@return		trueならばモード、Movementのパラメータを切り替えた
	*/
	virtual bool ChangeSprint();

	/**
	掃射姿勢開始
	@param[in]	useControllerRotationYaw	trueならばコントローラーの方向にオーナーアクターを向ける
	@return		trueならばモード、Movementのパラメータを切り替えた
	*/
	virtual bool ChangeStrafe(const bool useControllerRotationYaw = false);

	/**
	中腰移動開始
	ACharacter::Crouchを利用します
	*/
	virtual bool ChangeCrouch();

	/**
	現在の移動モードを判定します
	@param[in]	avatarMovementMode	EEnemyMovementMode
	@return		trueならば現在の移動モード
	*/
	bool Is(const EEnemyMovementMode avatarMovementMode) const noexcept;

	/**
	移動モードの有効性を取得します
	@param[in]		enable		EEnemyMovementMode
	@return		trueならば移動モードは有効
	*/
	bool IsEnable(const EEnemyMovementMode mode) const noexcept;

	/**
	移動モードの有効性を設定します
	@param[in]		enable		EEnemyMovementMode
	*/
	void SetEnable(const EEnemyMovementMode mode, const bool enable);

	/**
	最大移動速度を取得
	（現在の速度はGetMaxSpeed()で取得）
	*/
	float GetMoveMaxSpeed(const EEnemyMovementMode mode) const;

	/**
	デモ中の回転比率を取得します
	@return		掃射姿勢回転比率
	*/
	float GetDemoTargetTurnRatio() const;

	/**
	掃射姿勢回転比率を取得します
	@return		掃射姿勢回転比率
	*/
	float GetStrafeTargetTurnRatio() const;

protected:
	bool ChangeAvatarMovementMode(const EEnemyMovementMode avatarMovementMode, const FEnemyMovementParameter& avatarMovementParameter, const bool useControllerRotationYaw);
	EEnemyMovementMode GetLastAvatarMovementMode() const noexcept;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Debug
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	オーナーの名前を取得します
	@return		オーナーの名前。オーナー未設定の場合は自信の名前を返します。
	*/
	FString GetOwnerName() const;

#if WITH_EDITOR
	/**
	移動モードを取得します
	*/
	const FString& GetAvatarMovementMode() const;

	// AAvatar
	virtual void DebugInformation(TArray<FString>& output) const;
#endif

	// overrides
	virtual void BeginPlay() override;

protected:
	/**
	 * 歩き（通常移動）の移動速度
	 */
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Character Movement: Walking")
	FEnemyMovementParameter WalkMove;

	/**
	 * 掃射姿勢移動速度
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	FEnemyMovementParameter StrafeMove;

	/**
	 * 全力疾走の移動速度
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	FEnemyMovementParameter SprintMove;

	/**
	 * 中腰移動の回転速度
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Rotation Settings)")
	FRotator CrouchRotationRate;

	/**
	デモ中の回転比率
	AAvatarで回転していてUAvatarMovementComponentでは回転しないがパラメータがここにあると編集しやすいので
	回転の比率を定義しています
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Rotation Settings)")
	float DemoTargetTurnRatio = 0.1f;

	/**
	掃射姿勢回転比率
	AAvatarで回転していてUAvatarMovementComponentでは回転しないがパラメータがここにあると編集しやすいので
	回転の比率を定義しています
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (Rotation Settings)")
	float StrafeTargetTurnRatio = 0.2f;

	/*
	中腰移動速度は movement->MaxWalkSpeedCrouched が適用されるので
	FEnemyMovementParameter CrouchMove の定義は不要

	中腰移動回転速度はplayer::action::Crouch内で随時更新します
	*/

private:
	// EAvatarMovementModeの許可フラグ
	std::bitset<EAvatarMovementModeSize> mAvatarMovementModeEnable;

	EEnemyMovementMode mAvatarMovementMode;
	EEnemyMovementMode mLastAvatarMovementMode = EEnemyMovementMode::Strafe;
};

inline bool UEnemyMovementComponent::Is(const EEnemyMovementMode avatarMovementMode) const noexcept
{
	return mAvatarMovementMode == avatarMovementMode;
}
