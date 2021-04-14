//
//  CTIIAPSDKAdapter.m
//  Unity-iPhone
//
//  Created by tedyang on 16/7/19.
//
//

#if PLATFORM_IOS

#import "CTIIAPSDKAdapter.h"

#import <UIKit/UIKit.h>

//#import <CTIIAPSDK/IAPPayHelper.h>

#import <CTIIAPSDK/CTIIAPApi.h>
#import <CTIIAPSDK/CTIIAPObject.h>
#import <CTIIAPSDK/CTIIAPPayDelegate.h>
//#import <CTIIAPSDK/IAPGetProductListDelegate.h>

//#include "CTIPayService.h"
#include "CTIPayService.h"
#include <string>


using namespace CentauriPay;
using namespace std;

//#if ! __has_feature(objc_arc)
//#error This file must be compiled with ARC. Use -fobjc-arc flag (or convert project to ARC).
//#endif

#define UNITY_REQ_CHANNELEXTRAS_PAYITEM     @"payItem"  //channelExtras透传的payItem:道具，传“物品ID*单价(单位：角)*数量”；月卡，传“天数”；游戏币，传“数量”
//#define UNITY_REQ_CHANNELEXTRAS_SAVEVALUE   @"saveValue"
//#define UNITY_REQ_CHANNELEXTRAS             @"channelExtras"


#define GetInfoType_MpInfo       @"mp"
#define GetInfoType_ShortOpenid  @"get_short_openid"


#if defined(__cplusplus)
extern "C"{
#endif
    extern NSString *CreateNSString (string stlString);
#if defined(__cplusplus)
}
#endif



@interface CTIOCCallbackHanlder : NSObject <CTIIAPPayDelegate>

- (void)setInitCallback:(CTIInitCallback *) mInitCallback;
- (void)setPayCallback:(CTIPayCallback *) callback;
- (void)setGetInfoCallback:(CTIGetInfoCallback *) callback;
- (void)setGetShortOpenidCallback:(CTIGetInfoCallback *) callback;
- (void)setGetLocalPriceCallback:(CTIGetLocalPriceCallback *) callback;
- (void)setGetIntroPriceCallback:(CTIGetIntroPriceCallback *) callback;
-(void)setReprovideCallback:(CTIReprovideCallback *) callback;
- (void)setAppExtends:(NSString *) appextends;

@end

@implementation CTIOCCallbackHanlder
{
    CTIInitCallback           * mInitCallback;
    CTIPayCallback            * mPayCallback;
    CTIGetInfoCallback        * mGetInfoCallback;
    CTIGetInfoCallback        * mGetShortOpenidCallback;  //拉短码共用mpinfo的对外回调
    CTIGetLocalPriceCallback  * mGetLocalPriceCallback;
    CTIGetIntroPriceCallback  * mGetIntroPriceCallback;
    CTIReprovideCallback      * mReprovideCallback;
    
    NSString * mAppExtends;
}

- (void)setInitCallback:(CTIInitCallback *) callback
{
    mInitCallback = callback;
}

- (void)setPayCallback:(CTIPayCallback *) callback
{
    mPayCallback = callback;
}

- (void)setGetInfoCallback:(CTIGetInfoCallback *) callback
{
    mGetInfoCallback = callback;
}

- (void)setGetShortOpenidCallback:(CTIGetInfoCallback *) callback
{
    mGetShortOpenidCallback = callback;
}

- (void)setGetLocalPriceCallback:(CTIGetLocalPriceCallback *) callback
{
    mGetLocalPriceCallback = callback;
}

- (void)setGetIntroPriceCallback:(CTIGetIntroPriceCallback *) callback
{
    mGetIntroPriceCallback = callback;
}

-(void)setReprovideCallback:(CTIReprovideCallback *) callback
{
    mReprovideCallback = callback;
}

- (void)setAppExtends:(NSString *) appextends
{
    mAppExtends = appextends;
}

- (void)needLogin
{
    NSLog(@"CTIOCCallbackHanlder needLogin");
    
    
    if (mInitCallback)
    {
        mInitCallback->OnCentauriInitFinished(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
//        mInitCallback = NULL;
    }
    else if (mPayCallback)
    {
        mPayCallback->OnCentauriLoginExpired();
        mPayCallback = NULL;
    }
    else if (mGetInfoCallback)
    {
        mGetInfoCallback->OnCentauriGetInfoFinished(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
        mGetInfoCallback = NULL;
    }
    else if (mGetInfoCallback)
    {
        mGetShortOpenidCallback->OnCentauriGetInfoFinished(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
        mGetShortOpenidCallback = NULL;
    }
    else if (mGetLocalPriceCallback)
    {
        mGetLocalPriceCallback->OnCentauriGetLocalPrice(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
        mGetLocalPriceCallback = NULL;
    }
    else if (mGetIntroPriceCallback)
    {
        mGetIntroPriceCallback->OnCentauriGetIntroPrice(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
        mGetIntroPriceCallback = NULL;
    }
    else if (mReprovideCallback)
    {
        mReprovideCallback->OnCentauriReprovideFinished(string("{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}"));
        mReprovideCallback = NULL;
    }
}

- (void)onResp:(CTIIAPPayResp *)resp
{
    NSLog(@"CTIOCCallbackHanlder onResp called ...");

    int reCode = -1;
    
    switch (resp.resultCode)
    {
        case 1:
            reCode = 2; //android 2是取消
            break;
        case 2:
            reCode = 3; //android 3是参数错
            break;
        default:
            reCode = resp.resultCode;
            break;
    }
    
    if (mInitCallback)
    {
        if (resp.successProductIds.count > 0)
        {
            mInitCallback->OnCentauriInitFinished(string([self strFromResp:resp].UTF8String));
//            mInitCallback = NULL;
        }
        else
        {
            NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"reprovide failed\"}";
            mInitCallback->OnCentauriInitFinished(string(mResult.UTF8String));
//            mInitCallback = NULL;
        }
    }
    else if (mPayCallback)
    {
        CTIResponse response;
        response.resultCode = reCode;
        response.resultInerCode = string(resp.resultInnerCode ? resp.resultInnerCode.UTF8String : "");
        response.payChannel = string("iap");
        response.resultMsg = string(resp.resultMsg ? resp.resultMsg.UTF8String : "");
        response.appExtends = string(mAppExtends ? mAppExtends.UTF8String : "");
        
        mPayCallback->OnCentauriPayFinished(response);
        mPayCallback = NULL;
    }
    else if (mGetInfoCallback)
    {
        if (resp.resultCode == CTI_RESP_RESULT_OK)
        {
            NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":0,\"type\":\"%@\",\"err_code\":\"\",\"msg\":%@}", GetInfoType_MpInfo, resp.respString];
            mGetInfoCallback->OnCentauriGetInfoFinished(string(mResult.UTF8String));
            mGetInfoCallback = NULL;
        }
        else
        {
            NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":-1,\"type\":\"%@\",\"err_code\":\"\",\"msg\":\"get mp info failed\"}", GetInfoType_MpInfo];
            mGetInfoCallback->OnCentauriGetInfoFinished(string(mResult.UTF8String));
            mGetInfoCallback = NULL;
        }
    }
    else if (mGetShortOpenidCallback)
    {
        if (resp.resultCode == CTI_RESP_RESULT_OK)
        {
            NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":0,\"type\":\"%@\",\"err_code\":\"\",\"msg\":%@}", GetInfoType_ShortOpenid, resp.respString];
            mGetShortOpenidCallback->OnCentauriGetInfoFinished(string(mResult.UTF8String));
            mGetShortOpenidCallback = NULL;
        }
        else
        {
            NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":-1,\"type\":\"%@\",\"err_code\":\"\",\"msg\":\"get short openid failed\"}",GetInfoType_ShortOpenid];
            mGetShortOpenidCallback->OnCentauriGetInfoFinished(string(mResult.UTF8String));
            mGetShortOpenidCallback = NULL;
        }
    }
    else if (mGetLocalPriceCallback)
    {
        if (resp.resultCode == CTI_RESP_RESULT_OK)
        {
            mGetLocalPriceCallback->OnCentauriGetLocalPrice(string(resp.respString.UTF8String));
            mGetLocalPriceCallback = NULL;
        }
        else
        {
            NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"get localprice failed\"}";
            mGetLocalPriceCallback->OnCentauriGetLocalPrice(string(mResult.UTF8String));
            mGetLocalPriceCallback = NULL;
        }
    }
    else if (mGetIntroPriceCallback)
    {
        if (resp.resultCode == CTI_RESP_RESULT_OK)
        {
            mGetIntroPriceCallback->OnCentauriGetIntroPrice(string(resp.respString.UTF8String));
            mGetIntroPriceCallback = NULL;
        }
        else
        {
            NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"get introprice failed\"}";
            mGetIntroPriceCallback->OnCentauriGetIntroPrice(string(mResult.UTF8String));
            mGetIntroPriceCallback = NULL;
        }
    }
    else if (mReprovideCallback)
    {
        if (resp.successProductIds.count > 0)
        {
            mReprovideCallback->OnCentauriReprovideFinished(string([self strFromResp:resp].UTF8String));
            mReprovideCallback = NULL;
        }
        else
        {
            NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"reprovide failed\"}";
            mReprovideCallback->OnCentauriReprovideFinished(string(mResult.UTF8String));
            mReprovideCallback = NULL;
        }
    }
}

#pragma mark Private method
- (NSString *)strFromResp:(CTIIAPPayResp *)resp
{
    NSString * successProductIds = @"[]";
    if (resp.successProductIds.count > 0)
    {
        successProductIds = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:resp.successProductIds options:0 error:nil] encoding:NSUTF8StringEncoding];
    }
    
    NSString * failedProductIds = @"{}";
    if (resp.failedProductIds.count > 0)
    {
        failedProductIds = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:resp.failedProductIds options:0 error:nil] encoding:NSUTF8StringEncoding];
    }
    
    NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":0,\"successProductIds\":%@,\"failedProductIds\":%@}", successProductIds,failedProductIds];
    
    return mResult;
}

@end





@interface CTIIAPSDKAdapter()

@property(nonatomic, assign) BOOL showLog;

@end

@implementation CTIIAPSDKAdapter
{
    CTIIAPBaseReq *mInitReq;
    CTIIAPBaseReq * _mReq;
    NSString * _appExtends;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.showLog = NO;
    }
    return self;
}

//设置处理流程，国内还是海外。 注意：必须在CTI所有接口调用之前进行设置
- (void)setProcess:(NSString *)processName
{
    [CTIIAPApi setProcess:processName];
}

//设置调试模式
-(void)enableLog:(BOOL)enable
{
    self.showLog = enable;
    NSLog(@"is show log:%d",self.showLog);
}

//获取版本号
-(NSString *)getVersion
{
    return [CTIIAPApi getVersion];
}

//支付是否可用
-(BOOL)payEnable
{
    //注：必须在主线程调用
    __block BOOL flag = YES;
    
    dispatch_block_t b = ^{
        flag = [CTIIAPApi isIAPEnable];
    };
    if ([NSThread isMainThread]) {
        b();
    } else {
        dispatch_sync(dispatch_get_main_queue(), b);
    }
    
    b = nil;
    
    return flag;
}

-(void)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString *)idcInfo withReq:(void *)req callback:(void *)callback
{
    NSLog(@"sdkIapAdapter initWithEnv called ...");
    
    if (![self checkReq:req andCallback:callback]) return;
    
    CTIBaseRequest *_initReq = (CTIBaseRequest *)req;
    
    if (self.showLog)
    {
        NSLog(@"offerid:%@", CreateNSString(_initReq->offerId));
        NSLog(@"openId:%@", CreateNSString(_initReq->openId));
        NSLog(@"openKey:%@", CreateNSString(_initReq->openKey));
        NSLog(@"sessionId:%@", CreateNSString(_initReq->sessionId));
        NSLog(@"sessionType:%@", CreateNSString(_initReq->sessionType));
        NSLog(@"pf:%@", CreateNSString(_initReq->pf));
        NSLog(@"pfKey:%@", CreateNSString(_initReq->pfKey));
        NSLog(@"goodsZoneId:%@", CreateNSString(_initReq->goodsZoneId));
        NSLog(@"zoneId:%@", CreateNSString(_initReq->zoneId));
    }
    
    
    CTIIAPBaseReq * bReq = [CTIIAPBaseReq new];
    bReq.offerId = CreateNSString(_initReq->offerId);
    bReq.openId = CreateNSString(_initReq->openId);
    bReq.openKey = CreateNSString(_initReq->openKey);
    bReq.sessionId = CreateNSString(_initReq->sessionId);
    bReq.sessionType =  CreateNSString(_initReq->sessionType);
    bReq.pf = CreateNSString(_initReq->pf);
    bReq.pfKey = CreateNSString(_initReq->pfKey);
    bReq.zoneId = CreateNSString(_initReq->zoneId);
    bReq.dictExtend = @{@"scene":@"login"};//SDK内部已实现
    
    mInitReq = bReq;
    
    //kAppExtra = @"app_extra"。(idip分区)
    NSDictionary *dictExtra = @{@"app_extra" : CreateNSString(_initReq->goodsZoneId)};
    
    CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
    [_callbackHandler setInitCallback:(CTIInitCallback *)callback];
    
    //回调设置
    BOOL isInitSucc = [CTIIAPApi initializeWithReq:bReq environment:env locale:idc extra:dictExtra reprovideDelegate:_callbackHandler];

    NSLog(@"CTI isInitSucc: %d",isInitSucc);
}

-(void)payWithReq:(void *)req callback:(void *)callback viewController:(UIViewController *)vc
{
    NSLog(@"sdkIapAdapter payWithBizType called ...");
    
    if (![self checkReq:req andCallback:callback]) return;
    
    CTIBaseRequest *basePayReq = (CTIBaseRequest *)req;
    
    NSString *reqType = CreateNSString(basePayReq->className);
        
    NSString *payItem = CreateNSString(basePayReq->payItem);
//    NSString *iapExtras = CreateNSString(basePayReq->channelExtras);
//    NSMutableDictionary *dict = [self parseKvString:iapExtras];
//    NSString *payItem = [self DV:dict K:UNITY_REQ_CHANNELEXTRAS_PAYITEM];
    
    
    BOOL isDeposit = YES;//只有游戏币模式是YES，其它都是NO
    
    CTIIapProductType pType = CTI_PRODUCT_CONSUMABLE;
    
    //bizType:CTIGameRequest,CTIGoodsRequest,CTISubscribeRequest
    if ([reqType isEqualToString:@"CTIGameRequest"])
    {
        isDeposit = YES;
        pType = CTI_PRODUCT_CONSUMABLE;
    }
    else if ([reqType isEqualToString:@"CTIGoodsRequest"])
    {
        isDeposit = NO;
        pType = CTI_PRODUCT_NAT_RENEW_SUBS;
    }
    else if ([reqType isEqualToString:@"CTIMonthRequest"])
    {
        isDeposit = NO;
        if (((CTIMonthRequest *)basePayReq)->autoPay)
        {
            pType = CTI_PRODUCT_AT_RENEW_SUBS;//包月自动续费
        }
        else
        {
            pType = CTI_PRODUCT_NAT_RENEW_SUBS;//包月非自动续费
        }
        CTIMonthRequest * monPayReq = (CTIMonthRequest *)basePayReq;
        
        if (self.showLog)
        {
            NSLog(@"autoPay:%d",monPayReq->autoPay);
        }
    }
    else if ([reqType isEqualToString:@"CTISubscribeRequest"])
    {
        isDeposit = NO;
        if (((CTISubscribeRequest *)basePayReq)->autoPay)
        {
            pType = CTI_PRODUCT_AT_RENEW_SUBS;//月卡自动续费
        }
        else
        {
            pType = CTI_PRODUCT_NAT_RENEW_SUBS;//包卡月非自动续费
        }
        CTISubscribeRequest * subPayReq = (CTISubscribeRequest *)basePayReq;
        
        if (self.showLog)
        {
            NSLog(@"autoPay:%d",subPayReq->autoPay);
        }
    }
    else
    {
        NSLog(@"%@ %@ bizType error %@", self, NSStringFromSelector(_cmd), reqType);
    }
    
    //varItem做urlencode
    _appExtends = CreateNSString(basePayReq->appExtends);
    NSString * varItem = [_appExtends copy];
    
    //如果有goodsZoneId，则拼到varItem里面
    NSString * goodsZoneId = CreateNSString(basePayReq->goodsZoneId);
    if (goodsZoneId.length > 0 && varItem.length > 0)
    {
        varItem = [[NSString alloc] initWithFormat:@"%@&goods_zoneid=%@", _appExtends, goodsZoneId];
    }
    else
    {
        if (goodsZoneId.length > 0)
        {
            varItem = [[NSString alloc] initWithFormat:@"goods_zoneid=%@", goodsZoneId];
        }
    }
    
    //    //drmInfo拼到varItem.
    //    NSString * drmInfo = CreateNSString(basePayReq->drmInfo);
    //    if (drmInfo.length > 0 && varItem.length > 0)
    //    {
    //        varItem = [[NSString alloc] initWithFormat:@"%@&drm_info=%@", varItem, drmInfo];
    //    }
    //    else
    //    {
    //        if (drmInfo.length > 0)
    //        {
    //            varItem = [[NSString alloc] initWithFormat:@"drm_info=%@", drmInfo];
    //        }
    //    }
    
    if (self.showLog)
    {
        NSLog(@"bizType:%@", reqType);
        NSLog(@"offerid:%@", CreateNSString(basePayReq->offerId));
        NSLog(@"openId:%@", CreateNSString(basePayReq->openId));
        NSLog(@"openKey:%@", CreateNSString(basePayReq->openKey));
        NSLog(@"sessionId:%@", CreateNSString(basePayReq->sessionId));
        NSLog(@"sessionType:%@", CreateNSString(basePayReq->sessionType));
        NSLog(@"pf:%@", CreateNSString(basePayReq->pf));
        NSLog(@"pfKey:%@", CreateNSString(basePayReq->pfKey));
        NSLog(@"payItem:%@", payItem);
        NSLog(@"productId:%@", CreateNSString(basePayReq->productId));
        NSLog(@"isDeposit:%d", isDeposit);
        NSLog(@"pType:%d", pType);
        NSLog(@"zoneId:%@", CreateNSString(basePayReq->zoneId));
        NSLog(@"varItem:%@", varItem);
        NSLog(@"varItem encoded:%@", [self urlEncode:varItem]);
        //        NSLog(@"payChannel:%@", CreateNSString(basePayReq->payChannel));
        //        NSLog(@"currencyType:%@", CreateNSString(basePayReq->currencyType));
        //        NSLog(@"country:%@", CreateNSString(basePayReq->country));
        //        NSLog(@"drmInfo:%@", CreateNSString(basePayReq->drmInfo));
    }
    
    CTIIAPPayReq * iapreq = [CTIIAPPayReq new];
    iapreq.offerId = CreateNSString(basePayReq->offerId);
    iapreq.openId = CreateNSString(basePayReq->openId);
    iapreq.openKey = CreateNSString(basePayReq->openKey);
    iapreq.sessionId = CreateNSString(basePayReq->sessionId);
    iapreq.sessionType = CreateNSString(basePayReq->sessionType);
    iapreq.pf = CreateNSString(basePayReq->pf);
    iapreq.pfKey = CreateNSString(basePayReq->pfKey);
    iapreq.zoneId = CreateNSString(basePayReq->zoneId);
    
    //varItem做urlencode
    iapreq.varItem = [self urlEncode:varItem]; //IAP中varItem的值为drmInfo=xxx&remark=xxx
    iapreq.saveValue = payItem;    //payItem格式:id*数量*单价;
    iapreq.productId = CreateNSString(basePayReq->productId);
    iapreq.depositGameCoin = isDeposit;
    iapreq.productType = pType;
    
    iapreq.contextController = vc;
    
    _mReq = iapreq;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        NSLog(@"startPayOnMainThread");
        
        CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
        [_callbackHandler setPayCallback:(CTIPayCallback *)callback];
        [_callbackHandler setAppExtends:_appExtends];
        [CTIIAPApi launchPay:_mReq delegate:_callbackHandler];
    });
}

-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(void *)callback;
{
    //android格式：{"ret":"0","msg":"success","productInfo":[{"productId":"cti_product_1","price":"HK$7.75","currency":"HKD","microprice":"7750000"}]}
    
    NSLog(@"sdkIapAdapter getProductInfo called ...");
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.showLog)
        {
            NSLog(@"offerid:%@", mInitReq.offerId);
            NSLog(@"openId:%@", mInitReq.openId);
            NSLog(@"openKey:%@", mInitReq.openKey);
            NSLog(@"sessionId:%@", mInitReq.sessionId);
            NSLog(@"sessionType:%@", mInitReq.sessionType);
            NSLog(@"pf:%@", mInitReq.pf);
            NSLog(@"pfKey:%@", mInitReq.pfKey);
            NSLog(@"zoneId:%@", mInitReq.zoneId);
            
            NSLog(@"pids:%@", pids);
        }
        
        //主线程
        //到苹果拉物品信息
        CTIIAPProductInfoReq * iapreq = [CTIIAPProductInfoReq new];
        iapreq.offerId = mInitReq.offerId;
        iapreq.openId = mInitReq.openId;
        iapreq.openKey = mInitReq.openKey;
        iapreq.sessionId = mInitReq.sessionId;
        iapreq.sessionType = mInitReq.sessionType;
        iapreq.pf = mInitReq.pf;
        iapreq.pfKey = mInitReq.pfKey;
        iapreq.zoneId = mInitReq.zoneId;
        iapreq.useCache = YES;
        [iapreq.productIds addObjectsFromArray:pids];
        
        CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
        [_callbackHandler setGetLocalPriceCallback:(CTIGetLocalPriceCallback *)callback];
        
        [CTIIAPApi launchNet:iapreq delegate:_callbackHandler];
    });
}

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(void *)callback;
{
    //android格式：{"ret":"0","msg":"success","productInfo":[{"productId":"cti_product_1","price":"HK$7.75","currency":"HKD","microprice":"7750000"}]}
    
    NSLog(@"sdkIapAdapter getIntroPrice called ...");
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.showLog)
        {
            NSLog(@"offerid:%@", mInitReq.offerId);
            NSLog(@"openId:%@", mInitReq.openId);
            NSLog(@"openKey:%@", mInitReq.openKey);
            NSLog(@"sessionId:%@", mInitReq.sessionId);
            NSLog(@"sessionType:%@", mInitReq.sessionType);
            NSLog(@"pf:%@", mInitReq.pf);
            NSLog(@"pfKey:%@", mInitReq.pfKey);
            NSLog(@"zoneId:%@", mInitReq.zoneId);
            
            NSLog(@"pids:%@", pids);
        }
        
        //主线程
        //到苹果拉物品信息
        CTIIAPIntroPriceInfoReq * iapreq = [CTIIAPIntroPriceInfoReq new];
        iapreq.offerId = mInitReq.offerId;
        iapreq.openId = mInitReq.openId;
        iapreq.openKey = mInitReq.openKey;
        iapreq.sessionId = mInitReq.sessionId;
        iapreq.sessionType = mInitReq.sessionType;
        iapreq.pf = mInitReq.pf;
        iapreq.pfKey = mInitReq.pfKey;
        iapreq.zoneId = mInitReq.zoneId;
        iapreq.fromApple = YES;
        [iapreq.introPriceProuctIds addObjectsFromArray:pids];
        
        CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
        [_callbackHandler setGetIntroPriceCallback:(CTIGetIntroPriceCallback *)callback];
        
        [CTIIAPApi launchNet:iapreq delegate:_callbackHandler];
    });
}


-(void)getInfoWithType:(NSString *)type withReq:(void *)req callback:(void *)callback
{
    NSLog(@"sdkIapAdapter getInfo (%@) called ...",type);
    
    if (![self checkReq:req andCallback:callback]) return;
    
    CTIBaseRequest *basePayReq = (CTIBaseRequest *)req;
    NSString *reqType = CreateNSString(basePayReq->className);
    
    //varItem做urlencode
    _appExtends = CreateNSString(basePayReq->appExtends);
    NSString * varItem = [_appExtends copy];
    
    if (self.showLog)
    {
        NSLog(@"bizType:%@", reqType);
        NSLog(@"offerid:%@", CreateNSString(basePayReq->offerId));
        NSLog(@"openId:%@", CreateNSString(basePayReq->openId));
        NSLog(@"openKey:%@", CreateNSString(basePayReq->openKey));
        NSLog(@"sessionId:%@", CreateNSString(basePayReq->sessionId));
        NSLog(@"sessionType:%@", CreateNSString(basePayReq->sessionType));
        NSLog(@"pf:%@", CreateNSString(basePayReq->pf));
        NSLog(@"pfKey:%@", CreateNSString(basePayReq->pfKey));
        NSLog(@"zoneId:%@", CreateNSString(basePayReq->zoneId));
        NSLog(@"varItem:%@", varItem);
        NSLog(@"varItem encoded:%@", [self urlEncode:varItem]);
    }
         
     if ([type isEqualToString:@"mp"])
     {
         //1.拉营销活动只支持游戏币; 2.IAP中varItem的值为drmInfo=xxx&remark=xxx
         CTIIAPMpInfoReq * iapreq = [CTIIAPMpInfoReq new];
         iapreq.offerId = CreateNSString(basePayReq->offerId);
         iapreq.openId = CreateNSString(basePayReq->openId);
         iapreq.openKey = CreateNSString(basePayReq->openKey);
         iapreq.sessionId = CreateNSString(basePayReq->sessionId);
         iapreq.sessionType = CreateNSString(basePayReq->sessionType);
         iapreq.pf = CreateNSString(basePayReq->pf);
         iapreq.pfKey = CreateNSString(basePayReq->pfKey);
         iapreq.zoneId = CreateNSString(basePayReq->zoneId);
         iapreq.depositGameCoin = CTI_PRODUCT_CONSUMABLE;
         iapreq.varItem = [self urlEncode:varItem];
         
         _mReq = iapreq;
         dispatch_async(dispatch_get_main_queue(), ^{
             NSLog(@"startGetInfoOnMainThread");
             
             CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
             [_callbackHandler setGetInfoCallback:(CTIGetInfoCallback *)callback];
             [_callbackHandler setAppExtends: _appExtends];
             [CTIIAPApi launchNet:_mReq delegate:_callbackHandler];
        });
     }
     else if ([type isEqualToString:@"get_short_openid"])
     {
         CTIIAPShortOpenidReq * iapreq = [CTIIAPShortOpenidReq new];
         iapreq.offerId = CreateNSString(basePayReq->offerId);
         iapreq.openId = CreateNSString(basePayReq->openId);
         iapreq.openKey = CreateNSString(basePayReq->openKey);
         iapreq.sessionId = CreateNSString(basePayReq->sessionId);
         iapreq.sessionType = CreateNSString(basePayReq->sessionType);
         iapreq.pf = CreateNSString(basePayReq->pf);
         iapreq.pfKey = CreateNSString(basePayReq->pfKey);
         iapreq.zoneId = CreateNSString(basePayReq->zoneId);
         
         _mReq = iapreq;
         dispatch_async(dispatch_get_main_queue(), ^{
             NSLog(@"startGetShortOpenidOnMainThread");
             
             CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
             [_callbackHandler setGetShortOpenidCallback:(CTIGetInfoCallback *)callback];
             [_callbackHandler setAppExtends: _appExtends];
             [CTIIAPApi launchNet:_mReq delegate:_callbackHandler];
         });
     }
     else
     {
         NSLog(@"Error:Does not support the type:%@",type);
     }
}

-(void)reprovideWithCallback:(void *)callback
{
    NSLog(@"sdkIapAdapter reprovide called ...");
    
    if(mInitReq == NULL)
    {
        NSLog(@"sdkIapAdapter Please call Initialize method first ...");
        return;
    }
    
    //补发货接口可以异步调用
    CTIIAPReprovideReq *iapreq = [CTIIAPReprovideReq new];
    iapreq.offerId = mInitReq.offerId;
    iapreq.openId = mInitReq.openId;
    iapreq.openKey = mInitReq.openKey;
    iapreq.sessionId = mInitReq.sessionId;
    iapreq.sessionType = mInitReq.sessionType;
    iapreq.pf = mInitReq.pf;
    iapreq.pfKey = mInitReq.pfKey;
    iapreq.zoneId = mInitReq.zoneId;
    
    CTIOCCallbackHanlder * _callbackHandler = [[CTIOCCallbackHanlder alloc] init];
    [_callbackHandler setReprovideCallback:(CTIReprovideCallback *)callback];
    
    [CTIIAPApi launchPay:iapreq delegate:_callbackHandler];
}



#pragma mark Private method --

-(BOOL)checkReq:(void *)req andCallback:(void *)callback
{
    if (!req)
    {
        NSLog(@"%@, req is null, ", NSStringFromClass([self class]));
        return NO;
    }
    
    if (!callback)
    {
        NSLog(@"%@, callback is null", NSStringFromClass([self class]));
        return NO;
    }
    
    return YES;
}

-(NSString *)DV:(NSDictionary *)dict K:(NSString *)key
{
    if (dict && dict.count > 0)
    {
        NSString *v = dict[key];
        if (v.length > 0) {
            return v;
        }
    }
    return @"";
}

//将字符串转成字典，kvStr如"CurrencyAmt=6000&CurrencyType=CNY&SignKey=vpkYef59hWQw7uR9zMbgiWQwCn84uL51FCn84";
- (NSMutableDictionary *)parseKvString:(NSString *)kvStr
{
    NSMutableDictionary *tmpDict = [NSMutableDictionary dictionary];
    if (kvStr.length > 0) {
        NSArray *array = [kvStr componentsSeparatedByString:@"&"];
        for (NSString *str in array) {
            NSArray *arr = [str componentsSeparatedByString:@"="];
            [tmpDict setObject:arr[1] forKey:arr[0]];
        }
    }
    return tmpDict;
}

- (NSString *)urlEncode:(NSString *)str
{
    if (str.length == 0) {
            return @"";
    }
    NSString * strCpy = str;
    NSString * result = [strCpy stringByAddingPercentEncodingWithAllowedCharacters:[[NSCharacterSet characterSetWithCharactersInString:@"!*'();:@&=+$,/?%#[]"] invertedSet]];
    if (nil != result)
    {
        strCpy = [NSString stringWithString:result];
    }
   
    return strCpy;
}

@end

#endif
