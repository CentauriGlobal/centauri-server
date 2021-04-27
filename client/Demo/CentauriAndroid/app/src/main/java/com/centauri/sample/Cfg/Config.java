package com.centauri.sample.Cfg;

import android.content.Context;

/**
 * Created by zachzeng on 2017/12/25.
 */

public class Config {

    public static String KEY_OPEN_ID = "KEY_OPEN_ID";
    public static String KEY_OFFER_ID = "KEY_OFFER_ID";
    public static String KEY_PRODUCT_ID = "KEY_PRODUCT_ID";
    public static String KEY_COUNTRY = "KEY_COUNTRY";
    public static String KEY_CURRENCY = "KEY_CURRENCY";

    public String openId = "midasoversea";
    //    public String offerid = "1450005285";
    public String offerid = "1450027732";

    public String mGameProductID = "test_product1";  //游戏币的productid
    public String mGoodsProductID = "test_product3"; //道具的productid
    public String mSubsProductID = "sub_test_2";

    public String amazonGameProductId = "gamecoins1";
    public String amazonGoodsProductId = "goodspackage";

    public String mServiceCode = "test";
    public String mServiceName = "test";

    public String mCountry = "US";
    public String mCurrency = "USD";

    private Context mContext;

    //是否使用特殊渠道配置
    public boolean isSpecialChannel = false;

    public Config(Context context) {
        mContext = context.getApplicationContext();
        refreshData();
    }

    public void refreshData() {
        offerid = SPTools.getString(mContext, KEY_OFFER_ID, "1450027732");
        openId = SPTools.getString(mContext, KEY_OPEN_ID, "centauri0001");
        mGameProductID = SPTools.getString(mContext, KEY_PRODUCT_ID, "test_product1");
        mCountry = SPTools.getString(mContext, KEY_COUNTRY, "");
        mCurrency = SPTools.getString(mContext, KEY_CURRENCY, "");
    }

    public void setOpenId(String openId) {
        this.openId = openId;
        SPTools.putString(mContext, KEY_OPEN_ID, openId);
    }

    public void setOfferid(String offerid) {
        this.offerid = offerid;
        SPTools.putString(mContext, KEY_OFFER_ID, offerid);
    }

    public void setmGameProductID(String mGameProductID) {
        this.mGameProductID = mGameProductID;
        SPTools.putString(mContext, KEY_PRODUCT_ID, mGameProductID);
    }

    public void setmSubsProductID(String mSubsProductID) {
        this.mSubsProductID = mSubsProductID;
    }

    public void setmServiceCode(String mServiceCode) {
        this.mServiceCode = mServiceCode;
    }

    public void setmServiceName(String mServiceName) {
        this.mServiceName = mServiceName;
    }

    public void setmCountry(String mCountry) {
        this.mCountry = mCountry;
        SPTools.putString(mContext, KEY_COUNTRY, mCountry);
    }

    public void setmCurrency(String mCurrency) {
        this.mCurrency = mCurrency;
        SPTools.putString(mContext, KEY_CURRENCY, mCurrency);
    }
}
