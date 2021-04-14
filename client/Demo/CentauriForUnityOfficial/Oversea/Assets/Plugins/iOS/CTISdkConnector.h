//
//  CentauriSdkConnector.h
//  TestUnity
//
//  Created by tedyang on 16/7/14.
//  Copyright © 2016年 tedyang. All rights reserved.
//

#import <Foundation/Foundation.h>


#define UNITY_GAMEOBJ                   @"CentauriPay.CallBackUtils"

#define UNITY_CALLBACK_INIT             @"CentauriInitCallback"
#define UNITY_CALLBACK_LOGINEXPIRE      @"CentauriLoginExpiredCallback"
#define UNITY_CALLBACK_PAY              @"CentauriPayCallback"
#define UNITY_CALLBACK_GETPRODUCT       @"CentauriGetLocalPriceCallback"
#define UNITY_CALLBACK_GETINFO          @"CentauriGetInfoCallback"
#define UNITY_CALLBACK_GETSHORTOPENID   @"CentauriGetShortOpenidCallback"
#define UNITY_CALLBACK_GETINTROPRICE    @"CentauriGetIntroPriceCallback"
#define UNITY_CALLBACK_REPROVIDE        @"CentauriReProvidetCallback"

//请求参数＝＝＝＝

#define UNITY_REQ_OFFERID               @"offerId"
#define UNITY_REQ_OPENID                @"openId"
#define UNITY_REQ_OPENKEY               @"openKey"
#define UNITY_REQ_SESSIONID             @"sessionId"
#define UNITY_REQ_SESSIONTYPE           @"sessionType"
#define UNITY_REQ_PF                    @"pf"
#define UNITY_REQ_PFKEY                 @"pfKey"
#define UNITY_REQ_PRODUCTID             @"productId"
#define UNITY_REQ_ZONEID                @"zoneId"
#define UNITY_REQ_APPEXTENDS            @"appExtends"
#define UNITY_REQ_IAPEXTRA              @"iapInitExtra"
#define UNITY_REQ_CHANNELEXTRAS         @"channelExtras"
#define UNITY_REQ_SERVICECODE           @"serviceCode"
#define UNITY_REQ_SERVICENAME           @"serviceName"
#define UNITY_REQ_AUTOPAY               @"autoPay"
#define UNITY_REQ_PAYCHANNEL            @"payChannel"
#define UNITY_REQ_COUNTRY               @"country"
#define UNITY_REQ_CURRENCYTYPE          @"currencyType"
#define UNITY_REQ_GOODSZONEID           @"goodsZoneId"


//返回参数＝＝＝＝

#define UNTIY_RESP_RETCODE              @"resultCode"
#define UNTIY_RESP_PAYSTATE             @"payState"
#define UNTIY_RESP_PROVIDESTATE         @"provideState"
#define UNTIY_RESP_RETINNERCODE         @"resultInerCode"
#define UNTIY_RESP_BILLNO               @"billno"
#define UNTIY_RESP_PAYCHANNEL           @"payChannel"
#define UNTIY_RESP_RETMSG               @"resultMsg"
#define UNTIY_RESP_APPEXTENDS           @"extend"




//回调错误码，与C#层对应
#define UNITY_RESP_RESULT_ERROR                  @"-1"
#define UNITY_RESP_RESULT_OK                     @"0"
#define UNITY_RESP_RESULT_CANCEL                 @"1"
#define UNITY_RESP_RESULT_PARAM_ERROR            @"2"
#define UNITY_RESP_RESULT_NET_ERROR              @"3"

@protocol CTISdkConnDelegate <NSObject>

@required//必须实现的代理方法

-(void)setProcess:(NSString *)processName;

//设置调试模式
-(void)enableLog:(BOOL)enable;


-(bool)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString *)idcInfo withJsonParams:(NSString *)jsonParams;

-(void)payWithBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams;

-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSString *)pids;

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSString *)pids;

-(void)getInfoWithType:(NSString *)type withBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams;

-(BOOL)payEnable;

-(NSString *)getVersion;

-(void)reprovide;

@end

