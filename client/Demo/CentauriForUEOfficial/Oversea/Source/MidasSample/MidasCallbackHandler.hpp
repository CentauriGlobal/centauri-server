#if PLATFORM_ANDROID || PLATFORM_IOS
#ifndef MidasCallbackHandler_hpp
#define MidasCallbackHandler_hpp

#include <stdio.h>
#include <string>
#include "CTIPayService.h"

class CTIInitCallbackHandler : public CentauriPay::CTIInitCallback
{
public:
    CTIInitCallbackHandler();
    virtual ~CTIInitCallbackHandler();
    
    virtual void OnCentauriInitFinished(const std::string result);
};

class CTIReprovideCallbackHandler : public CentauriPay::CTIReprovideCallback
{
public:
    CTIReprovideCallbackHandler();
    virtual ~CTIReprovideCallbackHandler();
    
    virtual void OnCentauriReprovideFinished(const std::string result);
};

class CTIPayCallbackHandler : public CentauriPay::CTIPayCallback
{
public:
    CTIPayCallbackHandler();
    virtual ~CTIPayCallbackHandler();
    
    virtual void OnCentauriLoginExpired();
    virtual void OnCentauriPayFinished(const CentauriPay::CTIResponse& result);
};

class CTIGetInfoCallbackHandler : public CentauriPay::CTIGetInfoCallback
{
public:
    CTIGetInfoCallbackHandler();
    virtual ~CTIGetInfoCallbackHandler();
    
    virtual void OnCentauriGetInfoFinished(const std::string result);
};

class CTIGetLocalPriceCallbackHandler : public CentauriPay::CTIGetLocalPriceCallback
{
public:
    CTIGetLocalPriceCallbackHandler();
    virtual ~CTIGetLocalPriceCallbackHandler();
    
    virtual void OnCentauriGetLocalPrice(const std::string result);
};

class CTIGetIntroPriceCallbackHandler : public CentauriPay::CTIGetIntroPriceCallback
{
public:
    CTIGetIntroPriceCallbackHandler();
    virtual ~CTIGetIntroPriceCallbackHandler();
    
    virtual void OnCentauriGetIntroPrice(const std::string result);
};

#endif /* MidasCallbackHandler_hpp */
#endif
