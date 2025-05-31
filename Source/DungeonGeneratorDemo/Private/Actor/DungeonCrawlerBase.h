/**
@author		Shun Moriya

ダンジョンクローラーアクター

クライアント側でナビゲーションメッシュを有効にするには
「プロジェクト設定＞ナビゲーションシステム＞クライアント側のナビゲーションメッシュを許可」の設が必要
https://forums.unrealengine.com/t/allow-client-side-navigation-walks-in-place/361878/4
*/

#pragma once
#include <GameFramework/Character.h>
#include "DungeonCrawlerBase.generated.h"

// 定義するとジャンプをします
//#define DUNGEON_CRAWLER_BASE_ENABLE_JUMP

class ADungeonGenerateActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDungeonCrawlerBaseEventSignature);

UCLASS(Blueprintable, BlueprintType)
class ADungeonCrawlerBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ADungeonCrawlerBase(const FObjectInitializer& objectInitializer);
	virtual ~ADungeonCrawlerBase() override = default;

public:
	// AActor overrides
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

private:
	void TickImpl(float deltaSeconds);
	void StateInitialize();
	void StateWaitForNavigationGeneration(const float deltaSeconds);
	void StatePatrol(const float deltaSeconds);

	void ShuffleDungeonRoomSensorBases();
	TArray<FVector> FindPathToLocation(const FVector& location) const;

protected:
	// 停止したと判定する最低速度（cm/秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	double LowestSpeed = 10.0;

	// 停止したと判定する時間（秒）
	UPROPERTY(EditAnywhere)
	float StoppingTime = 3.f;

	// 到達失敗通知
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDungeonCrawlerBaseEventSignature OnReachFailed;

	// 巡回完了通知
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FDungeonCrawlerBaseEventSignature OnPatrolCompleted;

private:
	TObjectPtr<ADungeonGenerateActor> mDungeonGenerateActor;
	TArray<AActor*> mDungeonRoomSensorBases;
	int32 mDungeonRoomSensorBaseIndex = 0;

	float mIdleTimer;
	float mCameraBoomTimer = 0.f;
	bool mCameraBoomDirection = true;

	enum class AiStatus : uint8
	{
		SetNextLocation,
		WaitReachLocation,
		Idle,
	};
	AiStatus mAiStatus = AiStatus::SetNextLocation;
	TArray<FVector> mPathPoints;
	FVector mLastActorLocation;
	float mStackElapsedTime = 0.f;

	FRotator mRotation;
	float mSpeed = 0.f;

#if defined(DUNGEON_CRAWLER_BASE_ENABLE_JUMP)
	float mJump = 0.f;
#endif

	enum class InitializeState : uint8_t
	{
		Initialize,
		WaitForNavigationGeneration,
		Patrol,
	};
	InitializeState mInitializeState = InitializeState::Initialize;
};
