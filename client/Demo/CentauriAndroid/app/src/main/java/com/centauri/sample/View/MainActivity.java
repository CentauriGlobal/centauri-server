package com.centauri.sample.View;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import com.centauri.sample.Cfg.Config;
import com.centauri.sample.Cfg.DC;
import com.centauri.sample.Cfg.JooxConfig;
import com.tencent.imsdk.samples.R;
import com.centauri.comm.CTILog;
import com.centauri.oversea.comm.MCycleTimer;
import com.centauri.oversea.newapi.CTIPayNewAPI;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "ScrollingActivity";
    private RecyclerView mRecyclerView = null;
    private ReprovideTimerBroadcastReceiver receiver = null;

    public static String OldSku = "";//上一次开通的订阅productid

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestPermissions();
        setContentView(R.layout.activity_scrolling);

        mRecyclerView = (RecyclerView) findViewById(R.id.rec_content);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.setAdapter(new MyRecyclerViewAdapter(this));

        CTIPayNewAPI.singleton().setLogEnable(true);
        registerReprovideTimerBR();
        DC.instance().init(this);
    }

    //功能测试
    private void test() {
        Log.d(TAG, "start test...");
        MCycleTimer timer = new MCycleTimer.Builder()
                .setCount(5)
                .setPeriod(1000)
                .setUpdateNotifier(new MCycleTimer.CycleTimerUpdateNotifier() {
                    @Override
                    public void onUpdate() {
                        Log.d(TAG, "CycleTimerUpdateNotifier update...");
                    }
                })
                .build();
        timer.start();
    }

    //注册补发广播
    private void registerReprovideTimerBR() {
        receiver = new ReprovideTimerBroadcastReceiver();
        IntentFilter broadcastFilter = new IntentFilter(CTIPayNewAPI.RE_PROVIDE_ACTION);
        LocalBroadcastManager.getInstance(this)
                .registerReceiver(receiver, broadcastFilter);
        CTILog.d(TAG, "success register reprovideBroadcastReceiver");
    }

    //动态申请权限
    private void requestPermissions() {
        ArrayList<String> permissions = new ArrayList<>();

        try {
            int permissio1 = ActivityCompat.checkSelfPermission(this,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permissio1 != PackageManager.PERMISSION_GRANTED) {
                permissions.add("android.permission.WRITE_EXTERNAL_STORAGE");
            }

            int permission2 = ActivityCompat.checkSelfPermission(this,
                    "android.permission.READ_EXTERNAL_STORAGE");
            if (permission2 != PackageManager.PERMISSION_GRANTED) {
                permissions.add("android.permission.READ_EXTERNAL_STORAGE");
            }

            if (!permissions.isEmpty()) {
                String[] str = new String[permissions.size()];
                for (int i = 0; i < permissions.size(); i++) {
                    str[i] = permissions.get(i);
                }
                ActivityCompat.requestPermissions(this, str, permissions.size());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (receiver != null) {
            LocalBroadcastManager.getInstance(this)
                    .unregisterReceiver(receiver);
            CTILog.d(TAG, "success unRegister reprovideBroadcastReceiver");
        }
    }

    static class ReprovideTimerBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            int resultCode = intent.getIntExtra("resultCode", -1);
            String resultMsg = intent.getStringExtra("resultMsg");

            CTILog.d(TAG, "resultCode: " + resultCode);
            CTILog.d(TAG, "reslutMsg: " + resultMsg);
        }
    }
}
