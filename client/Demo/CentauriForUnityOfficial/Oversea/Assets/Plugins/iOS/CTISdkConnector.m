//
//  CTISdkConnector.m
//  TestUnity
//
//  Created by tedyang on 16/7/14.
//  Copyright © 2016年 tedyang. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CTISdkConnector.h"


#if defined(__cplusplus)
extern "C"{
#endif
    extern void UnitySendMessage(const char *, const char *, const char *);
    extern NSString *CreateNSString (const char* string);
#if defined(__cplusplus)
}
#endif

@interface CTISdkConnector : NSObject

@property (nonatomic,strong) id<CTISdkConnDelegate> sdkConn;
@property (nonatomic, assign) BOOL inited;
@end

@implementation CTISdkConnector

@synthesize sdkConn = _sdkConn;

- (instancetype)init
{
    if (self = [super init])
    {
        NSString *iapAdapter = @"CTIIAPSDKAdapter";
        Class iapClz = NSClassFromString(iapAdapter);
        
        if (iapClz)
        {
            _sdkConn = [[iapClz alloc] init];
        }
        else
        {
            NSLog(@"Error: can not find %@", iapAdapter);
        }
        
    }
    
    return self;
}


-(void)setProcess:(NSString *)processName
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

-(BOOL)payEnable
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(payEnable)])
    {
        return [_sdkConn payEnable];
    }
    return YES;
}

-(NSString *)getVersion
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getVersion)])
    {
        return [_sdkConn getVersion];
    }
    return @"";
}

-(bool)initWithEnv:(NSString *)env withIdc:(NSString *)idc withIdcInfo:(NSString *)idcInfo withJsonParams:(NSString *)jsonParams
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(initWithEnv:withIdc:withIdcInfo:withJsonParams:)])
    {
        self.inited = [_sdkConn initWithEnv:env withIdc:idc withIdcInfo:idcInfo withJsonParams:jsonParams];
        return self.inited;
    }
    else
    {
        return false;
    }
    
}

-(void)payWithBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(payWithBizType:withJsonParams:)])
    {
        [_sdkConn payWithBizType:bizType withJsonParams:jsonParams];
    }
}

-(void)getProductInfoWithChannel:(NSString *)channel withProductIds:(NSString *)pids
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getProductInfoWithChannel:withProductIds:)])
    {
        [_sdkConn getProductInfoWithChannel:channel withProductIds:pids];
    }
}

-(void)getGrnProductInfoWithChannel:(NSString *)channel withJsonParams:(NSString *)jsonParams withBizType:(NSString *)bizType
{
    NSLog(@"GetGrnProductInfo not support");
}

-(void)getIntroPriceWithChannel:(NSString *)channel withProductIds:(NSString *)pids
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getIntroPriceWithChannel:withProductIds:)])
    {
        [_sdkConn getIntroPriceWithChannel:channel withProductIds:pids];
    }
}

-(void)getInfoWithType:(NSString *)type withBizType:(NSString *)bizType withJsonParams:(NSString *)jsonParams
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(getInfoWithType:withBizType:withJsonParams:)])
    {
        [_sdkConn getInfoWithType:type withBizType:bizType withJsonParams:jsonParams];
    }
}

-(void)reprovide
{
    if (_sdkConn && [_sdkConn respondsToSelector:@selector(reprovide)])
    {
        [_sdkConn reprovide];
    }
}

@end


#if defined(__cplusplus)
extern "C"{
#endif
    NSString * CreateNSString(const char* string)
    {
        if (string)
        {
            return [NSString stringWithUTF8String:string];
        }
        else
        {
            return [NSString stringWithUTF8String:""];
        }
    }
    
    static CTISdkConnector *CTISdk;
    
    void centauriSdkSetProcess(const char* processName)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        return [CTISdk setProcess:CreateNSString(processName)];
    }
    
    void centauriSdkLogEnable(const bool enable)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk enableLog:enable];
    }
    bool centauriSdkPayEnable()
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        return [CTISdk payEnable];
    }
    const char* centauriSdkGetVersion()
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        
        NSString * str = [CTISdk getVersion] ? : @"";
        char * version = (char *)malloc(strlen([str UTF8String]) + 1);
        strcpy(version, [str UTF8String]);
        
        return version;
    }
    bool centauriSdkInitWithIdcInfo(const char* idc, const char* env, const char* idcInfo, const char* jsonParams)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        return [CTISdk initWithEnv:CreateNSString(env) withIdc:CreateNSString(idc) withIdcInfo:CreateNSString(idcInfo) withJsonParams:CreateNSString(jsonParams)];
    }
    void centauriSdkPay(const char* bizType, const char* jsonParams)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk payWithBizType:CreateNSString(bizType) withJsonParams:CreateNSString(jsonParams)];
    }
    
    void centauriSdkGetProductInfo(const char* channel, const char* productIds)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk getProductInfoWithChannel:CreateNSString(channel) withProductIds:CreateNSString(productIds)];
    }
    
    void centauriSdkGetGrnProductInfo(const char* bizType, const char* channel, const char* jsonParams)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk getGrnProductInfoWithChannel:CreateNSString(channel) withJsonParams:CreateNSString(jsonParams) withBizType:CreateNSString(bizType)];
    }
    
    void centauriSdkGetIntroPrice(const char* channel, const char* productIds)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk getIntroPriceWithChannel:CreateNSString(channel) withProductIds:CreateNSString(productIds)];
    }
    
    void centauriSdkGetInfo(const char* type, const char* bizType, const char* jsonParams)
    {
        if (CTISdk == NULL) {
            CTISdk = [[CTISdkConnector alloc] init];
        }
        [CTISdk getInfoWithType:CreateNSString(type) withBizType:CreateNSString(bizType) withJsonParams:CreateNSString(jsonParams)];
    }
    
    void centauriSdkReprovide()
    {
        [CTISdk reprovide];
    }
    
#if defined(__cplusplus)
}
#endif
