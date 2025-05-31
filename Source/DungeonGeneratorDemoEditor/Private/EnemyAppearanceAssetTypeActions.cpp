/**
@author		Shun Moriya
*/

#include "EnemyAppearanceAssetTypeActions.h"
#include "DataAsset/EnemyAppearanceAsset.h"

FEnemyAppearanceAssetTypeActions::FEnemyAppearanceAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: mAssetCategory(InAssetCategory)
{
}

FText FEnemyAppearanceAssetTypeActions::GetName() const
{
	return FText::FromName(TEXT("EnemyAppearanceAsset"));
}

FColor FEnemyAppearanceAssetTypeActions::GetTypeColor() const
{
	return FColor::Red;
}

void FEnemyAppearanceAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
}

UClass* FEnemyAppearanceAssetTypeActions::GetSupportedClass() const
{
	return UEnemyAppearanceAsset::StaticClass();
}

uint32 FEnemyAppearanceAssetTypeActions::GetCategories()
{
	return mAssetCategory;
}
