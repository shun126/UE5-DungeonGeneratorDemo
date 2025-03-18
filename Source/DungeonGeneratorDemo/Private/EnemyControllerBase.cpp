#include "EnemyControllerBase.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AIPerceptionTypes.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISense_Sight.h>

AEnemyControllerBase::AEnemyControllerBase(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void AEnemyControllerBase::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	GetHeadLocRot(out_Location, out_Rotation);
}

void AEnemyControllerBase::BeginPlay()
{
	// 親クラスを呼び出す
	Super::BeginPlay();

	// チームを設定
	SetGenericTeamId(FGenericTeamId(100));

	// ターゲットが失効したことを通知
	ExpireTarget.AddDynamic(this, &AEnemyControllerBase::OnExpireTarget);

	// 首（頭）のエイミングコンポーネントを検索
	mAiPerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (IsValid(mAiPerceptionComponent))
	{
		mAiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerBase::OnPerceptionUpdated);
	}
}

void AEnemyControllerBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	// 親クラスを呼び出す
	Super::EndPlay(endPlayReason);

	// ターゲットが失効したことの通知を解除
	ExpireTarget.RemoveDynamic(this, &AEnemyControllerBase::OnExpireTarget);

	// 首（頭）のエイミングコンポーネントを解除
	if (IsValid(mAiPerceptionComponent) == true)
	{
		mAiPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyControllerBase::OnPerceptionUpdated);
	}
	mAiPerceptionComponent = nullptr;
}

void AEnemyControllerBase::OnExpireTarget(const AActor* targetActor)
{
	if (auto blackboard = GetBlackboardComponent())
	{
		blackboard->SetValueAsObject(TEXT("TargetActor"), nullptr);
	}
}

void AEnemyControllerBase::OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	if (stimulus.WasSuccessfullySensed())
	{
		if (stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			// Handle updates to the hearing sense
			// Example: calculate the distance between the stimulus receiver and the source
			//const auto distance = FVector::Dist(stimulus.ReceiverLocation, stimulus.StimulusLocation);

			if (auto blackboard = GetBlackboardComponent())
			{
				blackboard->SetValueAsBool(TEXT("IsStimulusEnable"), true);
				blackboard->SetValueAsVector(TEXT("StimulusLocation"), stimulus.StimulusLocation);
			}
		}
		else if (stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (auto blackboard = GetBlackboardComponent())
			{
				blackboard->SetValueAsObject(TEXT("TargetActor"), actor);
			}
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
