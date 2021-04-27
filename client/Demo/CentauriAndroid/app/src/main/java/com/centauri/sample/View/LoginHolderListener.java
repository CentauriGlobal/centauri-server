package com.centauri.sample.View;

/**
 * Created by zachzeng on 2017/11/1.
 */

import android.app.Activity;
import android.support.design.widget.Snackbar;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
import android.widget.RadioButton;

import com.centauri.sample.Cfg.DC;
import com.centauri.sample.Cfg.SpecialChannel;
import com.centauri.sample.CentauriAPISample;
import com.tencent.imsdk.samples.R;
import com.centauri.comm.CTILog;
import com.centauri.oversea.api.ICTIPayUpdateCallBack;
import com.centauri.oversea.comm.CTIBase64;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * 登录监听
 */
public class LoginHolderListener implements View.OnClickListener{
    public static final String TAG = "LoginHolderListener";
    private MyRecyclerViewAdapter.LoginHolder holder;
    private Activity mActivity;
    private LoginCallback mCallback = null;

    interface LoginCallback{
        void loginFinish(int retCode, String info);
    }

    public LoginHolderListener(Activity activity,MyRecyclerViewAdapter.LoginHolder holder){
        mActivity = activity;
        this.holder = holder;
    }

    public void setLoginCallback(LoginCallback callback){
        mCallback = callback;
    }

    @Override
    public void onClick(View view) {
        for(int i = 0;i<holder.env.getChildCount();i++){
            RadioButton rb = (RadioButton)holder.env.getChildAt(i);
            if(rb.isChecked()){
                DC.instance().env = rb.getText().toString();
                break;
            }
        }

        for(int i = 0;i<holder.idc.getChildCount();i++){
            RadioButton ri = (RadioButton)holder.idc.getChildAt(i);
            if(ri.isChecked()){
                DC.instance().idc = ri.getText().toString();
                break;
            }
        }

//        for(int i = 0;i<holder.switchGoogleVersion.getChildCount();i++){
//            RadioButton op = (RadioButton)holder.switchGoogleVersion.getChildAt(i);
//            if(op.isChecked()){
//                if (TextUtils.equals("isNewGoogle", op.getText())) {
//                    GlobalData.isGoogleNew = true;
//                } else {
//                    GlobalData.isGoogleNew = false;
//                }
//                break;
//            }
//        }

        EditText myOpenid = holder.mEtOpenid;
        String myOpenidStr = myOpenid.getText().toString();
        if(!TextUtils.isEmpty(myOpenidStr)){
            DC.instance().cfg.setOpenId(myOpenidStr);
        }
        DC.instance().cfg.setOfferid(holder.mEtOfferid.getText().toString());

        //登录
        startLogin();
    }


    private void startLogin()  {

        //补发货回调
        final ICTIPayUpdateCallBack callBack = new ICTIPayUpdateCallBack() {
            @Override
            public void onUpdate(int retCode, String info) {
                CTILog.d(TAG,"retCode = "+retCode+" info = "+info);
                try{
                    JSONObject js = new JSONObject(info);
                    String strSubArray = js.optString("subInfo");
                    JSONArray jsonArray = new JSONArray(new String(CTIBase64.decode(strSubArray)));
                    for(int i=0;i<jsonArray.length();i++){
                        CTILog.d(TAG,"jSubItem: "+jsonArray.get(i).toString());
                    }

                }catch (JSONException e){
                    CTILog.d(TAG,"startLogin exception: "+e.getMessage());
                }

                StringBuilder sb = new StringBuilder();
                sb.append("Login Finished!").append("\n")
                        .append(DC.instance().cfg.offerid)
                        .append("  openId: ").append(DC.instance().cfg.openId)
                        .append(" env: ").append(DC.instance().env)
                        .append("  idc: ").append(DC.instance().idc);

                //show snackbar
                if(holder.mDecorView != null ) {
                    Snackbar snackbar = Snackbar.make(holder.mDecorView, sb.toString(), Snackbar.LENGTH_LONG);
                    View view = snackbar.getView();
                    if (view != null) {
                        view.setBackgroundColor(mActivity.getResources().getColor(R.color.colorPrimary));
                    }
                    snackbar.show();
                }

                if (mCallback != null) {
                    mCallback.loginFinish(retCode, info);
                }
            }
        };


        //使用特殊渠道配置
        if(DC.instance().cfg.isSpecialChannel){
            SpecialChannel specialChannel = (SpecialChannel)DC.instance().cfg;

            if(!TextUtils.isEmpty(specialChannel.idc)){
                DC.instance().idc = specialChannel.idc;
            }

            String idcInfo = specialChannel.getIdcInfo();
            if(!TextUtils.isEmpty(idcInfo)){
                DC.instance().idcInfo = idcInfo;
            }
        }

        CentauriAPISample.MidasInit(mActivity,callBack);
    }
}