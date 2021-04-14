/*!
 @header CTIIAPObject.h
 @abstract 米大师苹果支付SDK对象和枚举定义，头文件
 @author bladebao
 @version 1.6.5c-4.6.1
 */
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif

/*!
@enum APCTIRespResultCode
@abstract 米大师SDK返回码
 @constant CTI_RESP_RESULT_RISK_ERROR 柔性风控，业务在完成一定任务后，通过风控上报接口到 CTI 解除支付锁定，附：SDK 回调新接口 retCode为-5，innerCode 透传分级信息 rc_task_level，老接口 onOrderFailure 回调 errorCode 为 -5，rc_task_level 放在 RequestInfo 的 context 里面
@constant CTI_RESP_RESULT_ERROR 错误
@constant CTI_RESP_RESULT_OK 成功
@constant CTI_RESP_RESULT_CANCEL 取消
@constant CTI_RESP_RESULT_PARAM_ERROR 参数错误
@constant CTI_RESP_RESULT_NET_ERROR 网络错误
@constant CTI_RESP_RESULT_IAP_ERROR IAP支付失败
@constant CTI_RESP_RESULT_IAP_NEED_REBOOT IAP 需要重启补发
@constant CTI_RESP_RESULT_IAP_ALREADY_IN_PROGRESS 已经在流程处理中，可能同时调了多次支付，或补发时再次调起了补发
@constant CTI_RESP_RESULT_AUTOSUBS_DISCOUNT_SYSTEMVERSION_UNSUPPORT 自动续费订阅折扣购买时，系统版本不能小于 iOS 12.2 / macOS 10.14.4
@constant CTI_RESP_RESULT_IN_TESTFLIGHT_MODE 当前 App 是 TF 版本
@constant CTI_RESP_RESULT_PAY_ERROR_CAUSED_BY_REPROVIDING 因同一物品补发支付失败导致支付不能继续进行（当此错误码出现时，此时 resultInnerCode 为 APCTIRespResultCode 值）
@constant CTI_RESP_RESULT_IAP_RECEIPT_ERROR macOS 获取票据失败
@author bladebao
*/
typedef enum : SInt32
{
    CTI_RESP_RESULT_RISK_ERROR                                 = -5,
    CTI_RESP_RESULT_ERROR                                      = -1,
    CTI_RESP_RESULT_OK                                         = 0,
    CTI_RESP_RESULT_CANCEL                                     = 1,
    CTI_RESP_RESULT_PARAM_ERROR                                = 2,
    CTI_RESP_RESULT_NET_ERROR                                  = 3,
    CTI_RESP_RESULT_IAP_ERROR                                  = 4,
    CTI_RESP_RESULT_IAP_NEED_REBOOT                            = 5,
    CTI_RESP_RESULT_IAP_ALREADY_IN_PROGRESS                    = 6,
    CTI_RESP_RESULT_AUTOSUBS_DISCOUNT_SYSTEMVERSION_UNSUPPORT  = 8,
    CTI_RESP_RESULT_IN_TESTFLIGHT_MODE                         = 9,
    CTI_RESP_RESULT_PAY_ERROR_CAUSED_BY_REPROVIDING            = 13,
#if TARGET_OS_OSX
    CTI_RESP_RESULT_IAP_RECEIPT_ERROR                          = 20,
#endif
} APCTIRespResultCode;


/*!
 @enum CTIIapProductType
 @abstract 米大师IAP支付物品类型，根据iTC中苹果的物品类型映射而来
 @constant CTI_PRODUCT_CONSUMABLE 消耗型，一般对应于钻石等
 @constant CTI_PRODUCT_N_CONSUMABLE 非消耗型，比较少用，如果想支持此种类型，请先咨询米大师
 @constant CTI_PRODUCT_AT_RENEW_SUBS 自动续费订阅型，视频、杂志、图书、动漫等才能接入，接入前也请先咨询米大师
 @constant CTI_PRODUCT_FREE_SUBS 免费订阅型，目前暂不支持
 @constant CTI_PRODUCT_NAT_RENEW_SUBS 非自动续费订阅型，目前公司主要的包月、月卡均采用此种类型的商品
 @author bladebao
 */
typedef enum : SInt32
{
    CTI_PRODUCT_CONSUMABLE             = 0,
    CTI_PRODUCT_N_CONSUMABLE           = 1,
    CTI_PRODUCT_AT_RENEW_SUBS          = 2,
    CTI_PRODUCT_FREE_SUBS              = 3,
    CTI_PRODUCT_NAT_RENEW_SUBS         = 4
} CTIIapProductType;

#if TARGET_OS_IOS
/*!
 @enum CTIIAPUIType
 @abstract 米大师IAP界面类型
 @constant CTI_UI_DEFAULT 默认界面，只有loading和少量错误提示，防止业务没有做锁屏保护而导致用户重复购买
 @constant CTI_UI_FULL 全程带有米大师商城页和结果页，以及营销活动展示等界面
 @author bladebao
 */
typedef enum : SInt32
{
    CTI_UI_DEFAULT                        = 0,
    CTI_UI_FULL                           = 1,
} CTIIAPUIType;
#endif

/*!
 @class CTIIAPBaseReq
 @superclass NSObject
 @abstract 米大师IAP支付SDK支付请求基类
 @author bladebao
 */
@interface CTIIAPBaseReq : NSObject

/*!
 @property offerId
 @abstract 支付账户Id，米大师支付后台申请到的账户Id
 @author bladebao
 */
@property (nonatomic, copy) NSString * offerId;

/*!
 @property openId
 @abstract 用户登录的openId
 @author bladebao
 */
@property (nonatomic, copy) NSString * openId;

/*!
 @property openKey
 @abstract 用户登录的session
 @author bladebao
 */
@property (nonatomic, copy) NSString * openKey;

/*!
 @property sessionId
 @abstract openId对应的session类型，如uin等
 @author bladebao
 */
@property (nonatomic, copy) NSString * sessionId;

/*!
 @property sessionType
 @abstract openKey对应的的sessionType，如skey，t_skey，openKey等
 @author bladebao
 */
@property (nonatomic, copy) NSString * sessionType;

/*!
 @property zoneId
 @abstract 分区
 @author bladebao
 */
@property (nonatomic, copy) NSString * zoneId;

/*!
 @property pf
 @abstract 平台来源，一般是平台+注册渠道+版本+安装渠道+业务标识（自定义）
 @author bladebao
 */
@property (nonatomic, copy) NSString * pf;

/*!
 @property pfKey
 @abstract 由平台来源和openKey根据规则生成的密钥，对于公司自研应用固定为“pfKey”
 @author bladebao
 */
@property (nonatomic, copy) NSString * pfKey;

/*!
 @property depositGameCoin
 @abstract 是否托管游戏币
 @author bladebao
 */
@property (nonatomic, assign) BOOL depositGameCoin;

/*!
 @property varItem
 @abstract varItem字段，给他人购买时使用，传入URLEncode(provide_uin=openId&provide_no_type=sessionId)
 @author bladebao
 */
@property (nonatomic, copy) NSString * varItem;

/*!
 @property extend
 @abstract 扩展字段，kv 形式，目前用于供业务传入 URLEncode(game_coin_unit=游戏币单位)
 @author bladebao
 */
@property (nonatomic, copy) NSString * extend;

/*!
 @property dictExtend
 @abstract 扩展字段
 @author bladebao
 */
@property (nonatomic, strong) NSDictionary * dictExtend;

/*!
 @property urlParams
 @abstract 业务侧在米大师下单后拿到的url_params参数，需要原样传入
 @author bladebao
 */
@property (nonatomic, strong) NSString * urlParams;

/*!
 @property
 @abstract 检查参数
 @author bladebao
 @result 参数正确返回true,否则返回false
 */
- (BOOL)validateParams;

@end

/*!
 @class CTIIAPPayReq
 @superclass CTIIAPBaseReq
 @abstract 米大师IAP支付SDK支付请求类
 @author bladebao
 */
@interface CTIIAPPayReq : CTIIAPBaseReq

/*!
 @property productId
 @abstract 物品ID
 @author bladebao
 */
@property (nonatomic, copy) NSString * productId;

/*!
 @property productType
 @abstract 商品类型，参见CTIIapProductType定义
 @author bladebao
 */
@property (nonatomic, assign) CTIIapProductType productType;

/*!
 @property saveValue
 @abstract 充值个数，当充值游戏币时，输入游戏币数量，包月时输入月数，月卡时输入天数，道具按照 道具ID*道具单价（角）*数量 的格式
 @author bladebao
 */
@property (nonatomic, copy) NSString * saveValue;

/*!
 @property drmInfo
 @abstract 营销活动：
     (1)支持IAP自动续费订阅优惠促销活动，group_id%3DMUTP19051019366yuan，group_id为活动ID，对应于苹果自动续费订阅优惠促销的折扣ID。
        注意，此功能只支持 iOS12.2 / macOS10.14.4 以上版本，要使用Xcode10.2进行编译。
        苹果优惠促销相关内容，参见：https://developer.apple.com/cn/app-store/subscriptions/#subscription-offers 
     (2)支持使用3.0版本的营销，version%3d3.0；
     (3)支持营销分组，currency_group%3Dxxx
     (4)支持号码包营销活动，如果是号码包营销活动，应在此字段传入经过url编码的drm_info=xxxx（号码包ID）；
 @author bladebao
 */
@property (nonatomic, copy) NSString * drmInfo;

/*!
 @property 抵扣券信息
 @abstract 抵扣券相关参数，传入格式为：URLEncode(card_id={batch_id}&batch_id={batch_id}&code_id={code_id}")
 @author bladebao
 */
@property (nonatomic, copy) NSString * couponInfo;

#if TARGET_OS_IOS
/*!
 @property resImg
 @abstract 表示充值物品（如包月、游戏币）的图片资源，建议分辨率为32x32，或者64x64（retina下）
 @author bladebao
 */
@property (nonatomic, strong) UIImage * resImg;

/*!
 @property resData 和resImg，优先使用resData
 @abstract 表示充值物品（如包月、游戏币）的图片资源，建议分辨率为32x32，或者64x64（retina下）
 @author bladebao
 */
@property (nonatomic, copy) NSString * resData;

/*!
 @property showCTIUI
 @abstract 米大师界面类型，默认为CTI_UI_DEFAULT
 @author bladebao
 */
@property (nonatomic, assign) CTIIAPUIType ctiUIType;
#endif

#if TARGET_OS_IPHONE
/*!
 @property contextController
 @abstract 用于展示界面上下文的视图控制器，不能为空
 @author bladebao
 */
@property (nonatomic, weak) UIViewController * contextController;
#endif

/**
 @property stopPayAfterPossibleReprovide
 @abstract 如果在支付前有补发，则补发后是否支付。默认值为NO，补发后继续支付。如果在单笔购买场景
          （如某些只能购买一次的场景）传入YES，则假如需要补发此物品，补发完成后，即不再进行支付
          （仅对同物品ID生效，如需要补发的物品是A，请求支付的物品是B，则补发后继续拉起新支付，
           无视此开关）
 @author bladebao
 */
@property (nonatomic, assign) BOOL stopPayAfterPossibleReprovide;

/**
 @property metadata
 @abstract 支付透传字段，支付后能够从回调接口获取
 @author seahubhuang
 */
@property (nonatomic, copy) NSString * metadata;

@end

#if TARGET_OS_IPHONE
/*!
 @class CTIIAPPromotedPayReq
 @superclass CTIIAPPayReq
 @abstract 米大师IAP支付SDK推介促销支付请求类
 @author seahubhuang
 */
@interface CTIIAPPromotedPayReq : CTIIAPPayReq

@end
#endif

/*!
 @class CTIIAPMpInfoReq
 @superclass CTIIAPBaseReq
 @abstract 米大师拉取特殊cgi请求
 @author bladebao
 */
@interface CTIIAPMpInfoReq : CTIIAPBaseReq

@end

/*!
 @class CTIIAPShortOpenidReq
 @superclass CTIIAPBaseReq
 @abstract 米大师拉取短码请求
 @author bladebao
 */
@interface CTIIAPShortOpenidReq : CTIIAPBaseReq

@end

/*!
 @class CTIIAPReprovideReq
 @superclass CTIIAPBaseReq
 @abstract 米大师补发货请求，sdk内部检测是否需要进行补发
 @author bladebao
 */
@interface CTIIAPReprovideReq : CTIIAPBaseReq

@end

/*!
 @class CTIIAPRestoreReq
 @superclass CTIIAPBaseReq
 @abstract 米大师恢复非消耗型物品请求
 @author bladebao
 */
@interface CTIIAPRestoreReq : CTIIAPBaseReq

#if TARGET_OS_IPHONE
/*!
 @property contextController
 @abstract 用于展示界面上下文的视图控制器，不能为空
 @author bladebao
 */
@property (nonatomic, weak) UIViewController * contextController;
#endif

#if TARGET_OS_IOS
/*!
 @property showCTIUI
 @abstract 米大师界面类型，默认为CTI_UI_DEFAULT
 @author bladebao
 */
@property (nonatomic, assign) CTIIAPUIType ctiUIType;
#endif

@end

/*!
 @class CTIIAPProductInfoReq
 @superclass CTIIAPBaseReq
 @abstract 米大师物品信息请求类，从苹果请求物品信息
 @author bladebao
 */
@interface CTIIAPProductInfoReq : CTIIAPBaseReq

/*!
 @property useCache
 @abstract 是否使用上次请求的缓存，如果是NO，则每次重新请求网络，否则采用上次的缓存，如果缓存不存在，也重新从网络侧取
 @author bladebao
 */
@property (nonatomic, assign) BOOL useCache;

/*!
 @property productIds
 @abstract 需要请求基本信息的物品ID列表
 @author bladebao
 */
@property (nonatomic, strong) NSMutableArray<NSString *> * productIds;

@end

/*!
 @class CTIIAPIntroPriceInfoReq
 @abstract 查询IAP自动续订类的低价促销信息，其中，返回的结果包括原价和促销价格（目前暂支持人民币），以及用户在此部手机登陆的AppleId内购买过的低价促销记录（物品ID+次数）
 @description 接入方需要根据此请求回调中的信息，判断用户界面展示给用户的价格信息。本API受苹果机制影响，有一定概率调起用户输入AppleID和密码进行授权，如果用户取消，则此接口调用失败。
 @author bladebao
 */
@interface CTIIAPIntroPriceInfoReq : CTIIAPBaseReq

/*!
 @property introPriceProuctIds
 @abstract 请求这些物品ID的原价和促销价格信息
 @author bladebao
 */
@property (nonatomic, strong) NSMutableArray <NSString *> * introPriceProuctIds;

/*!
 @property fromApple
 @abstract 是否从Apple请求物品价格信息，从Apple请求物品价格信息时耗较长，如无多币种展示需求，可以忽略此字段
 @description 默认为NO，如果为YES，则从米大师后台请求与从Apple请求是并发的，结果均在回调json中有体现，详情请参考接入文档
 @author bladebao
 */
@property (nonatomic, assign) BOOL fromApple;

@end

/// CTIIAPReceiptQueryType：查询类型
/// - CTIIAPReceiptQueryTypeAutoSubscriptionInfo：查询自动续费订阅相关信息（如发货号码）
/// - CTIIAPReceiptQueryTypePreorder：查询预订功能相关信息（如用户是否有进行 AppStore 预订）
typedef NS_OPTIONS(NSUInteger, CTIIAPReceiptQueryType) {
    CTIIAPReceiptQueryTypeAutoSubscriptionInfo = 1 << 0,
    CTIIAPReceiptQueryTypePreorder = 1 << 1,
};

/// 用于查询苹果票据相关信息
@interface CTIIAPReceiptInfoReq: CTIIAPBaseReq

/// 查询类型（非必传，不传默认为查询票据所有信息）
/// 注意：如果确定只需要某部分信息，一定要传入具体的类型，否则请求时耗很长
@property (nonatomic, assign) CTIIAPReceiptQueryType type;

@end

/*!
 @class CTIIAPPayResp
 @superclass NSObject
 @abstract 米大师支付SDK应答类
 @author bladebao
 */
@interface CTIIAPPayResp : NSObject

/*!
 @property resultCode
 @abstract 应答结果码，参见APCTIRespResultCode定义
 @author bladebao
 */
@property (nonatomic, assign) APCTIRespResultCode resultCode;

/*!
 @property resultInnerCode
 @abstract 内部错误码
 @author bladebao
 */
@property (nonatomic, copy) NSString * resultInnerCode;

/*!
 @property resultMsg
 @abstract 结果消息字符串
 @author bladebao
 */
@property (nonatomic, copy) NSString * resultMsg;

/*!
 @property respString
 @abstract CGI直接调用时，返回
 @author bladebao
 */
@property (nonatomic, copy) NSString * respString;

/*!
 @property extendJson
 @abstract 预留的扩展字段json，格式形如{key1:val1, key2: val2}
 @author bladebao
 */
@property (nonatomic, copy) NSString * extendJson;

/*!
 @property successProductIds
 @abstract 成功完成支付并发货的物品ID
 @author bladebao
 */
@property (nonatomic, copy) NSArray * successProductIds;

/*!
 @property failedProductIds
 @abstract 发货失败的物品ID
 @disscussion key是错误码，value是物品ID队列
              // 错误码解释
              // -1/10001/5002 发货失败，等待下次app启动补发货
              // 其他错误码 支付失败/发货失败，不补发
 @author bladebao
 */
@property (nonatomic, copy) NSDictionary<NSString *, NSArray *> * failedProductIds;

/*!
 @property metadata
 @abstract 支付透传字段
 @author seahubhuang
 */
@property (nonatomic, copy) NSString * metadata;

/*!
 @property req
 @abstract 支付请求对象
 @author tedyang
 */
@property (nonatomic, strong) CTIIAPBaseReq * req;

/*!
 @method
 @abstract 类方法，方便生成APCTIResp类实例
 @param retCode 同属性resultCode
 @param inCodeStr 同属性resultInnerCode
 @param retMsg 同属性resultMsg
 @param respString CGI直接返回的报文，在launchNet接口时使用
 @result 返回初始化后的APCTIResp类实例
 @author bladebao
 */
+ (instancetype)respWithResultCode:(APCTIRespResultCode)retCode
                         innerCode:(NSString *)inCodeStr
                         resultMsg:(NSString *)retMsg
                        respString:(NSString *)respString;

@end





