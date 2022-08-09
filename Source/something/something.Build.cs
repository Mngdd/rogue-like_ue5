// Fill out your copyright notice in the Description page of Project Settings.
// noscope 360 omg 420 mlg doritos mtndew wtf iluminati nonstop gaming

using UnrealBuildTool;

public class something : ModuleRules
{
	public something(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicDependencyModuleNames.AddRange(new string[] {"PolygonalMapGenerator", "Delaunator"});
		
		//PublicIncludePaths.AddRange(new string[] {"Delaunator/Public"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
