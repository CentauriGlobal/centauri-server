package com.centauri.sample.View;

import android.app.Application;

import com.squareup.leakcanary.LeakCanary;

/**
 * Created by zachzeng on 2017/12/11.
 */

public class LeakApplication extends Application{
    @Override
    public void onCreate() {
        super.onCreate();
        LeakCanary.install(this);
    }
}
