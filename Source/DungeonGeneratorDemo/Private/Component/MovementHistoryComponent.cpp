/**
@author		Shun Moriya



移動履歴
*/

#include "MovementHistoryComponent.h"

UMovementHistoryComponent::UMovementHistoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Resize(100);

	// Tickを有効化
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = 30.0f / 60.0f;
}

void UMovementHistoryComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 親クラスの呼び出し
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 移動履歴更新
	if (const AActor* ownerActor = GetValid(GetOwner()))
	{
		SetLocation(ownerActor->GetActorLocation());
	}
}

void UMovementHistoryComponent::Clear()
{
	mHistoryPointer = 0;
	mInitialized = false;
	mLoop = false;
}

void UMovementHistoryComponent::Resize(const int32 size)
{
	mLocationHistory.Init(FVector3f::Zero(), size);
	Clear();
}

int32 UMovementHistoryComponent::Size() const
{
	if (mLoop)
		return mLocationHistory.Num();
	else
		return mHistoryPointer;
}

FVector3f UMovementHistoryComponent::Get(const int32 index) const
{
	if (mLoop)
		return mLocationHistory[(mHistoryPointer + index) % Size()];
	else
		return mLocationHistory[index];
}

FVector UMovementHistoryComponent::GetLocation(const int32 index) const
{
	return FVector(Get(index));
}

FVector2D UMovementHistoryComponent::GetLocation2D(const int32 index) const
{
	const FVector3f& position = Get(index);
	return FVector2D(position.X, position.Y);
}

void UMovementHistoryComponent::SetIntervalDistance(const double intervalDistance)
{
	IntervalDistance = intervalDistance;
	Clear();
}

void UMovementHistoryComponent::SetLocation(const FVector& location)
{
	const FVector3f position(location);

	if (mInitialized)
	{
		const int32 previousHistoryPointer = static_cast<uint32>(mHistoryPointer - 1) % mLocationHistory.Num();
		const FVector3f& previousPosition = mLocationHistory[previousHistoryPointer];

		const auto dx = previousPosition.X - position.X;
		const auto dy = previousPosition.Y - position.Y;
		if ((dx * dx + dy * dy) < (IntervalDistance * IntervalDistance))
			return;
	}

	mLocationHistory[mHistoryPointer] = position;
	++mHistoryPointer;

	if (mLoop == false)
	{
		if (mHistoryPointer >= mLocationHistory.Num())
			mLoop = true;
	}
	mInitialized = true;

	mHistoryPointer %= mLocationHistory.Num();
}
