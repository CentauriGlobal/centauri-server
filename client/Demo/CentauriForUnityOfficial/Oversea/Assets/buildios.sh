# #!/bin/bash

#参数判断
if [ $# != 2 ];then
echo "Params error!"
echo "Need two params: 1.path of project 2.name of ipa file"
exit 1
elif [ ! -d $1 ];then
echo "The first param is not a dictionary."+$1
exit 1

fi
#工程路径
project_path=$1

#IPA名称
ipa_name=$2

#build文件夹路径
build_path=${project_path}/build

#ipa路径
result_path=${project_path}/../result

mkdir -p $result_path

#清理#
xcodebuild  clean

#编译工程
cd $project_path
xcodebuild || exit

#打包 下面代码我是新加的#
xcrun -sdk iphoneos PackageApplication -v ${build_path}/Release-iphoneos/*.app -o ${result_path}/${ipa_name}.ipa

#删除
#rm -rf $build_path


