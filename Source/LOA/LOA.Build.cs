// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LOA : ModuleRules
{
	public LOA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG"});
	}
}
