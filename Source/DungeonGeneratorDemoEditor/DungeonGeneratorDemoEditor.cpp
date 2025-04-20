/**
@author		Shun Moriya
*/

#include "DungeonGeneratorDemoEditor.h"
#include "Public/EnemyAppearanceAssetTypeActions.h"
#include <IAssetTools.h>
#include <AssetToolsModule.h>

#define LOCTEXT_NAMESPACE "DungeonGeneratorDemoEditorModule"

void FDungeonGeneratorDemoEditorModule::StartupModule()
{
	InitializeAssetTools();
	InitializePropertyEditor();
}

void FDungeonGeneratorDemoEditorModule::ShutdownModule()
{
	FinalizeAssetTools();
	FinalizePropertyEditor();
}

void FDungeonGeneratorDemoEditorModule::InitializeAssetTools()
{
	// AssetToolsモジュールを取得
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// DungeonGeneratorDemoAsset
	EAssetTypeCategories::Type gameAssetCategory = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("DungeonGeneratorDemoAssets")),
		FText::FromName(TEXT("DungeonGeneratorDemo"))
	);

	// FEnemyAppearanceTypeActionsを追加
	{
		TSharedPtr<IAssetTypeActions> actionType = MakeShareable(new FEnemyAppearanceAssetTypeActions(gameAssetCategory));
		AssetTools.RegisterAssetTypeActions(actionType.ToSharedRef());
	}
}

void FDungeonGeneratorDemoEditorModule::FinalizeAssetTools()
{
}

void FDungeonGeneratorDemoEditorModule::InitializePropertyEditor()
{
	// PropertyEditorModule 取得
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// 変更の通知
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FDungeonGeneratorDemoEditorModule::FinalizePropertyEditor()
{
	// PropertyEditorModule 取得
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// FMeleeWeaponSelectorProperty の登録を解除
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout(("MeleeWeaponSelectorProperty"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDungeonGeneratorDemoEditorModule, DungeonGeneratorDemoEditor);
