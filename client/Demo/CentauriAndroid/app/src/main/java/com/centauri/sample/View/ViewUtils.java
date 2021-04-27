package com.centauri.sample.View;

import android.content.Context;

/**
 * Created by zachzeng on 2017/2/20.
 */

public class ViewUtils {
    public static int dp2px(Context context, int dp){
        float density = context.getResources().getDisplayMetrics().density;
        return (int)(density*dp+0.5);
    }

    public static int px2dp(Context context, int px){
        float density = context.getResources().getDisplayMetrics().density;
        return (int)(px/density+0.5);
    }

}
