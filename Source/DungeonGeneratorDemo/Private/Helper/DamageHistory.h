/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <list>
#include "DamageHistory.generated.h"

UENUM(BluePrintType)

enum class EDamageHistoryRegisterResult : uint8
{
	Failure,
	Success,
};

USTRUCT()
struct FDamageHistory
{
	GENERATED_BODY()

	bool Register(const AActor* actor);
	bool Contains(const AActor* actor) const;
	void Update(const float deltaSecond);

protected:
	UPROPERTY(EditAnywhere)
	float TimeUntilExpiration = .1f;

private:
	struct DamageLog final
	{
		const AActor* mActor;
		float mTime;

		explicit DamageLog(const AActor* actor);
	};
	std::list<DamageLog> History;
};
