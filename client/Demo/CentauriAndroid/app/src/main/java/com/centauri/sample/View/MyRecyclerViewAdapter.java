package com.centauri.sample.View;

import android.app.Activity;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.centauri.sample.Cfg.DC;
import com.tencent.imsdk.samples.R;


/**
 * Created by zachzeng on 2017/11/1.
 */

public class MyRecyclerViewAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder>
        implements LoginHolderListener.LoginCallback {

    public static final String TAG = "MyRecyclerViewAdapter";
    private final String MIDAS = "米大师外部接口";
    private final String GW = "Google Play";
    private final String MOL = "MOL";
    private final String PAYMENTWALL = "PaymentWall";
    private final String MyCard = "MyCard";
    private final String BOKU = "BoKu";
    private final String FORTUMO = "Fortumo";
    private final String XSOLLA = "Xsolla";
    private final String DOKU = "DoKu";
    private final String AMAZON = "Amazon";
    private final String MARKET = "商城页";
    private final String NetTools = "网络探测工具";
    private final String h5 = "h5渠道";

    //View Type
    public static final int LOGIN_TYPE = 1;
    public static final int CUSTOM_TYPE = 2;

    private Activity mActivity = null;
    private PayHelper mPayHelper = null;
    private boolean isLoginFinished = false;

    private String[] mData = {
            MIDAS, GW, h5, NetTools
    };

    public MyRecyclerViewAdapter(Activity activity) {
        mActivity = activity;
        mPayHelper = new PayHelper(activity);
    }

    @Override
    public void loginFinish(int retCode, String info) {
        isLoginFinished = true;
        notifyDataSetChanged(); //登录完成后，视图刷新
    }

    /**
     * 登录Holder
     */
    static class LoginHolder extends RecyclerView.ViewHolder {
        public View mDecorView;
        public Button mLogin;
        public RadioGroup env, idc;
        public EditText mEtOfferid;
        public EditText mEtOpenid;


        public LoginHolder(View view) {
            super(view);
            mDecorView = view;
            mLogin = view.findViewById(R.id.login);
            env = view.findViewById(R.id.env_group);
            idc = view.findViewById(R.id.idc_group);
//            switchGoogleVersion = view.findViewById(R.id.switchGoogleVersion);
            mEtOfferid = view.findViewById(R.id.et_offerid);
            mEtOpenid = view.findViewById(R.id.et_openid);
            mEtOfferid.setText(DC.instance().cfg.offerid);
            mEtOpenid.setText(DC.instance().cfg.openId);
        }
    }

    /**
     * 普通Holder
     */
    static class CustomHolder extends RecyclerView.ViewHolder {
        public TextView mTextView;
        public CardView mDecorView;

        public CustomHolder(View view) {
            super(view);
            mDecorView = (CardView) view;
            mTextView = view.findViewById(R.id.item_content);
        }
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        if (viewType == LOGIN_TYPE) {
            View layout = LayoutInflater.from(mActivity).inflate(R.layout.layout_login, parent, false);
            return new LoginHolder(layout);
        } else {
            View layout = LayoutInflater.from(mActivity).inflate(R.layout.layout_recy_item, parent, false);
            return new CustomHolder(layout);
        }
    }

    @Override
    public void onBindViewHolder(final RecyclerView.ViewHolder holder, int position) {
        if (holder instanceof CustomHolder) {
            if (isLoginFinished) {    //已登录
                ((CustomHolder) holder).mDecorView.setVisibility(View.VISIBLE);
                ((CustomHolder) holder).mTextView.setOnClickListener(mTextViewListener);
                ((CustomHolder) holder).mTextView.setText(mData[position - 1]);
            } else {  //未登录
                ((CustomHolder) holder).mDecorView.setVisibility(View.GONE);
            }

        } else if (holder instanceof LoginHolder) {  //登录
            LoginHolderListener listener = new LoginHolderListener(mActivity, (LoginHolder) holder);
            listener.setLoginCallback(this);
            ((LoginHolder) holder).mLogin.setOnClickListener(listener);
        }
    }

    @Override
    public int getItemCount() {
        return mData.length + 1;
    }

    @Override
    public int getItemViewType(int position) {
        if (position == 0) {
            return LOGIN_TYPE;
        } else {
            return CUSTOM_TYPE;
        }
    }

    private View.OnClickListener mTextViewListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            String content = ((TextView) view).getText().toString();
            Log.i("TAG", "you have click: " + content);
            if (MIDAS.equals(content)) {
                mPayHelper.MidasAPI(view);
            } else if (GW.equals(content)) {
                mPayHelper.GWPay(view);
            } else if (h5.equals(content)) {
                mPayHelper.h5Pay();
            } else if (DOKU.equals(content)) {
                mPayHelper.DoKuPay();
            } else if (NetTools.equals(content)) {
                mPayHelper.NetTools(view);
            } else if (AMAZON.equals(content)) {
                mPayHelper.AmazonPay(view);
            }
        }
    };
}
