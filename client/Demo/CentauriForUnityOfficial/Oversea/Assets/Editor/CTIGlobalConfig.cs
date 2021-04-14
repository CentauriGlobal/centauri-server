﻿using UnityEditor;
 using UnityEngine;
/**
 * <pre>
 *     @ author : dongbingliu
 *     @ e-mail : dongbingliu@tencent.com
 *     @ time   : 2020年11月3日
 *     @ desc   : 自动填充Unity签名密码
 *     version: 1.0
 * </pre>
 */
[InitializeOnLoad]
public class CTIGlobalConfig
{
    static CTIGlobalConfig()
    {
        PlayerSettings.Android.keystoreName = Application.dataPath+"/Plugins/Android/centauri_sign.keystore";
        PlayerSettings.Android.keystorePass = "123456";
        PlayerSettings.Android.keyaliasName = "centauri";
        PlayerSettings.Android.keyaliasPass = "123456";
    }
}