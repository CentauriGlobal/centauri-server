package com.centauri.sample;

import android.content.Context;
import android.os.Process;
import android.util.Log;

public class CrashHandler implements Thread.UncaughtExceptionHandler{
    public static final String TAG = "CrashHandler";

    //system default crash uncaughtExceptionHandler
    private Thread.UncaughtExceptionHandler mSystemDefaultHandler = null;
    private Context appContext = null;

    private static CrashHandler mInstance = new CrashHandler();
    private CrashHandler(){};
    public static CrashHandler getInstance(){
        return mInstance;
    }

    public void init(Context context){
        mSystemDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
        //replace system handler
        Thread.setDefaultUncaughtExceptionHandler(this);
        appContext = context.getApplicationContext();
    }

    @Override
    public void uncaughtException(Thread t, Throwable e) {
        //1.print
        Log.d(TAG,e.getMessage());

        //2. handle exception
        if(mSystemDefaultHandler != null){
            mSystemDefaultHandler.uncaughtException(t,e);
        }else{
            Process.killProcess(Process.myPid());
        }
    }
}
