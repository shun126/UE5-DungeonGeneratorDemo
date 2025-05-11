/**
@author		Shun Moriya
*/

#pragma once
#include "DamageHistory.h"

FDamageHistory::DamageLog::DamageLog(const AActor* actor)
	: mActor(actor)
	, mTime(0.f)
{
}

bool FDamageHistory::Register(const AActor* actor)
{
	if (Contains(actor) == true)
		return false;

	History.emplace_front(actor);
	return true;
}

bool FDamageHistory::Contains(const AActor* actor) const
{
	const auto i = std::find_if(History.begin(), History.end(), [actor](const DamageLog& log)
		{
			return log.mActor == actor;
		}
	);
	return i != History.end();
}

void FDamageHistory::Update(const float deltaSecond)
{
	auto i = History.begin();
	while (i != History.end())
	{
		i->mTime += deltaSecond;
		if (i->mTime >= TimeUntilExpiration)
		{
			History.erase(i, History.end());
			return;
		}
		++i;
	}
}
