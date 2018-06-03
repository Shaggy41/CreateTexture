// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class CreateTexture : ModuleRules
{
	public CreateTexture(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // Allow include "Android/AndroidJNI.h"
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            // Add Android_UPL.xml this file must be in <project>/Source/<projectname>/
            string UPLPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(UPLPath, "Android_UPL.xml")));
        }
        // Uncomment if you are using Slate UI
         PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
