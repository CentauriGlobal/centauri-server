
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEditor.Callbacks;
using UnityEditor.iOS.Xcode.Custom;
using System.IO;
using System;


public static class XCodePostProcess
{

#if UNITY_EDITOR
	[PostProcessBuild (1000)]
	public static void OnPostProcessBuild (BuildTarget target, string pathToBuiltProject)
	{
		if (target != BuildTarget.iOS) return;

		string projPath = PBXProject.GetPBXProjectPath(pathToBuiltProject);
		PBXProject project = new PBXProject();
		project.ReadFromString(File.ReadAllText(projPath));


		//XcodeAPI源代码：https://bitbucket.org/Unity-Technologies/xcodeapi/src/stable/
		//PBXProject的API说明：https://docs.unity3d.com/ScriptReference/iOS.Xcode.PBXProject.html

		GenerateProjectFile(project, pathToBuiltProject);

		string plistPath = pathToBuiltProject + "/Info.plist";
		PlistDocument plist = new PlistDocument();
		plist.ReadFromString(File.ReadAllText(plistPath));

		GeneratePlistFile(plist.root, plistPath);

		File.WriteAllText(projPath, project.WriteToString());
		File.WriteAllText(plistPath, plist.WriteToString());

//			DeployIOS.EditorCode(pathToBuiltProject);	
		
	}


	/// <summary>
	/// 处理XCode工程
	/// </summary>
	/// <param name="project"></param>
	/// <param name="pathToBuiltProject"></param>
	private static void GenerateProjectFile(PBXProject project, string pathToBuiltProject)
	{
		string target = project.TargetGuidByName(PBXProject.GetUnityTargetName());

		project.SetBuildProperty(target, "CODE_SIGN_IDENTITY", "iPhone Developer: Deren Su (6CLGCK4S32)");
		project.SetBuildProperty(target, "PROVISIONING_PROFILE_SPECIFIER", "wild_card_profile");
		project.SetBuildProperty(target, "DEVELOPMENT_TEAM", "3663N95WE7");
		project.SetBuildProperty(target, "CODE_SIGNING_STYLE", "MANUAL");
		project.SetBuildProperty(target, "ENABLE_BITCODE", "NO");

		// 添加flag
		project.AddBuildProperty(target, "OTHER_LDFLAGS", "-ObjC -all_load");

		// 添加系统库
		AddSystemFramework(project, target);

		// 添加第3方
		AddOther(project, target, pathToBuiltProject);

		AddKeyChain(project, pathToBuiltProject);
//		project.AddCapability(target, PBXCapabilityType.KeychainSharing);
//		project.AddCapability(target, PBXCapabilityType.PushNotifications);
//		project.AddCapability(target, PBXCapabilityType.InAppPurchase);
//		project.AddCapability(target, PBXCapabilityType.BackgroundModes);

	}

	private static void AddSystemFramework(PBXProject project, string target)
	{
		string[] frameworks =
		{
			"libz.dylib",
			"libc++.dylib",

			"UIKit.framework",
		};

		foreach (var framework in frameworks)
		{
			project.AddFrameworkToProject(target, framework, false);
		}
	}

	private static void AddOther(PBXProject project, string target, string pathToBuiltProject)
	{
//		// 添加米大师相关
//		project.AddBuildProperty(target, "FRAMEWORK_SEARCH_PATHS", "$(PROJECT_DIR)/Third/Midas/local");
//		AddFile(project, target, "Third/Midas/local/MidasIAPSDK.bundle");
//		AddFile(project, target, "Third/Midas/local/MidasIAPSDK.framework");
//		AddFile(project, target, "Third/Midas/Script/MidasLocalIAPSDKAdapter.h");
//		AddFile(project, target, "Third/Midas/Script/MidasSdkConnector.h");
//		AddFile(project, target, "Third/Midas/Script/MidasLocalIAPSDKAdapter.m");
//		AddFile(project, target, "Third/Midas/Script/MidasSdkConnector.m");
	}

	private static void AddFile(PBXProject project, string target, string destPath)
	{
		project.AddFileToBuild(target, project.AddFile(destPath, destPath, PBXSourceTree.Source));
	}

	/// <summary>
	/// 处理Plist工程
	/// </summary>
	/// <param name="project"></param>
	/// <param name="plistPath"></param>
	private static void GeneratePlistFile(PlistElementDict rootDict, string plistPath)
	{
//		PlistElementArray urlArray = null;
//		if (!rootDict.values.ContainsKey("CFBundleURLTypes"))
//			urlArray = rootDict.CreateArray("CFBundleURLTypes");
//		else
//			urlArray = rootDict.values["CFBundleURLTypes"].AsArray();
//		var urlTypeDict = urlArray.AddDict();
//		urlTypeDict.SetString("CFBundleTypeRole", "Editor");
//		urlTypeDict.SetString("CFBundleURLName", "weixin");
//		var urlScheme = urlTypeDict.CreateArray("CFBundleURLSchemes");
//		urlScheme.AddString(DeploySettings.Instance.WxAppId);
//
//		urlTypeDict = urlArray.AddDict();
//		urlTypeDict.SetString("CFBundleTypeRole", "Editor");
//		urlTypeDict.SetString("CFBundleURLName", "tencentopenapi");
//		urlScheme = urlTypeDict.CreateArray("CFBundleURLSchemes");
//		urlScheme.AddString("tencent" + DeploySettings.Instance.QqAppId);
//
//		urlTypeDict = urlArray.AddDict();
//		urlTypeDict.SetString("CFBundleTypeRole", "Editor");
//		urlTypeDict.SetString("CFBundleURLName", "tencentvideo");
//		urlScheme = urlTypeDict.CreateArray("CFBundleURLSchemes");
//		urlScheme.AddString("tencentvideo" + DeploySettings.Instance.QqAppId);
//
//		urlTypeDict = urlArray.AddDict();
//		urlTypeDict.SetString("CFBundleTypeRole", "Editor");
//		urlTypeDict.SetString("CFBundleURLName", "QQ");
//		urlScheme = urlTypeDict.CreateArray("CFBundleURLSchemes");
//		urlScheme.AddString(DeploySettings.Instance.QqScheme);
//
//		urlTypeDict = urlArray.AddDict();
//		urlTypeDict.SetString("CFBundleTypeRole", "Editor");
//		urlTypeDict.SetString("CFBundleURLName", "QQLaunch");
//		urlScheme = urlTypeDict.CreateArray("CFBundleURLSchemes");
//		urlScheme.AddString("tencentlaunch" + DeploySettings.Instance.QqAppId);
//
//		rootDict.SetString("CHANNEL_DENGTA", "***");
//		rootDict.SetString("MSDK_OfferId", DeploySettings.Instance.IOSOfferId);
//		rootDict.SetString("MSDK_ENV", ConfigSettings.Instance.MsdkUrlEnv);
//		rootDict.SetString("QQAppID", DeploySettings.Instance.QqAppId);
//		rootDict.SetString("WXAppID", DeploySettings.Instance.WxAppId);
//		rootDict.SetString("MSDKKey", DeploySettings.Instance.MsdkKey);
//		rootDict.SetBoolean("MSDK_PUSH_SWITCH", true);
//		rootDict.SetBoolean("AutoRefreshToken", true);
//		rootDict.SetBoolean("NeedNotice", true);
//		rootDict.SetInteger("NoticeTime", 900);
//		rootDict.SetInteger("MSDK_REAL_NAME_AUTH_SWITCH", 1);
//		rootDict.SetBoolean("MSDK_Webview_Landscape_NavBar_Hideable", false);
//		rootDict.SetBoolean("MSDK_Webview_Portrait_NavBar_Hideable", false);
//
//		if (!rootDict.values.ContainsKey("LSApplicationQueriesSchemes"))
//			urlArray = rootDict.CreateArray("LSApplicationQueriesSchemes");
//		else
//			urlArray = rootDict.values["LSApplicationQueriesSchemes"].AsArray();
//		urlArray.AddString("mqq");
//		urlArray.AddString("mqqapi");
//		urlArray.AddString("wtloginmqq2");
//		urlArray.AddString("mqqopensdkapiV4");
//		urlArray.AddString("mqqopensdkapiV3");
//		urlArray.AddString("mqqopensdkapiV2");
//		urlArray.AddString("mqqwpa");
//		urlArray.AddString("mqqOpensdkSSoLogin");
//		urlArray.AddString("mqqgamebindinggroup");
//		urlArray.AddString("mqqopensdkfriend");
//		urlArray.AddString("mqzone");
//		urlArray.AddString("weixin");
//		urlArray.AddString("wechat");
//
//		rootDict.SetString("NSCameraUsageDescription", "");
//		rootDict.SetString("NSPhotoLibraryUsageDescription", "");
//
//
		if (rootDict.values.ContainsKey("NSAppTransportSecurity"))
			rootDict.values.Remove("NSAppTransportSecurity");
		PlistElementDict urlDict = rootDict.CreateDict("NSAppTransportSecurity");
		urlDict.SetBoolean("NSAllowsArbitraryLoads", false);	//false表示要走ATS
	}


	private static void AddKeyChain(PBXProject project, string pathToBuiltProject)
	{
		string unityTargetName = PBXProject.GetUnityTargetName ();
		string target = project.TargetGuidByName(unityTargetName);

		//Create entitlements
		string relativeEntitlementFilePath = unityTargetName + "/" + unityTargetName + ".entitlements";
		string absoluteEntitlementFilePath = pathToBuiltProject + "/" + relativeEntitlementFilePath;

		PlistDocument tempEntitlements = new PlistDocument();

		string key_KeychainSharing = "keychain-access-groups";
		var arr = (tempEntitlements.root[key_KeychainSharing] = new PlistElementArray()) as PlistElementArray;

		arr.values.Add(new PlistElementString("$(AppIdentifierPrefix)$(CFBundleIdentifier)"));
		arr.values.Add(new PlistElementString("$(AppIdentifierPrefix)com.centauri.wsj.keystoregroup"));

		project.AddCapability(target, PBXCapabilityType.KeychainSharing, relativeEntitlementFilePath);

		string projPath = PBXProject.GetPBXProjectPath(pathToBuiltProject);
		File.WriteAllText(projPath, project.WriteToString());
		tempEntitlements.WriteToFile(absoluteEntitlementFilePath);

		ModifyEntitlementFile(absoluteEntitlementFilePath);
	}

	private static void ModifyEntitlementFile(string absoluteEntitlementFilePath)
	{
		if (!File.Exists(absoluteEntitlementFilePath)) return;

		try
		{
			StreamReader reader = new StreamReader(absoluteEntitlementFilePath);
			var content = reader.ReadToEnd().Trim();
			reader.Close();

			var needFindString = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			var changeString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + "\n" + "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">";
			Debug.Log("Before: " + content);
			content = content.Replace(needFindString, changeString);
			Debug.Log("After: " + content);
			StreamWriter writer = new StreamWriter(new FileStream(absoluteEntitlementFilePath, FileMode.Create));
			writer.WriteLine(content);
			writer.Flush();
			writer.Close();
		}
		catch (Exception e)
		{
			Debug.Log("ModifyEntitlementFile - Failed: " + e.Message);
		}
	}


	private static void AddCode(string targetPath)
	{ 
		string UnityAppControllerH      = Application.dataPath+  "/Editor/CPPCource/UnityAppController.h";
		string UnityAppControllerMM     = Application.dataPath + "/Editor/CPPCource/UnityAppController.mm";

		string tagUnityAppControllerH   = targetPath + "/Classes/UnityAppController.h";
		string tagUnityAppControllerMM  = targetPath + "/Classes/UnityAppController.mm";

		if (File.Exists(tagUnityAppControllerH))
		{
			File.Delete(tagUnityAppControllerH);
		}
		if (File.Exists(tagUnityAppControllerMM))
		{
			File.Delete(tagUnityAppControllerMM);
		}

		File.Copy(UnityAppControllerH,  tagUnityAppControllerH);
		File.Copy(UnityAppControllerMM, tagUnityAppControllerMM);

	}

#endif
	
}