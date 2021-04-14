#if PLATFORM_ANDROID || PLATFORM_IOS
#ifndef __CCX_CTI_PAYCALLBACKENTITIES_H__
#define __CCX_CTI_PAYCALLBACKENTITIES_H__

#include <map>
#include <string>
#include <vector>

namespace CentauriPay
{
#if defined(__cplusplus)
    extern "C" {
#endif
    extern std::string GET_INFO_TYPE_MP;
    extern std::string GET_INFO_TYPE_SHORT_OPENID;
#if defined(__cplusplus)
    }
#endif


    struct CTIInitProduct {
        ///<summary>
        ///物品ID。
        ///</summary>
        std::string productid;

        ///<summary>
        ///米大师订单号。
        ///</summary>
        std::string billno;

        ///<summary>
        ///google/iap原始的订单信息。
        ///</summary>
        std::string originalJson;

        CTIInitProduct();
    };


    struct CTIInitResult {
        ///<summary>
        ///支付渠道的key。
        ///</summary>
        std::string paychannelid;

        ///<summary>
        ///内部错误码，业务不用关注。
        ///</summary>
        std::string resultInerCode;

        ///<summary>
        ///补发的物品信息列表。
        ///</summary>
        std::vector<CentauriPay::CTIInitProduct> mInitList;

        CTIInitResult();

    };



    struct CTIGetProduct {
        ///<summary>
        ///google/iap物品编号。
        ///</summary>
        std::string productid;

        ///<summary>
        ///google/iap物品的价格，此价格已经格式化，业务可以用来展示。
        ///</summary>
        std::string price;

        ///<summary>
        ///google/iap物品的货币类型，是用户真实支付的货币类型。
        ///</summary>
        std::string currency;

        ///<summary>
        ///google/iap物品的价格。是以元为单位的价格乘了100w。
        ///</summary>
        long microprice;

        CTIGetProduct();
    };


    struct CTIGetProductResult {
        ///<summary>
        ///返回值。成功为0。
        ///</summary>
        std::string ret;

        ///<summary>
        ///返回错误信息。
        ///</summary>
        std::string msg;

        ///<summary>
        ///获取的物品的列表。
        ///</summary>
        std::vector<CentauriPay::CTIGetProduct> mProductList;

        CTIGetProductResult();
    };
}


#endif
#endif
