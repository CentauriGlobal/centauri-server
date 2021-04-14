#include "CTIPayService.h"


#if PLATFORM_ANDROID || PLATFORM_IOS

#include <stdio.h>
#include <string>
#include <map>

#if PLATFORM_ANDROID || PLATFORM_WINDOWS
#include "Android/AndroidApplication.h"
#endif

using namespace std;

namespace CentauriPay
{
#if defined(__cplusplus)
extern "C"
{
#endif

    const std::string version = "1.0.5";

    std::string GET_INFO_TYPE_MP = "mp";
    std::string GET_INFO_TYPE_SHORT_OPENID = "get_short_openid";
	std::string CTI_WE = "we";
	std::string CTI_CHAT = "chat";
    std::string CTI_WCT = CTI_WE+ CTI_CHAT;
    std::string BANK = "bank";
    std::string CTI_Q = "q";
    std::string CTI_WALLET = "wallet";
    std::string CTI_QW = CTI_Q+ CTI_Q+ CTI_WALLET;
    std::string GOOGLEPLAY = "googleplay"; //
    std::string APPLEIAP = "APPLEIAP";     //

#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C"
{
#endif
    extern void centauriSdkSetProcess(string processName);
    extern void centauriSdkInit(const void *context, string idc, string env, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIInitCallback *callback);
    extern void centauriSdkInitWithIdcInfo(const void *context, string idc, string env, string idcInfo, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIInitCallback *callback);
    extern void centauriSdkPay(const void *context, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIPayCallback *callback);
    extern void centauriSdkLogEnable(bool enable);
    extern void centauriSdkSetPath(string path);
    extern void centauriSdkLaunchWeb(const void *context, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIPayCallback *callback);
    extern const char* centauriSdkGetVersion(const void *context);
    extern const bool centauriSdkPayEnable();
    extern void centauriSdkCouponsRollBack(const void *context, string s);
    extern void centauriSdkGetProductInfo(const void *context, string channel, const map<string, string> &productList, const CentauriPay::CTIGetLocalPriceCallback *callback);
    extern void centauriSdkGetIntroPrice(const void *context, string channel, const map<string, string> &productList, const CentauriPay::CTIGetIntroPriceCallback *callback);
    extern void centauriSdkGetInfo(const void *context, string type, const CentauriPay::CTIBaseRequest *req, const CentauriPay::CTIGetInfoCallback *callback);
    extern void centauriSdkReprovide(const void *context, const CTIReprovideCallback *callback);

#if defined(__cplusplus)
}
#endif

CTIBaseRequest::CTIBaseRequest()
{
    className = "CTIBaseRequest";
}
CTIBaseRequest::~CTIBaseRequest()
{
}

CTIGameRequest::CTIGameRequest()
{
    className = "CTIGameRequest";
}
CTIGameRequest::~CTIGameRequest()
{
}

CTIGoodsRequest::CTIGoodsRequest()
{
    className = "CTIGoodsRequest";
}
CTIGoodsRequest::~CTIGoodsRequest()
{
}

CTIMonthRequest::CTIMonthRequest() : autoPay(false)
{
    className = "CTIMonthRequest";
}
CTIMonthRequest::~CTIMonthRequest()
{
}

CTISubscribeRequest::CTISubscribeRequest()
{
    className = "CTISubscribeRequest";
}
CTISubscribeRequest::~CTISubscribeRequest()
{
}

CTIPayService *CTIPayService::s_pCTIPayService = NULL;

CTIPayService::CTIPayService()
{
}

CTIPayService::~CTIPayService()
{
}

CTIPayService *CTIPayService::getInstance()
{
    if (!s_pCTIPayService)
    {
        s_pCTIPayService = new CTIPayService();
    }

    return s_pCTIPayService;
}

void CTIPayService::SetProcess(std::string processName)
{
#if PLATFORM_IOS
    centauriSdkSetProcess(processName);
#endif
}

void CTIPayService::SetLogEnable(bool enable)
{
    centauriSdkLogEnable(enable);
}

//void CTIPayService::Initialize(const void* context,std::string idc, std::string env, const CTIBaseRequest * req, const CTIInitCallback * callback)
//{
//    centauriSdkInit(context,idc, env, req, callback);
//}

void CTIPayService::Initialize(const void *context, std::string idc, std::string env, std::string idcInfo, const CTIBaseRequest *req, const CTIInitCallback *callback)
{
    centauriSdkInitWithIdcInfo(context, idc, env, idcInfo, req, callback);
}

void CTIPayService::Pay(const void *context, const CTIBaseRequest *req, const CTIPayCallback *callback)
{
    centauriSdkPay(context, req, callback);
}

void CTIPayService::GetLocalPrice(const void *context, std::string channel, map<string, string> &productList, const CTIGetLocalPriceCallback *callback)
{
    centauriSdkGetProductInfo(context, channel, productList, callback);
}
    
void CTIPayService::GetIntroPrice(const void *context, std::string channel, map<string, string> &productList, const CTIGetIntroPriceCallback *callback)
{
#if PLATFORM_IOS
    centauriSdkGetIntroPrice(context, channel, productList, callback);
#endif
}

void CTIPayService::GetInfo(const void *context, std::string type, const CTIBaseRequest *req, const CTIGetInfoCallback *callback)
{
    centauriSdkGetInfo(context, type, req, callback);
}

void CTIPayService::Reprovide(const void *context, const CTIReprovideCallback *callback)
{
    centauriSdkReprovide(context, callback);
}

std::string CTIPayService::GetCentauriSDKVersion(const void *context)
{
    return centauriSdkGetVersion(context);
}

bool CTIPayService::IsPayEnable()
{
    return centauriSdkPayEnable();
}

void CTIPayService::LaunchWeb(const void *context, const CTIBaseRequest *req, const CTIPayCallback *callback)
{
    centauriSdkLaunchWeb(context, req, callback);
}

void CTIPayService::SetPath(std::string path)
{
    centauriSdkSetPath(path);
}

void CTIPayService::CouponsRollBack(std::string s)
{
#if PLATFORM_ANDROID
    centauriSdkCouponsRollBack(FAndroidApplication::GetGameActivityThis(), s);
#endif
}

} // namespace CentauriPay
#endif
