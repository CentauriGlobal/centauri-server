package com.centauri.sample.View;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.centauri.comm.CTILog;
import com.centauri.oversea.api.ICTICallBack;
import com.centauri.oversea.api.ICTIPayUpdateCallBack;
import com.centauri.oversea.business.CTIPayManager;
import com.centauri.oversea.business.pay.CTIResponse;
import com.centauri.oversea.newapi.CTIPayNewAPI;
import com.centauri.sample.CentauriAPISample;
import com.centauri.sample.Cfg.DC;
import com.tencent.imsdk.samples.R;

import java.text.SimpleDateFormat;

public class GoogleBillingFragment extends Fragment implements View.OnClickListener, ICTICallBack {

    private static final String TAG = "GoogleBillingFragment";
    private TextView mTvLog;
    private SettingDialogFragment mSettingDialogFragment;

    public static GoogleBillingFragment newInstance() {
        GoogleBillingFragment fragment = new GoogleBillingFragment();
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_google_billing, container, false);
        return view;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        DC.instance().init(getContext());
        initView();
        initData();
    }

    private void initView() {
        mTvLog = getView().findViewById(R.id.tv_log);
        getView().findViewById(R.id.buy_bg).setOnClickListener(this);
        getView().findViewById(R.id.buy_game).setOnClickListener(this);
        getView().findViewById(R.id.sub).setOnClickListener(this);
        getView().findViewById(R.id.sub_upgrade).setOnClickListener(this);
        getView().findViewById(R.id.query).setOnClickListener(this);
        getView().findViewById(R.id.reprovide).setOnClickListener(this);
        getView().findViewById(R.id.force_consume).setOnClickListener(this);
        getView().findViewById(R.id.clear_log).setOnClickListener(this);
        getView().findViewById(R.id.btn_copy_log).setOnClickListener(this);
        getView().findViewById(R.id.btn_setting).setOnClickListener(this);
        mTvLog.setMovementMethod(ScrollingMovementMethod.getInstance());
    }

    Handler handler = new Handler();

    @Override
    public void onDestroy() {
        super.onDestroy();
        CTILog.removeILogCallback();
    }

    private void initData() {
        CTILog.setILogCallback(new CTILog.ILogCallback() {
            @Override
            public void logCallback(String s) {
                handler.post(new Runnable() {
                    @Override
                    public void run() {
                        setLogText(s);
                    }
                });
            }
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            //google play
            case R.id.buy_game:
                Log.i(TAG, "buy");
                CentauriAPISample.buyGameCoins(getActivity(), new ICTICallBack() {
                    @Override
                    public void CentauriPayNeedLogin() {
                        setLogText("login expired,please login again \n");
                    }

                    @Override
                    public void CentauriPayCallBack(CTIResponse ctiResponse) {
                        setLogText(ctiResponse.toString());
                    }
                });
                break;
            case R.id.buy_bg:
                Log.i(TAG, "buy");
                CentauriAPISample.buyMonth(getActivity(), new ICTICallBack() {
                    @Override
                    public void CentauriPayNeedLogin() {
                        setLogText("login expired,please login again \n");
                    }

                    @Override
                    public void CentauriPayCallBack(CTIResponse ctiResponse) {
                        setLogText(ctiResponse.toString() + " \n");
                    }
                });
                break;
            case R.id.sub:  //订阅
                Log.i(TAG, "sub");
                CentauriAPISample.subClick(getActivity(), new ICTICallBack() {
                    @Override
                    public void CentauriPayNeedLogin() {
                        setLogText("login expired,please login again \n");
                    }

                    @Override
                    public void CentauriPayCallBack(CTIResponse ctiResponse) {
                        setLogText(ctiResponse.toString() + " \n");
                    }
                });
                break;
            case R.id.sub_upgrade:  //订阅升级
                Log.i(TAG, "subUpgrade");
                CentauriAPISample.subUpgradeClick(getActivity(), new ICTICallBack() {
                    @Override
                    public void CentauriPayNeedLogin() {
                        setLogText("login expired,please login again \n");
                    }

                    @Override
                    public void CentauriPayCallBack(CTIResponse ctiResponse) {
                        setLogText(ctiResponse.toString());
                    }
                });
                break;
            case R.id.query:    //查询
                Log.i(TAG, "query");
                CentauriAPISample.queryGWInfo(s -> {
                    setLogText(s);
                });
                break;
            case R.id.reprovide:    //补发货
                Log.i(TAG, "reprovide");
                CentauriAPISample.reProvide((ICTIPayUpdateCallBack) (retCode, info) -> {
                    setLogText("reProvide finished: retCode = " + retCode + " info = " + info);
                });
                break;
            case R.id.btn_copy_log:
                ClipboardManager cm = (ClipboardManager) getActivity().getSystemService(Context.CLIPBOARD_SERVICE);
                ClipData mClipData = ClipData.newPlainText("Label", mTvLog.getText());
                cm.setPrimaryClip(mClipData);
                break;
            case R.id.force_consume:
                CTIPayManager.instance().clearPurchase(getContext());
                break;
            case R.id.clear_log:
                mTvLog.setText("");
                break;
            case R.id.btn_setting:
                if (null == mSettingDialogFragment) {
                    mSettingDialogFragment = SettingDialogFragment.newInstance();
                }
                mSettingDialogFragment.show(getFragmentManager(), "");
                break;
        }
    }

    @Override
    public void CentauriPayNeedLogin() {

    }

    @Override
    public void CentauriPayCallBack(CTIResponse ctiResponse) {

    }

    private void setLogText(String text) {
        mTvLog.setText(mTvLog.getText() + getTime() + text + " \n");
        int offset = mTvLog.getLineCount() * mTvLog.getLineHeight();
        if (offset > mTvLog.getHeight()) {
            mTvLog.scrollTo(0, offset - mTvLog.getHeight());
        }
    }

    private String getTime() {
        long currentTime = System.currentTimeMillis();
        String timeNow = new SimpleDateFormat("HH:mm:ss").format(currentTime);
        return timeNow + "  ";
    }
}
