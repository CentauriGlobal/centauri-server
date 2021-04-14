#if PLATFORM_ANDROID
#include "../../Public/CTIPayService.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "Android/AndroidApplication.h"
#include <string>
#include <vector>
#include <map>


#if 1

#if PLATFORM_ANDROID

#include <android/log.h>
#define  LOG_TAG    "PluginJniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#elif PLATFORM_IOS

#define LOGD(fmt, args...) NSLog(@"CTI Log | %s[%d] " fmt, __FUNCTION__, __LINE__, ## args);

#else
#define  LOGD(...)

#endif

#endif



using namespace std;
using namespace CentauriPay;

extern "C" {


static CentauriPay::CTIInitCallback* pCTIInitCallback = NULL;
static CentauriPay::CTIPayCallback*  pCTIPayCallback = NULL;
static CentauriPay::CTIGetLocalPriceCallback* pCTIGetProductCallback = NULL;
static CentauriPay::CTIGetInfoCallback* pCTIGetInfoCallback = NULL;
static CentauriPay::CTIReprovideCallback* pCTIReprovideCallback = NULL;

void outputLog(const char *logTag, const char *pFormat, ...);

static std::string mAppExtends;


#define MAX_LOG_LEN            256
void outputLog(const char *logTag, const char *pFormat, ...) {
    char buf[MAX_LOG_LEN + 1];

    va_list args;
    va_start(args, pFormat);
    vsnprintf(buf, MAX_LOG_LEN, pFormat, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_DEBUG, logTag, "%s", buf);
}

JNI_METHOD  void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnInitializeResult(JNIEnv *env, jobject thiz,
                                                                  jstring msg) {
    std::string strMsg = PluginJniHelper::jstring2string(msg);
    if(pCTIInitCallback != NULL)
    {
        pCTIInitCallback->OnCentauriInitFinished(strMsg);
    }
}

JNI_METHOD  void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnCentauriLoginExpired(JNIEnv *env, jobject thiz) {
    if(pCTIPayCallback != NULL)
    {
        pCTIPayCallback->OnCentauriLoginExpired();
    }
}

JNI_METHOD  void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnCentauriPayFinished(JNIEnv *env, jobject thiz,
                                                                  jobject obj) {
    if(pCTIPayCallback != NULL)
    {
        if(obj != NULL)
        {
            CTIResponse response ;
            response.resultCode = PluginJniHelper::readIntField(obj,"resultCode");
            response.resultMsg = PluginJniHelper::readJObjectField(obj,"resultMsg");
            response.appExtends= mAppExtends;
            response.payChannel = PluginJniHelper::readJObjectField(obj,"payChannel");
            response.resultInerCode = PluginJniHelper::readJObjectField(obj,"resultInerCode");

            pCTIPayCallback->OnCentauriPayFinished(response);
        }

    }

}

JNI_METHOD  void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnCentauriGetProdcut(JNIEnv *env, jobject thiz,
                                                                  jstring msg) {
    std::string strMsg = PluginJniHelper::jstring2string(msg);
    if(pCTIGetProductCallback != NULL)
    {
        pCTIGetProductCallback->OnCentauriGetLocalPrice(strMsg);
    }
}


JNI_METHOD  void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnCentauriGetInfoFinished(JNIEnv *env, jobject thiz,
                                                                 jstring msg) {
    std::string strMsg = PluginJniHelper::jstring2string(msg);
    if(pCTIGetInfoCallback != NULL)
    {
        pCTIGetInfoCallback->OnCentauriGetInfoFinished(strMsg);
    }
}
    
JNI_METHOD void JNICALL
Java_com_centauri_oversea_api_CocosPayHelper_OnReprovideResult(JNIEnv *env, jobject thiz,
                                                                     jstring msg) {
    std::string strMsg = PluginJniHelper::jstring2string(msg);
    if(pCTIReprovideCallback != NULL)
    {
        pCTIReprovideCallback->OnCentauriReprovideFinished(strMsg);
    }
}


jobject createInitRequest(const CTIBaseRequest *req) {

    jobject obj = PluginJniHelper::createJavaObject(
            "com/centauri/oversea/api/CocosCTIInitRequest");
    //CocosCTIBaseRequest
    PluginJniHelper::setJObjectField(obj, "offerId", req->offerId);
    PluginJniHelper::setJObjectField(obj, "openId", req->openId);
    PluginJniHelper::setJObjectField(obj, "zoneId", req->zoneId);
    PluginJniHelper::setJObjectField(obj, "goodsZoneId", req->goodsZoneId);
    PluginJniHelper::setJObjectField(obj, "pf", req->pf);
    PluginJniHelper::setJObjectField(obj, "channelExtras", req->channelExtras);
    PluginJniHelper::setJObjectField(obj, "openKey", req->openKey);
    PluginJniHelper::setJObjectField(obj, "sessionId", req->sessionId);
    PluginJniHelper::setJObjectField(obj, "sessionType", req->sessionType);
    PluginJniHelper::setJObjectField(obj, "pfKey", req->pfKey);
    PluginJniHelper::setJObjectField(obj, "appExtends", req->appExtends);


    return obj;
}

jobject createBaseRequest(const CTIBaseRequest *req) {

    jobject obj = PluginJniHelper::createJavaObject(
            "com/centauri/oversea/api/CocosCTIInitRequest");
    //CocosCTIBaseRequest
    PluginJniHelper::setJObjectField(obj, "offerId", req->offerId);
    PluginJniHelper::setJObjectField(obj, "openId", req->openId);
    PluginJniHelper::setJObjectField(obj, "zoneId", req->zoneId);
    PluginJniHelper::setJObjectField(obj, "goodsZoneId", req->goodsZoneId);
    PluginJniHelper::setJObjectField(obj, "pf", req->pf);
    PluginJniHelper::setJObjectField(obj, "channelExtras", req->channelExtras);
    PluginJniHelper::setJObjectField(obj, "openKey", req->openKey);
    PluginJniHelper::setJObjectField(obj, "sessionId", req->sessionId);
    PluginJniHelper::setJObjectField(obj, "sessionType", req->sessionType);
    PluginJniHelper::setJObjectField(obj, "pfKey", req->pfKey);
    PluginJniHelper::setJObjectField(obj, "appExtends", req->appExtends);

    return obj;
}

jobject createPayRequest(const CTIBaseRequest *req) {


    string clsName = "com/centauri/oversea/api/";
    clsName.append("Cocos");
    clsName.append(req->className);
    jobject obj = PluginJniHelper::createJavaObject(
            clsName);
    //CocosCTIBaseRequest
    PluginJniHelper::setJObjectField(obj, "offerId", req->offerId);
    PluginJniHelper::setJObjectField(obj, "openId", req->openId);
    PluginJniHelper::setJObjectField(obj, "zoneId", req->zoneId);
    PluginJniHelper::setJObjectField(obj, "goodsZoneId", req->goodsZoneId);
    PluginJniHelper::setJObjectField(obj, "pf", req->pf);
    PluginJniHelper::setJObjectField(obj, "channelExtras", req->channelExtras);
    PluginJniHelper::setJObjectField(obj, "openKey", req->openKey);
    PluginJniHelper::setJObjectField(obj, "sessionId", req->sessionId);
    PluginJniHelper::setJObjectField(obj, "sessionType", req->sessionType);
    PluginJniHelper::setJObjectField(obj, "pfKey", req->pfKey);
    PluginJniHelper::setJObjectField(obj, "appExtends", req->appExtends);



    //CocosCTIBasePayRequest
    PluginJniHelper::setJObjectField(obj,"extras",req->extras); // resid, drmInfo需要在extras中解析
    PluginJniHelper::setJObjectField(obj,"currencyType",req->currencyType);
    PluginJniHelper::setJObjectField(obj,"country",req->country);
    
    PluginJniHelper::setJObjectField(obj,"payChannel",req->payChannel);
    PluginJniHelper::setJObjectField(obj,"productId",req->productId);

    if(req->className == "CTISubscribeRequest"){
        const CTISubscribeRequest* subReq = static_cast<const CTISubscribeRequest *>(req);
        PluginJniHelper::setJObjectField(obj,"serviceCode",subReq->serviceCode);
        PluginJniHelper::setJObjectField(obj,"serviceName",subReq->serviceName);
        PluginJniHelper::setJObjectFieldBoolean(obj,"autoPay",subReq->autoPay);
    }

    return obj;
}

void centauriSdkLogEnable(bool enable)
{



    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t, "com/centauri/oversea/api/CocosPayHelper",
                                                     "SetLogEnable", "(Z)V")) {
        outputLog("CTIPayService",
                  "Can't find method SetLogEnable in com.centauri.oversea.api.Cocoshelper");
        return;
    }

    t.env->CallStaticVoidMethod(t.classID, t.methodID, enable);
    t.env->DeleteLocalRef(t.classID);
}

void centauriSdkInit(const void* context,std::string idc, std::string env, const CentauriPay::CTIBaseRequest *req,
                  const CentauriPay::CTIInitCallback *callback) {
    
    pCTIInitCallback = const_cast<CentauriPay::CTIInitCallback *>(callback);

    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                     "com/centauri/oversea/api/CocosPayHelper",
                                                     "Initialize",
                                                     "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Lcom/centauri/oversea/api/CocosCTIInitRequest;)V")) {
        outputLog("CTIPayService",
                  "Can't find method Initialize in class com.centauri.oversea.api.CocosPayHelper");
        pCTIInitCallback->OnCentauriInitFinished("-1");
        return;
    }


    jobject initObj = createInitRequest(req);
    jstring jIdc = t.env->NewStringUTF(idc.c_str());
    jstring jEnv = t.env->NewStringUTF(env.c_str());
    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context,
                                jIdc,
                                jEnv,
                                initObj
    );
    t.env->DeleteLocalRef(jIdc);
    t.env->DeleteLocalRef(jEnv);
    t.env->DeleteLocalRef(t.classID);
	t.env->DeleteLocalRef(initObj);
}
void centauriSdkInitWithIdcInfo(const void* context,std::string idc, std::string env, std::string idcInfo,
                             const CentauriPay::CTIBaseRequest *req,
                             const CentauriPay::CTIInitCallback *callback)
{
    pCTIInitCallback = const_cast<CentauriPay::CTIInitCallback *>(callback);

    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                     "com/centauri/oversea/api/CocosPayHelper",
                                                     "Initialize",
                                                     "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/centauri/oversea/api/CocosCTIInitRequest;)V")) {
        outputLog("CTIPayService",
                  "Can't find method Initialize in class com.centauri.oversea.api.CocosPayHelper");
        pCTIInitCallback->OnCentauriInitFinished("-1");
        return;
    }


    jobject initObj = createInitRequest(req);
    jstring jIdc = t.env->NewStringUTF(idc.c_str());
    jstring jIdcCfg = t.env->NewStringUTF(idcInfo.c_str());
    jstring jEnv = t.env->NewStringUTF(env.c_str());
    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context,
                                jIdc,
                                jEnv,
                                jIdcCfg,
                                initObj
    );
    t.env->DeleteLocalRef(jIdc);
    t.env->DeleteLocalRef(jEnv);
    t.env->DeleteLocalRef(jIdcCfg);
	t.env->DeleteLocalRef(initObj);
    t.env->DeleteLocalRef(t.classID);
}
void centauriSdkPay(const void* context,const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIPayCallback *callback)
{
    pCTIPayCallback = const_cast<CentauriPay::CTIPayCallback *>(callback);

    //保存，方便回传
    mAppExtends = req->appExtends;

    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                     "com/centauri/oversea/api/CocosPayHelper",
                                                     "Pay",
                                                     "(Landroid/app/Activity;Ljava/lang/String;Lcom/centauri/oversea/api/CocosCTIBasePayRequest;)V")) {
        outputLog("CTIPayService",
                  "Can't find method Initialize in class com.centauri.oversea.api.CocosPayHelper");
        //callback->OnCentauriInitFinished("-1");
        return;
    }


    jobject payObj = createPayRequest(req);
    jstring jBizType = t.env->NewStringUTF(req->className.c_str());
    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context,
                                jBizType,
                                payObj
    );
    t.env->DeleteLocalRef(payObj);
    t.env->DeleteLocalRef(jBizType);
    t.env->DeleteLocalRef(t.classID);
}

void centauriSdkGetProductInfo(const void* context,std::string channel, const std::map<string, string> &productList,
                            const CentauriPay::CTIGetLocalPriceCallback *callback)
{
    pCTIGetProductCallback = const_cast<CentauriPay::CTIGetLocalPriceCallback *>(callback);

    if(productList.size() == 0){
        pCTIGetProductCallback->OnCentauriGetLocalPrice(NULL);
        return;
    }

    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                     "com/centauri/oversea/api/CocosPayHelper",
                                                     "GetProductInfo",
                                                     "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;)V")) {
        outputLog("CTIPayService",
                  "Can't find method GetProductInfo in class com.centauri.oversea.api.CocosPayHelper");
        //callback->OnCentauriInitFinished("-1");
        return;
    }


	
	std::string list;
	list.append("[");
	map<string, string>::const_iterator it = productList.begin();
	// map<string, string>::const_iterator iter = productList.begin();
	while (it != productList.end()) {
		outputLog("CTIPayService", "map iter");
		list.append("\"");
		list.append(it->first+":"+ it->second);
		list.append("\"");
		it++;
		if(it !=productList.end())
		{
		  list.append(",");
		}
		
		
	}
	list.append("]");

    // std::string list;
    // list.append("[");
    // for(int i=0; i<productList.size(); i++){
    //     list.append("\"");
    //     list.append(productList[i]);
    //     list.append("\"");
    //     if(i < productList.size() -1){
    //         list.append(",");
    //     }
    // }
    // list.append("]");

    outputLog("CTIPayService", string("product list:").append(list).c_str());

    jstring jlist = t.env->NewStringUTF(list.c_str());
    jstring jchannel = t.env->NewStringUTF(channel.c_str());

    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context,
                                jchannel,
                                jlist
    );
    t.env->DeleteLocalRef(jlist);
    t.env->DeleteLocalRef(jchannel);
    t.env->DeleteLocalRef(t.classID);
}
void centauriSdkGetInfo(const void* context,std::string type, const CentauriPay::CTIBaseRequest *req,
                     const CentauriPay::CTIGetInfoCallback *callback)
{
    CentauriPay::PluginJniMethodInfo t;
    pCTIGetInfoCallback = const_cast<CentauriPay::CTIGetInfoCallback *>(callback);
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t, "com/centauri/oversea/api/CocosPayHelper",
                                                     "GetInfo",
                                                     "(Landroid/app/Activity;Ljava/lang/String;Ljava/lang/String;Lcom/centauri/oversea/api/CocosCTIBaseRequest;)V")) {
        outputLog("CTIPayService",
                  "Can't find method GetInfo in com.centauri.oversea.api.Cocoshelper");
        return;
    }

    jobject jobj = createPayRequest(req);
    jstring jBizType = t.env->NewStringUTF(req->className.c_str());
    jstring jReqType = t.env->NewStringUTF(type.c_str());

    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context,
                                jReqType,
                                jBizType,
                                jobj);
    t.env->DeleteLocalRef(jobj);
    t.env->DeleteLocalRef(t.classID);
    t.env->DeleteLocalRef(jBizType);
    t.env->DeleteLocalRef(jReqType);
}
void centauriSdkReprovide(const void* context, const CTIReprovideCallback* callback)
{
    LOGD("CentauriPayAdapter midsaSdkReprovide called");
    pCTIReprovideCallback = const_cast<CentauriPay::CTIReprovideCallback *>(callback);
    
    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                     "com/centauri/oversea/api/CocosPayHelper",
                                                     "Reprovide",
                                                     "(Landroid/app/Activity;)V")) {
        outputLog("CTIPayService",
                  "Can't find method Reprovide in class com.centauri.oversea.api.CocosPayHelper");
        pCTIReprovideCallback->OnCentauriReprovideFinished("-1");
        LOGD("CentauriPayAdapter midsaSdkReprovide failed: not found");
        return;
    }
    
    t.env->CallStaticVoidMethod(t.classID, t.methodID, (jobject)context);
   LOGD("CentauriPayAdapter midsaSdkReprovide call complete");
    t.env->DeleteLocalRef(t.classID);
}

const char* centauriSdkGetVersion(const void* context)
{
    LOGD("CentauriPayAdapter midsaSdkReprovide called");
    CentauriPay::PluginJniMethodInfo t;
    if (!CentauriPay::PluginJniHelper::getStaticMethodInfo(t,
                                                        "com/centauri/oversea/api/CocosPayHelper",
                                                        "getSDKVersion",
                                                        "(;)V")) {
        outputLog("CTIPayService",
                  "Can't find method getSDKVersion in class com.centauri.oversea.api.CocosPayHelper");
        
        LOGD("CentauriPayAdapter getSDKVersion failed: not found");
        return "unknown";
    }
    jstring sdkVersion = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, (jobject)context);
    
    t.env->DeleteLocalRef(t.classID);
    return t.env->GetStringUTFChars(sdkVersion, 0);
    
}
    
 void centauriSdkLaunchWeb(const void *context, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIPayCallback *callback){
    LOGD("centauriSdkLaunchWeb not supported");
    return;
}

 void centauriSdkSetPath(std::string path){
    LOGD("centauriSdkSetPath not supported");
    return;
}

 void centauriSdkCouponsRollBack(const void *context, string s){
    LOGD("centauriSdkCouponsRollBack not supported");
    return;
}
    
const bool centauriSdkPayEnable(){
    return true;
}

}
#endif
