/**
@author		Shun Moriya
*/

using UnrealBuildTool;
using System.Collections.Generic;

public class DungeonGeneratorDemoEditorTarget : TargetRules
{
	public DungeonGeneratorDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange(
			new string[]
			{
                "DungeonGeneratorDemoEditor",
				"DungeonGeneratorDemo"
			}
		);
	}
}
