// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkyrimLike : ModuleRules
{
	public SkyrimLike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "SkyrimLike" });
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" ,"EnhancedInput", "UMG", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
