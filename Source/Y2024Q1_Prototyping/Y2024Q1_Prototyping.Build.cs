// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Y2024Q1_Prototyping : ModuleRules
{
	public Y2024Q1_Prototyping(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
