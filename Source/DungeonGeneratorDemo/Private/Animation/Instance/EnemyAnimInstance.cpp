/**
@author		Shun Moriya

敵アニメーションインスタンス ソースファイル
*/

#include "Animation/Instance/EnemyAnimInstance.h"
//#include "../Notify/AttackAnimNotifyState.h"
//#include "Component/BoneEffector/InterestAimComponent.h"
#include "Component/Movement/EnemyMovementComponent.h"
//#include <CharacterTrajectoryComponent.h>
#include "Helper/Math.h"
#include <array>

UEnemyAnimInstance::UEnemyAnimInstance(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

float UEnemyAnimInstance::TryGetFootOffset(const AActor* ownerActor, const float socketX, const float socketY, const float fromLocationZ, const float toLocationZ) const
{
	const FVector fromLocation(socketX, socketY, fromLocationZ);
	const FVector toLocation(socketX, socketY, toLocationZ);

	FCollisionQueryParams traceParams(TEXT("UEnemyAnimInstance:TryGetFootOffset"), false, ownerActor);
	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, fromLocation, toLocation, ECollisionChannel::ECC_Pawn, traceParams))
	{
		return hitResult.ImpactPoint.Z - toLocationZ;
	}

	return 0.f;
}
#if 0
void UEnemyAnimInstance::UpdateFootIk(const TArray<FFootIkParameter>& footIKParameter, FFootIkSignature footIkDelegate)
{
	if (const USkeletalMeshComponent* meshComponent = GetValid(GetSkelMeshComponent()))
	{
		if (const AActor* ownerActor = GetValid(meshComponent->GetOwner()))
		{
			const float fromLocationZ = ownerActor->GetActorLocation().Z;

			TArray<FVector> socketLocations;
			socketLocations.Reserve(footIKParameter.Num());

			float toLocationZ = std::numeric_limits<float>::max();
			for (const auto& parameter : footIKParameter)
			{
				const FVector& socketLocation = meshComponent->GetSocketLocation(parameter.SocketName);
				socketLocations.Add(socketLocation);
				toLocationZ = std::min(toLocationZ, static_cast<float>(socketLocation.Z));
			}

			toLocationZ -= 15.f;
			FootIkDelegate = footIkDelegate;

			for (int32 i = 0; i < footIKParameter.Num(); ++i)
			{
				const FVector& socketLocation = socketLocations[i];
				const float offset = TryGetFootOffset(ownerActor, socketLocation.X, socketLocation.Y, fromLocationZ, toLocationZ);
				footIkDelegate.ExecuteIfBound(i, footIKParameter[i].Coordinate, offset * footIKParameter[i].Scale);
			}
		}
	}
}
#endif

void UEnemyAnimInstance::NativeBeginPlay()
{
	// 親クラスを呼び出し
	Super::NativeBeginPlay();
#if 0
	// オーナーアクターを取得
	if (const APawn* owner = GetValid(TryGetPawnOwner()))
	{
		CharacterTrajectoryComponent = owner->FindComponentByClass<UCharacterTrajectoryComponent>();
		if (IsValid(CharacterTrajectoryComponent) == false)
		{
			NARCIS_ACTION_GAME_WARNING(TEXT("オーナーアクター(%s)にCharacterTrajectoryComponentを追加して下さい"),
				GetOwningActor() ? *GetOwningActor()->GetName() : *GetName());
		}

		// エイミングコンポーネントを検索
		mInterestAimComponent = owner->FindComponentByClass<UInterestAimComponent>();
		if (IsValid(mInterestAimComponent) == false)
		{
			NARCIS_ACTION_GAME_WARNING(TEXT("オーナーアクター(%s)はInterestAimComponentを所有していないのでエイム機能が無効になります"),
				GetOwningActor() ? *GetOwningActor()->GetName() : *GetName());
		}
	}
#endif
}

void UEnemyAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	// 親クラスを呼び出し
	Super::NativeUpdateAnimation(deltaSeconds);

	// 前回の移動状態を記録
	LastMovementMode = MovementMode;
	LastMovementStance = MovementStance;
	LastMovementState = MovementState;
	LastMovementGait = MovementGait;

	// オーナーアクターを取得
	if (const ACharacter* owner = GetValid(TryGetCharacterOwner()))
	{
		// ムーブメントコンポーネントを取得
		if (const UCharacterMovementComponent* movementComponent = GetValid(owner->GetCharacterMovement()))
		{
			// 加速中判定
			const float acceleration = movementComponent->GetCurrentAcceleration().Length();
			IsAccelerating = acceleration > std::numeric_limits<float>::epsilon();

			// 加速中の角度を記録
			LastAcceleratingSpeed = FMath::FInterpTo(LastAcceleratingSpeed, GetMovementSpeed(), deltaSeconds, 20.f);
			if (IsAccelerating && GetMovementSpeedRatio() > 0.25f)
				LastAcceleratingAngle = GetMovementAngle();

			// 状態を保存
			IsInAir = movementComponent->IsFalling();
			IsInWater = movementComponent->IsSwimming();





			MovementMode = UEnemyAnimMovementMode::OnGround;
			if (movementComponent->IsFalling() == true)
				MovementMode = UEnemyAnimMovementMode::InAir;
			//else if (movementComponent->IsSwimming() == true)

			MovementStance = UEnemyAnimMovementStance::Stand;
			if (owner->bIsCrouched)
				MovementStance = UEnemyAnimMovementStance::Crouch;
#if 0
			// プレイヤーアクション状態に応じた移動状態の反映
			if (const auto* playerEnemy = Cast<AEnemyBase>(owner))
			{
				switch (playerEnemy->GetActionChecker().GetCurrentStateId())
				{
				case PlayerActionStateId::Climb:
					MovementMode = UEnemyAnimMovementMode::OnWall;
					MovementStance = UEnemyAnimMovementStance::Climb;
					break;

				default:
					break;
				}
			}
#endif
		}

		// 転回角度を更新
		TurnYaw = 0.f;
		const FVector& velocity = owner->GetVelocity();
		if (velocity.SizeSquared2D() > std::numeric_limits<float>::epsilon())
		{
			const FVector& forwardVector = owner->GetActorForwardVector();
			const float velocityYaw = std::atan2(velocity.Y, velocity.X);
			const float forwardYaw = std::atan2(forwardVector.Y, forwardVector.X);
			TurnYaw = math::ToDegree(math::RoundRadianWithSign(velocityYaw - forwardYaw));
		}
	}

	// アニメーションの再生レートを計算
	UpdateMoveAnimationSpeedRatio(MovementSpeed, MovementSpeedRatio, MovementAngle);

	MovementState = UEnemyAnimMovementState::Idle;
	MovementGait = UEnemyAnimMovementGait::Walk;
	if (MovementSpeed > std::numeric_limits<float>::epsilon())
		MovementState = UEnemyAnimMovementState::Moving;
	if (MovementSpeed > 301.f)
		MovementGait = UEnemyAnimMovementGait::Run;
	if (MovementSpeed > 601.f)
		MovementGait = UEnemyAnimMovementGait::Sprint;


#if 0
	// エイミングコンポーネントの値に更新
	if (IsValid(mInterestAimComponent))
	{
		AimPitch = mInterestAimComponent->InterestAimingPitch;
		AimYaw = mInterestAimComponent->InterestAimingYaw;
	}
#endif
}
#if 0
EPoseSearchInterruptMode UEnemyAnimInstance::GetMotionMatchingInterruptMode() const noexcept
{
	if (LastMovementMode != MovementMode || MovementMode == UEnemyAnimMovementMode::OnGround && (LastMovementStance != MovementStance || LastMovementState != MovementState || LastMovementGait != MovementGait))
		return EPoseSearchInterruptMode::InterruptOnDatabaseChange;
	return EPoseSearchInterruptMode::DoNotInterrupt;
}
#endif
FRotator UEnemyAnimInstance::GetHeadAimRotation() const noexcept
{
	return FRotator(AimPitch, AimYaw, 0.f);
}

void UEnemyAnimInstance::SetHeadAimRotation(const float pitch, const float yaw) noexcept
{
	AimPitch = pitch;
	AimYaw = yaw;
}

bool UEnemyAnimInstance::UpdateMoveAnimationSpeedRatio(float& movementSpeed, float& movementSpeedRatio, float& movementDegree) const
{
	// オーナーアクターを取得
	const auto owner = TryGetPawnOwner();
	if (!IsValid(owner))
		return false;

	// ムーブメントコンポーネントを取得
	const auto movementComponent = owner->GetMovementComponent();
	if (!IsValid(movementComponent))
		return false;

	// 移動速度と移動角度を計算
	const auto& velocity = owner->GetVelocity();
	const auto forwardSpeed = FVector::DotProduct(owner->GetActorForwardVector(), velocity);
	const auto rightSpeed = FVector::DotProduct(owner->GetActorRightVector(), velocity);
	movementSpeed = std::sqrt(math::Square(forwardSpeed) + math::Square(rightSpeed));
	movementDegree = math::ToDegree(std::atan2(rightSpeed, forwardSpeed));

	const auto maxSpeed = std::max(movementComponent->GetMaxSpeed(), std::numeric_limits<float>::epsilon());
	movementSpeedRatio = movementSpeed / maxSpeed;

	return true;
}



float UEnemyAnimInstance::GetLastAcceleratingSpeed() const
{
	return LastAcceleratingSpeed;
}

float UEnemyAnimInstance::GetLastAcceleratingAngle() const
{
	return LastAcceleratingAngle;
}

bool UEnemyAnimInstance::Accelerating() const
{
	return IsAccelerating;
}

ACharacter* UEnemyAnimInstance::TryGetCharacterOwner() const
{
	return Cast<ACharacter>(TryGetPawnOwner());
}
#if 0
void UEnemyAnimInstance::EnterDamageReaction(EDefenseReactionResult defenseReactionResult)
{
	DefenseReactionResult = defenseReactionResult;
}

void UEnemyAnimInstance::ExitDamageReaction()
{
	DefenseReactionResult = EDefenseReactionResult::None;
}
#endif

// アニメーションブループリントのヘルパー関数


// from JogForward
bool UEnemyAnimInstance::CanTransitionJogForwardToBrakeForward() const noexcept
{
	constexpr float angle = 95.0f;
	constexpr float speed = 400.f;

	return
		(GetMovementSpeed() > speed) &&
		(IsAccelerating == false) &&
		(LastAcceleratingAngle < angle && LastAcceleratingAngle > -angle);
}

bool UEnemyAnimInstance::CanTransitionJogForwardToBrakeBackward() const noexcept
{
	constexpr float angle = 95.0f;
	constexpr float speed = 400.f;

	return
		(GetMovementSpeed() > speed) &&
		(IsAccelerating == false) &&
		(LastAcceleratingAngle < -angle || LastAcceleratingAngle > angle);
}

bool UEnemyAnimInstance::CanTransitionJogForwardToJogBackward() const noexcept
{
	constexpr float angle = 95.0f;
	return GetMovementAngle() < -angle || GetMovementAngle() > angle;
}

// from JogBackward
bool UEnemyAnimInstance::CanTransitionJogBackwardToBrakeForward() const noexcept
{
	constexpr float angle = 95.0f;
	constexpr float speed = 400.f;

	return
		(GetMovementSpeed() > speed) &&
		(IsAccelerating == false) &&
		(LastAcceleratingAngle < angle && LastAcceleratingAngle > -angle);
}

bool UEnemyAnimInstance::CanTransitionJogBackwardToBrakeBackward() const noexcept
{
	constexpr float angle = 95.0f;
	constexpr float speed = 400.f;

	return
		(GetMovementSpeed() > speed) &&
		IsAccelerating == false &&
		(LastAcceleratingAngle < -angle || LastAcceleratingAngle > angle);
}

bool UEnemyAnimInstance::CanTransitionJogBackwardToJogForward() const noexcept
{
	constexpr float angle = 90.0f;
	return GetMovementAngle() < angle && GetMovementAngle() > -angle;
}

// BrakeForward
bool UEnemyAnimInstance::CanTransitionBrakeForwardToJogForward(const float animTimeRemaining) const noexcept
{
	constexpr float angle = 95.0f;
	const bool a = GetMovementAngle() <= angle && GetMovementAngle() >= -angle;
	const bool b = IsAccelerating == true || animTimeRemaining < 0.01f;
	return a && b;
}

bool UEnemyAnimInstance::CanTransitionBrakeForwardToJogBackward(const float animTimeRemaining) const noexcept
{
	constexpr float angle = 95.0f;

	const bool a = GetMovementAngle() < -angle || GetMovementAngle() > angle;
	const bool b = IsAccelerating == true || animTimeRemaining < 0.01f;
	return a && b;
}

// BrakeBackward
bool UEnemyAnimInstance::CanTransitionBrakeBackwardToJogForward(const float animTimeRemaining) const noexcept
{
	constexpr float speed = 400.f;

	const bool a = (GetMovementSpeed() <= speed && GetMovementSpeed() >= -speed);
	const bool b = (IsAccelerating == true) || (animTimeRemaining < 0.01f);
	return a && b;
}

bool UEnemyAnimInstance::CanTransitionBrakeBackwardToJogBackward(const float animTimeRemaining) const noexcept
{
	constexpr float angle = 95.0f;

	const bool a = (GetMovementAngle() < -angle || GetMovementAngle() > angle);
	const bool b = a && (IsAccelerating == true);
	const bool c = a && (animTimeRemaining < 0.01f);
	return b || c;
}

bool UEnemyAnimInstance::CanTranslationWaitToJog() const
{
	return IsAccelerating && GetMovementSpeed() <= 0.f;
	// TODO: && IsWalk == false
}

bool UEnemyAnimInstance::CanTranslationJogToWait(const float animTimeRemaining) const
{
	const bool a = animTimeRemaining < 0.01f;
	const bool b = IsAccelerating == false;
	return a || b;
}

bool UEnemyAnimInstance::CanTranslationNormalToAlert() const
{
	return IsInAlert();
}

bool UEnemyAnimInstance::CanTranslationAlertToNormal() const
{
	const bool a = IsInAlert() == false;
	const bool b = IsAccelerating == false;
	return a && b;
}




























AEnemyBase* UEnemyAnimInstance::TryGetEnemyOwner() const
{
	return Cast<AEnemyBase>(TryGetPawnOwner());
}
#if 0
bool UEnemyAnimInstance::UpdateMoveAnimationSpeedRatio(float& speed, float& speedRatio, float& degree) const
{
	// オーナーアクターを取得
	const AEnemy* owner = TryGetEnemyOwner();
	if (!IsValid(owner))
		return false;

	// ムーブメントコンポーネントを取得
	const UEnemyMovementComponent* movementComponent = owner->GetEnemyMovement();
	if (!IsValid(movementComponent))
		return false;

	// 移動速度と移動角度を計算
	const FVector velocity = owner->GetVelocity();
	const float forwardSpeed = FVector::DotProduct(owner->GetActorForwardVector(), velocity);
	const float rightSpeed = FVector::DotProduct(owner->GetActorRightVector(), velocity);
	speed = std::sqrt(math::Square(forwardSpeed) + math::Square(rightSpeed));
	degree = math::ToDegree(std::atan2(rightSpeed, forwardSpeed));

	const float maxSpeed = std::max(movementComponent->GetMaxSpeed(), std::numeric_limits<float>::epsilon());
	speedRatio = speed / maxSpeed;

	return true;
}
#endif
void UEnemyAnimInstance::ChangeWalk() noexcept
{
	mMovingType = static_cast<uint8_t>(MovingType::Walk);
}

bool UEnemyAnimInstance::IsWalkMove() const noexcept
{
	return mMovingType == static_cast<uint8_t>(MovingType::Walk);
}

void UEnemyAnimInstance::ChangeSprint() noexcept
{
	mMovingType = static_cast<uint8_t>(MovingType::Sprint);
}

bool UEnemyAnimInstance::IsSprintMove() const noexcept
{
	return mMovingType == static_cast<uint8_t>(MovingType::Sprint);
}

void UEnemyAnimInstance::BeginCrouch(const bool withStartAnimation) noexcept
{
	mMovingType = withStartAnimation
		? static_cast<uint8_t>(MovingType::Crouch)
		: static_cast<uint8_t>(MovingType::QuickCrouch);
}

bool UEnemyAnimInstance::IsCrouchMoveStart(const bool withStartAnimation) const noexcept
{
	const uint8_t movingType = withStartAnimation
		? static_cast<uint8_t>(MovingType::Crouch)
		: static_cast<uint8_t>(MovingType::QuickCrouch);
	return mMovingType == movingType;
}

bool UEnemyAnimInstance::IsCrouchMove() const noexcept
{
	const auto movingType = static_cast<uint8_t>(mMovingType);
	return
		movingType == static_cast<uint8_t>(MovingType::Crouch) ||
		movingType == static_cast<uint8_t>(MovingType::QuickCrouch);
}

bool UEnemyAnimInstance::IsCrouchMoveEnd() const noexcept
{
	const auto movingType = static_cast<uint8_t>(mMovingType);
	return
		movingType != static_cast<uint8_t>(MovingType::Crouch) &&
		movingType != static_cast<uint8_t>(MovingType::QuickCrouch);
}

bool UEnemyAnimInstance::IsFalling() const noexcept
{
	return mIsFalling;
}

void UEnemyAnimInstance::SetFalling(const bool falling) noexcept
{
	mIsFalling = falling;
}

void UEnemyAnimInstance::RetainFireCount() noexcept
{
	++mAttackCount;
}

void UEnemyAnimInstance::ReleaseFireCount() noexcept
{
	--mAttackCount;
}

bool UEnemyAnimInstance::IsAttacking() const noexcept
{
	return mAttackCount > 0;
}

void UEnemyAnimInstance::RetainComboAcceptCount() noexcept
{
	std::lock_guard<std::mutex> lock(mComboAcceptCountMutex);
	if (mComboAcceptCount == 0)
	{
		// コールバックを呼び出す
		if (mOnComboAcceptBegin)
			mOnComboAcceptBegin();
	}
	++mComboAcceptCount;
}

void UEnemyAnimInstance::ReleaseComboAcceptCount() noexcept
{
	std::lock_guard<std::mutex> lock(mComboAcceptCountMutex);
	--mComboAcceptCount;
	if (mComboAcceptCount == 0)
	{
		// コールバックを呼び出す
		if (mOnComboAcceptEnd)
			mOnComboAcceptEnd();
	}
}

bool UEnemyAnimInstance::IsComboAccepting() const noexcept
{
	std::lock_guard<std::mutex> lock(mComboAcceptCountMutex);
	return mComboAcceptCount > 0;
}

void UEnemyAnimInstance::OnComboAcceptBegin(const std::function<void()>& func)
{
	mOnComboAcceptBegin = func;
}

void UEnemyAnimInstance::OnComboAcceptEnd(const std::function<void()>& func)
{
	mOnComboAcceptEnd = func;
}

float UEnemyAnimInstance::GetEnemyControlRigBlendRatio() const
{
	return IsSprintMove() ? 0.f : 1.f;
}

#if WITH_EDITOR
const FString& UEnemyAnimInstance::GetMovingType() const
{
	static const std::array<FString, 4> names = { {
		TEXT("Walk"),
		TEXT("Sprint"),
		TEXT("Crouch"),
		TEXT("QuickCrouch"),
	} };
	return names.at(mMovingType);
}

void UEnemyAnimInstance::DebugInformation(TArray<FString>& output) const
{
	output.Add(TEXT(" ") + UEnum::GetValueAsString(MovementMode));
	output.Add(TEXT(" ") + UEnum::GetValueAsString(MovementStance));
	output.Add(TEXT(" ") + UEnum::GetValueAsString(MovementState));
	output.Add(TEXT(" ") + UEnum::GetValueAsString(MovementGait));

	output.Add(TEXT(" MovementSpeed : ") + FString::SanitizeFloat(MovementSpeed));
	output.Add(TEXT(" MovementAngle : ") + FString::SanitizeFloat(MovementAngle));
	output.Add(TEXT(" LastAcceleratingSpeed : ") + FString::SanitizeFloat(LastAcceleratingSpeed));
	output.Add(TEXT(" LastAcceleratingAngle : ") + FString::SanitizeFloat(LastAcceleratingAngle));
	output.Add(TEXT(" IsAccelerating : ") + FString(IsAccelerating ? TEXT("ON") : TEXT("OFF")));

	output.Add(TEXT(" IsGuarding : ") + FString(Guarding ? TEXT("ON") : TEXT("OFF")));
	output.Add(TEXT(" InAlert : ") + FString(IsInAlert() ? TEXT("ON") : TEXT("OFF")));

	output.Add(TEXT(" Moving Type : ") + GetMovingType());
	output.Add(TEXT(" Attack Request Count : ") + FString::FromInt(mAttackCount));
}
#endif
