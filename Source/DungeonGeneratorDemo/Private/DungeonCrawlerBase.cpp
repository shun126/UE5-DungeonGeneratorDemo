/**
@author		Shun Moriya
@copyright	2024- Shun Moriya
All Rights Reserved.

ダンジョンクローラーアクター
*/

#include "DungeonCrawlerBase.h"
#include <DungeonGenerateActor.h>
#include <SubActor/DungeonRoomSensorBase.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

/*
 * 定義するとクライアントのプレイヤーも巡回します
 *
 * クライアント側でナビゲーションメッシュを有効にするには
 * 「プロジェクト設定＞ナビゲーションシステム＞クライアント側のナビゲーションメッシュを許可」の設定が必要
 * https://forums.unrealengine.com/t/allow-client-side-navigation-walks-in-place/361878/4
 */
#define ENABLE_MULTI_CRAWLER

#if WITH_EDITOR
/*
有効にするとデバッグ情報を描画します
*/
#define _ENABLE_LOCAL_DEBUG_
#endif

namespace
{
	template<typename T>
	static constexpr T RoundDegreeWithSign(const T degree)
	{
		// https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
		if (degree > static_cast<T>(0.))
			return degree - static_cast<T>(360.) * std::floor((degree + static_cast<T>(180.)) * static_cast<T>(1. / 360.));
		else
			return degree + static_cast<T>(360.) * std::floor((degree - static_cast<T>(180.)) * static_cast<T>(1. / -360.));
	}

	template<typename T>
	static constexpr T ChaseRatio(const T ratio, const T deltaSeconds) noexcept
	{
		constexpr T baseTime = static_cast<T>(1.) / static_cast<T>(60.);
		const T frameScale = deltaSeconds / baseTime;
		return static_cast<T>(1.) - std::pow(static_cast<T>(1.) - ratio, frameScale);
	}
}

ADungeonCrawlerBase::ADungeonCrawlerBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	// Tick Enable
	PrimaryActorTick.bCanEverTick = PrimaryActorTick.bStartWithTickEnabled = true;

	// Replicates
	bReplicates = true;

	// 移動情報のリプリケートを有効化
	SetReplicateMovement(true);

	if (auto component = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		component->bUseControllerDesiredRotation = true;
		component->bOrientRotationToMovement = true;
		component->RotationRate.Yaw = 360 * 2;
		bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;
	}
}

void ADungeonCrawlerBase::BeginPlay()
{
	// 親クラスの呼び出し
	Super::BeginPlay();

	// アクターの回転を記録
	mRotation = GetActorRotation();
}

void ADungeonCrawlerBase::Tick(float deltaSeconds)
{
	// 親クラスの呼び出し
	Super::Tick(deltaSeconds);

	// サーバーなら巡回する
	if (HasAuthority()
#if defined(ENABLE_MULTI_CRAWLER)
		|| IsPlayerControlled()
#endif
	){
		TickImpl(deltaSeconds);
	}
}

void ADungeonCrawlerBase::TickImpl(float deltaSeconds)
{
	if (mInitializeState == InitializeState::Initialize)
	{
		UGameplayStatics::GetAllActorsOfClass(this, ADungeonRoomSensorBase::StaticClass(), mDungeonRoomSensorBases);
		if (mDungeonRoomSensorBases.IsEmpty() == false)
		{
			ShuffleDungeonRoomSensorBases();
			mInitializeState = InitializeState::WaitForNavigationGeneration;
		}

		if (bReplicates)
		{
			TArray<AActor*> dungeonGenerateActors;
			UGameplayStatics::GetAllActorsOfClass(this, ADungeonGenerateActor::StaticClass(), dungeonGenerateActors);
			if (dungeonGenerateActors.IsEmpty() == false)
			{
				mDungeonGenerateActor = Cast<ADungeonGenerateActor>(dungeonGenerateActors[0]);
			}
		}
	}
	if (mInitializeState == InitializeState::WaitForNavigationGeneration)
	{
		if (UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		{
			if (navSys->IsNavigationBuildInProgress() == false)
				mInitializeState = InitializeState::Patrol;
		}
	}
	else
	{
		switch (mAiStatus)
		{
		case AiStatus::SetNextLocation:
		{
			auto dungeonRoomSensorBase = Cast<ADungeonRoomSensorBase>(mDungeonRoomSensorBases[mDungeonRoomSensorBaseIndex]);
			if (IsValid(dungeonRoomSensorBase))
			{
				FVector location;
				FVector extent;
				dungeonRoomSensorBase->GetActorBounds(true, location, extent);
				location.Z -= extent.Z;

				TArray<FVector> pathPoints = FindPathToLocation(location);
				if (pathPoints.IsEmpty() == false)
				{
					mPathPoints = pathPoints;
				}
				else
				{
					mPathPoints.Add(location);
				}
				mAiStatus = AiStatus::WaitReachLocation;
			}
		}
		break;

		case AiStatus::WaitReachLocation:
			if (FVector::Dist2D(GetActorLocation(), mPathPoints[0]) <= 0.75 * 100)
			{
				mPathPoints.RemoveAt(0);
				if (mPathPoints.IsEmpty())
				{
					++mDungeonRoomSensorBaseIndex;
					if (mDungeonRoomSensorBaseIndex >= mDungeonRoomSensorBases.Num())
					{
						mDungeonRoomSensorBaseIndex = 0;
						ShuffleDungeonRoomSensorBases();
						OnPatrolCompleted.Broadcast();
					}
					mIdleTimer = 0;
					mAiStatus = AiStatus::Idle;
				}
			}
			if (mPathPoints.IsEmpty() == false)
			{
				static float RotationSpeed = 360.f * 2.f;


				const FVector& currentLocation = GetActorLocation();
				const FVector& nextLocation = mPathPoints[0];
				double dx = nextLocation.X - currentLocation.X;
				double dy = nextLocation.Y - currentLocation.Y;
				double dl = std::sqrt(dx * dx + dy * dy);
				if (dl > 0.0)
				{
					const double normalize = 1.0 / dl;
					dx *= normalize;
					dy *= normalize;
					const double targetYaw = std::atan2(dy, dx) * 57.295779513082320876798154814105;
					double deltaYaw = RoundDegreeWithSign(targetYaw - mRotation.Yaw);

					deltaYaw *= ChaseRatio(0.1f, deltaSeconds);
					mRotation.Yaw += deltaYaw;
				}

				double speed = dl / (5.0 * 100.0);
				if (speed < 0.5)
					speed = 0.5;
				else if (speed > 1.)
					speed = 1.;

				// キャラクターの移動
				{
					const FVector& movement = mRotation.Vector();
					AddMovementInput(movement, speed, true);
#if defined(_ENABLE_LOCAL_DEBUG_)
					UKismetSystemLibrary::DrawDebugArrow(GetWorld(), currentLocation, currentLocation + movement * 100, 20, FLinearColor::Green, 0, 3.0f);
#endif
				}

				// カメラの回転
				if (false)
				{
					double deltaDegree;
					if (mCameraBoomDirection)
						deltaDegree = GetControlRotation().Yaw - (mRotation.Yaw + 180.);
					else
						deltaDegree = GetControlRotation().Yaw - mRotation.Yaw;
					deltaDegree = RoundDegreeWithSign(deltaDegree);
#if 0
					deltaDegree *= ChaseRatio(0.1f, deltaSeconds);
					AddControllerYawInput(deltaDegree * deltaSeconds);
#else
					static float ControlRotationSpeed = 11.25f;
					if (deltaDegree > 1)
					{
						if (deltaDegree > ControlRotationSpeed * deltaSeconds)
							deltaDegree = ControlRotationSpeed * deltaSeconds;
					}
					else if (deltaDegree < -1)
					{
						if (deltaDegree < -ControlRotationSpeed * deltaSeconds)
							deltaDegree = -ControlRotationSpeed * deltaSeconds;
					}
					AddControllerYawInput(deltaDegree);
#endif
					mCameraBoomTimer += deltaSeconds;
					if (mCameraBoomTimer >= 20.f)
					{
						mCameraBoomTimer = 0.f;
						mCameraBoomDirection = !mCameraBoomDirection;
					}
				}

#if defined(_ENABLE_LOCAL_DEBUG_)
				// 経路の表示
				UKismetSystemLibrary::DrawDebugArrow(GetWorld(), currentLocation, mPathPoints[0], 100.0f, FLinearColor::Red, 0, 3.0f);
				for (int32 i = 1; i < mPathPoints.Num(); ++i)
				{
					UKismetSystemLibrary::DrawDebugArrow(GetWorld(), mPathPoints[i - 1], mPathPoints[i], 100.0f, FLinearColor::Yellow, 0, 3.0f);
				}
#endif

				// 何かに引っかかった時の対処
				if (FVector::Distance(mLastActorLocation, currentLocation) < (LowestSpeed * deltaSeconds))
				{
					mStackElapsedTime += deltaSeconds;
					if (mStackElapsedTime >= StoppingTime)
					{
						mStackElapsedTime = 0.f;
						++mDungeonRoomSensorBaseIndex;
						if (mDungeonRoomSensorBaseIndex >= mDungeonRoomSensorBases.Num())
							mDungeonRoomSensorBaseIndex = 0;
						mAiStatus = AiStatus::SetNextLocation;

						OnReachFailed.Broadcast();
					}
				}
				else
				{
					mStackElapsedTime = 0.f;
				}

				// 現在の位置を記録
				mLastActorLocation = currentLocation;
			}

			break;

		case AiStatus::Idle:
			mIdleTimer += deltaSeconds;
			if (mIdleTimer > 2)
			{
				mAiStatus = AiStatus::SetNextLocation;
			}
		}

#if defined(_ENABLE_LOCAL_DEBUG_)
		{
			const int32 currentIndex = mDungeonRoomSensorBaseIndex % mDungeonRoomSensorBases.Num();
			FString message;

			if (IsValid(mDungeonGenerateActor))
			{
				message = FString::Format(TEXT("Received hash value: {0}\n"), { mDungeonGenerateActor->GetGeneratedDungeonCRC32() });
				message += FString::Format(TEXT("Self hash value: {0}\n"), { mDungeonGenerateActor->CalculateCRC32() });
			}

			for (int32 i = 0; i < mDungeonRoomSensorBases.Num(); ++i)
			{
				const AActor* actor = mDungeonRoomSensorBases[i];
				if (actor)
				{
					if (i == currentIndex)
						message += TEXT("=> ");

					FString name;
					actor->GetName(name);
					const FVector& location = actor->GetActorLocation();
					message += FString::Format(TEXT("{0}, {1}, {2}: {3}\n"), { *name, location.X, location.Y, location.Z });
				}
			}
			DrawDebugString(GetWorld(), GetPawnViewLocation(), message, nullptr, FColor::White, 0, true, 1.f);
		}
#endif

#if defined(DUNGEON_CRAWLER_BASE_ENABLE_JUMP)
		// ジャンプ
		mJump += deltaSeconds;
		if (mJump >= 2.f)
		{
			mJump = 0.f;
			Jump();
		}
#endif
		}
	}

void ADungeonCrawlerBase::ShuffleDungeonRoomSensorBases()
{
	for (int32 i = mDungeonRoomSensorBases.Num() - 1; i > 0; i--)
	{
		const int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % mDungeonRoomSensorBases.Num();
		const auto temp = mDungeonRoomSensorBases[i];
		mDungeonRoomSensorBases[i] = mDungeonRoomSensorBases[j];
		mDungeonRoomSensorBases[j] = temp;
	}
}

TArray<FVector> ADungeonCrawlerBase::FindPathToLocation(const FVector& location) const
{
	TArray<FVector> result;
	FNavPathSharedPtr path;

	if (UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		if (const ANavigationData* navData = navSys->GetNavDataForProps(GetNavAgentPropertiesRef(), GetNavAgentLocation()))
		{
			const FSharedConstNavQueryFilter navFilter = UNavigationQueryFilter::GetQueryFilter(*navData, this, nullptr);
			FPathFindingQuery query = FPathFindingQuery(*this, *navData, GetNavAgentLocation(), location, navFilter);
			query.SetAllowPartialPaths(true);

			FPathFindingResult pathResult = navSys->FindPathSync(query);
			if (pathResult.Result != ENavigationQueryResult::Error)
			{
				if (pathResult.IsSuccessful() && pathResult.Path.IsValid())
				{
					pathResult.Path->EnableRecalculationOnInvalidation(true);
					path = pathResult.Path;
				}
			}
		}
	}

	if (path)
	{
		const TArray<FNavPathPoint>& points = path->GetPathPoints();
		for (int pointIndex = 0; pointIndex < points.Num(); ++pointIndex)
		{
			result.Add(points[pointIndex].Location);
		}
	}

	return result;
}
