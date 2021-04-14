/*!
 @header APCTIApi.h
 @abstract 米大师苹果支付SDK，头文件
 @author bladebao
 @version 1.6.5c-4.6.1
 */
#import <Foundation/Foundation.h>
#import "CTIIAPPayDelegate.h"

#if TARGET_OS_IOS
#import <WebKit/WebKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@class CTIIAPBaseReq;
@class CTIIAPPayReq;
@class CTIIAPMpInfoReq;
@class CTIIAPPayResp;

extern NSString * CTI_ENV_DEV;
extern NSString * CTI_ENV_SANDBOX;
extern NSString * CTI_ENV_RELEASE;

extern NSString * CTI_SDK_PROCESS_SEA;
extern NSString * CTI_SDK_PROCESS_LOCAL;

extern NSString * CTI_LOCALE_LOCAL;
extern NSString * CTI_LOCALE_HK;
extern NSString * CTI_LOCALE_CA;

extern NSString * CTI_APP_EXTRA;
extern NSString * CTI_APP_DEFINE_INFO;
extern NSString * CTI_APP_RESERVE1;
extern NSString * CTI_APP_RESERVE2;
extern NSString * CTI_APP_RESERVE3;

extern NSString * kCTIUploadDeviceInfoScene;

typedef void(^CTIPromotedIAPCallback)(NSString *_Nonnull productID);

/*!
 @class APCTIApi
 @superclass NSObject
 @abstract 米大师支付SDK的API接口类
 @author bladebao
 */
@interface CTIIAPApi : NSObject

#if TARGET_OS_IPHONE
/// 设置 AppStore 推广促销回调
/// @param callback  AppStore 拉起后执行的回调
+ (void)initPromotedIAPWithCallback:(CTIPromotedIAPCallback)callback NS_AVAILABLE_IOS(11_0);
#endif

/// 设置国内 / 海外包流程
/// 国内流程与海外流程有所不同，对于国内应用，统一使用国内流程
/// 若需使用海外流程，建议在使用前和米大师侧进行沟通确认
/// 该接口需要在调用所有接口之前调用
/// 当应用有多线程逻辑时，注意保证该接口第一个执行
/// @param processName 流程名，可传以下米大师枚举值：
/// CTI_SDK_PROCESS_LOCAL：国内流程
/// CTI_SDK_PROCESS_SEA：海外流程
+ (void)setProcess:(NSString *)processName;

/// 初始化
/// 推荐在主线程调用该接口。所有参数输入正确时，返回 YES，否则返回 NO
/// 注意，在调用该接口前，需先调用 [setProcess:] 方法
/// @param req CTIIAPBaseReq 类的实例，控制初始化参数
/// @param env 请求的米大师环境，可传以下米大师枚举值：
/// CTI_ENV_RELEASE：米大师现网环境
/// CTI_ENV_SANDBOX：米大师沙箱环境
/// CTI_ENV_DEV：米大师开发环境
/// 注意，该值仅控制米大师环境而非苹果环境，所有非上线包（包括 TestFilght 包）连接的都是苹果沙箱环境，即不用扣款
/// @param locale 连接环境的地区，可传以下米大师枚举值：
/// CTI_LOCALE_LOCAL：中国大陆区域（一般使用该值即可）
/// CTI_LOCALE_HK：香港区域
/// CTI_LOCALE_CA：加拿大区域
/// @param dictExtra 扩展字段，业务必须通过 CTI_APP_EXTRA 传入游戏 IDIP 的 Partition值，否则会导致初始化失败
/// @param delegate 补发货回调，仅用作初始化补发，供刷新余额使用，建议和发货回调区分
+ (BOOL)initializeWithReq:(CTIIAPBaseReq *)req
              environment:(NSString *)env
                   locale:(NSString *)locale
                    extra:(NSDictionary *)dictExtra
        reprovideDelegate:(id<CTIIAPPayDelegate>)delegate;

/// 获取版本号
/// 调用该接口后，获取形如 1.6.5c 版本号的字符串
/// 注意，在调用该接口前，需先调用 [setProcess:] 方法
+ (NSString *)getVersion;

/// 设置是否存储本地日志
/// 默认不存，若开关打开后，日志会存储在 ~/Library/GNLLog 目录
/// 当设备内存小于 100 M，磁盘空间小于 200 M 时，不会创建日志文件
/// 若业务自身具备日志捞取功能，可以将 ~/Library/GNLLog 目录的文件一并上传并提供至米大师侧进行分析
/// 此开关仅决定是否本地写日志文件，建议将此开关通过后台下发进行控制
/// 无论该日志开关与否，控制台日志都会正常输出，控制台日志使用 NSLog 进行输出
/// 注意，该接口需在 [setProcess:] 方法调用之后，并在初始化方法调用之前调用
/// @param bEnable 是否存储本地日志
+ (void)enableLog:(BOOL)bEnable;

/// 判断当前 iOS 设备是否支持 IAP 支付
/// 当用户通过家长控制关闭 IAP 购买时，返回 NO
/// 同时，部分越狱设备，也会返回 NO，但不建议从此接口判断设备是否越狱
/// 注意，在调用该接口前，需要先调用 [setProcess:] 方法及初始化方法
+ (BOOL)isIAPEnable;
    
/// 设置关闭 TestFilght 版本支付
/// 由于 TestFilght 版本可能会有外网用户进行使用，但 TestFilght 版本支付无需付款
/// 所以当业务进行 TestFilght 版本发行时，需调用该接口关闭 IAP 支付
/// 避免因为 TestFilght 版本外泄造成损失，建议将此开关通过后台下发进行控制
/// 注意，该接口需在 [setProcess:] 方法调用之后，并在初始化方法调用之前调用
/// @param isTf 当前版本是否 TestFilght 版本
+ (void)enableTestFlight:(BOOL)isTf;

/// 米大师支付相关接口
/// 注意，在调用该接口前，需在调用 [setProcess:] 方法及初始化方法之后调用
/// @param req 需传入 CTIIAPBaseReq 子类实例，当前支持传入以下子类实例：
/// CTIIAPPayReq：用于发起普通的 IAP 支付请求
/// CTIIAPPromotedPayReq：用于发起由 AppStore 拉起的 IAP 支付请求
/// CTIIAPReprovideReq：用于发起主动补发请求
/// CTIIAPRestoreReq：用于发起恢复请求，仅适用于非消耗类型及自动续费订阅类型
/// @param delegate 支付回调，不能为空
+ (void)launchPay:(CTIIAPBaseReq *)req
         delegate:(id<CTIIAPPayDelegate>)delegate;

/// 米大师非支付相关接口
/// 注意，在调用该接口前，需在调用 [setProcess:] 方法及初始化方法之后调用
/// @param req 需传入 CTIIAPBaseReq 子类实例，当前支持传入以下子类实例：
/// CTIIAPMpInfoReq：用于发起营销活动查询请求
/// CTIIAPShortOpenidReq：用于发起短码查询请求
/// CTIIAPProductInfoReq：用于发起 IAP 物品价格查询请求
/// CTIIAPIntroPriceInfoReq：用于发起优惠促销信息查询请求
/// @param delegate 非支付相关接口回调，不能为空
+ (void)launchNet:(CTIIAPBaseReq *)req
         delegate:(id<CTIIAPPayDelegate>)delegate;

#if TARGET_OS_IOS
/// 米大师 JSAPI 注册
/// 注意，在调用该接口前，需在调用 [setProcess:] 方法及初始化方法之后调用
/// @param webView 需要支持米大师 JSAPI 的 WebView，同一时间仅支持一个 WebView
/// @param ctxController 用于展示界面上下文的视图控制器，不能为空
+ (void)prepareWithWebview:(WKWebView *)webView
            viewController:(UIViewController *)ctxController;
#endif

@end

NS_ASSUME_NONNULL_END

