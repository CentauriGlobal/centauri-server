package com.centauri.sample.View;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.PopupWindow;

import com.tencent.imsdk.samples.R;

/**
 * Created by zachzeng on 2016/12/27.
 */

public class BottomWM {
    private final int DEFAULT_HEIGHT = 330;

    private int width;
    private int height = DEFAULT_HEIGHT;
    private int gravity = Gravity.BOTTOM;

    private static PopupWindow mWindow = null;  //维护一个window实例
    private static BottomWM instance = null;
    private boolean hasSetWidth = false;
    private boolean hasSetHeight = false;

    private BottomWM() {
    }

    //单位dp
    public BottomWM setHeight(int height) {
        hasSetHeight = true;
        this.height = height;
        return this;
    }

    //单位dp
    public BottomWM setWidth(int width) {
        hasSetWidth = true;
        this.width = width;
        return this;
    }

    public BottomWM build(Context context) {
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        if (!hasSetWidth) {  //外部未设置宽带，用默认屏幕宽带
            width = wm.getDefaultDisplay().getWidth();
        }

        if (!hasSetHeight) {  //外部未设置高度，用默认值
            height = DEFAULT_HEIGHT;
        }

        mWindow = new PopupWindow(context);
        mWindow.setWidth(width);
        mWindow.setHeight(ViewUtils.dp2px(context, height));
        return this;
    }

    public BottomWM setGravity(int Gravity) {
        gravity = Gravity;
        return this;
    }

    public static BottomWM instance() {
        if (instance == null) {
            synchronized (BottomWM.class) {
                if(instance == null) {
                    instance = new BottomWM();
                }
            }
        }
        return instance;
    }

    public void show(View base, View contentView) {
        mWindow.setContentView(contentView);
        mWindow.setOutsideTouchable(false);
        mWindow.setFocusable(true);
        mWindow.setBackgroundDrawable(new ColorDrawable(Color.GRAY));

        if (mWindow.isShowing()) {
            mWindow.dismiss();
        }
        mWindow.setAnimationStyle(R.style.popwin_anim_style);
        mWindow.showAtLocation(base, gravity, 0, 0);
        //置位
        hasSetWidth = false;
        hasSetHeight = false;
    }

    public void close() {
        if (mWindow.isShowing()) {
            mWindow.dismiss();
        }
    }
}
