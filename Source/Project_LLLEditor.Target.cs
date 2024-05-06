// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_LLLEditorTarget : TargetRules
{
	public Project_LLLEditorTarget(TargetInfo Target) : base(Target)
	{
		bOverrideBuildEnvironment = true;
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Project_LLL");
	}
}
