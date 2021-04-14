// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class CentauriSDK : ModuleRules
{
	public CentauriSDK(ReadOnlyTargetRules Target) : base(Target)
	{
        	bEnableShadowVariableWarnings = false;
        	bEnableUndefinedIdentifierWarnings = false;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"CentauriSDK/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"CentauriSDK/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		//add CTI
		if (Target.Platform == UnrealTargetPlatform.Android) {
		            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
		            string pluginPath = Utils.MakePathRelativeTo(ModuleDirectory, "..\\");
			string aplPath = pluginPath + "/CTIAndroid_APL.xml";
			AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", aplPath));
			System.Console.WriteLine("aplPath = " + aplPath);
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS) {
		        System.Console.WriteLine("--------------Add iOS CTI SDK");
		        
		        //Definitions.Add("With_iCentauri=1");
            PrivateIncludePaths.Add("CentauriSDK/Private/iOS");
            PublicIncludePaths.AddRange(new string[] {"Runtime/ApplicationCore/Public/Apple", "Runtime/ApplicationCore/Public/IOS"});
            
            PrivateDependencyModuleNames.AddRange(
                new string[]{
                    "ApplicationCore"
                // ... add private dependencies that you statically link with here ...
                }
                );
                
            		//CTI
			PublicAdditionalFrameworks.Add(new UEBuildFramework("CTIIAPSDK", "Private/iOS/libs/CTIIAPSDK.embeddedframework.zip", "CTIIAPSDK.bundle"));
			
			//system
			PublicFrameworks.AddRange( 
					new string[] 
					{
						"Security",
						"AdSupport",
						"CoreGraphics",
						"CoreLocation",
						"CoreTelephony",
						"EventKit",
						"EventKitUI",
						"Foundation",
						"MediaPlayer",
						"QuartzCore",
						"StoreKit",
						"SystemConfiguration",
						"UIKit",
						"AudioToolbox",
						"AVFoundation",
						"iAd",
						"MessageUI",
						"MobileCoreServices",
						"Social",
						"GameKit",
						"MobileCoreServices",
						"Metal",
						"CoreText",
						"CFNetwork",
						"CoreMedia",
						"CoreMotion",
						"CoreVideo",
						"OpenAL",
						"OpenGLES",
						//"UserNotifications",
					}
				);
			PublicAdditionalLibraries.AddRange(
				new string[] {"z","c++","z.1.1.3","sqlite3","xml2"}
				);
		}
	}
}
