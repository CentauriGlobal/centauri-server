package com.centauri.sample.Cfg;

import android.content.Context;

/**
 * Created by zachzeng on 2017/11/1.
 */

public class DC {

    private static DC instance = null;

    private DC(){
    }


    public static DC instance(){
        if(instance == null){
            synchronized (DC.class){
                if(instance == null){
                    instance = new DC();
                }
            }
        }
        return instance;
    }

    public void init(Context context) {

        cfg = new Config(context); //默认Demo配置
    }
    //配置
    public Config cfg = null;
    public String env = "test";
    public String idc = "local";
    public String idcInfo = "";
}
