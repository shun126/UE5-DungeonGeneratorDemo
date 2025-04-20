/**
@author		Shun Moriya
*/

#include "EnemyAppearanceAssetFactory.h"
#include "DataAsset/EnemyAppearanceAsset.h"

UEnemyAppearanceAssetFactory::UEnemyAppearanceAssetFactory()
{
	SupportedClass = UEnemyAppearanceAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UEnemyAppearanceAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UEnemyAppearanceAsset>(InParent, InClass, InName, Flags);
}
