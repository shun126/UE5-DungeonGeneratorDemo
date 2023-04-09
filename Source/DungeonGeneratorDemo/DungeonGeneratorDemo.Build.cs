// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonGeneratorDemo : ModuleRules
{
	public DungeonGeneratorDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"HeadMountedDisplay",
				"EnhancedInput",
				"DungeonGenerator"
			});
	}
}
