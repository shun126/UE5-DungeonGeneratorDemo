/**
@author		Shun Moriya
*/

#include "EnemyControllerBase.h"
#include "Component/AIPerceptionPlusComponent.h"
#include <BrainComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AIPerceptionTypes.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Damage.h>

namespace
{
	static int32 CharacterTypeCount = 0;
}

AEnemyControllerBase::AEnemyControllerBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	CharacterType = CharacterTypeCount++;
}

void AEnemyControllerBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetHeadLocRot(out_Location, out_Rotation);
}

const FFloatInterval& AEnemyControllerBase::GetAttackPreparationDistanceRange() const noexcept
{
	return AttackPreparationDistanceRange;
}

void AEnemyControllerBase::BeginPlay()
{
	// 親クラスを呼び出す
	Super::BeginPlay();

	// チームを設定
	SetGenericTeamId(FGenericTeamId(100));

	mAiPerceptionComponent = FindComponentByClass<UAIPerceptionPlusComponent>();
	if (IsValid(mAiPerceptionComponent))
	{
		mAiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerBase::OnPerceptionUpdated);
		mAiPerceptionComponent->OnExpiredStimulus.AddDynamic(this, &AEnemyControllerBase::OnHandleExpiredStimulus);
	}
}

void AEnemyControllerBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	// 親クラスを呼び出す
	Super::EndPlay(endPlayReason);

	// 首（頭）のエイミングコンポーネントを解除
	if (mAiPerceptionComponent)
	{
		mAiPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyControllerBase::OnPerceptionUpdated);
		mAiPerceptionComponent->OnExpiredStimulus.RemoveDynamic(this, &AEnemyControllerBase::OnHandleExpiredStimulus);
		mAiPerceptionComponent = nullptr;
	}
}

void AEnemyControllerBase::Dead()
{
	auto brain = FindComponentByClass<UBrainComponent>();
	if (IsValid(brain) == true && brain->IsPaused() == false)
	{
		brain->PauseLogic(TEXT("Dead"));

		StopMovement();
		ClearFocus(EAIFocusPriority::Move);
	}
}

void AEnemyControllerBase::OnHandleExpiredStimulus(FAIStimulus stimulus)
{
	check(stimulus.IsExpired());
	if (auto blackboard = GetBlackboardComponent())
	{
		if (stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			blackboard->SetValueAsObject(TEXT("TargetActor"), nullptr);
		}
	}
}

void AEnemyControllerBase::OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	if (stimulus.WasSuccessfullySensed())
	{
		if (stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (auto blackboard = GetBlackboardComponent())
			{
				blackboard->SetValueAsObject(TEXT("TargetActor"), actor);

				/*
				最後に目視した場所を記録する
				アクターを見失った場合はStimulusLocationを基準に調べる
				*/
				blackboard->SetValueAsBool(TEXT("IsStimulusEnable"), true);
				blackboard->SetValueAsVector(TEXT("StimulusLocation"), stimulus.StimulusLocation);
			}
		}
		else if (stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())
		{
			if (auto blackboard = GetBlackboardComponent())
			{
				blackboard->SetValueAsBool(TEXT("IsStimulusEnable"), true);
				blackboard->SetValueAsVector(TEXT("StimulusLocation"), stimulus.StimulusLocation);
			}
		}
		else if (stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			if (auto blackboard = GetBlackboardComponent())
			{
				blackboard->SetValueAsBool(TEXT("IsStimulusEnable"), true);
				blackboard->SetValueAsVector(TEXT("StimulusLocation"), stimulus.StimulusLocation);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Unsupported AIStimulus type (%s)"), *stimulus.Type.Name.ToString());
		}
	}
}

ETeamAttitude::Type AEnemyControllerBase::GetTeamAttitudeTowards(const AActor& other) const
{
	if (const APawn* otherPawn = Cast<APawn>(&other))
	{
		if (const IGenericTeamAgentInterface* teamAgent = Cast<IGenericTeamAgentInterface>(otherPawn->GetController()))
		{
			const FGenericTeamId otherTeamID = teamAgent->GetGenericTeamId();

			// どのチームにも所属していないなら中立
			static const FGenericTeamId noTeamId;
			if (otherTeamID.GetId() == noTeamId.GetId())
				return ETeamAttitude::Neutral;

			// 通常の判定
			const ETeamAttitude::Type type =
				Super::GetTeamAttitudeTowards(*otherPawn->GetController());
			return type;
		}
	}

	return ETeamAttitude::Neutral;
}

int32 AEnemyControllerBase::GetCharacterType() const
{
	return CharacterType;
}

#if WITH_EDITOR
void AEnemyControllerBase::DebugInformation(TArray<FString>& output) const
{
}
#endif
