// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Task1_Labirint : ModuleRules
{
	public Task1_Labirint(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
