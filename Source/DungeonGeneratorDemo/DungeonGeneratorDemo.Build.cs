// Narcis software

using UnrealBuildTool;

public class DungeonGeneratorDemo : ModuleRules
{
	public DungeonGeneratorDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore"
		});


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		PrivateDependencyModuleNames.AddRange(new string[] {
			"AIModule", "NavigationSystem",
			"OnlineSubsystemSteam", "OnlineSubsystem",
			"DungeonGenerator"
		});

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
