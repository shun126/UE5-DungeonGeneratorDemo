/**
@author		Shun Moriya
*/

using UnrealBuildTool;

public class DungeonGeneratorDemoEditor : ModuleRules
{
	public DungeonGeneratorDemoEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] {
            "DungeonGeneratorDemoEditor/Public",
            "DungeonGeneratorDemo/Public"
        });
        PrivateIncludePaths.AddRange(new string[] {
            "DungeonGeneratorDemoEditor/Private",
            "DungeonGeneratorDemo/Private"
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "InputCore",
            "AssetTools",
            "Slate",
            "SlateCore",
            "PropertyEditor",
            "DungeonGeneratorDemo",
            "UnrealEd"
        });
    }
}
