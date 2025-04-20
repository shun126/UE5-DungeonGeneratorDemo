/**
@author		Shun Moriya

Task to play the montage specified by member variables
*/

#include "Controller/Task/BT_PlayMontage.h"
#include <BehaviorTree/BTTaskNode.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>
#include <GameFramework/Character.h>

EBTNodeResult::Type UBT_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IsValid(Animation) == false)
		return EBTNodeResult::Failed;

	if (GetCurrentMontage() == nullptr)
	{
		auto* ownerCharacter = Cast<ACharacter>(OwnerComp.GetOwner());
		if (IsValid(ownerCharacter) == false)
			return EBTNodeResult::Failed;

		Play(ownerCharacter, Animation);
	}
	else
	{
		if (IsPlaying() == false)
			return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

float UBT_PlayMontage::Play(const ACharacter* character, UAnimMontage* montage, float playRate, float inTimeToStartMontageAt, bool stopAllMontages)
{
	if (!IsValid(character))
		return 0.f;

	if (!IsValid(montage))
		return 0.f;

	auto mesh = character->GetMesh();
	if (!IsValid(character->GetMesh()))
		return 0.f;

	CurrentAnimInstance = mesh->GetAnimInstance();
	if (!IsValid(CurrentAnimInstance))
		return 0.f;

	mLength = CurrentAnimInstance->Montage_Play(montage, playRate, EMontagePlayReturnType::MontageLength, inTimeToStartMontageAt, stopAllMontages);
	CurrentMontage = montage;
	mInPlayRate = playRate;

	return mLength;
}

float UBT_PlayMontage::Play(const ACharacter* character, UAnimMontage* montage, FName sectionName, float playRate, float inTimeToStartMontageAt, bool stopAllMontages)
{
	const float length = Play(character, montage, playRate, inTimeToStartMontageAt, stopAllMontages);
	if (length > 0.f)
	{
		if (sectionName != NAME_None)
		{
			CurrentAnimInstance->Montage_JumpToSection(sectionName, montage);
#if 0
			const auto sectionIndex = CurrentMontage->GetSectionIndex(sectionName);
			if (sectionIndex != INDEX_NONE)
			{
				float outStartTime, outEndTime;
				CurrentMontage->GetSectionStartAndEndTime(sectionIndex, outStartTime, outEndTime);
				CurrentAnimInstance->Montage_SetPosition(montage, playRate >= 0.f ? outStartTime : outEndTime);
			}
#endif
		}
	}
	return length;
}

float UBT_PlayMontage::Play(const ACharacter* character, UAnimSequence* animSequence, FName slotNodeName, const float blendInTime, const float blendOutTime, const float inPlayRate, const int32 loopCount, const float blendOutTriggerTime, const float inTimeToStartMontageAt)
{
	if (!IsValid(character))
		return 0.f;

	auto mesh = character->GetMesh();
	if (!IsValid(character->GetMesh()))
		return 0.f;

	CurrentAnimInstance = mesh->GetAnimInstance();
	if (!IsValid(CurrentAnimInstance))
		return 0.f;

	CurrentMontage = CurrentAnimInstance->PlaySlotAnimationAsDynamicMontage(animSequence, slotNodeName, blendInTime, blendOutTime, inPlayRate, loopCount, blendOutTriggerTime, inTimeToStartMontageAt);
	if (!IsValid(CurrentMontage))
		return 0.f;

	mInPlayRate = inPlayRate;
	mLength = CurrentMontage->GetPlayLength();
	return mLength;
}

float UBT_PlayMontage::Play(const ACharacter* character, UAnimMontage* montage, const FRandomStream& random, float playRate, float inTimeToStartMontageAt, bool stopAllMontages)
{
	const float length = Play(character, montage, playRate, inTimeToStartMontageAt, stopAllMontages);
	if (length > 0.f)
	{
		const auto count = montage->GetNumSections();
		if (count >= 2)
		{
			const auto index = random.GetUnsignedInt() % count;
			const auto& sectionName = montage->GetSectionName(index);
			if (sectionName != NAME_None)
			{
				CurrentAnimInstance->Montage_JumpToSection(sectionName, montage);
			}
		}
	}

	return mLength;
}

float UBT_PlayMontage::GetPosition() const
{
	if (!IsValid(CurrentMontage))
		return 0.f;

	if (!IsValid(CurrentAnimInstance))
		return 0.f;

	return CurrentAnimInstance->Montage_GetPosition(CurrentMontage);
}

float UBT_PlayMontage::GetBlendOutTriggerTime() const
{
	if (!IsValid(CurrentMontage))
		return 0.f;

	const float blendOutTriggerTime = mLength - CurrentMontage->BlendOut.GetBlendTime();
	return std::max(blendOutTriggerTime, 0.f);
}

float UBT_PlayMontage::GetPlayRate() const
{
	if (!IsValid(CurrentMontage))
		return 0.f;

	if (!IsValid(CurrentAnimInstance))
		return 0.f;

	return CurrentAnimInstance->Montage_GetPlayRate(CurrentMontage);
}

void UBT_PlayMontage::SetPlayRate(const float rate)
{
	if (IsValid(CurrentMontage) && IsValid(CurrentAnimInstance))
	{
		CurrentAnimInstance->Montage_SetPlayRate(CurrentMontage, rate);
		mInPlayRate = rate;
	}
}

bool UBT_PlayMontage::GetSectionStartAndEndTime(float& outStartTime, float& outEndTime, const FName& sectionName) const
{
	if (!IsValid(CurrentMontage))
		return false;

	const auto sectionIndex = CurrentMontage->GetSectionIndex(sectionName);
	if (sectionIndex == INDEX_NONE)
		return false;

	CurrentMontage->GetSectionStartAndEndTime(sectionIndex, outStartTime, outEndTime);

	return true;
}

bool UBT_PlayMontage::IsFinish(const float deltaSecond) const
{
	if (!IsValid(CurrentMontage))
		return true;

	if (!IsValid(CurrentAnimInstance))
		return true;

	if (CurrentAnimInstance->Montage_GetIsStopped(CurrentMontage))
		return true;

	const float position = CurrentAnimInstance->Montage_GetPosition(CurrentMontage);
	return (position + (deltaSecond * mInPlayRate)) >= mLength;
}

bool UBT_PlayMontage::IsFinish() const
{
	return IsPlaying() == false;
}

bool UBT_PlayMontage::IsFinishAll() const
{
	if (!IsValid(CurrentAnimInstance))
		return false;

	return CurrentAnimInstance->Montage_IsPlaying(nullptr) == false;
}

bool UBT_PlayMontage::IsPlaying() const
{
	if (!IsValid(CurrentMontage))
		return false;

	if (!IsValid(CurrentAnimInstance))
		return false;

	return CurrentAnimInstance->Montage_IsPlaying(CurrentMontage);
}

bool UBT_PlayMontage::IsOnTime(const float time, const float deltaSecond) const
{
	if (!IsValid(CurrentMontage))
		return false;

	if (!IsValid(CurrentAnimInstance))
		return false;

	const float nowPosition = CurrentAnimInstance->Montage_GetPosition(CurrentMontage);
	const float lastPosition = nowPosition - (deltaSecond * mInPlayRate);
	return lastPosition <= time && time <= nowPosition;
}

FName UBT_PlayMontage::GetCurrentSection() const
{
	if (!IsValid(CurrentMontage))
		return TEXT("");

	if (!IsValid(CurrentAnimInstance))
		return TEXT("");

	return CurrentAnimInstance->Montage_GetCurrentSection(CurrentMontage);
}

void UBT_PlayMontage::Stop(const float blendOutTime)
{
	if (IsValid(CurrentAnimInstance) && IsValid(CurrentMontage))
	{
		CurrentAnimInstance->Montage_Stop(blendOutTime, CurrentMontage);
	}
}

void UBT_PlayMontage::Reset(const float blendOutTime)
{
	Stop(blendOutTime);
	CurrentMontage = nullptr;
	CurrentAnimInstance = nullptr;
}

UAnimMontage* UBT_PlayMontage::GetCurrentMontage()
{
	return CurrentMontage;
}

const UAnimMontage* UBT_PlayMontage::GetCurrentMontage() const
{
	return CurrentMontage;
}

UAnimInstance* UBT_PlayMontage::GetCurrentAnimInstance()
{
	return CurrentAnimInstance;
}

const UAnimInstance* UBT_PlayMontage::GetCurrentAnimInstance() const
{
	return CurrentAnimInstance;
}

float UBT_PlayMontage::GetLength() const
{
	return mLength;
}

bool UBT_PlayMontage::IsRootMotionEnabled() const
{
	return CurrentAnimInstance && CurrentAnimInstance->RootMotionMode != ERootMotionMode::Type::IgnoreRootMotion;
}
