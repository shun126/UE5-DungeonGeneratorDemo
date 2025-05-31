#include "DataAssetHelper.h"

namespace DungeonGenerator
{
	namespace DataAsset
	{
		bool ContainsInStringArray(const TArray<FString>& array, const FString& target)
		{
			const FString name = TEXT("\"") + target;
			for (const auto& line : array)
			{
				if (line.StartsWith(name))
					return true;
			}
			return false;
		}
	}
}
