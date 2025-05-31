/**
@author		Shun Moriya
*/

#pragma once
#include <CoreMinimal.h>
#include <AssetTypeActions_Base.h>

/**
UEnemyAppearanceAsset型のアクション クラス
*/
class DUNGEONGENERATORDEMOEDITOR_API FEnemyAppearanceAssetTypeActions : public FAssetTypeActions_Base
{
public:
	// コンストラクタ
	FEnemyAppearanceAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);
	virtual ~FEnemyAppearanceAssetTypeActions() override = default;

	virtual FColor GetTypeColor() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	// IAssetTypeActions Implementation
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type mAssetCategory;
};
