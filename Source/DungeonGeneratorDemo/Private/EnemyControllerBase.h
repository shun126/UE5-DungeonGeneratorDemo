/**
AI Perceptionの向きはRoot Componentの回転を使用しているため、必ずしも頭の回転と一致していない事があります。
AI Perception を頭の向きと一致させる方法については以下のURLを参照して下さい。
https://qiita.com/Dv7Pavilion/items/0849201fcb8e3524ec25
*/

#pragma once
#include <AIController.h>
#include "EnemyControllerBase.generated.h"

class UAIPerceptionComponent;

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

private:
	UFUNCTION()
	void OnExpireTarget(const AActor* targetActor);

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

protected:
    // AIController overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	// IGenericTeamAgentInterface overrides
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	//! ターゲット失効を叫ぶイベント
	UPROPERTY(BlueprintAssignable)
	FEnemyAIControllerTargetSignature ExpireTarget;

private:
	// 知覚コンポーネント
	TObjectPtr<UAIPerceptionComponent> mAiPerceptionComponent;
};
