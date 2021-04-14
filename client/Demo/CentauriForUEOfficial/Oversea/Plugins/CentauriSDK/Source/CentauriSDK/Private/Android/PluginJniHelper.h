#if PLATFORM_ANDROID
#ifndef __PLUGIN_JNI_HELPER_H__
#define __PLUGIN_JNI_HELPER_H__
#if PLATFORM_ANDROID
#include <jni.h>
#endif
#include <string>
#include <map>

namespace CentauriPay {
#if PLATFORM_ANDROID
typedef struct PluginJniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} PluginJniMethodInfo;

class PluginJniHelper
{
public:
    static JavaVM* getJavaVM();
    static void setJavaVM(JavaVM *javaVM);
    static JNIEnv* getEnv();

    static bool getStaticMethodInfo(PluginJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode);
    static bool getMethodInfo(PluginJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode);
    static std::string jstring2string(jstring jstr);
    static std::map<std::string, std::string> JSONObject2Map(jobject json);

    static bool setClassLoaderFrom(jobject nativeActivityInstance);

    static jmethodID loadclassMethod_methodID;
    static jobject classloader;
    static jobject createJavaMapObject(std::map<std::string, std::string>* paramMap);
    static jobject createJavaObject(std::string clsName);
    static void setJObjectField(jobject obj,std::string fldName,std::string value);
    static std::string  readJObjectField(jobject obj,std::string fldName);
    static void setJObjectFieldBoolean(jobject obj,std::string fldName,bool value);
    static int readIntField(jobject obj, std::string fldName);
private:
    static JavaVM *_psJavaVM;
    static bool getMethodInfo_DefaultClassLoader(PluginJniMethodInfo &methodinfo,
                                                 const char *className,
                                                 const char *methodName,
                                                 const char *paramCode);
};
#endif
}

#endif // __PLUGIN_JNI_HELPER_H__
#endif
