
#include "MidasBlueprintFunctionLibrary.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

#if PLATFORM_ANDROID ||  PLATFORM_IOS
#include "CTIPayService.h"
#include "MidasCallbackHandler.hpp"
using namespace CentauriPay;
#if PLATFORM_IOS
#include "iOSWrapper.h"
#endif
#endif

#include <string>
#include <map>

using namespace std;

static string gCurContry = "US";         //The Counry of your project
static string gCurCurrency = "USD";      //The Currency of your project
static string gPayChannel = "os_offical";   //Fixed value "os_offical"

//env:test: in sandbox environment; release: in payment release environment;Please use release environment only after fully tested in test environment.
static string gEnv = "test";
// static string gEnv = "dev";
static string gIdc = "local"; //local：香港      singapore：新加坡      guigu：美国
//When idcInfo is passed in, the payment service will use idcInfo's domain name and ip to request connection
static string gIdcInfo = "";

#if PLATFORM_ANDROID
static string gGameProductID = "midas_product_1";//The game productID had configed on Midas and googleplay platform.
static string gGoodsProductID = "midas_product_2";//The goods productID you had configed on Midas and googleplay platform.
static string gSubsProductID = "midas_product_3";//The subscribe productID you had configed on Midas and googleplay platform.
static string gServiceCode = "gp_month";//Monthly card business code
static string gServiceName = "gp_month";//Monthly card business name
static string gOfferID = "1450005285";//The offerID you had configed on Midas platform.
static string gOpenID = "mdiascocos";//User unique account ID
static string gPf = "IEG_iTOP-2001-android-2011-FB-1130-67137586594922-AppCode";
//Game server region ID, the default value is "1" for games regardless of region. If the application selects a supported role, then the role is connected to the partition ID number with "_", and the role needs to be urlencoded
static string gZoneID = "1";//The zoneId you had configed on Midas platform.idefault value is "1"
#elif PLATFORM_IOS
static string gGameProductID = "com.tencent.pay.test1";//The game productID you had configed on Midas and iap platform.
static string gGoodsProductID = "com.tencent.pay.test1";//The goods productID you had configed on Midas and iap platform.
static string gSubsProductID = "com.tencent.pay.test1";//The subscribe productID you had configed on Midas and iap platform.

static string gOfferID = "1450000495";//The offerID you had configed on Midas platform.
static string gOpenID = "midas_test500";
static string gPf = "huyu_m-2001-iap";//The pf you had configed on Midas platform.

//Game server region ID, the default value is "1" for games regardless of region. If the application selects a supported role, then the role is connected to the partition ID number with "_", and the role needs to be urlencoded
static string gZoneID = "1";
#endif

string getIdcString()
{
    return "";
}


//Midas Sample Login
FString UMidasBlueprintFunctionLibrary::midasSampleLogin(){
    
    //补发货
#if PLATFORM_ANDROID
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIReprovideCallbackHandler* handler = new CTIReprovideCallbackHandler();
    CTIPayService::getInstance()->Reprovide(activity, handler);
#elif PLATFORM_IOS
    const void * ctrl = getRootViewController();
    CTIReprovideCallbackHandler* handler = new CTIReprovideCallbackHandler();
    CTIPayService::getInstance()->Reprovide(ctrl, handler);
#endif
    return FString("call reprovide Pay");
}

//Midas Sample buy month card
FString UMidasBlueprintFunctionLibrary::midasSampleBuyMonth() {
#if PLATFORM_ANDROID

#elif PLATFORM_IOS
        CTISubscribeRequest req;
        req.openId = gOpenID;
        req.offerId = "1450011344";
        req.zoneId = gZoneID;
        req.pf = gPf;
        req.productId = "com.tencent.qpaytest.nonautorenew.monthcard_31";//自动续费月卡
    //The value of payItem: props, pass "item ID unit price (unit: angle) quantity"; monthly pass, pass "days"; game currency, pass "quantity"
        req.payItem = "31";
      //Game transparent field
        req.appExtends = "midas_time_zone=8&midas_user_age=16&midas_user_zone=JP";
    //Goods zone id number
        req.goodsZoneId = "1";
        req.payChannel = gPayChannel;
        req.autoPay = false;//Whether to renew automatically
        const void * ctrl = getRootViewController();
        CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
        CTIPayService::getInstance()->Pay(ctrl,&req, handler);
#endif
    
    return FString("call BuyMonth Card");
}

//Midas Sample init
FString UMidasBlueprintFunctionLibrary::midasSampleInit() {
#if PLATFORM_ANDROID
    CTIBaseRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    //Goods zone ID number
    req.goodsZoneId = "1";
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIPayService::getInstance()->SetLogEnable(true);
    CTIInitCallbackHandler* handler = new CTIInitCallbackHandler();
    CTIPayService::getInstance()->Initialize(activity, gIdc, gEnv, gIdcInfo,&req, handler);
    return FString("call Android Init ....");
#elif PLATFORM_IOS
    //For IOS platform you must be setProcess before initialize
    CTIPayService::getInstance()->SetProcess("sea");
    
    CTIBaseRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.goodsZoneId = "1";
    const void * ctrl = getRootViewController();
    
    CTIPayService::getInstance()->SetLogEnable(true);
    CTIInitCallbackHandler* handler = new CTIInitCallbackHandler();
    CTIPayService::getInstance()->Initialize(ctrl, gIdc, gEnv, "", &req, handler);
    
    //    getInfo();
    //    getProductInfo();
#endif
    return FString("call Init");
}

//Midas Sample buy Game currency
FString UMidasBlueprintFunctionLibrary::midasSampleBuyGame(){
    
#if PLATFORM_ANDROID
    CTIGameRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.country = gCurContry;
    req.currencyType = gCurCurrency;
    req.payChannel = gPayChannel;
    req.productId = gGameProductID;
    req.channelExtras = "payItem=100";
    req.appExtends = "a=b&c=d";
    req.goodsZoneId = "122";
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(activity,&req, handler);
    return FString("call Android game pay");
#elif PLATFORM_IOS
    UMidasBlueprintFunctionLibrary::startIOSGamePay();
    //    UMidasBlueprintFunctionLibrary::startIOSGoodsPay();
    //    UMidasBlueprintFunctionLibrary::startIOSSubscribePay();
#endif
    
    return FString("call Pay");
}

//Midas Sample buy Game Goods
FString UMidasBlueprintFunctionLibrary::midasSampleBuyGoods(){
    
#if PLATFORM_ANDROID
    CTIGoodsRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.country = gCurContry;
    req.currencyType = gCurCurrency;
    req.payChannel = gPayChannel;
    req.productId = gGoodsProductID;
    //Third-party payment channels: general parameters transparently transmitted to third-party payment channels, generally roleId, roleName, etc., different channel parameters key/value
    req.channelExtras = "payItem=com.tencent.pay.noautorenew*1*1";
    //Goods zone id number
    req.goodsZoneId = "122";
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(activity,&req, handler);
    return FString("call Android goods Pay");
#elif PLATFORM_IOS
    //        UMidasBlueprintFunctionLibrary::startIOSGamePay();
    UMidasBlueprintFunctionLibrary::startIOSGoodsPay();
    //    UMidasBlueprintFunctionLibrary::startIOSSubscribePay();
#endif
    
    return FString("call goods Pay");
}

//Midas Sample buy Game Subscribe
FString UMidasBlueprintFunctionLibrary::midasSampleBuySubscribe(){
    
#if PLATFORM_ANDROID
    CTISubscribeRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.country = gCurContry;
    req.currencyType = gCurCurrency;
    req.payChannel = gPayChannel;
    req.productId = gSubsProductID;
    req.channelExtras = "payItem=100";
    req.appExtends = "a=b&c=d";
    //goods zone id number
    req.goodsZoneId = "122";
    req.serviceCode = gServiceCode;
    req.serviceName = gServiceName;
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(activity,&req, handler);
    return FString("call android subs Pay");
#elif PLATFORM_IOS
    //    UMidasBlueprintFunctionLibrary::startIOSGamePay();
    //    UMidasBlueprintFunctionLibrary::startIOSGoodsPay();
    UMidasBlueprintFunctionLibrary::startIOSSubscribePay();
#endif
    
    return FString("call subs Pay");
}


/*
 * Midas Sample get product Information
 */
FString UMidasBlueprintFunctionLibrary::midasSampleGetLocalPrice(){
#if PLATFORM_ANDROID
    jobject activity = FAndroidApplication::GetGameActivityThis();
    std::string channel = "gwallet";
    std::string productId = gGameProductID;
    CTIGetLocalPriceCallbackHandler* handler = new CTIGetLocalPriceCallbackHandler();
    
    map<string, string> products;
    products.insert(pair<string, string>(productId, "inapp"));
    
    CTIPayService::getInstance()->GetLocalPrice(activity,channel, products, handler);
#elif PLATFORM_IOS
    
    //拉物品信息
    const void * ctrl = getRootViewController();
    std::string channel = "iap";
    std::string productId = gGameProductID;
    CTIGetLocalPriceCallbackHandler* handler = new CTIGetLocalPriceCallbackHandler();

    map<string, string> products;
    products.insert(pair<string, string>(productId, ""));

    CTIPayService::getInstance()->GetLocalPrice(ctrl,channel, products, handler);
    
    
    
    
    //拉取促销价格
//    const void * ctrl = getRootViewController();
//    std::string channel = "iap";
//    std::string productId = gGameProductID;
//    CTIGetIntroPriceCallbackHandler* handler = new CTIGetIntroPriceCallbackHandler();
//    map<string, string> products;
//    products.insert(pair<string, string>(productId, ""));
//    CTIPayService::getInstance()->GetIntroPrice(ctrl,channel, products, handler);
#endif
    return FString("call getLocalPrice");
}

/*
* Midas Sample get market activities Information
*/
FString UMidasBlueprintFunctionLibrary::midasSampleGetInfo(){
#if PLATFORM_ANDROID
    CTIGameRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.country = gCurContry;
    req.currencyType = gCurCurrency;
    req.payChannel = gPayChannel;
    req.productId = gGameProductID;
    //Game transparent field
    req.appExtends = "a=b&c=d";
    //goods zone id number
    req.goodsZoneId = "122";
    jobject activity = FAndroidApplication::GetGameActivityThis();
    CTIGetInfoCallbackHandler* handle = new CTIGetInfoCallbackHandler();
    std::string type = "mp";//Fixed value "mp"
    CTIPayService::getInstance()->GetInfo(activity,type, &req,  handle);
    return FString("call Android getInfo");
#elif PLATFORM_IOS
    CTIGameRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.payChannel = gPayChannel;
    const void * ctrl = getRootViewController();
    CTIGetInfoCallbackHandler* handle = new CTIGetInfoCallbackHandler();
    std::string type = "mp";//Fixed value "mp"
    CTIPayService::getInstance()->GetInfo(ctrl,type, &req,  handle);
#endif
    return FString("call getInfo");
}

#if PLATFORM_IOS
void UMidasBlueprintFunctionLibrary::startIOSGamePay()
{
    CTIGameRequest req;
    req.openId = gOpenID;
    req.offerId = gOfferID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.productId = gGameProductID;
    req.payItem = "100";
    //Game transparent field
    req.appExtends = "midas_time_zone=8&midas_user_age=16&midas_user_zone=JP";
    
    req.goodsZoneId = "1";
    req.payChannel = gPayChannel;
    const void * ctrl = getRootViewController();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(ctrl,&req, handler);
}
void UMidasBlueprintFunctionLibrary::startIOSGoodsPay()
{
    CTIGoodsRequest req;
    req.offerId = "1450005935";
    req.openId = gOpenID;
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.productId = "com.tencent.qpaytest.qqvip";
    //The value of payItem: props, pass "item ID unit price (unit: angle) quantity"; monthly pass, pass "days"; game currency, pass "quantity"
    req.payItem = "com.tencent.qpaytest.qqvip*120*1";
//    req.appExtends = "midas_time_zone=8&midas_user_age=16&midas_user_zone=JP";
    req.goodsZoneId = "1";
    req.payChannel = gPayChannel;
    const void * ctrl = getRootViewController();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(ctrl,&req, handler);
}
void UMidasBlueprintFunctionLibrary::startIOSSubscribePay()
{
    CTISubscribeRequest req;
    req.openId = gOpenID;
    req.offerId = "1450008464";
    req.zoneId = gZoneID;
    req.pf = gPf;
    req.productId = "com.tencent.qpaytest.otherautorenew1month";//自动续费月卡
   //The value of payItem: props, pass "item ID unit price (unit: angle) quantity"; monthly pass, pass "days"; game currency, pass "quantity"
    req.payItem = "31";
//    req.appExtends = "midas_time_zone=8&midas_user_age=16&midas_user_zone=JP";
    req.goodsZoneId = "1";
    req.payChannel = gPayChannel;
    req.autoPay = true;
    const void * ctrl = getRootViewController();
    CTIPayCallbackHandler* handler = new CTIPayCallbackHandler();
    CTIPayService::getInstance()->Pay(ctrl,&req, handler);
}

#endif

