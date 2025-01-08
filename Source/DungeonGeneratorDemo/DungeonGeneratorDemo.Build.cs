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

public class DungeonGeneratorDemo : ModuleRules
{
	public DungeonGeneratorDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
<<<<<<< HEAD
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
=======
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore"
		});

>>>>>>> origin/54-steam

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
<<<<<<< HEAD

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
=======
		PrivateDependencyModuleNames.AddRange(new string[] {
			"AIModule", "NavigationSystem",
			"OnlineSubsystem",
			"DungeonGenerator"
		});

        if (
			Target.Platform == UnrealTargetPlatform.Win64 ||
			Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateDependencyModuleNames.Add("OnlineSubsystemSteam");
        }

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
>>>>>>> origin/54-steam
}
