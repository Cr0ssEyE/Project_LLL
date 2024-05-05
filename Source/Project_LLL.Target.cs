// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_LLLTarget : TargetRules
{
	public Project_LLLTarget(TargetInfo Target) : base(Target)
	{
		bOverrideBuildEnvironment = true;
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Project_LLL");
	}
}
