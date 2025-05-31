/**
@author		Shun Moriya
レベルアップに必要な経験値
*/

#include "DataAsset/LevelExperienceAsset.h"
#include "DataAssetHelper.h"
#include <Internationalization/StringTable.h>

#if WITH_EDITOR
#include <Algo/Sort.h>
#include <Misc/FileHelper.h>
#include <Misc/MessageDialog.h>
#endif

FText FLevelExperienceData::GetTitle() const
{
	if (mOwnerAsset && mOwnerAsset->GetStringTable())
	{
		return FText::FromStringTable(
			mOwnerAsset->GetStringTable()->GetStringTableId(),
			Name + TEXT("_Name"),
			EStringTableLoadingPolicy::FindOrFullyLoad
		);
	}

	return FText();
}

const UStringTable* ULevelExperienceAsset::GetStringTable() const
{
	return StringTable;
}

void ULevelExperienceAsset::Build()
{
#if WITH_EDITORONLY_DATA
	// データテーブルの設定チェック
	if (DataTable == nullptr)
	{
		const FText title = FText::FromString(TEXT("LevelExperienceDataAsset Error"));
		const FText body = FText::FromString(TEXT("LevelExperienceDataTable is Null !!"));
		FMessageDialog::Open(EAppMsgType::Ok, body, &title);
		return;
	}
	
	// データテーブルの型チェック
	if(!DataTable->GetRowStruct()->IsChildOf(FLevelExperienceTableRow::StaticStruct()))
	{
		const FText title = FText::FromString(TEXT("LevelExperienceDataAsset Error"));
		const FText body = FText::FromString(TEXT("LevelExperienceDataTable type does not match !!"));
		FMessageDialog::Open(EAppMsgType::Ok, body, &title);
		return;
	}

	// データテーブルの行の要素を配列で取得
	auto names = DataTable->GetRowNames();
	Algo::Sort(names, [this](const FName& l, const FName& r)
		{
			const auto* rowL = DataTable->FindRow<FLevelExperienceTableRow>(l, FString());
			const auto* rowR = DataTable->FindRow<FLevelExperienceTableRow>(r, FString());
			return rowL->Experience < rowR->Experience;
		}
	);

	Data.Empty();
	for (int32 i = 0; i < names.Num(); ++i)
	{
		const auto* row = DataTable->FindRow<FLevelExperienceTableRow>(names[i], FString());
		FLevelExperienceData data;
		data.Name = names[i].ToString();
		data.Experience = row->Experience;
		data.RoomCharges = row->RoomCharges;

		Data.Add(data);
	}
	
	if (!GenerateStringTableCsv(names))
		return;	

	// データアセットに編集フラグを追加
	MarkPackageDirty();

#if 0
	// データアセットを保存対象に追加
	PackagesToSave.Add(GetOutermost());

	// 関連アセットを全て保存（SourceControl使用時はチェックアウトするかメッセージウィンドウを出す）
	// ファイル編集フラグ（Dirty)が付いてるもののみを保存対象にしたいので第一引数はtrue
	// 保存する際に確認のメッセージウィンドウを出さない場合は第二引数をfalseにする
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, true); 
#endif
#endif
}

bool ULevelExperienceAsset::GenerateStringTableCsv(const TArray<FName>& names) const
{
#if WITH_EDITORONLY_DATA
	if (OutputStringTableCsvPath.IsEmpty())
	{
		const FText title = FText::FromString(TEXT("LevelExperienceDataAsset Error"));
		const FText body = FText::FromString(TEXT("No level experience string table csv name."));
		FMessageDialog::Open(EAppMsgType::Ok, body, &title);
		return false;
	}

	FString pathPart, filenamePart, extensionPart;
	FPaths::Split(OutputStringTableCsvPath, pathPart, filenamePart, extensionPart);
	const FString outputPath = FPaths::ProjectContentDir() + OutputStringTableCsvPath;

	TArray<FString> output;

	if (FFileHelper::LoadFileToStringArray(output, *outputPath))
	{
		for (int32 i = 0; i < names.Num(); ++i)
		{
			const FString& name = names[i].ToString();
			if (DungeonGenerator::DataAsset::ContainsInStringArray(output, name) == false)
			{
				output.Add(TEXT("\"") + name + TEXT("_Name\", \"\""));
				output.Add(TEXT("\"") + name + TEXT("_Description\", \"\""));
			}
		}
	}
	else
	{
		output.Add(TEXT("Key,SourceString"));

		for (int32 i = 0; i < names.Num(); ++i)
		{
			output.Add(TEXT("\"") + names[i].ToString() + TEXT("_Name\",\"\""));
			output.Add(TEXT("\"") + names[i].ToString() + TEXT("_Description\",\"\""));
		}
	}

	if (!FFileHelper::SaveStringArrayToFile(output, *outputPath, FFileHelper::EEncodingOptions::ForceUTF8))
	{
		const FText title = FText::FromString(TEXT("LevelExperienceDataAsset Error"));
		const FText body = FText::FromString(TEXT("Write level experience string table csv failed."));
		FMessageDialog::Open(EAppMsgType::Ok, body, &title);
		return false;
	}
#endif

	return true;
}

const FLevelExperienceData& ULevelExperienceAsset::Get(const int32 index) const
{
	if (index >= Data.Num())
	{
		static const FLevelExperienceData dummy;
		return dummy;
	}

	const auto* data = &Data[index];
	const_cast<FLevelExperienceData*>(data)->mOwnerAsset = this;
	return *data;
}
