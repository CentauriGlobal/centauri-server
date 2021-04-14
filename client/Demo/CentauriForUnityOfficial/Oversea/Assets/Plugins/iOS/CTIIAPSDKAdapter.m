//
//  CTIIAPSDKAdapter.m
//  Unity-iPhone
//
//  Created by tedyang on 16/7/19.
//
//

#import "CTIIAPSDKAdapter.h"

#import <CTIIAPSDK/CTIIAPPayDelegate.h>
//#import <CTIIAPSDK/IAPGetProductListDelegate.h>
#import <CTIIAPSDK/CTIIAPApi.h>
#import <CTIIAPSDK/CTIIAPObject.h>

#define UNITY_REQ_CHANNELEXTRAS_PAYITEM     @"payItem"  //channelExtras透传的payItem:道具，传“物品ID*单价(单位：角)*数量”；月卡，传“天数”；游戏币，传“数量”
#define UNITY_REQ_CHANNELEXTRAS_SAVEVALUE   @"saveValue"
#define UNITY_REQ_CHANNELEXTRAS @"channelExtras"

#define GetInfoType_MpInfo       @"mp"
#define GetInfoType_ShortOpenid  @"get_short_openid"

enum {
    kCentauriInitCallback = 0,
    kCentauriPayCallback,
    kCentauriGetInfoCallback,
    kCentauriGetLocalPriceCallback,
    kCentauriGetIntroPriceCallback,
    kCentauriGetShortOpenid,
    kCentauriReprovideCallback,
};
typedef    uint32_t CTIOCCallbackType;

@interface CTIOCCallbackHanlder : NSObject <CTIIAPPayDelegate>

@property(nonatomic, assign) CTIOCCallbackType cbType;

@end

@implementation CTIOCCallbackHanlder

- (void)needLogin
{
    NSLog(@"CTIInitCallbackHanlder needLogin");
    
    NSString *mResult = @"{\"ret\":1018,\"err_code\":\"1018-1018-0\",\"msg\":\"login expired\"}";
    
    switch (_cbType) {
        case kCentauriInitCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_INIT.UTF8String, mResult.UTF8String);
        }
            break;
        case kCentauriPayCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_LOGINEXPIRE.UTF8String, "0");
        }
            break;
        case kCentauriGetInfoCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINFO.UTF8String, mResult.UTF8String);
        }
            break;
        case kCentauriGetShortOpenid:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETSHORTOPENID.UTF8String, mResult.UTF8String);
        }
            break;
        case kCentauriGetLocalPriceCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETPRODUCT.UTF8String, mResult.UTF8String);
        }
            break;
        case kCentauriGetIntroPriceCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINTROPRICE.UTF8String, mResult.UTF8String);
        }
            break;
        case kCentauriReprovideCallback:
        {
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_REPROVIDE.UTF8String, mResult.UTF8String);
        }
            break;
        default:
            break;
    }
}

- (void)onResp:(CTIIAPPayResp *)resp
{
    //    ios错误码
    //    AP_Centauri_RESP_RESULT_ERROR                  = -1,
    //    AP_Centauri_RESP_RESULT_OK                     = 0,
    //    AP_Centauri_RESP_RESULT_CANCEL                 = 1,
    //    AP_Centauri_RESP_RESULT_PARAM_ERROR            = 2,
    //    AP_Centauri_RESP_RESULT_NET_ERROR              = 3,
    //    AP_Centauri_RESP_RESULT_IAP_ERROR              = 4,
    //    AP_Centauri_RESP_RESULT_IAP_NEED_REBOOT        = 5,
    
    //    Android错误码
    //    //支付流程失败
    //    PAYRESULT_ERROR        = -1;
    //    //支付流程成功
    //    PAYRESULT_SUCC         = 0;
    //    //用户取消
    //    PAYRESULT_CANCEL       = 2;
    //    //参数错误
    //    PAYRESULT_PARAMERROR   = 3;
    
    
    int reCode = -1;
    
    switch (resp.resultCode) {
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
    
    switch (_cbType) {
        case kCentauriInitCallback:
        {
            if (resp.successProductIds.count > 0)
            {
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_INIT.UTF8String, [self strFromResp:resp].UTF8String);
            }
            else
            {
                NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"reprovide failed\"}";
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_INIT.UTF8String, mResult.UTF8String);
            }
        }
            break;
        case kCentauriPayCallback:
        {
            NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:7];
            [dict setObject:[[NSString alloc] initWithFormat:@"%i",(int)reCode] forKey:UNTIY_RESP_RETCODE];
            [dict setObject:resp.resultInnerCode forKey:UNTIY_RESP_RETINNERCODE];
            [dict setObject:@"iap" forKey:UNTIY_RESP_PAYCHANNEL];
            [dict setObject:resp.resultMsg forKey:UNTIY_RESP_RETMSG];
            //    [dict setObject:@"" forKey:UNTIY_RESP_APPEXTENDS]; //C#层实现
            
            NSError *parseError = nil;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&parseError];
            NSString *respStr = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
            UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_PAY.UTF8String, respStr.UTF8String);
        }
            break;
        case kCentauriGetShortOpenid:
        {
            if (resp.resultCode == CTI_RESP_RESULT_OK)
            {
                NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":0,\"type\":\"%@\",\"err_code\":\"\",\"msg\":%@}", GetInfoType_ShortOpenid, resp.respString];
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETSHORTOPENID.UTF8String, mResult.UTF8String);
            }
            else
            {
                NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":-1,\"type\":\"%@\",\"err_code\":\"\",\"msg\":\"get short openid failed\"}",GetInfoType_ShortOpenid];
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETSHORTOPENID.UTF8String, mResult.UTF8String);
            }
        }
            break;
        case kCentauriGetInfoCallback:
        {
            if (resp.resultCode == CTI_RESP_RESULT_OK)
            {
                 NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":0,\"type\":\"%@\",\"err_code\":\"\",\"msg\":%@}", GetInfoType_MpInfo, resp.respString];
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINFO.UTF8String, mResult.UTF8String);
            }
            else
            {
                NSString *mResult = [[NSString alloc] initWithFormat:@"{\"ret\":-1,\"type\":\"%@\",\"err_code\":\"\",\"msg\":\"get mp info failed\"}", GetInfoType_MpInfo];
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINFO.UTF8String, mResult.UTF8String);
            }
        }
            break;
        case kCentauriGetLocalPriceCallback:
        {
            if (resp.resultCode == CTI_RESP_RESULT_OK)
            {
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETPRODUCT.UTF8String, resp.respString.UTF8String);
            }
            else
            {
                NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"get info failed\"}";
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETPRODUCT.UTF8String, mResult.UTF8String);
            }
        }
            break;
        case kCentauriGetIntroPriceCallback:
        {
            if (resp.resultCode == CTI_RESP_RESULT_OK)
            {
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINTROPRICE.UTF8String, resp.respString.UTF8String);
            }
            else
            {
                NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"get intro price failed\"}";
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_GETINTROPRICE.UTF8String, mResult.UTF8String);
            }
        }
            break;
        case kCentauriReprovideCallback:
        {
            if (resp.successProductIds.count > 0)
            {
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_REPROVIDE.UTF8String, [self strFromResp:resp].UTF8String);
            }
            else
            {
                NSString *mResult = @"{\"ret\":-1,\"err_code\":\"\",\"msg\":\"reprovide failed\"}";
                UnitySendMessage(UNITY_GAMEOBJ.UTF8String, UNITY_CALLBACK_REPROVIDE.UTF8String, mResult.UTF8String);
            }
        }
            break;
        default:
            break;
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

@property(nonatomic, copy) NSString * paramsInit;
@property(nonatomic, strong) NSMutableArray< CTIOCCallbackHanlder*> * callbacks; //保存回调，防止回收

@end

@implementation CTIIAPSDKAdapter

-(void)setProcess:(NSString *)processName
{
    return [CTIIAPApi setProcess:processName];
}


//设置调试模式
-(void)enableLog:(BOOL)enable
{
//    [CTIIAPApi enableLog:enable];   //此接已废弃
}

-(BOOL)payEnable
{
    return [CTIIAPApi isIAPEnable];
}

-(NSString *)getVersion
{
    return [CTIIAPApi getVersion];
}

-(bool)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString *)idcInfo withJsonParams:(NSString *)jsonParams
{
    NSLog(@"sdkIapAdapter initWithEnv:%@ idc: %@",env, idc);
    
    //初始化回调数组
    _callbacks = [NSMutableArray arrayWithCapacity:1];
    
    self.paramsInit = jsonParams;
    NSMutableDictionary *mDictParams = [self convertParamsFromString:jsonParams];
    
    CTIIAPBaseReq * req = [CTIIAPBaseReq new];
    req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
    req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
    req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
    req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
    req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
    req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
    req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
    req.dictExtend = @{@"scene":@"login"};
    req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];
    
    NSDictionary *dictExtra = @{@"app_extra" : mDictParams[UNITY_REQ_GOODSZONEID]};
    NSLog(@"dictExtra:%@",dictExtra);
    
    CTIOCCallbackHanlder * callbackHandler = [self getCallbackHandler:kCentauriInitCallback];
    return [CTIIAPApi initializeWithReq:req environment:env locale:idc extra:dictExtra reprovideDelegate:callbackHandler] ? true : false;
}

-(void)payWithBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams
{
    NSLog(@"sdkIapAdapter payWithBizType called ...");
    
    //解析购买参数
    NSMutableDictionary *mDictParams = [self convertParamsFromString:jsonParams];
    
    NSString *payItem = [self DV:mDictParams K:UNITY_REQ_CHANNELEXTRAS_PAYITEM];

    BOOL isDeposit = YES;//只有游戏币模式是YES，其它都是NO
    
    CTIIapProductType pType = CTI_PRODUCT_CONSUMABLE;
    
    
    //bizType:CTIGameRequest,CTIGoodsRequest,CTISubscribeRequest
    if ([bizType isEqualToString:@"CTIGameRequest"])
    {
        isDeposit = YES;
        pType = CTI_PRODUCT_CONSUMABLE;
    }
    
    if ([bizType isEqualToString:@"CTIGoodsRequest"])
    {
        isDeposit = NO;
        pType = CTI_PRODUCT_NAT_RENEW_SUBS;
    }
    
    if ([bizType isEqualToString:@"CTIMonthRequest"])
    {
        isDeposit = NO;
        //是否自动续费
        if ([self DVBOOL:mDictParams K:UNITY_REQ_AUTOPAY])
        {
#if DEBUG
            NSLog(@"包月自动续费");
#endif
            pType = CTI_PRODUCT_AT_RENEW_SUBS;
        }
        else
        {
#if DEBUG
            NSLog(@"包月非自动续费");
#endif
            pType = CTI_PRODUCT_NAT_RENEW_SUBS;
        }
    }
    
    if ([bizType isEqualToString:@"CTISubscribeRequest"])
    {
        isDeposit = NO;
        //是否自动续费
        if ([self DVBOOL:mDictParams K:UNITY_REQ_AUTOPAY])
        {
#if DEBUG
            NSLog(@"月卡自动续费");
#endif
            pType = CTI_PRODUCT_AT_RENEW_SUBS;
        }
        else
        {
#if DEBUG
            NSLog(@"月卡非自动续费");
#endif
            pType = CTI_PRODUCT_NAT_RENEW_SUBS;
        }
    }
    
    CTIIAPPayReq * req = [CTIIAPPayReq new];
    req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
    req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
    req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
    req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
    req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
    req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
    req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
    req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];

    //varItem做urlencode
    NSString *varItem = [self DV:mDictParams K:UNITY_REQ_APPEXTENDS];
    req.varItem = [self urlEncode:varItem]; //IAP中varItem的值为drmInfo=xxx&remark=xxx
    req.saveValue = payItem;
    req.productId = [self DV:mDictParams K:UNITY_REQ_PRODUCTID];
    req.depositGameCoin = isDeposit;
    req.productType = pType;
    
    UIViewController* vc = UnityGetGLViewController();
    req.contextController = vc;
    
    CTIOCCallbackHanlder * callbackHandler = [self getCallbackHandler:kCentauriPayCallback];
    
    [CTIIAPApi launchPay:req delegate:callbackHandler];
}



-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSString *)pids
{
    NSLog(@"sdkIapAdapter getProductInfo called ...");
    
    NSMutableArray *productIds = [self convertParamsFromString:pids];
    
    NSMutableDictionary *mDictParams = [self convertParamsFromString:self.paramsInit];
    
    CTIIAPProductInfoReq * req = [[CTIIAPProductInfoReq alloc] init];
    req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
    req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
    req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
    req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
    req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
    req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
    req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
    req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];
    req.useCache = YES;
    
    [req.productIds addObjectsFromArray:productIds];
    
    CTIOCCallbackHanlder * callbackHandler = [self getCallbackHandler:kCentauriGetLocalPriceCallback];
    [CTIIAPApi launchNet:req delegate:callbackHandler];
}


-(void)getInfoWithType:(NSString *)type withBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams
{
    NSLog(@"sdkIapAdapter getInfo (%@) called ...",type);
    
    NSMutableDictionary *mDictParams = [self convertParamsFromString:jsonParams];
    
    //varItem做urlencode
    NSString *varItem = [self DV:mDictParams K:UNITY_REQ_APPEXTENDS];
    
    if ([type isEqualToString:GetInfoType_MpInfo])
    {
        //1.拉营销活动只支持游戏币; 2.IAP中varItem的值为drmInfo=xxx&remark=xxx
        CTIIAPMpInfoReq * req = [CTIIAPMpInfoReq new];
        req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
        req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
        req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
        req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
        req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
        req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
        req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
        req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];
        req.depositGameCoin = CTI_PRODUCT_CONSUMABLE;
        req.varItem = [self urlEncode:varItem];
        
        CTIOCCallbackHanlder *callbackHandler = [self getCallbackHandler:kCentauriGetInfoCallback];
        [CTIIAPApi launchNet:req delegate:callbackHandler];
    }
    else if ([type isEqualToString:GetInfoType_ShortOpenid])
    {
        //拉取短码
        CTIIAPShortOpenidReq * req = [[CTIIAPShortOpenidReq alloc] init];
        req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
        req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
        req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
        req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
        req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
        req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
        req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
        req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];
        
        CTIOCCallbackHanlder *callbackHandler = [self getCallbackHandler:kCentauriGetShortOpenid];
        [CTIIAPApi launchNet:req delegate:callbackHandler];
    }
}

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSString *)pids
{
    NSLog(@"sdkIapAdapter getInftroPrice called ...");
    
    NSMutableArray *productIds = [self convertParamsFromString:pids];
    
    NSMutableDictionary *mDictParams = [self convertParamsFromString:self.paramsInit];
    
    CTIIAPIntroPriceInfoReq * req = [[CTIIAPIntroPriceInfoReq alloc] init];
    req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
    req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
    req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
    req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
    req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
    req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
    req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
    req.zoneId = req.zoneId = [self DV:mDictParams K:UNITY_REQ_ZONEID];;
    req.fromApple = YES;
    
    [req.introPriceProuctIds addObjectsFromArray:productIds];
    
    CTIOCCallbackHanlder * callbackHandler = [self getCallbackHandler:kCentauriGetIntroPriceCallback];
    [CTIIAPApi launchNet:req delegate:callbackHandler];
    
}

-(void)reprovide
{
    NSLog(@"sdkIapAdapter reprovide called ...");
    
    NSMutableDictionary *mDictParams = [self convertParamsFromString:self.paramsInit];
    
    //补发货接口可以异步调用
    CTIIAPReprovideReq * req = [[CTIIAPReprovideReq alloc] init];
    req.offerId = [self DV:mDictParams K:UNITY_REQ_OFFERID];
    req.openId = [self DV:mDictParams K:UNITY_REQ_OPENID];
    req.openKey = [self DV:mDictParams K:UNITY_REQ_OPENKEY];
    req.sessionId = [self DV:mDictParams K:UNITY_REQ_SESSIONID];
    req.sessionType = [self DV:mDictParams K:UNITY_REQ_SESSIONTYPE];
    req.pf = [self DV:mDictParams K:UNITY_REQ_PF];
    req.pfKey = [self DV:mDictParams K:UNITY_REQ_PFKEY];
    req.zoneId = @"1";
    
    CTIOCCallbackHanlder * callbackHandler = [self getCallbackHandler:kCentauriReprovideCallback];
    [CTIIAPApi launchPay:req delegate:callbackHandler];
}



#pragma mark Private method

- (CTIOCCallbackHanlder *)getCallbackHandler:(CTIOCCallbackType)cbType
{
    CTIOCCallbackHanlder * callbackHandler = nil;
    for (CTIOCCallbackHanlder * cbh in _callbacks)
    {
        if (cbh.cbType == cbType)
        {
            callbackHandler = cbh;
            return cbh;
        }
    }
    
    //没有找到回调实例
    CTIOCCallbackHanlder * newCallbackHandler = [[CTIOCCallbackHanlder alloc] init];
    newCallbackHandler.cbType = cbType;
    [_callbacks addObject:newCallbackHandler];
    return newCallbackHandler;
}

- (id)convertParamsFromString:(NSString *)jsonParams
{
    NSData * mJsonData = [jsonParams dataUsingEncoding:NSUTF8StringEncoding];
    
    id tmpParams = [NSJSONSerialization JSONObjectWithData:mJsonData
                                                   options:NSJSONReadingMutableContainers | NSJSONReadingMutableLeaves
                                                     error:nil];
#if DEBUG
    NSLog(@"CTI | Params: %@", tmpParams);
#endif
    
    return tmpParams;
}

-(NSString *)DV:(NSDictionary *)dict K:(NSString *)key
{
    if (dict && dict.count > 0)
    {
        NSString *v = dict[key];
        
        if (v.length > 0)
        {
            return v;
        }
    }
    return @"";
}

-(BOOL)DVBOOL:(NSDictionary *)dict K:(NSString *)key
{
    if (dict && dict.count > 0)
    {
        id v = dict[key];
        
        if ([v isKindOfClass:[NSNumber class]])
        {
            return ((NSNumber *)v).boolValue;
        }
    }
    return NO;
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
    NSString *result = (NSString *)CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, (CFStringRef)strCpy, NULL, CFSTR("!*'();:@&=+$,/?%#[]"), kCFStringEncodingUTF8));
    if (nil != result)
    {
        strCpy = result;
    }
    return strCpy;
}


@end
