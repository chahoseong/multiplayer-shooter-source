// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dust : ModuleRules
{
	public Dust(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.AddRange([
			"Dust"
		]);
		
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
		]);

		PrivateDependencyModuleNames.AddRange([
			"InputCore",
			"EnhancedInput",
			"NetCore",
		]);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
