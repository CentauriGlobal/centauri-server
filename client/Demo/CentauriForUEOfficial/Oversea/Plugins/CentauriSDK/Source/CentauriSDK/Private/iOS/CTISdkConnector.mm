//
//  CTISdkConnector.m
//  TestUnity
//
//  Created by tedyang on 16/7/14.
//  Copyright © 2016年 tedyang. All rights reserved.
//
#if PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "CTISdkConnector.h"
#include <string>
#include <map>

using namespace std;

//#if ! __has_feature(objc_arc)
//#error This file must be compiled with ARC. Use -fobjc-arc flag (or convert project to ARC).
//#endif

#if defined(__cplusplus)
extern "C"{
#endif
    extern NSString *CreateNSString (string stlString);
#if defined(__cplusplus)
}
#endif

namespace Centauri {
    class CTIInitCallback;
    class CTIBaseRequest;
    class CTIPayCallback;
    class CTIGetLocalPriceCallback;
    class CTIGetIntroPriceCallback;
    class CTIGetInfoCallback;
    class CTIReprovideCallback;
};

@interface CTISdkConnector()

@property (nonatomic,strong) id<CTISdkConnDelegate> sdkConn;

@end

@implementation CTISdkConnector

@synthesize sdkConn = _sdkConn;

- (instancetype)init
{
    self = [super init];
    if (self) {
        NSString *iapAdapter = @"CTIIAPSDKAdapter";
        
        Class iapClz = NSClassFromString(iapAdapter);
        
        if (iapClz)
        {
            _sdkConn = [[iapClz alloc] init];
        }
        else
        {
            NSLog(@"Error: can not find CTIIAPSDKAdapter");
        }
    }
    return self;
}

- (void)setProcess:(NSString *)processName
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(setProcess:)])
    {
        [_sdkConn setProcess:processName];
    }
}

-(void)enableLog:(BOOL)enable
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(enableLog:)])
    {
        [_sdkConn enableLog:enable];
    }
}

-(void)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString*)idcInfo withReq:(Centauri::CTIBaseRequest *)req callback:(Centauri::CTIInitCallback *)callback
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(initWithEnv:withIdc:withIdcInfo:withReq:callback:)])
    {
        [_sdkConn initWithEnv:env withIdc:idc withIdcInfo:idcInfo withReq:req callback:callback];
    }
}

-(void)payWithReq:(Centauri::CTIBaseRequest *)req callback:(Centauri::CTIPayCallback *)callback viewController:(UIViewController *)vc
{
    
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(payWithReq:callback:viewController:)])
    {
        [_sdkConn payWithReq:req callback:callback viewController:vc];
    }
}

-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(Centauri::CTIGetLocalPriceCallback *)callback
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getProductInfoWithChannel:withProductIds:callback:)])
    {
        [_sdkConn getProductInfoWithChannel:channel withProductIds:pids callback:callback];
    }
}

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSArray<NSString *> *)pids callback:(Centauri::CTIGetIntroPriceCallback *)callback
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getIntroPriceWithChannel:withProductIds:callback:)])
    {
        [_sdkConn getIntroPriceWithChannel:channel withProductIds:pids callback:callback];
    }
}

-(void)getInfoWithType:(NSString *)type withReq:(Centauri::CTIBaseRequest *)req callback:(Centauri::CTIGetInfoCallback *)callback
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getInfoWithType:withReq:callback:)])
    {
        [_sdkConn getInfoWithType:type withReq:req callback:callback];
    }
}

-(NSString *)getVersion
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getVersion)])
    {
        return [_sdkConn getVersion];
    }
    return @"";
}

//支付是否允许
-(BOOL)payEnable
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(payEnable)])
    {
        return [_sdkConn payEnable];
    }
    return YES;
}

-(void)reprovideWithCallback:(Centauri::CTIReprovideCallback *)callback
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(reprovideWithCallback:)])
    {
       [_sdkConn reprovideWithCallback:callback];
    }
}

@end

#if defined(__cplusplus)
extern "C"{
#endif
    NSString * CreateNSString(string stlString)
    {
        if (stlString.length() > 0)
        {
            return [NSString stringWithFormat:@"%s", stlString.c_str()];
        }
        else
        {
            return @"";
        }
    }
    
    static CTISdkConnector *ctiSdk;
    
    void centauriSdkSetProcess(string processName)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        [ctiSdk setProcess:CreateNSString(processName)];
    }
    
    void centauriSdkLogEnable(const bool enable)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        [ctiSdk enableLog:enable];
    }
    
    void centauriSdkInitWithIdcInfo(const void * context, string idc, string env, string idcInfo, const Centauri::CTIBaseRequest * req, const Centauri::CTIInitCallback * callback)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        
        Centauri::CTIBaseRequest * myReq = const_cast<Centauri::CTIBaseRequest *>(req);
        Centauri::CTIInitCallback * myCallback = const_cast<Centauri::CTIInitCallback *>(callback);
        
        [ctiSdk initWithEnv:CreateNSString(env) withIdc:CreateNSString(idc) withIdcInfo:CreateNSString(idcInfo) withReq:myReq callback:myCallback];
    }
    
    void centauriSdkPay(const void * context, const Centauri::CTIBaseRequest * req, const Centauri::CTIPayCallback * callback)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        Centauri::CTIBaseRequest * myReq = const_cast<Centauri::CTIBaseRequest *>(req);
        
        Centauri::CTIPayCallback * myCallback = const_cast<Centauri::CTIPayCallback *>(callback);
        [ctiSdk payWithReq:myReq callback:myCallback viewController:(__bridge UIViewController *)context];
    }
    
    void centauriSdkGetProductInfo(const void * context, string channel, const map<string, string>& productList, const Centauri::CTIGetLocalPriceCallback * callback)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        
        Centauri::CTIGetLocalPriceCallback * myCallback = const_cast<Centauri::CTIGetLocalPriceCallback *>(callback);
        
        
        NSMutableArray<NSString *> * array = [NSMutableArray array];

        map<string,string>::const_iterator it = productList.begin();
        while(it != productList.end())
        {
            [array addObject:CreateNSString(it->first)];
            it++;
        }
        
//        vector<string>::const_iterator iter = productList.begin();
//        for (;iter != productList.end(); ++iter)
//        {
//            [array addObject:CreateNSString(*iter)];
//        }
        
        [ctiSdk getProductInfoWithChannel:CreateNSString(channel) withProductIds:array callback:myCallback];
    }
    
    void centauriSdkGetIntroPrice(const void * context, string channel, const map<string, string>& productList, const Centauri::CTIGetIntroPriceCallback * callback)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        
        Centauri::CTIGetIntroPriceCallback * myCallback = const_cast<Centauri::CTIGetIntroPriceCallback *>(callback);
        
        NSMutableArray<NSString *> * array = [NSMutableArray array];
        
        map<string,string>::const_iterator it = productList.begin();
        while(it != productList.end())
        {
            [array addObject:CreateNSString(it->first)];
            it++;
        }
        
//        vector<string>::const_iterator iter = productList.begin();
//        for (;iter != productList.end(); ++iter)
//        {
//            [array addObject:CreateNSString(*iter)];
//        }
        
        [ctiSdk getIntroPriceWithChannel:CreateNSString(channel) withProductIds:array callback:myCallback];
    }
    
    void centauriSdkGetInfo(const void * context, string type, const Centauri::CTIBaseRequest * req, const Centauri::CTIGetInfoCallback * callback)
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        
        Centauri::CTIBaseRequest * myReq = const_cast<Centauri::CTIBaseRequest *>(req);
        Centauri::CTIGetInfoCallback * myCallback = const_cast<Centauri::CTIGetInfoCallback *>(callback);
        
        [ctiSdk getInfoWithType:CreateNSString(type) withReq:myReq callback:myCallback];
    }
    
    const char* centauriSdkGetVersion()
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        const char  *  cVersion = [[ctiSdk getVersion] UTF8String];
        //        std::string   cVersion=  [[ctiSdk getVersion] UTF8String];
        return cVersion;
    }
    
    const bool centauriSdkPayEnable()
    {
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        return [ctiSdk payEnable];
    }
    
    void centauriSdkReprovide(const void *context, const Centauri::CTIReprovideCallback *callback)
    {
        
        if (ctiSdk == NULL) {
            ctiSdk = [[CTISdkConnector alloc] init];
        }
        Centauri::CTIReprovideCallback * myCallback = const_cast<Centauri::CTIReprovideCallback *>(callback);
        [ctiSdk reprovideWithCallback:myCallback];
    }
    
    void centauriSdkSetPath(string path)
    {
        //iOS not support
    }
    
    void centauriSdkLaunchWeb(const void *context, const Centauri::CTIBaseRequest *req, const Centauri::CTIPayCallback *callback)
    {
        //iOS not support
    }
    
    void centauriSdkCouponsRollBack(const void *context, string s)
    {
        //iOS not support
    }
    
#if defined(__cplusplus)
}
#endif

#endif
