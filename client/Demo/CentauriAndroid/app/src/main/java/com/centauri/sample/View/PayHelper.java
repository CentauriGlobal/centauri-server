package com.centauri.sample.View;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.support.design.widget.Snackbar;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.centauri.sample.Cfg.DC;
import com.centauri.sample.CentauriAPISample;
import com.tencent.imsdk.samples.R;
import com.centauri.comm.CTILog;
import com.centauri.http.centaurihttp.CTIHttpAns;
import com.centauri.oversea.api.CTIPayAPI;
import com.centauri.oversea.api.ICTINetCallBack;
import com.centauri.oversea.api.ICTICallBack;
import com.centauri.oversea.api.ICTIPayUpdateCallBack;
import com.centauri.oversea.api.request.CTIBaseRequest;
import com.centauri.oversea.api.request.CTIGameRequest;
import com.centauri.oversea.api.request.CTIMonthRequest;
import com.centauri.oversea.api.request.ICTIProductInfoCallback;
import com.centauri.oversea.business.pay.CTIResponse;
import com.centauri.oversea.comm.CTIBase64;
import com.centauri.oversea.comm.MConstants;
import com.centauri.oversea.newapi.CTIPayNewAPI;
import com.centauri.oversea.newapi.params.MallParams;
import com.centauri.oversea.newnetwork.http.NetworkManager;
import com.centauri.oversea.newnetwork.service.CTIIPDetectTools;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.List;

/**
 * Created by zachzeng on 2017/11/1.
 */

public class PayHelper
        implements View.OnClickListener,
        ICTICallBack,ICTINetCallBack,ICTIPayUpdateCallBack,ICTIProductInfoCallback{
    public static final String TAG = "PayHelper";
    private Activity mActivity;



    public PayHelper(Activity activity) {
        mActivity = activity;
    }

    /**
     * 米大师外部接口
     */
    public void MidasAPI(View base){
        View view = LayoutInflater.from(mActivity).inflate(R.layout.layout_midas_api, null);
        TextView net = (TextView) view.findViewById(R.id.midas_net);
        TextView reProvide = view.findViewById(R.id.midas_reprovide);
        TextView getProductInfo = view.findViewById(R.id.midas_getProductInfo);
        TextView getIntroPriceInfo = view.findViewById(R.id.midas_getIntroPriceInfo);
        TextView deInit = view.findViewById(R.id.midas_deinit);
        TextView showMidasUI = (TextView)view.findViewById(R.id.show_midas_ui);

        net.setOnClickListener(this);
        reProvide.setOnClickListener(this);
        getProductInfo.setOnClickListener(this);
        getIntroPriceInfo.setOnClickListener(this);
        deInit.setOnClickListener(this);
        showMidasUI.setOnClickListener(this);

        BottomWM.instance().setHeight(400).build(mActivity).show(base, view);
    }


    /**
     * google play支付
     */
    public void GWPay(View base) {
        GoogleBillingActivity.startActivity(mActivity);
//        View view = LayoutInflater.from(mActivity).inflate(R.layout.layout_gwpay, null);
//        TextView buy = view.findViewById(R.id.buy_bg);
//        TextView game = (TextView)view.findViewById(R.id.buy_game);
//        TextView sub = view.findViewById(R.id.sub);
//        TextView sub_upgrade = view.findViewById(R.id.sub_upgrade);
//        TextView query = view.findViewById(R.id.query);
//        TextView reProvide = view.findViewById(R.id.reprovide);
//        TextView consume = view.findViewById(R.id.force_consume);
//        game.setOnClickListener(this);
//        buy.setOnClickListener(this);
//        sub.setOnClickListener(this);
//        sub_upgrade.setOnClickListener(this);
//        query.setOnClickListener(this);
//        reProvide.setOnClickListener(this);
//        consume.setOnClickListener(this);
//
//        BottomWM.instance().setHeight(400).build(mActivity).show(base, view);
    }

    /**
     * amazon
     */

    public void AmazonPay(View base) {
        View view = LayoutInflater.from(mActivity).inflate(R.layout.layout_amzon, null);
        TextView buy = view.findViewById(R.id.amazon_buy_bg);
        TextView game = (TextView)view.findViewById(R.id.amazon_buy_game);
        TextView sub = view.findViewById(R.id.amazon_sub);
        TextView query = view.findViewById(R.id.amazon_query);
        TextView reProvide = view.findViewById(R.id.amazon_reprovide);
        TextView consume = view.findViewById(R.id.amazon_force_consume);
        game.setOnClickListener(this);
        buy.setOnClickListener(this);
        sub.setOnClickListener(this);
        query.setOnClickListener(this);
        reProvide.setOnClickListener(this);
        consume.setOnClickListener(this);

        BottomWM.instance().setHeight(400).build(mActivity).show(base, view);
    }

    /**
     * 网络探测工具
     */
    public void NetTools(View base) {
        View view = LayoutInflater.from(mActivity).inflate(R.layout.layout_nettools, null);
        ((TextView)view.findViewById(R.id.ping_btn)).setOnClickListener(this);
        ((TextView)view.findViewById(R.id.getTask_btn)).setOnClickListener(this);
        ((TextView)view.findViewById(R.id.dns_btn)).setOnClickListener(this);
        ((TextView)view.findViewById(R.id.dns_parse_btn)).setOnClickListener(this);
        ((TextView)view.findViewById(R.id.socket_btn)).setOnClickListener(this);
        BottomWM.instance().setHeight(400).build(mActivity).show(base, view);
    }

    /**
     * 调用Net接口
     */
    private void callNet(){
        String reqType = "mp";
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        CTIPayAPI.singleton().net(reqType,this);
    }


    /**
     * 调用reProvide接口
     */
    private void callReProvide(){
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        CTIPayAPI.singleton().reProvide(this);
    }


    /**
     * 调用getProductInfo接口
     */
    private void callGetProductInfo(){
        HashMap<String, String> skuList = new HashMap<String, String>();
        skuList.put(DC.instance().cfg.mGoodsProductID, "inapp");
        skuList.put("midas_product_1", "inapp");
        skuList.put("midas_product_1", "inapp");
        skuList.put("midas_product_3", "inapp");
        skuList.put("testsub1", "subs");
        skuList.put("testsub2", "subs");
        skuList.put("testsub3", "subs");
        skuList.put("testsub444", "subs");
        CTIPayAPI.singleton().getProductInfo("gwallet",skuList,this);
    }

    /**
     * 查物品折扣信息
     */
    private void callGetIntroPriceInfo(){
        HashMap<String, String> skuList = new HashMap<String, String>();
//        skuList.add("midas_product_1");
//        skuList.add("midas_product_2");
//        skuList.add("testsub1");
//        skuList.add("freetrail1");
//        skuList.add("testwithdiffprice");
        skuList.put("testwithdiffprice1", "inapp");
        skuList.put("testwithdiffprice2", "inapp");

        CTIPayAPI.singleton().getIntroPriceInfo("gwallet", skuList, new ICTIProductInfoCallback() {
            @Override
            public void onProductInfoResp(String resp) {
                CTILog.d("PayHelper","getIntroPriceInfo response: "+resp);
                //测试代码
                try{
                    JSONObject js = new JSONObject(resp);
                    JSONArray jInfo = js.getJSONArray("gwallet_productInfo");
                    for(int i=0;i<jInfo.length();i++){
                        JSONObject jItem = jInfo.getJSONObject(i);
                        String productId = jItem.getString("productId");
                        String price = jItem.getString("price");
                        String currency = jItem.getString("currency");
                        long microprice = jItem.getLong("microprice");
                        String introPrice = jItem.getString("introPrice");
                        long introMicroPrice = jItem.getLong("introMicroPrice");
                        String introPeriod = jItem.getString("introPeriod");
                        String introCycles = jItem.getString("introCycles");

                        CTILog.d(TAG,productId+"|"+price+"|"+currency+"|"+microprice+"|"+introPrice+"|"+introMicroPrice+"|"+introPeriod+"|"+introCycles);
                    }
                }catch (JSONException e){
                    CTILog.e("PayHelper","getIntroPriceInfo res error.");
                }
            }
        });
    }

    /**
     * 调用deinit接口
     */
    private void callDeinit(View view){
        CTIPayAPI.singleton().deinit();
        Snackbar snackbar = Snackbar.make(view,"已注销，请重新登录使用！",Snackbar.LENGTH_LONG);
        View view1 = snackbar.getView();
        if(view1 != null){
            view1.setBackgroundColor(mActivity.getResources().getColor(R.color.colorPrimary));
        }
        snackbar.show();
    }


    /**
     * 调用showMidasUI接口
     */
    public void callShowMidasUI(View view){
        final String[] pList = {
                "closeLoading", "closeResult", "closeAll"
        };
        AlertDialog dialog = new AlertDialog.Builder(mActivity)
                .setSingleChoiceItems(pList, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        switch (which){
                            case 0:
                                CTIPayAPI.singleton().showCentauriUI(CTIPayAPI.CLOSE_LOADING);
                                break;
                            case 1:
                                CTIPayAPI.singleton().showCentauriUI(CTIPayAPI.CLOSE_RESULT);
                                break;
                            case 2:
                                CTIPayAPI.singleton().showCentauriUI(CTIPayAPI.CLOSE_ALL);
                                break;
                        }

                        BottomWM.instance().close();
                    }
                })
                .setCancelable(true)
                .create();

        dialog.setCanceledOnTouchOutside(true);
        dialog.show();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            //米大师外部接口
            case R.id.midas_net:
                callNet();
                break;
            case R.id.midas_reprovide:
                callReProvide();
                break;
            case R.id.midas_getProductInfo:
                callGetProductInfo();
                break;
            case R.id.midas_getIntroPriceInfo:
                callGetIntroPriceInfo();
                break;
            case R.id.midas_deinit:
                callDeinit(view);
                break;
            case R.id.show_midas_ui:
                callShowMidasUI(view);
                break;

            //google play
            case R.id.buy_game:
                Log.i(TAG, "buy");
                CentauriAPISample.buyGameCoins(mActivity,this);
                break;
            case R.id.buy_bg:
                Log.i(TAG, "buy");
                CentauriAPISample.buyMonth(mActivity,this);
                break;
            case R.id.sub:  //订阅
                Log.i(TAG, "sub");
                subClick();
                break;
            case R.id.sub_upgrade:  //订阅升级
                Log.i(TAG, "subUpgrade");
                subUpgradeClick();
                break;
            case R.id.query:    //查询
                Log.i(TAG, "query");
                queryGWInfo();
                break;
            case R.id.reprovide:    //补发货
                Log.i(TAG, "reprovide");
                reProvide();
                break;
            case R.id.getTask_btn:
                Log.i(TAG, "getTask_btn");
                NetworkManager.singleton().detectTaskQuery(new com.centauri.http.centaurihttp.ICTIHttpCallback() {
                    @Override
                    public void onSuccess(CTIHttpAns ctiHttpAns) {
                        Log.i(TAG, "getTask_btn success");
                    }

                    @Override
                    public void onFailure(CTIHttpAns ctiHttpAns) {
                        Log.i(TAG, "getTask_btn onFailure");
                    }

                    @Override
                    public void onStop(CTIHttpAns ctiHttpAns) {
                        Log.i(TAG, "getTask_btn onStop");
                    }
                });
                break;
            case R.id.force_consume:    //强制消耗
                Log.i(TAG, "force_consume");

//                if (mGWHelper != null) {
//                    mGWHelper.consume();
//                }
                break;
            case R.id.ping_btn:
                CTIIPDetectTools.ping("203.195.160.159", 0, 4,2);
                break;
            case R.id.dns_btn:
                List<String> dns = CTIIPDetectTools.getDnsServer(CTIPayNewAPI.singleton().getApplicationContext());
                CTILog.d(this.getClass().getName(), "dns Servers:" + dns);
                break;
            case R.id.dns_parse_btn:
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        CTIIPDetectTools.dnsRequest("www.parallelworld.club", "114.114.114.114");
                    }
                }).start();
               break;
            case R.id.amazon_buy_bg:
                AmazonGoodsClick();
                break;
            case R.id.amazon_buy_game:
                AmazonGameClick();
                break;
            case R.id.amazon_force_consume:
//                mGWHelper.consume();
                break;
            case R.id.amazon_reprovide:
                callReProvide();
                break;
            case R.id.amazon_query:
                queryAmazonProductInfo();
                break;


        }
    }


    //替换腾讯视频订阅物品
    private void subClick() {
        final String[] pList = {
                "test1", "sub_test_2", "auto_test_autopay_default_pid",
                "test_autopay_default_pid", "month_test_autopay_default_pid", "freetrailmonth", "freetrailmonth2", "freetrailmonth3",
                "testwithdiffprice", "testwithdiffprice1", "testwithdiffprice2",
                "kuanxian_sub1","sub_kuanxian_3","sub_kuanxian_7"
        };
//        final String[] pList = {
//                "test.tencent.month","com.tencent.pay.bossmonth", "com.tencent.qpaytest.otherautorenew1week","test.tencent.package"
//        };

        AlertDialog dialog = new AlertDialog.Builder(mActivity)
                .setSingleChoiceItems(pList, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        CTIMonthRequest request = new CTIMonthRequest();
                        initRequest(request);
                        request.autoPay = true;
                        request.mpInfo.payChannel = "gwallet";
                        request.serviceCode = DC.instance().cfg.mServiceCode;
                        request.serviceName = DC.instance().cfg.mServiceName;
//                        request.offerId = "1450013481";
//                        request.mType = "month";
//                        request.serviceCode = "TXSPTL";
//                        if("test.tencent.month".equals(pList[which])){
//                            request.currency_type = "USD";
//                        }else if("test.tencent.package".equals(pList[which])){
//                            request.offerId = "1450018807";
//                            request.currency_type = "USD";
//                        } else {
//                            request.currency_type = "CNY";
//                        }
                        request.mpInfo.productid = pList[which];

                        //把上次开通成功的参数保存下来
                        MainActivity.OldSku = request.mpInfo.productid;
                        CTIPayAPI.singleton().pay(mActivity, request,PayHelper.this);

                        BottomWM.instance().close();
                    }
                })
                .setCancelable(true)
                .create();

        dialog.setCanceledOnTouchOutside(true);
        dialog.show();
    }

    //替换腾讯视频订阅物品
    private void subUpgradeClick() {
        final String[] pList = {
                "testsub1", "testsub2", "testsub3",
                "freetrail1", "freetrail2", "freetrailmonth", "freetrailmonth2", "freetrailmonth3",
                "testwithdiffprice", "testwithdiffprice1", "testwithdiffprice2",
                "kuanxian_sub1","sub_kuanxian_3","sub_kuanxian_7"
        };
//        final String[] pList = {
//                "test.tencent.month","com.tencent.pay.bossmonth", "com.tencent.qpaytest.otherautorenew1week","test.tencent.package"
//        };

        AlertDialog dialog = new AlertDialog.Builder(mActivity)
                .setSingleChoiceItems(pList, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        CTIMonthRequest request = new CTIMonthRequest();
                        initRequest(request);
                        request.autoPay = true;
                        request.mpInfo.payChannel = "gwallet";
                        request.serviceCode = DC.instance().cfg.mServiceCode;
                        request.serviceName = DC.instance().cfg.mServiceName;
//                        request.offerId = "1450013481";
//                        request.mType = "month";
//                        request.serviceCode = "TXSPTL";
//                        if("test.tencent.month".equals(pList[which])){
//                            request.currency_type = "USD";
//                        }else if("test.tencent.package".equals(pList[which])){
//                            request.offerId = "1450018807";
//                            request.currency_type = "USD";
//                        } else {
//                            request.currency_type = "CNY";
//                        }
                        request.mpInfo.productid = pList[which];

                        //升级传递的协议
                        StringBuilder sb = new StringBuilder();
                        sb.append("OldSku=");
                        sb.append(MainActivity.OldSku);
                        sb.append("&");
                        sb.append("SubcribeProrationMode=4");

                        request.extras = sb.toString();
//                        channelExtras="OldSku=&SubcribeProrationMode=4"
                        CTIPayAPI.singleton().pay(mActivity, request,PayHelper.this);

                        BottomWM.instance().close();
                    }
                })
                .setCancelable(true)
                .create();

        dialog.setCanceledOnTouchOutside(true);
        dialog.show();
    }

    /**
     * h5支付
     */
    public void h5Pay(){

       // TestConfig.mWebViewUrl = "https://www.qq.com"; //

        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        if (CTIPayAPI.singleton().getEnv().equals(MConstants.DevEnv)){
            request.offerId="1450021702";
        }else{
            request.offerId="1450015065";
        }

        request.mpInfo.payChannel = "h5_mall";
        request.openId = "8227068844573992"; //35319745633191208
        request.extras = "charac_id=123456&charac_name=zach"; //charac_id=123456&charac_name=zach&from=s1a06sq&sc=s1a06sq"
        request.country = "id";
        CTIPayAPI.singleton().pay(mActivity, request,PayHelper.this);

        /*通过同时打开两个webview 进程来测试Android P下两个webview进程访问同一个目录crash的问题*/
        //mActivity.startActivity(new Intent(mActivity,DemoWebActivity.class));
    }


    /**
     * doku支付
     */
    public void DoKuPay(){
        CTIMonthRequest request = new CTIMonthRequest();
        initRequest(request);
        request.mpInfo.payChannel = "os_doku";
        request.extras = "pay_channel=31";
        request.serviceCode = DC.instance().cfg.mServiceCode;
        request.serviceName = DC.instance().cfg.mServiceName;
        request.mpInfo.productid = DC.instance().cfg.mSubsProductID;
        CTIPayAPI.singleton().pay(mActivity, request,PayHelper.this);
    }


    private void queryGWInfo(){
        HashMap<String, String> map = new HashMap<String, String>();
        map.put("test_product2", "inapp");
        CTIPayAPI.singleton().getProductInfo("gwallet",map,new ICTIProductInfoCallback(){
            @Override
            public void onProductInfoResp(String resp) {
                try {
                    JSONObject json = new JSONObject(resp);
                    String ret = json.getString("ret");
                    if("0".equals(ret)){
                        showDialog("result", JsonFormat.format(json.toString()));
                    }else {
                        showDialog("result","!!Error");
                    }
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        });
    }

    private void queryAmazonProductInfo(){
        HashMap<String, String>  list = new HashMap<String, String> ();
        list.put("gamecoins1", "inapp");
        list.put("goodspackage", "inapp");
        CTIPayAPI.singleton().getProductInfo("os_amazon", list, new ICTIProductInfoCallback() {
            @Override
            public void onProductInfoResp(String resp) {
                try {
                    JSONObject json = new JSONObject(resp);
                    String ret = json.getString("ret");
                    if("0".equals(ret)){
                        showDialog("result",JsonFormat.format(json.toString()));
                    }else {
                        showDialog("result","!!Error");
                    }
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        });
    }


    private void reProvide(){
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        CTIPayAPI.singleton().reProvide(new ICTIPayUpdateCallBack() {
            @Override
            public void onUpdate(int retCode, String info) {
                Log.i(TAG,"retCode = "+retCode+" info = "+info);
                Toast.makeText(mActivity,"reProvide finished: retCode = "+retCode+" info = "+info
                    ,Toast.LENGTH_SHORT).show();
            }
        });
    }


    private void initRequest(CTIBaseRequest request) {
        request.offerId = DC.instance().cfg.offerid;
        request.openId = DC.instance().cfg.openId;
        request.openKey = "openKey";
        request.sessionId = "hy_gameid";
        request.sessionType = "st_dummy";
        request.zoneId = "1";
        request.goodsZoneId="1001";
        request.pf = "huyu_m-2001-android-2001";
        request.pfKey = "DSSSSSSSSSSS";
        request.country = DC.instance().cfg.mCountry;
        request.currency_type = DC.instance().cfg.mCurrency;
    }

    private void AmazonGameClick() {
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        request.zoneId = "1";
        request.mpInfo.payChannel = "os_amazon";
        request.mpInfo.productid = DC.instance().cfg.amazonGameProductId;
        CTIPayAPI.singleton().pay(mActivity, request, this);
    }

    private void AmazonGoodsClick() {
        CTIGameRequest request = new CTIGameRequest();
        initRequest(request);
        request.zoneId = "1";
        request.mpInfo.payChannel = "os_amazon";
        request.mpInfo.productid = DC.instance().cfg.amazonGoodsProductId;
        CTIPayAPI.singleton().pay(mActivity, request, this);
    }

    private void molAccount(){
        CTIMonthRequest request = new CTIMonthRequest();
        initRequest(request);
        request.mpInfo.payChannel = "mol_acct";
        request.serviceCode = DC.instance().cfg.mServiceCode;
        request.serviceName = DC.instance().cfg.mServiceName;
        request.mpInfo.productid = "com_tencent_ibg_joox_mol_month12";
        CTIPayAPI.singleton().pay(mActivity,request,this);
    }


    /**
     * 商城页
     */
    public void mall(){
        MallParams params = new MallParams.Builder()
                .setPayChannel("gwallet")
                .build();
        CTIPayNewAPI.singleton().mall(mActivity,params,this);
    }


    private void showDialog(String title, String mes) {
        AlertDialog.Builder dialogBuilder = new AlertDialog.Builder(mActivity);
        dialogBuilder.setTitle(title);
        dialogBuilder.setMessage(mes);
        dialogBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });
        AlertDialog dialog = dialogBuilder.create();
        dialog.show();
    }

    //Midas支付回调1
    @Override
    public void CentauriPayCallBack(CTIResponse responseInfo) {
        showDialog(" info:", responseInfo.toString());
        CTILog.d(TAG+" MidasPayCallback",responseInfo.toString());
        try{
            JSONObject js = new JSONObject(responseInfo.getExtra());
            String jInfo = js.optString("info");
            JSONObject _jInfo = new JSONObject(new String(CTIBase64.decode(jInfo)));
            CTILog.d(TAG,"info "+_jInfo.toString());
        }catch (JSONException e){
            CTILog.d(TAG,"MidasPayCallBack Exception: "+e.getMessage());
        }
    }

    //Midas支付回调2
    @Override
    public void CentauriPayNeedLogin() {
        showDialog("info", "login expired,please login again");
    }

    //Midas net回调1
    @Override
    public void CentauriNetFinish(String reqType, String result) {
        Log.i(TAG,"reqType: "+reqType +",result: "+result);
    }

    //Midas net回调2
    @Override
    public void CentauriNetError(String reqType, int resultCode, String resultMsg) {
        Log.i(TAG,"reqType = "+reqType +" resultCode = "+resultCode+" resultMsg = "+resultMsg);
    }

    //Midas net回调3
    @Override
    public void CentauriNetStop(String reqType) {
        Log.i(TAG,"reqType = "+reqType);
    }

    //Midas reProvide回调
    @Override
    public void onUpdate(int retCode, String info) {
        Log.i(TAG,"reprovide retCode = "+retCode+" info = "+info);
    }

    //Midas getProductInfo回调
    @Override
    public void onProductInfoResp(String resp) {
        Log.i(TAG,"resp = "+resp);
    }


}
