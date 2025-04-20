/**
@author		Shun Moriya
*/

#include "DataAsset/EnemyAppearanceAsset.h"

void FEnemyAppearanceData::LoadEnemyAvatarClass()
{
	if (IsValid(EnemyAvatarClass) == false && EnemyAvatarPath.IsValid() == true)
	{
		const FSoftObjectPath path(EnemyAvatarPath.ToString() + "_C");
		TSoftClassPtr<AActor> softClassPointer(path);
		EnemyAvatarClass = TSoftClassPtr<AActor>(softClassPointer).LoadSynchronous();
	}
}

float FEnemyAppearanceData::GetPriority(const float depthFromStartRatio) const
{
	return 1.f - std::abs(DepthRatio - depthFromStartRatio);
}

UEnemyAppearanceAsset::UEnemyAppearanceAsset(const FObjectInitializer& initializer)
	: Super(initializer)
{
}

const FEnemyAppearanceData& UEnemyAppearanceAsset::Get(const int32 index) const
{
	FEnemyAppearanceData& enemyAppearanceData = const_cast<FEnemyAppearanceData&>(EnemyAppearanceData[index]);
	enemyAppearanceData.LoadEnemyAvatarClass();
	return EnemyAppearanceData[index];
}

const FEnemyAppearanceData& UEnemyAppearanceAsset::DrawEnemy(const float depthFromStartRatio) const
{
	float totalPriority = 0.f;
	for (const FEnemyAppearanceData& data : EnemyAppearanceData)
	{
		totalPriority += data.GetPriority(depthFromStartRatio);
	}

	float r = FMath::RandRange(0.f, totalPriority);
	
	for (const FEnemyAppearanceData& data : EnemyAppearanceData)
	{
		const auto priority = data.GetPriority(depthFromStartRatio);
		if (r < priority)
		{
			const_cast<FEnemyAppearanceData&>(data).LoadEnemyAvatarClass();
			return data;
		}
		r -= priority;
	}

	static FEnemyAppearanceData invalidEnemyAppearanceData;
	return invalidEnemyAppearanceData;
}
