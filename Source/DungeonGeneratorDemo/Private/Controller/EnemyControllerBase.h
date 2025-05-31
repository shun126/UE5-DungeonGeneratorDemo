/**
@author		Shun Moriya

AI Perceptionの向きはRoot Componentの回転を使用しているため、必ずしも頭の回転と一致していない事があります。
AI Perception を頭の向きと一致させる方法については以下のURLを参照して下さい。
https://qiita.com/Dv7Pavilion/items/0849201fcb8e3524ec25
*/

#pragma once
#include <AIController.h>
#include "EnemyControllerBase.generated.h"

class UAIPerceptionPlusComponent;

//! 動的マルチキャストイベントの宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyAIControllerTargetSignature, const AActor*, Target);

UCLASS(Blueprintable, BlueprintType)
class AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AEnemyControllerBase(const FObjectInitializer& ObjectInitializer);
	virtual ~AEnemyControllerBase() override = default;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetHeadLocRot(FVector& OutLocation, FRotator& OutRotation) const;

	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	const FFloatInterval& GetAttackPreparationDistanceRange() const noexcept;


	int32 GetCharacterType() const;

	UFUNCTION(BlueprintCallable)
	void Dead();

private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void OnHandleExpiredStimulus(FAIStimulus stimulus);

protected:
    // AIController overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	// IGenericTeamAgentInterface overrides
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
#if WITH_EDITOR
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Debug
	////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void DebugInformation(TArray<FString>& output) const;
#endif

protected:
	/*
	 * 攻撃準備の為のターゲットとの距離
	 * AttackDistanceRange < AttackWaittingDistanceRange < AttackPreparationDistanceRange になるようにしてください
	 */
	UPROPERTY(EditAnywhere)
	FFloatInterval AttackPreparationDistanceRange = { 600.f, 700.f };

	//! ターゲット失効を叫ぶイベント
	UPROPERTY(BlueprintAssignable)
	FEnemyAIControllerTargetSignature ExpireTarget;

	UPROPERTY()
	int32 CharacterType = 0;

private:
	// 知覚コンポーネント
	TObjectPtr<UAIPerceptionPlusComponent> mAiPerceptionComponent;
};
