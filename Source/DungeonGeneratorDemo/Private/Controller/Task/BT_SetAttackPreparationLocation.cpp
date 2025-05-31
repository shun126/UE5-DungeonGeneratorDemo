/**
@author		Shun Moriya
*/

#include "Controller/Task/BT_SetAttackPreparationLocation.h"
#include "Controller/EnemyControllerBase.h"
#include "Helper/Math.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>
#include <NavigationSystem.h>

namespace
{
	/*
	オーナーアクターの新しい位置を探します
	*/
	FVector GetNewOwnerLocation(const AActor* owner, const AActor* target, float additionalRange, const float randomDistance, std::function<float(const uint8_t, const float)> func)
	{
		check(IsValid(owner));
		check(IsValid(owner->GetWorld()));

		// ターゲットアクターが不正ならオーナーアクターの位置を返す
		if (IsValid(target) == false)
			return owner->GetActorLocation();

		// ナビゲーションシステムとデータを先に取得
		UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(owner->GetWorld());
		const ANavigationData* navData = nullptr;
		if (IsValid(navSys))
		{
			if (const APawn* ownerPawn = GetValid(Cast<APawn>(owner)))
				navData = navSys->GetNavDataForProps(ownerPawn->GetNavAgentPropertiesRef(), ownerPawn->GetNavAgentLocation());
		}

		// 最小移動距離（キャラクターコリジョンより近づかない）
		const float minimumMoveDistance
			= owner->GetSimpleCollisionRadius()
			+ target->GetSimpleCollisionRadius();

		// 追加距離がminimumMoveDistance未満ならminimumMoveDistance
		if (additionalRange < minimumMoveDistance)
			additionalRange = minimumMoveDistance;

		// 最小距離
		float minimumDistance = additionalRange + FMath::RandRange(-randomDistance, 0.f);
		if (minimumDistance < minimumMoveDistance)
			minimumDistance = minimumMoveDistance;

		// 最大距離
		float maximumDistance = additionalRange + FMath::RandRange(0.f, randomDistance);

		// オーナーの位置
		const FVector& ownerLocation = owner->GetActorLocation();

		// ターゲットの位置
		const FVector& targetLocation = target->GetActorLocation();

		// ターゲットから見たオーナーの方向と角度
		const FVector directionTargetToOwner = ownerLocation - targetLocation;
		const float angleTargetToOwner = std::atan2(directionTargetToOwner.Y, directionTargetToOwner.X);

		// 新しい目的地を探す
		FVector goalLocation;
		FCollisionQueryParams traceParams(TEXT("GetNewOwnerLocation"), true, owner);
		traceParams.AddIgnoredActor(target);

		// リトライ回数を超えたら中断
		constexpr int8_t maxRetryCount = 5;
		for (int8_t retryCountRemaining = maxRetryCount; retryCountRemaining >= 0; --retryCountRemaining)
		{
			// 新しい位置へのオフセットを求める
			const float newAngleTargetToOwner = func(retryCountRemaining, angleTargetToOwner);
			goalLocation.Set(std::cos(newAngleTargetToOwner), std::sin(newAngleTargetToOwner), 0.f);
			goalLocation *= FMath::RandRange(minimumDistance, maximumDistance);
			goalLocation += targetLocation;

			// ターゲットアクターから新しい位置の間に遮蔽物があるなら新しい位置を遮蔽物の前へ移動
			FHitResult hitResult;
			if (owner->GetWorld()->LineTraceSingleByChannel(hitResult, targetLocation, goalLocation, ECollisionChannel::ECC_Pawn, traceParams) == true)
			{
				FVector direction = hitResult.ImpactPoint - goalLocation;
				auto length = direction.Length();
				if (direction.Normalize())
				{
					goalLocation = direction * (length - owner->GetSimpleCollisionRadius());
				}
				else
				{
					goalLocation = hitResult.ImpactPoint;
				}
			}

			// 新しい位置からターゲットアクターの間（つまり進路上に）に遮蔽物が無いか調べる
			if (owner->GetWorld()->LineTraceTestByChannel(goalLocation, targetLocation, ECollisionChannel::ECC_Pawn, traceParams) == true)
			{
				continue;
			}

			// 地面へのスナップ
			if (IsValid(navSys))
			{
				// ナビゲーションデータから調べる
				FNavLocation outNavLocation;
				if (navSys->ProjectPointToNavigation(goalLocation, outNavLocation, INVALID_NAVEXTENT, navData) == false)
					continue;

				// 地面の位置を記録して終了
				goalLocation = outNavLocation.Location;
			}
			else
			{
				// コリジョン経由で調べる
				const FVector groundSnapEndLocation = goalLocation - FVector::UpVector * (10 * 100);
				if (owner->GetWorld()->LineTraceSingleByChannel(hitResult, goalLocation, groundSnapEndLocation, ECollisionChannel::ECC_Pawn, traceParams) == false)
					continue;

				// 地面の位置を記録して終了
				goalLocation = hitResult.ImpactPoint;
			}

			return goalLocation;
		}

		return targetLocation;
	}

    /*
    オイカケ　プレイヤーの後ろをひたすら追いかける
    */
    FVector GetAttackChaseLocation(const AActor* owner, const AActor* target, float additionalRange, const float randomDistance)
    {
		return GetNewOwnerLocation(owner, target, additionalRange, randomDistance, [target](const uint8_t retryCountRemaining, const float angleTargetToOwner)
			{
				if (retryCountRemaining == 0)
					return angleTargetToOwner;

				static constexpr float HalfPi = 3.14159265359f / 2.f;
				float newAngleTargetToOwner = -math::ToRadian<float>(target->GetActorRotation().Yaw);
				newAngleTargetToOwner += FMath::RandRange(-HalfPi, HalfPi);
				return newAngleTargetToOwner;
			}
		);
	}

	/*
	* マチブセ　プレイヤーのいる地点の少し前を目指し、先回りするように動く
	*/
    FVector GetAmbushLocation(const AActor* owner, const AActor* target, float additionalRange, const float randomDistance)
    {
        return GetNewOwnerLocation(owner, target, additionalRange, randomDistance, [target](const uint8_t retryCountRemaining, const float angleTargetToOwner)
            {
				if (retryCountRemaining == 0)
					return angleTargetToOwner;

        		static constexpr float HalfPi = 3.14159265359f / 2.f;
				float newAngleTargetToOwner = math::ToRadian<float>(target->GetActorRotation().Yaw);
				newAngleTargetToOwner += FMath::RandRange(-HalfPi, HalfPi);
				return newAngleTargetToOwner;
			}
		);
    }
	
	/*
	* キマグレ　プレイヤーを中心にした周辺を動く
	*/
	FVector GetWhimsicalLocation(const AActor* owner, const AActor* target, float additionalRange, const float randomDistance)
	{
		return GetNewOwnerLocation(owner, target, additionalRange, randomDistance, [](const uint8_t retryCountRemaining, const float angleTargetToOwner)
			{
				if (retryCountRemaining == 0)
					return angleTargetToOwner;

				static constexpr float QuarterPi = 3.14159265359f / 4.f;
				float newAngleTargetToOwner = FMath::RandRange(-QuarterPi, QuarterPi);
				newAngleTargetToOwner += angleTargetToOwner;
				return newAngleTargetToOwner;
			});
	}

    FVector GetAttackPreparationLocation(const AActor* owner, const AActor* target, const int32 characterType, float additionalRange, const float randomDistance)
    {
		switch (characterType & 3)
		{
		case 1:
			// オイカケ　プレイヤーの後ろをひたすら追いかける
			return GetAttackChaseLocation(owner, target, additionalRange, randomDistance);
		case 2:
			// マチブセ　プレイヤーのいる地点の少し前を目指し、先回りするように動く
			return GetAmbushLocation(owner, target, additionalRange, randomDistance);
		default:
			// キマグレ　プレイヤーを中心にした周辺を動く
			return GetWhimsicalLocation(owner, target, additionalRange, randomDistance);
		}
    }
}

UBT_SetAttackPreparationLocation::UBT_SetAttackPreparationLocation()
{
    NodeName = "TaskSetAttackPreparationLocation";

    TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBT_SetAttackPreparationLocation, TargetActorKey), AActor::StaticClass());
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBT_SetAttackPreparationLocation, BlackboardKey));
}

void UBT_SetAttackPreparationLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
    if (UBlackboardData* blackboardAsset = GetBlackboardAsset())
    {
        TargetActorKey.ResolveSelectedKey(*blackboardAsset);
    }

    Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBT_SetAttackPreparationLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    const auto controller = Cast<AEnemyControllerBase>(ownerComp.GetAIOwner());
    if (IsValid(controller) == false)
        return EBTNodeResult::Failed;

	const auto ownerActor = controller->GetPawn();
    if (IsValid(ownerActor) == false)
        return EBTNodeResult::Failed;

	const auto blackboard = ownerComp.GetBlackboardComponent();
    if (IsValid(blackboard) == false)
        return EBTNodeResult::Failed;

	const auto targetActor = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(TargetActorKey.GetSelectedKeyID()));
    if (IsValid(targetActor) == false)
        return EBTNodeResult::Failed;

    const auto& nextLocation = GetAttackPreparationLocation(
        ownerActor,
        targetActor,
        controller->GetCharacterType(),
		controller->GetAttackPreparationDistanceRange().Min,
		controller->GetAttackPreparationDistanceRange().Max - controller->GetAttackPreparationDistanceRange().Min
    );
    blackboard->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), nextLocation);

    return EBTNodeResult::Succeeded;
}
