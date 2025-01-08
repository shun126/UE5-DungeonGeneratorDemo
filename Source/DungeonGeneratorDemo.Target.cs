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

public class DungeonGeneratorDemoTarget : TargetRules
{
	public DungeonGeneratorDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "DungeonGeneratorDemo" } );
	}
}
