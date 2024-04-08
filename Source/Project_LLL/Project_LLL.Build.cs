// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_LLL : ModuleRules
{
	public Project_LLL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        {	
            "Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"NavigationSystem",
			"AIModule",
			"Niagara",
			"EnhancedInput",
			"GameplayTasks",
			"AdvancedWidgets",
			"GameFeatures",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"FMODStudio",
			"PhysicsCore",
			"LevelSequence",
			"MovieScene"
		});
        
		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"GameFeatures",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags"
		});
    }
}
