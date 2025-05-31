/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include "EnemyAppearanceAssetFactory.generated.h"

/**
UEnemyAppearanceAssetファクトリークラス
*/
UCLASS()
class DUNGEONGENERATORDEMOEDITOR_API UEnemyAppearanceAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	// コンストラクタ
	UEnemyAppearanceAssetFactory();
	virtual ~UEnemyAppearanceAssetFactory() override = default;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
