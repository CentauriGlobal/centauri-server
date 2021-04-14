原理：
利用iOS重签名机制，先根据mobileprovision描述文件生成entitlements.plist的授权文件，将Midas需要的com.tencent.qpaytest（当前app的bundleId）和com.tencent.wsj.keystoregroup（固定值）加入keychain sharing的keychain-access-groups里面（若业务有其它值需要设置的，也可在entitlements.plist里进行设置），然后进行重签名生成新的包，以解决UE4无法设置keychain sharing的问题。


# 使用方法: sh ios_set_keychain ../IOS/MidasSample.ipa "iPhone Developer: Deren Su (6CLGCK4S32)"
# 参数1：ipa文件路径；参数2：证书ID
# 可通过 security find-identity -v -p codesigning 命令查看当前mac上有可用的付费证书ID



