/**
@author		Shun Moriya

敵の移動コンポーネントクラス ソースファイル

`MovementComp->UseAccelerationForPathFollowing()`または
`Movement::bUseAccelerationForPaths`を有効にすると移動に慣性が加味される
*/

#include "Component/Movement/EnemyMovementComponent.h"
#include <Misc/EngineVersionComparison.h>
#include <array>

//#define ENABLE_OUTPUT_LOG

static constexpr float DefaultCrouchSpeed = 150.f;
static constexpr float DefaultStrafeSpeed = 180.f;
static constexpr float DefaultSprintSpeed = 600.f;

FEnemyMovementParameter::FEnemyMovementParameter(const float moveMaxSpeed, const FRotator& rotationRate)
	: MoveMaxSpeed(moveMaxSpeed)
	, RotationRate(rotationRate)
{
}

void FEnemyMovementParameter::Set(const float moveMaxSpeed, const FRotator& rotationRate)
{
	MoveMaxSpeed = moveMaxSpeed;
	RotationRate = rotationRate;
}

UEnemyMovementComponent::UEnemyMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, WalkMove(MaxWalkSpeed, RotationRate)
	, StrafeMove(DefaultStrafeSpeed, FRotator(0.f, 780.f, 0.f))
	, SprintMove(DefaultSprintSpeed, FRotator(0.f, 360.f, 0.f))
	, CrouchRotationRate(0.f, 360.f, 0.f)
	, mAvatarMovementModeEnable(~0)
	, mAvatarMovementMode(static_cast<EEnemyMovementMode>(~0))
{
	MaxWalkSpeed = DefaultSprintSpeed;
	MaxWalkSpeedCrouched = DefaultCrouchSpeed;
	bUseControllerDesiredRotation = bOrientRotationToMovement = true;


	// UNavMovementComponent
#if UE_VERSION_NEWER_THAN(5, 5, 0)
	check(GetNavMovementProperties());
	GetNavMovementProperties()->bUseAccelerationForPaths = true;
#else
	bUseAccelerationForPaths = true;
#endif
}

void UEnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	WalkMove.MoveMaxSpeed = MaxWalkSpeed;
	WalkMove.RotationRate = RotationRate;

	// 歩きを初期状態にする
	ChangeWalk();
}

bool UEnemyMovementComponent::ChangeWalk()
{
	// 進行方向へ体を向ける
	return ChangeAvatarMovementMode(EEnemyMovementMode::Walk, WalkMove, true);
}

bool UEnemyMovementComponent::ChangeSprint()
{
	// 進行方向へ体を向ける
	return ChangeAvatarMovementMode(EEnemyMovementMode::Sprint, SprintMove, true);
}

bool UEnemyMovementComponent::ChangeStrafe(const bool useControllerRotationYaw)
{
	return ChangeAvatarMovementMode(EEnemyMovementMode::Strafe, StrafeMove, useControllerRotationYaw);
}

bool UEnemyMovementComponent::ChangeCrouch()
{
	if (mAvatarMovementModeEnable.test(static_cast<size_t>(EEnemyMovementMode::Crouch)))
	{
		if (mAvatarMovementMode != EEnemyMovementMode::Crouch)
		{
			mAvatarMovementMode = EEnemyMovementMode::Crouch;

			/*
			中腰移動速度は movement->MaxWalkSpeedCrouched が適用されるので
			FEnemyMovementParameter CrouchMove の定義は不要

			中腰移動回転速度はplayer::action::Crouch内で随時更新します
			*/
			RotationRate = CrouchRotationRate;

			// 進行方向へ体を向ける
			bUseControllerDesiredRotation = bOrientRotationToMovement = true;
		}

		return true;
	}

	return false;
}

bool UEnemyMovementComponent::ChangeAvatarMovementMode(const EEnemyMovementMode avatarMovementMode, const FEnemyMovementParameter& avatarMovementParameter, const bool useControllerRotationYaw)
{
	if (mAvatarMovementModeEnable.test(static_cast<size_t>(avatarMovementMode)) == false)
		return false;

	if (mAvatarMovementMode != avatarMovementMode)
	{
		mAvatarMovementMode = avatarMovementMode;

		MaxWalkSpeed = avatarMovementParameter.MoveMaxSpeed;
		RotationRate = avatarMovementParameter.RotationRate;

		bUseControllerDesiredRotation = bOrientRotationToMovement = useControllerRotationYaw;
	}

	return true;
}

bool UEnemyMovementComponent::IsEnable(const EEnemyMovementMode mode) const noexcept
{
	return mAvatarMovementModeEnable.test(static_cast<size_t>(mode));
}

void UEnemyMovementComponent::SetEnable(const EEnemyMovementMode mode, const bool enable)
{
	if (enable)
		mAvatarMovementModeEnable.set(static_cast<size_t>(mode));
	else
		mAvatarMovementModeEnable.reset(static_cast<size_t>(mode));
}

float UEnemyMovementComponent::GetDemoTargetTurnRatio() const
{
	return DemoTargetTurnRatio;
}

float UEnemyMovementComponent::GetStrafeTargetTurnRatio() const
{
	return StrafeTargetTurnRatio;
}

float UEnemyMovementComponent::GetMoveMaxSpeed(const EEnemyMovementMode mode) const
{
	switch (mode)
	{
	case EEnemyMovementMode::Walk:
		return WalkMove.MoveMaxSpeed;
	case EEnemyMovementMode::Sprint:
		return SprintMove.MoveMaxSpeed;
	case EEnemyMovementMode::Strafe:
		return StrafeMove.MoveMaxSpeed;
	case EEnemyMovementMode::Crouch:
		return 0.f;

	default:
		check(0);
		return 0.f;
	}
}

EEnemyMovementMode UEnemyMovementComponent::GetLastAvatarMovementMode() const noexcept
{
	return mLastAvatarMovementMode;
}

FString UEnemyMovementComponent::GetOwnerName() const
{
	const AActor* owner = GetOwner();
	return owner ? owner->GetName() : GetName();
}

#if WITH_EDITOR
const FString& UEnemyMovementComponent::GetAvatarMovementMode() const
{
	static const std::array<FString, 4> names = { {
		TEXT("Walk"),
		TEXT("Sprint"),
		TEXT("Strafe"),
		TEXT("Crouch"),
	} };
	return names.at(static_cast<size_t>(mAvatarMovementMode));
}

void UEnemyMovementComponent::DebugInformation(TArray<FString>& output) const
{
	output.Add(bOrientRotationToMovement ? TEXT(" OrientRotationToMovement : ON") : TEXT(" OrientRotationToMovement : OFF"));
	output.Add(TEXT(" Mode : ") + GetAvatarMovementMode());
	output.Add(TEXT(" MaxSpeed : ") + FString::SanitizeFloat(GetMaxSpeed()));
	output.Add(TEXT(" RotationRate(Yaw) : ") + FString::SanitizeFloat(RotationRate.Yaw));
}
#endif
