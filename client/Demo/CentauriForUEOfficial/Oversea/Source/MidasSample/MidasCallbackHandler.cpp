#if PLATFORM_ANDROID || PLATFORM_IOS
#include "MidasCallbackHandler.hpp"
#include "CTIPayService.h"
#include "CTIPayCallbackEntities.h"
#include "Json.h"
#include <string>

#if 1

    #if PLATFORM_ANDROID

    #include <android/log.h>
    #define  LOG_TAG    "UeSampleCallback"
    #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

    #elif PLATFORM_IOS

    #define LOGD(fmt, args...) NSLog(@"APMidas Log | %s[%d] " fmt, __FUNCTION__, __LINE__, ## args);

    #else
    #define  LOGD(...)

    #endif

#else
    #define  LOGD(...)

#endif


using namespace CentauriPay;
using namespace std;



static FString TAG = "CentauriPay";
CTIInitCallbackHandler::CTIInitCallbackHandler()
{
    
}

CTIInitCallbackHandler::~CTIInitCallbackHandler()
{
    
}


CTIInitProduct::CTIInitProduct()
{

}

CTIGetProductResult::CTIGetProductResult()
{

}


CTIGetProduct::CTIGetProduct()
{

}


CTIInitResult::CTIInitResult()
{

}

CTIReprovideCallbackHandler::CTIReprovideCallbackHandler()
{

}
CTIReprovideCallbackHandler::~CTIReprovideCallbackHandler()
{

}
void CTIReprovideCallbackHandler::OnCentauriReprovideFinished(const std::string result)
{
    LOGD("onMidasReprovideFinished %s", result.c_str());
    FString jsonRaw = FString(result.c_str());
    TSharedPtr<FJsonObject> JsonParsed;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonRaw);

    UE_LOG(LogTemp, Display, TEXT("CTIReprovideCallbackHandler:%s"), *FString(result.c_str()));
    auto  resp = *new CTIInitResult();
    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
        if(JsonParsed->HasField("paychannelid")){
            resp.paychannelid = std::string(TCHAR_TO_UTF8(*(JsonParsed->GetStringField("paychannelid"))));
            vector<CTIInitProduct> products = *new vector<CTIInitProduct>;
            TArray<TSharedPtr<FJsonValue>> ProductsArray = JsonParsed->GetArrayField("products");
            for(TSharedPtr<FJsonValue> value : ProductsArray) {
                auto product = *new CTIInitProduct;
                TSharedPtr<FJsonObject> object = value.Get()->AsObject();
                product.productid = std::string(TCHAR_TO_UTF8(*(object->GetStringField("productid"))));
                product.billno = std::string(TCHAR_TO_UTF8(*(object->GetStringField("billno"))));
                product.originalJson = std::string(TCHAR_TO_UTF8(*(object->GetStringField("originalJson"))));
                products.push_back(product);
            }
            for(int i =0;i < products.size(); i++) {
                UE_LOG(LogTemp, Display,TEXT("onMidasReprovideFinished:---------------- start:%d"),i);
                UE_LOG(LogTemp, Display,TEXT("onMidasReprovideFinished productid:%s"), *FString(products[i].productid.c_str()));
                UE_LOG(LogTemp, Display,TEXT("onMidasReprovideFinished billno:%s"), *FString(products[i].billno.c_str()));
                UE_LOG(LogTemp, Display,TEXT("onMidasReprovideFinished originalJson:%s"), *FString(products[i].originalJson.c_str()));
                UE_LOG(LogTemp, Display,TEXT("OnCentauriReprovideFinished:----------------  end:%d"),i);
            }
        }
    }
}




void CTIInitCallbackHandler::OnCentauriInitFinished(const std::string result)
{
//    Document document;
//    document.Parse<0>(result.c_str());
    LOGD("onMidasInitFinished %s", result.c_str());
    FString jsonRaw = FString(result.c_str());
    TSharedPtr<FJsonObject> JsonParsed;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonRaw);
    
    UE_LOG(LogTemp, Display, TEXT("CTIInitCallbackHandler:%s"), *FString(result.c_str()));
    auto  resp = *new CTIInitResult();
    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
        if(JsonParsed->HasField("paychannelid")){
            resp.paychannelid = std::string(TCHAR_TO_UTF8(*(JsonParsed->GetStringField("paychannelid"))));
            vector<CTIInitProduct> products = *new vector<CTIInitProduct>;
            TArray<TSharedPtr<FJsonValue>> ProductsArray = JsonParsed->GetArrayField("products");
            for(TSharedPtr<FJsonValue> value : ProductsArray) {
                auto product = *new CTIInitProduct;
                TSharedPtr<FJsonObject> object = value.Get()->AsObject();
                product.productid = std::string(TCHAR_TO_UTF8(*(object->GetStringField("productid"))));
                product.billno = std::string(TCHAR_TO_UTF8(*(object->GetStringField("billno"))));
                product.originalJson = std::string(TCHAR_TO_UTF8(*(object->GetStringField("originalJson"))));
                products.push_back(product);
            }
            for(int i =0;i < products.size(); i++) {
                UE_LOG(LogTemp, Display,TEXT("OnCentauriInitFinished:---------------- start:%d"),i);
                UE_LOG(LogTemp, Display,TEXT("OnCentauriInitFinished productid:%s"), *FString(products[i].productid.c_str()));
                UE_LOG(LogTemp, Display,TEXT("OnCentauriInitFinished billno:%s"), *FString(products[i].billno.c_str()));
                UE_LOG(LogTemp, Display,TEXT("OnCentauriInitFinished originalJson:%s"), *FString(products[i].originalJson.c_str()));
                UE_LOG(LogTemp, Display,TEXT("OnCentauriInitFinished:----------------  end:%d"),i);
            }
        }
    }
    
}


CTIPayCallbackHandler::CTIPayCallbackHandler()
{
    
}

CTIPayCallbackHandler::~CTIPayCallbackHandler()
{
    
}

void CTIPayCallbackHandler::OnCentauriLoginExpired()
{
    LOGD("OnCentauriLoginExpired");
    UE_LOG(LogTemp, Display,TEXT("OnCentauriLoginExpired"));
}

void CTIPayCallbackHandler::OnCentauriPayFinished(const CentauriPay::CTIResponse& result)
{
    LOGD("onCentauriPayFinished code:%d, msg:%s", (int)result.resultCode, result.resultMsg.c_str());
    // LOGD("payState: %d", result.payState);
    // LOGD("provideState: %d", result.provideState);
    // LOGD("realSaveNum: %d", result.realSaveNum);
    LOGD("payChannel: %s", result.payChannel.c_str());
    LOGD("resultMsg: %s", result.resultMsg.c_str());
    LOGD("resultInerCode: %s", result.resultInerCode.c_str());
    // LOGD("extendInfo: %s", result.extendInfo.c_str());
    // LOGD("payReserve1: %s", result.payReserve1.c_str());
    // LOGD("payReserve2: %s", result.payReserve2.c_str());
    // LOGD("payReserve3: %s", result.payReserve3.c_str());

    LOGD("appExtends: %s", result.appExtends.c_str());
//    LOGD("billno: %s", result.billno.c_str());
    UE_LOG(LogTemp, Display,TEXT("OnCentauriPayFinished:\n"));
    UE_LOG(LogTemp, Display,TEXT("resultCode:%d\n"), (int)result.resultCode);
    UE_LOG(LogTemp, Display,TEXT("resultInerCode:%s\n"), result.resultInerCode.c_str());
//    UE_LOG(LogTemp, Display,TEXT("billno:%s\n"), result.billno.c_str());
    UE_LOG(LogTemp, Display,TEXT("payChannel:%s\n"), result.payChannel.c_str());
    UE_LOG(LogTemp, Display,TEXT("resultMsg:%s\n"), result.resultMsg.c_str());
    UE_LOG(LogTemp, Display,TEXT("appExtends:%s\n"), result.appExtends.c_str());
}

CTIGetInfoCallbackHandler::CTIGetInfoCallbackHandler()
{
    
}

CTIGetInfoCallbackHandler::~CTIGetInfoCallbackHandler()
{
    
}

void CTIGetInfoCallbackHandler::OnCentauriGetInfoFinished(const std::string result)
{
    FString jsonRaw = FString(result.c_str());
    TSharedPtr<FJsonObject> JsonParsed;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonRaw);

    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
        if(JsonParsed->HasField("ret")){
            int ret = JsonParsed->GetNumberField("ret");
            UE_LOG(LogTemp, Display, TEXT("CTIGetInfoCallbackHandler: ret:%d"), ret);
        }
        
        if(JsonParsed->HasField("rate")){
            int rate = JsonParsed->GetNumberField("rate");
            UE_LOG(LogTemp, Display, TEXT("CTIGetInfoCallbackHandler: rate:%d"), rate);
        }
    }
    
    LOGD("MidasGetInfocallback handler OnCentauriGetInfoFinished: msg: %s\n", result.c_str());
    
    UE_LOG(LogTemp, Display,TEXT("OnCentauriGetInfoFinished:%s"),*FString(result.c_str()));
}

CTIGetLocalPriceCallbackHandler::CTIGetLocalPriceCallbackHandler()
{
    
}

CTIGetLocalPriceCallbackHandler::~CTIGetLocalPriceCallbackHandler()
{
    
}

/*
 *
 * {"ret":"0",
 * "msg":"success",
 * "productInfo":
 * [
 *   {"productId":"midas_product_1","price":"US$1.00","currency":"USD","microprice":1000000},
 *   {"productId":"midas_product_3","price":"US$0.99","currency":"USD","microprice":990000}
 *  ]
 * }
 * */
void CTIGetLocalPriceCallbackHandler::OnCentauriGetLocalPrice(const std::string result)
{
    LOGD("MidasProductInfocallback handler : msg: %s\n", result.c_str());
    FString jsonRaw = FString(result.c_str());
    TSharedPtr<FJsonObject> JsonParsed;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonRaw);
    auto  resp = *new CTIGetProductResult();
    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
        if(JsonParsed->HasField("ret")){
            resp.ret = std::string(TCHAR_TO_UTF8(*JsonParsed->GetStringField("ret")));
            resp.msg = std::string(TCHAR_TO_UTF8(*JsonParsed->GetStringField("msg")));
            if(FString("0").Equals(JsonParsed->GetStringField("ret"))) {
                vector<CTIGetProduct> productInfo = *new vector<CTIGetProduct>;
                TArray<TSharedPtr<FJsonValue>> ProductsArray = JsonParsed->GetArrayField("productInfo");
                for (TSharedPtr<FJsonValue> productValue : ProductsArray){
                    auto product = *new CTIGetProduct;
                    TSharedPtr<FJsonObject> productObject = productValue.Get()->AsObject();
                    product.productid = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("productId"))));
                    product.price = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("price"))));
                    product.currency = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("currency"))));
                    product.microprice = productObject->GetIntegerField("microprice");
                    productInfo.push_back(product);
                }
                resp.mProductList = productInfo;
                for (int i = 0; i < productInfo.size(); i++){
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:---------------- start:%d"),i);
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler productid:%s"), productInfo[i].productid.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler price:%s"), productInfo[i].price.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler currency:%s"), productInfo[i].currency.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler microprice:%d"), productInfo[i].microprice);
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:----------------  end:%d"),i);
                }
            }else
            {
                UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:----------------  end:%s"),resp.msg.c_str());
            }
            
            
        }
    }
    
}

CTIGetIntroPriceCallbackHandler::CTIGetIntroPriceCallbackHandler()
{
    
}

CTIGetIntroPriceCallbackHandler::~CTIGetIntroPriceCallbackHandler()
{
    
}

/*
 *
 * {"ret":"0",
 * "msg":"success",
 * "productInfo":
 * [
 *   {"productId":"midas_product_1","price":"US$1.00","currency":"USD","microprice":1000000},
 *   {"productId":"midas_product_3","price":"US$0.99","currency":"USD","microprice":990000}
 *  ]
 * }
 * */
void CTIGetIntroPriceCallbackHandler::OnCentauriGetIntroPrice(const std::string result)
{
    LOGD("MidasIntroPricecallback handler : msg: %s\n", result.c_str());

    FString jsonRaw = FString(result.c_str());
    TSharedPtr<FJsonObject> JsonParsed;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonRaw);
    auto  resp = *new CTIGetProductResult();
    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
        if(JsonParsed->HasField("ret")){
            resp.ret = std::string(TCHAR_TO_UTF8(*JsonParsed->GetStringField("ret")));
            resp.msg = std::string(TCHAR_TO_UTF8(*JsonParsed->GetStringField("msg")));
            if(FString("0").Equals(JsonParsed->GetStringField("ret"))) {
                vector<CTIGetProduct> productInfo = *new vector<CTIGetProduct>;
                TArray<TSharedPtr<FJsonValue>> ProductsArray = JsonParsed->GetArrayField("productInfo");
                for (TSharedPtr<FJsonValue> productValue : ProductsArray){
                    auto product = *new CTIGetProduct;
                    TSharedPtr<FJsonObject> productObject = productValue.Get()->AsObject();
                    product.productid = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("productId"))));
                    product.price = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("price"))));
                    product.currency = std::string(TCHAR_TO_UTF8(*(productObject->GetStringField("currency"))));
                    product.microprice = productObject->GetIntegerField("microprice");
                    productInfo.push_back(product);
                }
                resp.mProductList = productInfo;
                for (int i = 0; i < productInfo.size(); i++){
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:---------------- start:%d"),i);
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler productid:%s"), productInfo[i].productid.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler price:%s"), productInfo[i].price.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler currency:%s"), productInfo[i].currency.c_str());
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler microprice:%d"), productInfo[i].microprice);
                    UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:----------------  end:%d"),i);
                }
            }else
            {
                UE_LOG(LogTemp, Display,TEXT("MidasProductInfoCallbackHandler:----------------  end:%s"),resp.msg.c_str());
            }
            
            
        }
    }
    
}

#endif



