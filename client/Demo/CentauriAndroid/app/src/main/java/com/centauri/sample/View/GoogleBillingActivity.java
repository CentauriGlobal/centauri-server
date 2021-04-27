package com.centauri.sample.View;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentActivity;

import com.tencent.imsdk.samples.R;

public class GoogleBillingActivity extends FragmentActivity {

    private static final String TAG = "GoogleBillingActivity";
    private GoogleBillingFragment googleBillingFragment;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_google_billing);
        initView();
    }

    private void initView() {
        googleBillingFragment = GoogleBillingFragment.newInstance();
        getSupportFragmentManager()
                .beginTransaction()
                .replace(R.id.fy_billing, googleBillingFragment)
                .commitAllowingStateLoss();
        getSupportFragmentManager().beginTransaction().show(googleBillingFragment);
    }

    public static void startActivity(Context context) {
        Intent intent = new Intent(context, GoogleBillingActivity.class);
        context.startActivity(intent);
    }
}
