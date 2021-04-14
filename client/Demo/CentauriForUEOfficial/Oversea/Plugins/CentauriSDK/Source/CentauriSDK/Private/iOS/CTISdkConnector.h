//
//  CTISdkConnector.h
//  TestUnity
//
//  Created by tedyang on 16/7/14.
//  Copyright © 2016年 tedyang. All rights reserved.
//

#if PLATFORM_IOS

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "CTIPayService.h"

#define UNITY_GAMEOBJ                   @"CTIUnityPay.CallBackUtils"

#define UNITY_CALLBACK_INIT             @"CTIInitCallback"
#define UNITY_CALLBACK_LOGINEXPIRE      @"CTILoginExpiredCallback"
#define UNITY_CALLBACK_PAY              @"CTIPayCallback"
#define UNITY_CALLBACK_GETPRODUCT       @"CTIGetProductCallback"
#define UNITY_CALLBACK_GETINFO          @"CTIGetInfoCallback"
#define UNITY_CALLBACK_REPROVIDE        @"CTIReProvidetCallback"

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
#define UNITY_REQ_GOODSZONEID           @"goodsZoneId"
#define UNITY_REQ_APPEXTENDS            @"appExtends"
#define UNITY_REQ_CHANNELEXTRAS         @"channelExtras"
#define UNITY_REQ_SERVICECODE           @"serviceCode"
#define UNITY_REQ_SERVICENAME           @"serviceName"
#define UNITY_REQ_AUTOPAY               @"autoPay"
#define UNITY_REQ_PAYCHANNEL            @"payChannel"
#define UNITY_REQ_COUNTRY               @"country"
#define UNITY_REQ_CURRENCYTYPE          @"currencyType"


//返回参数＝＝＝＝

#define UNTIY_RESP_RETCODE              @"resultCode"
#define UNTIY_RESP_RETINNERCODE         @"resultInerCode"
#define UNTIY_RESP_BILLNO               @"billno"
#define UNTIY_RESP_PAYCHANNEL           @"payChannel"
#define UNTIY_RESP_RETMSG               @"resultMsg"
#define UNTIY_RESP_APPEXTENDS           @"appExtends"


//回调错误码，与C#层对应
#define UNITY_RESP_RESULT_ERROR                  @"-1"
#define UNITY_RESP_RESULT_OK                     @"0"
#define UNITY_RESP_RESULT_CANCEL                 @"1"
#define UNITY_RESP_RESULT_PARAM_ERROR            @"2"
#define UNITY_RESP_RESULT_NET_ERROR              @"3"
#define UNITY_RESP_RESULT_PROVIDE_ERROR          @"4"

@protocol CTISdkConnDelegate <NSObject>

@required//必须实现的代理方法

//设置处理流程，国内还是海外。 注意：必须在cti所有接口调用之前进行设置
- (void)setProcess:(NSString *)processName;

//设置调试模式
-(void)enableLog:(BOOL)enable;

-(NSString *)getVersion;

//支付是否允许
-(BOOL)payEnable;

-(void)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString *)idcInfo withReq:(void *)req callback:(void *)callback;

//注意：用(CTI::APCTIBasePayRequest *)req定义形参类型，不要用void *，否则在有继承关系的类中，子类的成员变量会被丢掉
-(void)payWithReq:(void *)req callback:(void *)callback viewController:(UIViewController *)vc;

-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(void *)callback;

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(void *)callback;

-(void)getInfoWithType:(NSString *)type withReq:(void *)req callback:(void *)callback;

-(void)reprovideWithCallback:(void *)callback;

@end


@interface CTISdkConnector : NSObject


@end

#endif
