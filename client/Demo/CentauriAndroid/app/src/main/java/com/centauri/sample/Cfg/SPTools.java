
package com.centauri.sample.Cfg;


import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import com.centauri.oversea.newapi.CTIPayNewAPI;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 * SP统一处理
 */
public class SPTools {
    public static final String TAG = "SPTools";
    public static final String SP_NAME = "CentauriSampleSP";


    public static String getString(Context context, String key) {
        return context.getSharedPreferences(SP_NAME, 0).getString(key, "");
    }

    public static String getString(Context context, String key, String def) {
        return context.getSharedPreferences(SP_NAME, 0).getString(key, def);
    }

    public static Map<String, ?> getAll(String spName) {
        return CTIPayNewAPI.singleton().getApplicationContext().getSharedPreferences(spName, 0).getAll();
    }

    public static long getLong(String spName, String key) {
        return CTIPayNewAPI.singleton().getApplicationContext().getSharedPreferences(spName, 0).getLong(key, -1);
    }

    public static void putString(Context context, String key, String value) {
        context.getSharedPreferences(SP_NAME, 0).edit()
                .putString(key, value)
                .apply();
    }
    public static void putBoolean(Context context, String key, boolean value) {
        context.getSharedPreferences(SP_NAME, 0).edit()
                .putBoolean(key, value)
                .apply();
    }

    public static void putLong(String spName, String key, long value) {
        CTIPayNewAPI.singleton().getApplicationContext().getSharedPreferences(spName, 0).edit()
                .putLong(key, value)
                .apply();
    }

    public static void putString(Context context, String spName, String key, String value) {
        context.getSharedPreferences(spName, 0).edit()
                .putString(key, value)
                .apply();
    }

    public static int getInt(Context context, String key) {
        return context.getSharedPreferences(SP_NAME, 0).getInt(key, 0);
    }
    public static boolean getBoolean(Context context, String key) {
        return context.getSharedPreferences(SP_NAME, 0).getBoolean(key, true);
    }
    public static void putInt(Context context, String key, int value) {
        context.getSharedPreferences(SP_NAME, 0).edit()
                .putInt(key, value)
                .apply();
    }

    public static void removeByKey(String spName, String key) {
        CTIPayNewAPI.singleton().getApplicationContext().getSharedPreferences(spName, 0).edit()
                .remove(key)
                .apply();
    }

    public static SharedPreferences getSP(Context context, String spName) {
        return context.getSharedPreferences(spName, Context.MODE_PRIVATE);
    }
}