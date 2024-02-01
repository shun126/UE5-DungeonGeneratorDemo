// Narcis software

using UnrealBuildTool;
using System.Collections.Generic;

public class DungeonGeneratorDemoTarget : TargetRules
{
	public DungeonGeneratorDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "DungeonGeneratorDemo" } );
	}
}
