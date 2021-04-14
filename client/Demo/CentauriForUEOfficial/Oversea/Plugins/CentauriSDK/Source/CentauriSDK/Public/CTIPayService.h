#if PLATFORM_ANDROID || PLATFORM_IOS
#ifndef __CCX_CTIPAYSERVICE_H__
#define __CCX_CTIPAYSERVICE_H__

#include <map>
#include <string>

using namespace std;

namespace CentauriPay
{
#if defined(__cplusplus)
extern "C"
{
#endif
    extern std::string GET_INFO_TYPE_MP;
    extern std::string GET_INFO_TYPE_SHORT_OPENID;

    extern std::string CTI_WCT;
    extern std::string BANK;
    extern std::string CTI_QW;
    extern std::string GOOGLEPLAY; //
    extern std::string APPLEIAP;   //
#if defined(__cplusplus)
}
#endif

/**
 *
 */
class CTIBaseRequest
{
  public:
    ///<summary>
    ///应用编号。
    ///</summary>
    std::string offerId;

    ///<summary>
    ///用户ID。
    ///</summary>
    std::string openId;

    ///<summary>
    ///可以固定为"openKey"。
    ///</summary>
    std::string openKey = "openKey";

    ///<summary>
    ///货币分区ID，默认1。如果支持角色，需要用"_"拼接。
    ///</summary>
    std::string zoneId;

    ///<summary>
    ///可以固定为"hy_gameid"。
    ///</summary>
    std::string sessionId="hy_gameid";

    ///<summary>
    ///可以固定为"st_dummy"。
    ///</summary>
    std::string sessionType="st_dummy";

    ///<summary>
    ///pf。可以通过iMSDK的接口来获取。
    ///</summary>
    std::string pf;

    ///<summary>
    ///可以固定为"pfKey"。
    ///</summary>
    std::string pfKey="pfKey";

    ///<summary>
    ///游戏的partionid，没有则可和zoneId保持一致，在游戏侧定义，主要用于道具的发放。
    ///</summary>
    std::string goodsZoneId;

    ///<summary>
    ///指定物品编号。
    ///</summary>
    std::string productId;

    ///<summary>
    ///only for iOS：Goods，“productId*price(unit：jiao)*quantity”；Subscribe，“days”；Game，“quantity”
    /// [required]
    ///</summary>
    std::string payItem;

    ///<summary>
    /// buy quantity. if the value is null or "", it will show centauri mall page.
    /// [optional]
    ///</summary>
    std::string saveValue;

    ///<summary>
    ///app透传字段，最终会回传给app。
    ///</summary>
    std::string appExtends;

    ///<summary>
    ///一些支付渠道需要的特殊字段，比如roleid，rolename等。
    /// 注：走centauri IAP时必须传值：req.channelExtras ＝ “payItem＝xxx”  //xxx的值：道具，传“物品ID*单价(单位：角)*数量”；月卡，传“天数”；游戏币，传“数量”
    ///</summary>
    std::string channelExtras;

    ///<summary>
    ///指定的货币类型。米大师后台会根据货币类型来过滤物品。
    ///</summary>
    std::string currencyType;

    ///<summary>
    ///指定的国家。米大师后台会根据国家来过滤支付渠道。
    ///</summary>
    std::string country;

    ///<summary>
    ///指定支付渠道。
    ///</summary>
    std::string payChannel;

    ///<summary>
    ///account type。
    /// [optional]
    ///</summary>
    //[JsonProp("acctType")]
    std::string acctType = "common";

    ///<summary>
    ///类名，用来区分具体业务类型
    ///</summary>
    std::string className;

    ///<summary>
    ///extras for custom configuration。The format of this field is json。
    //{
    //  "uiconfig":
    //  {
    //     "unit":"ge",
    //     "isShowNum":false,
    //     "isShowListOtherNum":true,
    //     "isCanChange":true,
    //     "extras":"",
    //     "resData":"",
    //     "resId":0,
    //     "mallLogo":0
    //  },
    //  "drmConfig":
    //  {
    //     "discountType":"",
    //     "discountUrl":"",
    //     "discoutId":""
    //     "drmInfo":""
    //  }
    //  "others"
    //  {
    //      "mallType":"",
    //      "h5Url":""
    //  }
    //}
    ///
    ///
    /// [optional]
    ///</summary>
    std::string extras;

    CTIBaseRequest();
    virtual ~CTIBaseRequest();
};

//    class APCentauriInitRequest : public CTIBaseRequest {
//
//    };

//    class APCentauriBasePayRequest : public CTIBaseRequest {
//    public:
//        ///<summary>
//        ///类名，用来区分具体业务类型
//        ///</summary>
//        std::string className;
//
//        ///<summary>
//        ///米大师商城页的货币logo。可选字段，Android和IOS中的内容会不同。
//        ///</summary>
//        std::string resId;
//
//        ///<summary>
//        ///指定的货币类型。米大师后台会根据货币类型来过滤物品。
//        ///</summary>
//        std::string currencyType;
//
//        ///<summary>
//        ///指定的国家。米大师后台会根据国家来过滤支付渠道。
//        ///</summary>
//        std::string country;
//
//        ///<summary>
//        ///营销活动扩展信息，目前支持指定营销活动分组。
//        ///</summary>
//        std::string drmInfo;
//
//        ///<summary>
//        ///指定支付渠道。
//        ///</summary>
//        std::string payChannel;
//
//        ///<summary>
//        ///指定物品编号。
//        ///</summary>
//        std::string productId;
//
//    };

/**
 *
 */
class CTIGameRequest : public CTIBaseRequest
{
  public:
    CTIGameRequest();
    virtual ~CTIGameRequest();
};

/**
 *
 */
class CTIGoodsRequest : public CTIBaseRequest
{
  public:
    enum TOKEN_TYPE
    {
        GETTOKENTYPE_SERVER = 1, //make order from game server。note：this is not support for iOS
        GETTOKENTYPE_SDK = 2,    //make order in centauri sdk
        GETTOKENTYPE_CLIENT = 3  //make order in game client
    };
    ///<summary>
    ///order token type，set value with TOKEN_TYPE from the above。
    /// [required]
    ///</summary>
    int tokenType = -1;

    ///<summary>
    ///order token from the server,and set tokenType = GETTOKENTYPE_SERVER。
    /// [required]
    ///</summary>
    std::string goodsTokenUrl;

    ///<summary>
    ///game client provide diamonds
    /// [optional]
    ///</summary>
    std::string developerPayload;

    ///<summary>
    ///game client provide diamonds
    /// [optional]
    ///</summary>
    bool isReceiptMode = false;
    CTIGoodsRequest();
    virtual ~CTIGoodsRequest();
};

/**
 *
 */
class CTIMonthRequest : public CTIBaseRequest
{
  public:
    ///<summary>
    ///service code for month bussiness
    /// [required]
    ///</summary>
    std::string serviceCode = "";

    ///<summary>
    ///service name for month bussiness
    /// [required]
    ///</summary>
    std::string serviceName = "";

    ///<summary>
    ///service type for month bussiness, see SERVICE_TYPE
    /// [optional]
    ///</summary>
    int serviceType = 1;

    ///<summary>
    /// is enable auto continue pay.
    /// note:For iOS, it's not allowed to buy a month service, if you wan to use this for subscribe, please contact with centauri
    /// [optional]
    ///</summary>
    bool autoPay = false;

    enum SERVICE_TYPE
    {
        SERVICETYPE_NORMAL = 1, //开通包月
        SERVICETYPE_RENEW = 2,  //续费
        SERVICETYPE_UPGRADE = 3 //升级
    };
    CTIMonthRequest();
    virtual ~CTIMonthRequest();
};

/**
 *
 */
class CTISubscribeRequest : public CTIMonthRequest
{
  public:
    //        CTISubscribeRequest();
    //        virtual ~CTISubscribeRequest();
    //
    //        ///<summary>
    //        ///月卡serviceCode。
    //        ///</summary>
    //        std::string serviceCode;
    //
    //        ///<summary>
    //        ///月卡serviceName。
    //        ///</summary>
    //        std::string serviceName;
    //
    //        ///<summary>
    //        ///auto pay。
    //        ///</summary>
    //        bool autoPay;
    CTISubscribeRequest();
    virtual ~CTISubscribeRequest();
};

/**
 *
 */
struct CTIResponse
{

    //        typedef enum _APCentauriResultCode
    //        {
    //            AP_CTI_RESP_RESULT_ERROR              = -1, // 失败
    //            AP_CTI_RESP_RESULT_OK                 = 0,  // 成功
    //            AP_CTI_RESP_RESULT_CANCEL             = 1,  // 取消
    //            AP_CTI_RESP_RESULT_PARAM_ERROR        = 2,  // 参数错误
    //            AP_CTI_RESP_RESULT_NET_ERROR          = 3,  // 网络错误
    //            AP_CTI_RESP_RESULT_PROVIDE_ERROR      = 4,  // 发货错误
    //            AP_CTI_RESP_RESULT_IN_PROCESSSING     = 5,  // 正在补发中
    //            AP_CTI_RESP_NO_REPROVIDE_PRODUCT      = 6,  // 没有要补发的物品
    //        } APCentauriResultCode;

    ///<summary>
    ///结果码。
    ///</summary>
    int resultCode;

    ///<summary>
    ///支付内部错误码，用于定位问题用。
    ///</summary>
    std::string resultInerCode;

    ///<summary>
    ///game coin quantity
    ///</summary>
    std::string realSaveNum;

    ///<summary>
    ///支付渠道标示。目前保留未启用。
    ///</summary>
    std::string payChannel;

    ///<summary>
    ///支付错误信息
    ///</summary>
    std::string resultMsg;

    ///<summary>
    ///app透传信息。在支付接口中指定，此处回传。
    ///</summary>
    std::string appExtends;

    ///<summary>
    ///business type, game or goods or month or subscribe
    ///</summary>
    std::string reqType;

    ///<summary>
    /// Reserved field
    ///</summary>
    std::string payReserve1;

    ///<summary>
    /// Reserved field
    ///</summary>
    std::string payReserve2;

    ///<summary>
    /// Reserved field
    ///</summary>
    std::string payReserve3;
};

class CTIPayCallback
{
  public:
    /**
     *
     */
    virtual void OnCentauriLoginExpired() = 0;
    /**
     *
     * @param result
     */
    virtual void OnCentauriPayFinished(const CTIResponse &result) = 0;

  protected:
    CTIPayCallback(){};
    virtual ~CTIPayCallback(){};
};

///<summary>
///初始化的回调。如果涉及到补发货，会通过此回调通知app补发货的信息
///</summary>

/**
 *
 */
class CTIInitCallback
{
  public:
    /**
     *
     * @param result
     */
    virtual void OnCentauriInitFinished(const std::string result) = 0;

  protected:
    CTIInitCallback(){};
    virtual ~CTIInitCallback(){};
};
///<summary>
///查询是否有补发货的物品；例如兑换码兑换的物品以及发货失败的物品
///</summary>

/**
 *
 */
class CTIReprovideCallback
{
  public:
    /**
     *
     * @param result
     */
    virtual void OnCentauriReprovideFinished(const std::string result) = 0;

  protected:
    CTIReprovideCallback(){};
    virtual ~CTIReprovideCallback(){};
};


/**
 *Get the product information callback
 */
class CTIGetLocalPriceCallback
{
  public:
    /**
     *
     * @param result
     */
    virtual void OnCentauriGetLocalPrice(const std::string result) = 0;

  protected:
    CTIGetLocalPriceCallback(){};
    virtual ~CTIGetLocalPriceCallback(){};
};
    
///<summary>
///从苹果获取推介优惠物品及资格信息
///</summary>

/**
 *Get the promotion product Information callback
 */
class CTIGetIntroPriceCallback
{
public:
    /**
     *
     * @param result
     */
    virtual void OnCentauriGetIntroPrice(const std::string result) = 0;
    
protected:
    CTIGetIntroPriceCallback(){};
    virtual ~CTIGetIntroPriceCallback(){};
};

///<summary>
///GetInfo的回调。type为“mp”时，从米大师拉取营销活动信息；"get_short_openid"时，拉取短码
///</summary>

/**
 *Get market activies callback
 */
class CTIGetInfoCallback
{
  public:
    /**
     *
     * @param result
     */
    virtual void OnCentauriGetInfoFinished(const std::string result) = 0;

  protected:
    CTIGetInfoCallback(){};
    virtual ~CTIGetInfoCallback(){};
};

class CTIPayService
{
  public:
    /**
     * 构造函数
     */
    CTIPayService();
    /*
     * 析构函数
     */
    virtual ~CTIPayService();

    /**
     *
     * @return
     */
    static CTIPayService *getInstance();
    

    /**
     *is enable log, sugest to set false when after fully tested.
     * @param enable
     */
    void SetLogEnable(bool enable);

    /**
     *
     * @param context
     * @param idc  local：国内       singapore：新加坡      guigu：美国
     * @param env param env:test: in sandbox environment; release: in payment release environment;Please use release environment only after fully tested in test environment.
     * @param idcInfo   When idcInfo is passed in, the payment service will use idcInfo's domain name and ip to request connection
     * @param req  param req:Request payment base object
     * @param callback
     */
    void Initialize(const void *context, std::string idc, std::string env, std::string idcInfo, const CTIBaseRequest *req, const CTIInitCallback *callback);


    /**
     *param req:A specific request payment object, such as CTIGameRequest, CTIGoodsRequest, CTIMonthRequest or CTISubscribeRequest
     * @param context
     * @param req   the request of pay
     * @param callback
     */
    void Pay(const void *context, const CTIBaseRequest *req, const CTIPayCallback *callback);


    /**
     *is payment allowed, this just valid for ios ,android is always return true
     * @return
     */
    bool IsPayEnable();


    /**
     *Get market info and productInfo from centauri server:
     * @param context
     * @param type     param reqType:now is only support "mp"
     * @param req   A specific request payment object, such as CTIGameRequest, CTIGoodsRequest, CTIMonthRequest or CTISubscribeRequest
     * @param callback param callback: the callback of getInfo
     */
    void GetInfo(const void *context, std::string type, const CTIBaseRequest *req, const CTIGetInfoCallback *callback);

    ///*************************************       optional interfaces below       ********************************************************************************///


    /**
     *Get the version of CentauriSDK
     * @param context
     * @return
     */
    std::string GetCentauriSDKVersion(const void *context);

    /**
     *only for android
     * @param context
     * @param req
     * @param callback
     */
    void LaunchWeb(const void *context, const CTIBaseRequest *req, const CTIPayCallback *callback);

    /**
     *only for android domestic
     * @param path
     */
    void SetPath(std::string path);

    /**
     * only for android
     * @param s
     */
    void CouponsRollBack(std::string s);

  
    /**
     *Get the product information
     *only for iap and googlewallet
     * @param context
     * @param channel      the channel of pay
     * @param productList  List of product information to be obtained：If it is empty, get all products information
     * @param callback
     */
    void GetLocalPrice(const void *context, std::string channel, map<string, string> &productList, const CTIGetLocalPriceCallback *callback);
    
    /**
     *
     *Get the  Promotion Product Information
     *only for iap and googlewallet
     * @param context
     * @param channel   the channel of pay
     * @param productList  List of item information to be obtained：If it is empty, get all products information
     * @param callback
     */
    void GetIntroPrice(const void *context, std::string channel, map<string, string> &productList, const CTIGetIntroPriceCallback *callback);
    

    /**
     *check if it have undeliveried orders and do reprovide
     * @param context
     * @param callback you can get the callback only when it have undeliveried orders
     */
    void Reprovide(const void *context, const CTIReprovideCallback *callback);


    /**
     *Called before project initialization
     * Only for iOS
     * @param processName  param procesName:sea/local, oversea project default value is 'sea';
     */
    void SetProcess(std::string processName);

  private:
    /**
     *
     */
    static CTIPayService *s_pCTIPayService;
};
} // namespace CentauriPay

#endif
#endif
