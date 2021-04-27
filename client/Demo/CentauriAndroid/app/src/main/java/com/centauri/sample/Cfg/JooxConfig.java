package com.centauri.sample.Cfg;

import android.content.Context;

/**
 * Created by zachzeng on 2017/12/25.
 */

public class JooxConfig extends Config {
    public JooxConfig(Context context){
        super(context);
        offerid = "1450001031";

        mGameProductID = "hk.coin.1000";  //游戏币的productid
        mGoodsProductID = "midas_product_2"; //道具的productid
        mSubsProductID = " joox_doku_android_1_week_id";

        mServiceCode = "wemusic_vip_1";
        mServiceName = "wemusic_vip_1";

        mCountry = "HK";
        mCurrency= "HKD";
    }
}
