#!/bin/sh

UE4_VERSION=UE_4.23

#游戏程序路径#
PROJECT_PATH=$(cd `dirname $0`; pwd)
PROJECT_NAME=MidasSample.uproject

rm -rf $PROJECT_PATH/IOS
rm -rf $PROJECT_PATH/Intermediate
rm -rf $PROJECT_PATH/Saved
rm -rf $PROJECT_PATH/Binaries
rm -rf $PROJECT_PATH/Build
rm -rf $PROJECT_PATH/MidasSample.xcworkspace

# 1.生成xcode工程
msbuild "/Users/Shared/Epic Games/$UE4_VERSION/Engine/Source/Programs/UnrealBuildTool/UnrealBuildTool.csproj" /property:Configuration="Development" /verbosity:quiet /nologo | grep -i error

mono "/Users/Shared/Epic Games/$UE4_VERSION/Engine/Binaries/DotNET/UnrealBuildTool.exe" -projectfiles $PROJECT_PATH/$PROJECT_NAME


# 2. 编译xcode工程
xcode_workspace=$PROJECT_PATH/MidasSample.xcworkspace
xcodebuild -workspace $xcode_workspace -scheme MidasSample

# 3. 打包ipa
msbuild "/Users/Shared/Epic Games/$UE4_VERSION/Engine/Source/Programs/UnrealBuildTool/UnrealBuildTool.csproj" /property:Configuration="Development" /verbosity:quiet /nologo

mono "/Users/Shared/Epic Games/$UE4_VERSION/Engine/Binaries/DotNET/AutomationTool.exe" -ScriptsForProject=$PROJECT_PATH/$PROJECT_NAME BuildCookRun -nocompile -nocompileeditor -installed -nop4 -project=$PROJECT_PATH/$PROJECT_NAME -cook -stage -archive -archivedirectory=$PROJECT_PATH -package -clientconfig=Development -ue4exe=UE4Editor -clean -pak -prereqs -nodebuginfo -targetplatform=IOS -build -utf8output





