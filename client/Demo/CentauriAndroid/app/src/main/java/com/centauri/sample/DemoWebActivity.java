package com.centauri.sample;

import android.os.Build;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.JsResult;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.tencent.imsdk.samples.R;
import com.centauri.comm.CTILog;
import com.centauri.oversea.business.h5.webview.MWebView;
import com.centauri.oversea.comm.CTICommMethod;
import com.centauri.oversea.comm.CTITools;
import com.centauri.oversea.comm.MUIManager;

public class DemoWebActivity extends AppCompatActivity {


    private FrameLayout wvContainer = null;            //WebView容器
    private RelativeLayout refreshLayout = null;       //请求失败，刷新布局
    private TextView refreshContent = null;            //请求失败，显示内容
    private MWebView mWebView = null;
    private MUIManager uiManager = null;               //Midas通用UI管理
    private String mDebugUrl = "";                     //Debug Url for webview
    private ProgressBar mProgressBar = null;
    public static final String TAG = "DemoWebActivity";
    private String mUrl = "https://www.baidu.com";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(CTICommMethod.getLayoutId(this, "unipay_abroad_layout_webview_demo"));
        refreshLayout = findViewById(CTICommMethod.getId(this,"unipay_id_webview_demo_refresh_layout"));
        refreshContent = findViewById(CTICommMethod.getId(this,"unipay_id_webview_demo_err_content"));
        wvContainer = findViewById(CTICommMethod.getId(this,"unipay_id_webview_demo_container"));
        mProgressBar =  findViewById(CTICommMethod.getId(this,"progressBarDemo"));
        //android 9 上会crash
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            WebView.setDataDirectorySuffix(CTITools.getProcessName(this));
        }
        mWebView = new MWebView(this);   //fixbug ApplicationContext can not support UI selection components。
        mWebView.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT));
        wvContainer.addView(mWebView);


        mWebView.loadUrl(mUrl);

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if(mWebView.canGoBack()){
                mWebView.goBack();
            }else{
                callbackAndDestroy();
            }
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onDestroy() {
        CTILog.i(TAG,"onDestroy()");

        if(mWebView != null){
            ViewGroup parent = (ViewGroup)mWebView.getParent();
            if(parent != null){
                parent.removeView(mWebView);
                mWebView.destroy();

                try {
                    android.os.Process.killProcess(android.os.Process.myPid());
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }

        super.onDestroy();
    }

    //回调主进程，通知结束
    private void callbackAndDestroy(){

        this.finish();
    }

    private void setListeners() {
        //WebViewClient Listener
        mWebView.setWebViewClientListener(new MWebView.WebViewClientListener() {
            @Override
            public void onRequestError(String errMsg) {
//                uiManager.dismissWaitDialog();
                wvContainer.setVisibility(View.INVISIBLE);
                refreshLayout.setVisibility(View.VISIBLE);
                mProgressBar.setVisibility(View.GONE);
                if(!TextUtils.isEmpty(errMsg)){
                    refreshContent.setText(errMsg);
                }
            }

            @Override
            public void onPageFinished(WebView view, String url) {

            }

            @Override
            public void onPageStarted(WebView view, String url) {

            }
        });

        //WebChromeClient Listener
        mWebView.setWebChromeClientListener(new MWebView.WebChromeClientListener() {

            @Override
            public void onJsAlert(String url, String message, JsResult result) {
//                h5.onJsAlert(message);
//                //update intercept rules
//                mUrlIntercept.setInterceptItems(h5.getInterceptItems());
            }

            @Override
            public void onProgressChanged(int progress) {
                Log.d(TAG, "progress: " + progress);
                if (progress == 100) {
                    mProgressBar.setVisibility(View.GONE);
                } else {
                    mProgressBar.setVisibility(View.VISIBLE);
                    mProgressBar.setProgress(progress);
                }
            }
        });


        //Close onClick Listener
        findViewById(CTICommMethod.getId(this,"unipay_id_h5_close")).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                callbackAndDestroy();
            }
        });

        //RefreshLayout onClick Listener
        refreshLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                wvContainer.setVisibility(View.VISIBLE);
                refreshLayout.setVisibility(View.INVISIBLE);

                mWebView.loadUrl(mUrl);
            }
        });
    }
}
