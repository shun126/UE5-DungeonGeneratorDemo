// Narcis software

using UnrealBuildTool;
using System.Collections.Generic;

public class DungeonGeneratorDemoEditorTarget : TargetRules
{
	public DungeonGeneratorDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "DungeonGeneratorDemo" } );
	}
}
