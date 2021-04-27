package com.centauri.sample;

import android.app.Activity;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.centauri.oversea.api.request.ICTIProductInfoCallback;
import com.centauri.sample.Cfg.DC;
import com.centauri.oversea.api.CTIPayAPI;
import com.centauri.oversea.api.ICTICallBack;
import com.centauri.oversea.api.ICTIPayUpdateCallBack;
import com.centauri.oversea.api.request.CTIBaseRequest;
import com.centauri.oversea.api.request.CTIGameRequest;
import com.centauri.oversea.api.request.CTIGoodsRequest;
import com.centauri.oversea.api.request.CTIMonthRequest;
import com.centauri.sample.View.MainActivity;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashMap;

/**
 * 业务可参考该类实现海外midas api接入配置
 *
 * @author zachzeng
 */
public class CentauriAPISample {

    /**
     * midas初始化
     *
     * @param activity 业务外部传入，midas不持有
     * @param callBack 初始化回调，
     */
    public static void MidasInit(Activity activity, ICTIPayUpdateCallBack callBack) {
        //1、打开日志开关
        CTIPayAPI.singleton().setLogEnable(true);
        //2、设置支付环境，"test"：沙箱，"release"：现网
        CTIPayAPI.singleton().setEnv(DC.instance().env);
        //3、设置idc，具体参考文档
        CTIPayAPI.singleton().setReleaseIDC(DC.instance().idc);
        //4、设置idcInfo，可选：外部可在代码中传入idc配置信息等
        if (!TextUtils.isEmpty(DC.instance().idcInfo)) {
            CTIPayAPI.singleton().setReleaseIDC(DC.instance().idc, DC.instance().idcInfo);
        }

        //5、设置初始化参数
        CTIBaseRequest request = new CTIGameRequest();
        initRequest(request);
        request.offerId = DC.instance().cfg.offerid;        //midas测配置offerId
        request.openId = DC.instance().cfg.openId;          //midas测配置openId
        request.openKey = "";

        //初始化
        CTIPayAPI.singleton().init(activity, request, callBack);
    }


    /**
     * 购买游戏币
     *
     * @param activity 业务外部传入，midas不持有
     * @param callBack 支付回调
     */
    public static void buyGameCoins(Activity activity, ICTICallBack callBack) {
        //1、创建CTIGameRequest实例
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        request.offerId = DC.instance().cfg.offerid;
        request.openId = DC.instance().cfg.openId;
        request.mpInfo.payChannel = "gwallet";                           //支付渠道名
        request.mpInfo.productid = DC.instance().cfg.mGameProductID;    //midas测配置物品id
        request.country = DC.instance().cfg.mCountry;            //midas测对应的物品国家代码
        request.currency_type = DC.instance().cfg.mCurrency;    //midas测对应的物品币种代码

        CTIPayAPI.singleton().pay(activity, request, callBack);
    }


    /**
     * 购买包月（非自动续费）
     *
     * @param activity 业务外部传入，midas不持有
     * @param callBack 支付回调
     */
    public static void buyMonth(Activity activity, ICTICallBack callBack) {
        CTIGoodsRequest request = new CTIGoodsRequest();
        initRequest(request);
        request.offerId = "1450005285";
        request.openId = DC.instance().cfg.openId;
        request.currency_type = "CNY";
        request.country = "CN";
//        request.mType = "month";            //注意：包月mType必须传"mType"
        request.mpInfo.payChannel = "gwallet";

        try {
            request.reserv = "a=b&c=d&app_metadata=" + URLEncoder.encode("aaab=bbcc", "utf8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        request.mpInfo.productid = "midas_product_2";
        CTIPayAPI.singleton().pay(activity, request, callBack);
    }


    /**
     * 购买订阅
     * 订阅对应midas测物品类型：1、自动续费包月，2、自动续费月卡
     *
     * @param activity 业务外部传入，midas不持有
     * @param callBack 支付回调
     */
    public static void buySub(Activity activity, ICTICallBack callBack) {
        CTIMonthRequest request = new CTIMonthRequest();
        initRequest(request);
        request.offerId = DC.instance().cfg.offerid;
        request.openId = DC.instance().cfg.openId;
        request.mpInfo.productid = DC.instance().cfg.mGameProductID;    //midas测配置物品id
        request.country = DC.instance().cfg.mCountry;            //midas测对应的物品国家代码
        request.currency_type = DC.instance().cfg.mCurrency;    //midas测对应的物品币种代码

        request.mpInfo.payChannel = "gwallet";
        request.mType = "month";                //注意：自动续费包月，mType必须传"mType"
        request.autoPay = true;                //自动续费：设为true
        request.serviceCode = "TXSPTL";       //midas测配置的业务代码
        CTIPayAPI.singleton().pay(activity, request, callBack);
    }


    //可配置默认参数
    public static void initRequest(CTIBaseRequest request) {
        request.openKey = "openKey";
        request.sessionId = "hy_gameid";
        request.sessionType = "st_dummy";
        request.zoneId = "1";
        request.goodsZoneId = "1001";
        request.pf = "huyu_m-2001-android-2001";
        request.pfKey = "DSSSSSSSSSSS";
    }

    //可配置默认参数
    public static void initRequest(CTIBaseRequest request, String zoneId) {
        request.openKey = "openKey";
        request.sessionId = "hy_gameid";
        request.sessionType = "st_dummy";
        request.zoneId = zoneId;
        request.goodsZoneId = "1001";
        request.pf = "huyu_m-2001-android-2001";
        request.pfKey = "DSSSSSSSSSSS";
    }

    //订阅物品
    public static void subClick(Activity activity, ICTICallBack callBack) {
        CTIMonthRequest request = new CTIMonthRequest();
        initAllRequest(request);
        request.autoPay = true;
        request.mpInfo.payChannel = "gwallet";
        request.serviceCode = DC.instance().cfg.mServiceCode;
        request.serviceName = DC.instance().cfg.mServiceName;
        request.mpInfo.productid = DC.instance().cfg.mSubsProductID;
        //把上次开通成功的参数保存下来
        MainActivity.OldSku = request.mpInfo.productid;
        CTIPayAPI.singleton().pay(activity, request, callBack);
    }

    public static void subUpgradeClick(Activity activity, ICTICallBack callBack) {
        CTIMonthRequest request = new CTIMonthRequest();
        initAllRequest(request);
        request.autoPay = true;
        request.mpInfo.payChannel = "gwallet";
        request.serviceCode = DC.instance().cfg.mServiceCode;
        request.serviceName = DC.instance().cfg.mServiceName;
        request.mpInfo.productid = DC.instance().cfg.mSubsProductID;

        //升级传递的协议
        StringBuilder sb = new StringBuilder();
        sb.append("OldSku=");
        sb.append(MainActivity.OldSku);
        sb.append("&");
        sb.append("SubcribeProrationMode=4");

        request.extras = sb.toString();
        CTIPayAPI.singleton().pay(activity, request, callBack);
    }

    public static void queryGWInfo(ICTIProductInfoCallback callback) {
        HashMap<String, String> map = new HashMap<String, String>();
        map.put(DC.instance().cfg.mGameProductID, "inapp");
        CTIPayAPI.singleton().getProductInfo("gwallet", map, callback);
    }

    public static void reProvide(ICTIPayUpdateCallBack callBack) {
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        CTIPayAPI.singleton().reProvide(callBack);
    }

    private static void initAllRequest(CTIBaseRequest request) {
        request.offerId = DC.instance().cfg.offerid;
        request.openId = DC.instance().cfg.openId;
        request.openKey = "openKey";
        request.sessionId = "hy_gameid";
        request.sessionType = "st_dummy";
        request.zoneId = "1";
        request.goodsZoneId = "1001";
        request.pf = "huyu_m-2001-android-2001";
        request.pfKey = "DSSSSSSSSSSS";
        request.country = DC.instance().cfg.mCountry;
        request.currency_type = DC.instance().cfg.mCurrency;
    }
}
