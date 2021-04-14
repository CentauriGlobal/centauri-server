@echo off
//rem  本机unity安装应用程序的路径
set UNITY_PATH="C:\Program Files\Unity2018.4.15\Editor\Unity.exe"
//rem  unity项目工程的路径                
set UNITY_PROJECT_PATH=H:\GitCode\CentauriSDK\CentauriForUnityOfficial\Oversea
//rem  unity脚本中的类名.方法名       
set UNITY_METHOD_NAME=AutoProjectBuild.BuildForAndroid
//rem  unity的日志                                      
set UNITY_LOG_PATH=%cd%\log.txt                                             

//set SVN_BIN=C:\Program Files\TortoiseSVN\bin                                rem SVN安装目录
//set SVN_WORK="需要更新项目的目录"                                          rem 项目工程中需要更新的目录
//set SVN_APK="打包生成apk的目录"                                              rem 打包生成的目录
echo %UNITY_PATH%
echo %UNITY_PROJECT_PATH%
echo 开始编译

echo 更新SVN资源....
echo svn更新完毕!


echo 调用unity函数打包中。。。

%UNITY_PATH% -quit -batchmode -logFile %UNITY_LOG_PATH% -projectPath %UNITY_PROJECT_PATH% -executeMethod %UNITY_METHOD_NAME%
 
if not %errorlevel%==0 ( goto fail ) else ( goto success )

:success
echo 打包成功

//echo 提交包文件
//"%SVN_BIN%"\TortoiseProc.exe/command:commit /path:"%SVN_APK%" /notempfile /closeonend:0
//echo svn提交完毕!
//goto end


:fail
echo 打包失败
goto end

:end

echo 5.批处理结束

pause