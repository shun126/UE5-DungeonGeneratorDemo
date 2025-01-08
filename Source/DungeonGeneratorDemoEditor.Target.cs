<<<<<<< HEAD
// Narcis software
=======
/**
@author		Shun Moriya
@copyright	2024- Shun Moriya
All Rights Reserved.
*/
>>>>>>> origin/54-steam

using UnrealBuildTool;
using System.Collections.Generic;

public class DungeonGeneratorDemoEditorTarget : TargetRules
{
	public DungeonGeneratorDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "DungeonGeneratorDemo" } );
	}
}
