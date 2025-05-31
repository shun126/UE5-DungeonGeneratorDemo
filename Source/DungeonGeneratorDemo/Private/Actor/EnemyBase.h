/**
@author		Shun Moriya
*/

#pragma once
#include "DataAsset/DropDataId.h"
#include "Helper/DamageHistory.h"
#include <GameFramework/Character.h>
#include "EnemyBase.generated.h"

class UAnimMontage;
class UEnemyAnimInstance;
class UEnemyMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyBaseFireEventSignature);

UCLASS()
class AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AEnemyBase(const FObjectInitializer& objectInitializer);
	virtual ~AEnemyBase() override = default;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// アニメーションとモンタージュ
	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/**
	Plays an animation montage. Returns the length of the animation montage in seconds. Returns 0.f if failed to play.
	*/
	float PlayMontage(UAnimMontage* MontageToPlay, float InPlayRate = 1.f, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true) const;

	/**
	現在のモンタージュを逆再生します
	*/
	UFUNCTION(BlueprintCallable, Category = "Animation")
	UAnimMontage* ReverseMontage(float InPlayRate = 0.5f);

	/**
	指定のモンタージュの再生速度を取得します
	*/
	float GetPlayMontageRate(const UAnimMontage* AnimMontage) const;

	/**
	指定のモンタージュの再生速度を設定します
	*/
	void SetPlayMontageRate(const UAnimMontage* AnimMontage, float NewPlayRate = 1.f) const;

	/**
	指定のモンタージュが再生中か調べます
	*/
	bool IsPlayMontage(const UAnimMontage* animMontage) const;

	/**
	全てのモンタージュが再生中か調べます
	*/
	bool IsPlayAnyMontage() const;

	/**
	指定のモンタージュが次の更新で終了するか？
	または、既に終了しているか？
	*/
	bool IsLastFrameMontage(const UAnimMontage* animMontage, const float deltaTime) const;

	/**
	指定のモンタージュの再生時間を取得します
	*/
	float GetMontagePosition(const UAnimMontage* animMontage) const;

	/**
	現在再生中のモンタージュの再生時間を取得します
	*/
	float GetCurrentMontagePosition() const;

	/**
	モンタージュを一時停止します
	*/
	void PauseMontage(const UAnimMontage* animMontage) const;

	/**
	モンタージュを再開します
	*/
	void ResumeMontage(const UAnimMontage* animMontage) const;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// 移動
	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/**
	移動を歩行に切り替えます
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "移動")
	bool ChangeWalk();
	virtual bool ChangeWalk_Implementation();

	/**
	移動を全力疾走に切り替えます
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "移動")
	bool ChangeSprint();
	virtual bool ChangeSprint_Implementation();

	/**
	移動を掃射姿勢に切り替えます
	\param[in]	targetActor					nullptr以外なら指定アクターを注目する。nullptrならChangeWalkを呼び出します
	\param[in]	useControllerRotationYaw	trueならばコントローラーの方向にオーナーアクターを向ける
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "移動")
	bool ChangeStrafeToActor(const AActor* targetActor, const bool useControllerRotationYaw);
	virtual bool ChangeStrafeToActor_Implementation(const AActor* targetActor, const bool useControllerRotationYaw);

	/**
	移動を掃射姿勢に切り替えます
	\param[in]	targetLocation				ターゲットする座標
	\param[in]	useControllerRotationYaw	trueならばコントローラーの方向にオーナーアクターを向ける
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "移動")
	bool ChangeStrafeToLocation(const FVector& targetLocation, const bool useControllerRotationYaw);
	virtual bool ChangeStrafeToLocation_Implementation(const FVector& targetLocation, const bool useControllerRotationYaw);

	//ChangeCrouch;
	//ChangeSwim;

protected:
	/**
	掃射姿勢ロックオン中のアクターを設定します
	*/
	void SetStrafeToActor(const AActor* targetActor, const float dampingRatio = 0.5f);

	/**
	掃射姿勢ロックオン先の位置を設定します
	*/
	void SetStrafeToLocation(const FVector& targetLocation, const float dampingRatio = 0.5f);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// 転回
	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	/**
	アクターへ向く
	*/
	UFUNCTION(BlueprintCallable, Category = "方向")
	void TurnToActor(const AActor* targetActor, const float dampingRatio = 0.5f);

	/**
	指定座標へ向く
	*/
	UFUNCTION(BlueprintCallable, Category = "方向")
	void TurnToLocation(const FVector& targetLocation, const float dampingRatio = 0.5f);

	/**
	指定角度へ向く
	*/
	UFUNCTION(BlueprintCallable, Category = "方向")
	void TurnToYaw(const float targetDegree, const float dampingRatio = 0.5f);

	/**
	向くのをキャンセル
	*/
	UFUNCTION(BlueprintCallable, Category = "方向")
	void CancelTurn();

	/**
	方向転回中？
	*/
	bool IsTurning() const noexcept;


	/**
	掃射姿勢ロックオン中のアクターを取得します
	\return		ロックオン中のアクター
	*/
	UFUNCTION(BlueprintCallable, Category = "方向")
	const AActor* GetStrafeTarget() const;

private:
	/**
	体を転回させます
	*/
	void UpdateTurn(const float deltaSeconds);

public:

	void Alert(const AActor* actor);

private:
	void UpdateAim(const float deltaSeconds);

public:
	UFUNCTION(BlueprintCallable)
	void Dead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDropDataId DropType = EDropDataId::Invalid;

private:
	UFUNCTION(Server, Reliable)
	void DropObject();

	UFUNCTION(NetMulticast, Reliable)
	void DropObjectOnClient(TSubclassOf<AStockable> dropObjectClass);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// 通知
	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void NotifyBeginFire(const FName& notifyName);
	void NotifyEndFire(const FName& notifyName);

protected:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FEnemyBaseFireEventSignature OnBeginFire;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FEnemyBaseFireEventSignature OnEndFire;

public:
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Result"))
	void RegisterDamageHistory(EDamageHistoryRegisterResult& Result, const AActor* actor);

protected:
	UPROPERTY(EditAnywhere)
	FDamageHistory DamageHistory;

public:
	////////////////////////////////////////////////////////////////////////////////////////////////
	// コンポーネント
	////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	UAnimInstanceを取得します
	*/
	UAnimInstance* GetAnimInstance() const;

	/**
	UAvatarAnimInstanceを取得します
	*/
	UEnemyAnimInstance* GetEnemyAnimInstance() const;

	UEnemyMovementComponent* GetEnemyMovement() const;

	// AActor overrides
	//virtual void PostInitializeComponents() override;
	//virtual void BeginPlay() override;
	//virtual void Tick(float DeltaSeconds) override;
	//virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	void DebugInformation(TArray<FString>& output) const;

private:
	// 振り向き
	struct Turn final
	{
		//union Target final
		struct Target final
		{
			//! 注目アクター
			TWeakObjectPtr<const AActor> mActor;
			//! 注目座標
			FVector	mLocation;
			//! 転回する目標方向
			float mYawAngle;
		};
		Target mTarget;
		//! 転回速度比率
		float mSpeedRatio = 0.1f;
		//! 転回モード
		enum class Mode : uint8_t
		{
			None,				// 転回していない
			ToActorStrafe,		// 指定アクターの方向を向き続ける
			ToLocationStrafe,	// 指定座標の方向を向き続ける
			ToActorTurn,		// 指定アクターの方向を向いたら終了
			ToLocationTurn,		// 指定座標の方向を向いたら終了
			ToAngleTurn			// 指定角度を向いたら終了
		};
		//! 転回モード
		Mode mMode = Mode::None;
	};
	Turn mTurn;

	struct Aim final
	{
		//! 注目アクター
		TWeakObjectPtr<const AActor> mActor;
		float mPitch = 0.;
		float mYaw = 0.;
	};
	Aim mAim;
};
