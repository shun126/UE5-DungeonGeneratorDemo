/**
@author		Shun Moriya
*/

using UnrealBuildTool;
using System.Collections.Generic;

public class DungeonGeneratorDemoTarget : TargetRules
{
	public DungeonGeneratorDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.Add("DungeonGeneratorDemo");

		if (bBuildEditor)
		{
            ExtraModuleNames.Add("DungeonGeneratorDemoEditor");
		}
	}
}
