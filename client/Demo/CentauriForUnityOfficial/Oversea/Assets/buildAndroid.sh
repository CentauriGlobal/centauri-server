#!/bin/sh

if [ $# != 1 ];then  

    echo "需要一个参数。 参数是游戏包的名子"  

    exit    

fi
echo Set Path Param

UNITY_PATH="/c/Program Files/Unity2018.4.15/Editor"
#SvnPath="/Users/xieliujian/Desktop/xieliujian/unity/unitygameframework_vultr/"
PROJECT_PATH="/h/GitCode/CentauriSDK/CentauriForUnityOfficial/Oversea"

echo Step 1 : Update svn

#cd $SvnPath
#svn update --username 你的svn帐号 --password 你的svn密码 ./

echo Step 2 : Compile excel

#cd ./Tools/Excel2Unity
#chmod 777 ./main.py
#python3 main.py

echo Step 3 : Build apk
echo Step 3.1 : Build Android Resource

#$UnityPath -quit -batchmode -projectPath $SvnClientPath -executeMethod Packager.BuildiPhoneResource

echo Step 3.2 : Build Android Apk

#$UnityPath -quit -batchmode -projectPath $SvnClientPath -executeMethod Packager.BuildAndroidNoPlatform
$UNITY_PATH -projectPath $PROJECT_PATH -executeMethod AutoProjectBuild.BuildForAndroid 
