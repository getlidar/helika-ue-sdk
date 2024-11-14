// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Helika : ModuleRules
{
	public Helika(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Sockets",
                "Networking",
                "OpenSSL"
            }
			);

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd"	
				});
		}
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"OpenSSL"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicFrameworks.AddRange(new string[]
			{
				"AdSupport",
				"AppTrackingTransparency"
			});
		}
	}
}
