using System.Collections;
using System.IO;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using System;

class AutoProjectBuild : Editor{

	private static bool DEBUG_VERSION = true;
	private static string XcodeProjName = "XcodeProject";

	//在这里找出你当前工程所有的场景文件，假设你只想把部分的scene文件打包 那么这里可以写你的条件判断 总之返回一个字符串数组。
	static string[] GetBuildScenes()
	{
		List<string> names = new List<string>();
		
		foreach(EditorBuildSettingsScene e in EditorBuildSettings.scenes)
		{
			if(e==null)
				continue;
			if(e.enabled)
				names.Add(e.path);
		}
		return names.ToArray();
	}
	
	//得到项目的名称
	public static string projectName
	{
		get
		{
			//在这里分析shell传入的参数， 还记得上面我们说的哪个 xpj-$1 这个参数吗？
			//这里遍历所有参数，找到 XCodeProject开头的参数， 然后把-符号 后面的字符串返回，
			foreach(string arg in System.Environment.GetCommandLineArgs()) {
				if(arg.StartsWith(XcodeProjName))
				{
					return arg.Split("-"[0])[1];
				}
			}
			return XcodeProjName;
		}
	}
	//shell脚本直接调用这个静态方法
	static void BuildForIOS()
	{
		Debug.Log("BuildForIOS");

		//打包之前先设置一下 预定义标签, 这样在代码中可以灵活的开启或者关闭 一些代码。
		//因为 这里我是承接 上一篇文章， 我就以sharesdk做例子 ，这样方便大家学习 ，
		PlayerSettings.SetScriptingDefineSymbolsForGroup(BuildTargetGroup.iOS, projectName);


		BuildOptions ops = BuildOptions.None;
		if (DEBUG_VERSION)
		{
			ops |= BuildOptions.Development;
			ops |= BuildOptions.AllowDebugging;
			ops |= BuildOptions.ConnectWithProfiler;
		}
		else
		{
			ops |= BuildOptions.None;
		}


		//这里就是构建xcode工程的核心方法了，
		//参数1 需要打包的所有场景
		//参数2 需要打包的名子， 这里取到的就是 shell传进来的字符串 SEA/LOCAL
		//参数3 打包平台
		BuildPipeline.BuildPlayer(GetBuildScenes(), XcodeProjName+"-"+projectName, BuildTarget.iOS, ops);
	}




	private static string GetCommandLineArg(string name)
	{
		string[] args = System.Environment.GetCommandLineArgs();
		for (int i=0; i<args.Length; i++)
		{
			if(args[i].ToLower() == name)
			{
				if(args.Length > i+1)
				{
					return args[i+1];
				}
			}
		}
		return "";
	}
	
	
	private static string ANDROID_PATH = "Bin/MidasDemo.apk";
	public static void BuildForAndroid()
	{
		EditorPrefs.SetString("AndroidSdkRoot",GetCommandLineArg("-androidsdkroot"));
		
		PlayerSettings.Android.keystoreName = Application.dataPath+"/Plugins/Android/centauri_sign.keystore";
		PlayerSettings.Android.keystorePass = "123456";
		PlayerSettings.Android.keyaliasName = "centauri";
		PlayerSettings.Android.keyaliasPass = "123456";
		
		// PlayerSettings.Android.keystoreName = Application.dataPath+"/Plugins/Android/midas_debug2.keystore";
		// PlayerSettings.Android.keystorePass = "mima1234";
		// PlayerSettings.Android.keyaliasName = "midas";
		// PlayerSettings.Android.keyaliasPass = "!debug";
		PlayerSettings.applicationIdentifier = "com.tencent.imsdk.samples";
		PlayerSettings.bundleVersion = "1.3";
		PlayerSettings.Android.bundleVersionCode = 3;
		PlayerSettings.productName = "Google Play Demo";
		
		BuildOptions ops = BuildOptions.None;
		if (DEBUG_VERSION)
		{
			ops |= BuildOptions.Development;
			ops |= BuildOptions.AllowDebugging;
			ops |= BuildOptions.ConnectWithProfiler;
		}
		else
		{
			ops |= BuildOptions.None;
		}
		BuildPipeline.BuildPlayer(GetBuildScenes(), ANDROID_PATH, BuildTarget.Android, ops);
	}
}