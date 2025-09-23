/**
@author		Shun Moriya
*/

#include "EnemyBase.h"
#include "Animation/Instance/EnemyAnimInstance.h"
#include "Component/Movement/EnemyMovementComponent.h"
#include "Controller/EnemyControllerBase.h"
#include "Helper/Math.h"
#include <Components/CapsuleComponent.h>

#include "GameInstanceBase.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& objectInitializer)
: Super(objectInitializer
	.SetDefaultSubobjectClass<UEnemyMovementComponent>(ACharacter::CharacterMovementComponentName)
)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
}

void AEnemyBase::Dead_Implementation()
{
	if (auto controller = GetController<AEnemyControllerBase>())
		controller->Dead();

	if (auto mesh = GetMesh())
	{
		mesh->SetCollisionProfileName(TEXT("Ragdoll"));
		mesh->SetSimulatePhysics(true);
	}

	if (auto movement = GetEnemyMovement())
		movement->DisableMovement();
	
	if (auto component = GetCapsuleComponent())
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DropObject();
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTurn(DeltaTime);
	UpdateAim(DeltaTime);

	DamageHistory.Update(DeltaTime);

#if WITH_EDITOR && (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
	//if (ShowDebugInfomation)
	if (false)
	{
		FString message;
		{
			TArray<FString> output;

			// アバターのデバッグ情報を回収
			DebugInformation(output);

			// ムーブメントのデバッグ情報を回収
			if (const auto movement = GetValid(GetEnemyMovement()))
			{
				output.Add(TEXT("Movement Component"));
				movement->DebugInformation(output);
			}

			// アニメーションのデバッグ情報を回収
			const auto* animInstance = GetEnemyAnimInstance();
			if (IsValid(animInstance))
			{
				output.Add(TEXT("Animation Instance"));
				animInstance->DebugInformation(output);
			}

			// 文字列を展開
			for (const FString& line : output)
			{
				message.Append(line);
				message.Append(TEXT("\n"));
			}
		}
		DrawDebugString(GetWorld(), GetPawnViewLocation(), message, nullptr, FColor::White, 0, true, 1.f);
#if 0
		// 振り向き、注目アクター
		if (const AActor* targetActor = mTurn.mTarget.mActor.Get())
		{
			CDebugArrowWithString debugArrowWithString(TEXT("Turn:Target"));
			debugArrowWithString.Draw(GetWorld(), GetPawnViewLocation(), targetActor->GetActorLocation(), FColor::Magenta);
		}
#endif
		}
#endif
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// アニメーションとモンタージュ
////////////////////////////////////////////////////////////////////////////////////////////////////
float AEnemyBase::PlayMontage(UAnimMontage* MontageToPlay, float InPlayRate, float InTimeToStartMontageAt, bool bStopAllMontages) const
{
	if (UAnimInstance* animInstance = GetValid(GetAnimInstance()))
	{
		return animInstance->Montage_Play(
			MontageToPlay, InPlayRate, EMontagePlayReturnType::MontageLength, InTimeToStartMontageAt, bStopAllMontages
		);
	}
	return 0.f;
}

UAnimMontage* AEnemyBase::ReverseMontage(float InPlayRate)
{
	UAnimInstance* animInstance = GetAnimInstance();
	if (!IsValid(animInstance))
		return nullptr;

	UAnimMontage* currentMontage = animInstance->GetCurrentActiveMontage();
	if (!IsValid(currentMontage))
		return nullptr;

	const float inTimeToStartMontageAt = animInstance->Montage_GetPosition(currentMontage);
#if 0
	if (animInstance->Montage_Play(currentMontage, -InPlayRate, EMontagePlayReturnType::MontageLength, inTimeToStartMontageAt) <= 0.f)
		return nullptr;
#else
	const FMontageBlendSettings montageBlendSettings(0.f);
	if (animInstance->Montage_PlayWithBlendSettings(currentMontage, montageBlendSettings, -InPlayRate, EMontagePlayReturnType::MontageLength, inTimeToStartMontageAt) <= 0.f)
		return nullptr;
#endif
	return currentMontage;
}

float AEnemyBase::GetPlayMontageRate(const UAnimMontage* AnimMontage) const
{
	if (UAnimInstance* animInstance = GetValid(GetAnimInstance()))
	{
		return animInstance->Montage_GetPlayRate(AnimMontage);
	}

	return 0.f;
}

void AEnemyBase::SetPlayMontageRate(const UAnimMontage* AnimMontage, float NewPlayRate) const
{
	if (UAnimInstance* animInstance = GetValid(GetAnimInstance()))
	{
		animInstance->Montage_SetPlayRate(AnimMontage, NewPlayRate);
	}
}

bool AEnemyBase::IsPlayMontage(const UAnimMontage* animMontage) const
{
	const UAnimInstance* animInstance = GetAnimInstance();
	return IsValid(animInstance) && animInstance->Montage_IsPlaying(animMontage);
}

bool AEnemyBase::IsPlayAnyMontage() const
{
	const UAnimInstance* animInstance = GetAnimInstance();
	return IsValid(animInstance) && animInstance->IsAnyMontagePlaying();
}

bool AEnemyBase::IsLastFrameMontage(const UAnimMontage* animMontage, const float deltaTime) const
{
	if (IsValid(animMontage))
	{
		if (const UAnimInstance* animInstance = GetValid(GetAnimInstance()))
		{
			if (!animInstance->Montage_GetIsStopped(animMontage))
			{
				const float inTimeToStartMontageAt = const_cast<UAnimMontage*>(animMontage)->CalculateSequenceLength();
				const float position = animInstance->Montage_GetPosition(animMontage);
				return (position + deltaTime) >= inTimeToStartMontageAt;
			}
		}
	}

	return true;
}

float AEnemyBase::GetMontagePosition(const UAnimMontage* animMontage) const
{
	const UAnimInstance* animInstance = GetAnimInstance();
	return IsValid(animInstance) ? animInstance->Montage_GetPosition(animMontage) : 0.f;
}

float AEnemyBase::GetCurrentMontagePosition() const
{
	const UAnimInstance* animInstance = GetAnimInstance();
	if (IsValid(animInstance))
	{
		const UAnimMontage* currentMontage = animInstance->GetCurrentActiveMontage();
		return animInstance->Montage_GetPosition(currentMontage);
	}

	return 0.f;
}

void AEnemyBase::PauseMontage(const UAnimMontage* animMontage) const
{
	if (UAnimInstance* animInstance = GetValid(GetAnimInstance()))
		animInstance->Montage_Pause(animMontage);
}

void AEnemyBase::ResumeMontage(const UAnimMontage* animMontage) const
{
	if (UAnimInstance* animInstance = GetValid(GetAnimInstance()))
		animInstance->Montage_Resume(animMontage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 移動
////////////////////////////////////////////////////////////////////////////////////////////////////
// ChangeWalk
bool AEnemyBase::ChangeWalk_Implementation()
{
	// Movementに移動状態を設定
	UEnemyMovementComponent* avatarMovement = GetEnemyMovement();
	if (IsValid(avatarMovement) && avatarMovement->ChangeWalk())
	{
		// AnimationInstanceを設定
		if (UEnemyAnimInstance* animInstance = GetValid(GetEnemyAnimInstance()))
			animInstance->ChangeWalk();

		// 注目するアクターを解除
		CancelTurn();

		return true;
	}

	return false;
}

/*
 * ChangeSprint
 * 全力疾走ではAimが強制的に解除されます
 */
bool AEnemyBase::ChangeSprint_Implementation()
{
	// Movementに移動状態を設定
	UEnemyMovementComponent* avatarMovement = GetEnemyMovement();
	if (IsValid(avatarMovement) && avatarMovement->ChangeSprint())
	{
		// アニメーションインスタンスに移動状態を設定
		if (UEnemyAnimInstance* animInstance = GetValid(GetEnemyAnimInstance()))
			animInstance->ChangeSprint();

		// 注目するアクターを解除
		CancelTurn();

		return true;
	}

	return false;
}

/*
 * ChangeStrafeToActor
 * プレイヤー以外の射撃姿勢
 */
bool AEnemyBase::ChangeStrafeToActor_Implementation(const AActor* targetActor, const bool useControllerRotationYaw)
{
	if (IsValid(targetActor))
	{
		// Movementに移動状態を設定
		UEnemyMovementComponent* avatarMovement = GetEnemyMovement();
		if (IsValid(avatarMovement) && avatarMovement->ChangeStrafe(useControllerRotationYaw))
		{
			// AnimationInstanceを設定
			if (UEnemyAnimInstance* animInstance = GetValid(GetEnemyAnimInstance()))
				animInstance->ChangeWalk();

			// 注目するアクターを設定
			SetStrafeToActor(targetActor, avatarMovement->GetStrafeTargetTurnRatio());

			return true;
		}
	}
	else
	{
		ChangeWalk();
	}

	return false;
}

/*
 * ChangeStrafeToLocation
 * プレイヤー以外の射撃姿勢
 */
bool AEnemyBase::ChangeStrafeToLocation_Implementation(const FVector& targetLocation, const bool useControllerRotationYaw)
{
	// Movementに移動状態を設定
	UEnemyMovementComponent* avatarMovement = GetEnemyMovement();
	if (IsValid(avatarMovement) && avatarMovement->ChangeStrafe(useControllerRotationYaw))
	{
		// AnimationInstanceを設定
		if (UEnemyAnimInstance* animInstance = GetValid(GetEnemyAnimInstance()))
			animInstance->ChangeWalk();

		// 注目する座標を設定
		SetStrafeToLocation(targetLocation, avatarMovement->GetStrafeTargetTurnRatio());

		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 転回
////////////////////////////////////////////////////////////////////////////////////////////////////
const AActor* AEnemyBase::GetStrafeTarget() const
{
	return mTurn.mTarget.mActor.Get();
}

void AEnemyBase::SetStrafeToActor(const AActor* targetActor, const float dampingRatio)
{
	mTurn.mTarget.mActor = targetActor;
	mTurn.mSpeedRatio = std::clamp(dampingRatio, std::numeric_limits<float>::epsilon(), 1.0f);
	mTurn.mMode = Turn::Mode::ToActorStrafe;
}

void AEnemyBase::TurnToActor(const AActor* targetActor, const float rotationDampingRatio)
{
	mTurn.mTarget.mActor = targetActor;
	mTurn.mSpeedRatio = std::max(std::numeric_limits<float>::epsilon(), std::min(rotationDampingRatio, 1.0f));
	mTurn.mMode = IsValid(targetActor) ? Turn::Mode::ToActorTurn : Turn::Mode::None;
}

void AEnemyBase::SetStrafeToLocation(const FVector& targetLocation, const float dampingRatio)
{
	mTurn.mTarget.mLocation = targetLocation;
	mTurn.mSpeedRatio = std::clamp(dampingRatio, std::numeric_limits<float>::epsilon(), 1.0f);
	mTurn.mMode = Turn::Mode::ToLocationStrafe;
}

void AEnemyBase::TurnToLocation(const FVector& targetLocation, const float rotationDampingRatio)
{
	mTurn.mTarget.mLocation = targetLocation;
	mTurn.mSpeedRatio = std::max(std::numeric_limits<float>::epsilon(), std::min(rotationDampingRatio, 1.0f));
	mTurn.mMode = Turn::Mode::ToLocationTurn;
}

void AEnemyBase::TurnToYaw(const float targetDegree, const float rotationDampingRatio)
{
	mTurn.mTarget.mYawAngle = math::ToRadian<float>(targetDegree);
	mTurn.mSpeedRatio = std::max(std::numeric_limits<float>::epsilon(), std::min(rotationDampingRatio, 1.0f));
	mTurn.mMode = Turn::Mode::ToAngleTurn;
}

void AEnemyBase::CancelTurn()
{
	mTurn.mTarget.mActor = nullptr;
	mTurn.mMode = Turn::Mode::None;
}

bool AEnemyBase::IsTurning() const noexcept
{
	return mTurn.mMode != Turn::Mode::None;
}

void AEnemyBase::UpdateTurn(const float deltaSeconds)
{
	// 回転角度を求める
	float targetYaw = 0.f;
	switch (mTurn.mMode)
	{
	case Turn::Mode::ToActorStrafe:
	case Turn::Mode::ToActorTurn:
		if (const AActor* targetActor = mTurn.mTarget.mActor.Get())
		{
			const FVector delta = targetActor->GetActorLocation() - GetActorLocation();
			targetYaw = std::atan2(delta.Y, delta.X);
		}
		break;

	case Turn::Mode::ToLocationStrafe:
	case Turn::Mode::ToLocationTurn:
		{
			const FVector delta = mTurn.mTarget.mLocation - GetActorLocation();
			targetYaw = std::atan2(delta.Y, delta.X);
		}
		break;

	case Turn::Mode::ToAngleTurn:
		targetYaw = mTurn.mTarget.mYawAngle;
		break;

	default:
		return;
	}

	// 回転を設定
	FRotator rotation = GetActorRotation();
	float yaw = math::ToRadian(rotation.Yaw);
	if (math::ChaseDecreaseAngle(yaw, targetYaw, mTurn.mSpeedRatio, deltaSeconds))
	{
		if (
			mTurn.mMode == Turn::Mode::ToActorTurn ||
			mTurn.mMode == Turn::Mode::ToLocationTurn ||
			mTurn.mMode == Turn::Mode::ToAngleTurn)
		{
			mTurn.mMode = Turn::Mode::None;
		}
	}
	rotation.Yaw = math::ToDegree(yaw);
	SetActorRotation(rotation);
}

void AEnemyBase::Alert(const AActor* actor)
{
	if (const auto instance = GetEnemyAnimInstance())
	{
		instance->Alert(actor != nullptr);
	}

	mAim.mActor = actor;
}

void AEnemyBase::UpdateAim(const float deltaSeconds)
{
	float targetYaw = 0.f;
	float targetPitch = 0.f;

	if (const AActor* targetActor = mTurn.mTarget.mActor.Get())
	{
		const float bodyYaw = math::ToRadian(GetActorRotation().Yaw);

		const FVector delta = targetActor->GetActorLocation() - GetActorLocation();
		targetYaw = std::atan2(delta.Y, delta.X) - bodyYaw;
		targetPitch = std::atan2(delta.Z, std::sqrt(math::Square(delta.X) + math::Square(delta.Y)));
	}

	math::ChaseDecreaseAngle(mAim.mYaw, targetYaw, mTurn.mSpeedRatio, deltaSeconds);
	math::ChaseDecreaseAngle(mAim.mPitch, targetPitch, mTurn.mSpeedRatio, deltaSeconds);

	if (const auto instance = GetEnemyAnimInstance())
	{
		instance->SetHeadAimRotation(mAim.mPitch, mAim.mYaw);
	}
}

void AEnemyBase::DropObject_Implementation()
{
	if (DropType != EDropDataId::Invalid)
	{
		if (auto gameInstance = UGameInstanceBase::Instance())
		{
			const auto& itemDropData = gameInstance->GetDropDataAsset();
			const auto& dropObjectClass = itemDropData.Draw(DropType);
			if (IsValid(dropObjectClass))
				DropObjectOnClient(dropObjectClass);
		}
	}
}

void AEnemyBase::DropObjectOnClient_Implementation(TSubclassOf<AStockable> dropObjectClass)
{
	if (UWorld* world = GetValid(GetWorld()))
	{
		auto spawnLocationOffset = GetSimpleCollisionHalfHeight();
		spawnLocationOffset += 50.f;

		auto transform = GetActorTransform();
		transform.AddToTranslation(FVector(0, spawnLocationOffset, 0));

		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.Owner = this;
		actorSpawnParameters.Instigator = this;
		//actorSpawnParameters.SpawnCollisionHandlingOverride = spawnCollisionHandlingOverride;
		//actorSpawnParameters.TransformScaleMethod = transformScaleMethod;
		actorSpawnParameters.ObjectFlags |= RF_Transient;
		world->SpawnActor(dropObjectClass, &transform, actorSpawnParameters);
	}
}

void AEnemyBase::NotifyBeginFire(const FName& notifyName)
{
	OnBeginFire.Broadcast();
}

void AEnemyBase::NotifyEndFire(const FName& notifyName)
{
	OnEndFire.Broadcast();
}

UAnimInstance* AEnemyBase::GetAnimInstance() const
{
	USkeletalMeshComponent* mesh = GetMesh();
	return IsValid(mesh) ? mesh->GetAnimInstance() : nullptr;
}

UEnemyAnimInstance* AEnemyBase::GetEnemyAnimInstance() const
{
	return Cast<UEnemyAnimInstance>(GetAnimInstance());
}

UEnemyMovementComponent* AEnemyBase::GetEnemyMovement() const
{
	return Cast<UEnemyMovementComponent>(GetCharacterMovement());
}

void AEnemyBase::RegisterDamageHistory(EDamageHistoryRegisterResult& Result, const AActor* actor)
{
	Result = DamageHistory.Register(actor)
		? EDamageHistoryRegisterResult::Success
		: EDamageHistoryRegisterResult::Failure;
}

#if WITH_EDITOR
////////////////////////////////////////////////////////////////////////////////////////////////////
// デバッグ
////////////////////////////////////////////////////////////////////////////////////////////////////
void AEnemyBase::DebugInformation(TArray<FString>& output) const
{
	if (bUseControllerRotationYaw)
		output.Add(TEXT("UseControllerRotationYaw : ON"));
	else
		output.Add(TEXT("UseControllerRotationYaw : OFF"));

	// 転回情報
	{
		bool turning = IsTurning();
		if (const auto* movementComponent = GetValid(GetEnemyMovement()))
			if (movementComponent->Is(EEnemyMovementMode::Sprint))
				turning = false;
		if (turning)
			output.Add(TEXT("Turning : ON"));
		else
			output.Add(TEXT("Turning : OFF"));

		if (const AActor* strafeTarget = GetValid(GetStrafeTarget()))
			output.Add(TEXT("Target : ") + strafeTarget->GetFName().ToString());
		else
			output.Add(TEXT("Target : ---"));
	}
#if 0
	// コントローラー
	if (const auto* controller = GetController())
	{
		output.Add(TEXT("Controller"));
		output.Add(controller->IsMoveInputIgnored() ? TEXT(" MoveInputIgnored : Enable") : TEXT(" MoveInputIgnored : Disable"));

		if (const auto* avatarAIController = Cast<AEnemyControllerBase>(controller))
		{
			avatarAIController->DebugInformation(output);
		}
	}
#endif
}
#endif
