// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASMultiplayer : ModuleRules
{
	public GASMultiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore", 
			"EnhancedInput" 
		});
		
		PublicIncludePaths.Add("GASMultiplayer/");
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{ 
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
	}
}
