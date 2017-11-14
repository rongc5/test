package com.littleblackcat.secretapp;

import android.app.Application;
import android.os.Handler;
import android.os.Looper;
import android.widget.Toast;

import com.littleblackcat.util.Constant;
import com.littleblackcat.util.LogUtil;

/**
 * Created by Administrator on 2017/11/14.
 */

public class SecretApplication extends Application implements Thread.UncaughtExceptionHandler {
    public static SecretApplication app;
    private static Thread mUiThread;

    private static Handler handler;

    private static final String TAG;

    static {
        TAG = SecretApplication.class.getSimpleName();
    }

    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();
        mUiThread = Thread.currentThread();
        app = this;
        handler = new Handler();
    }

    public static void toast(final String msg) {
        if (Looper.myLooper() == Looper.getMainLooper()) {
            //LogUtil.e("DuoBaoDreamApplication", "主线程");
            Toast.makeText(app, msg, Toast.LENGTH_LONG).show();
        } else {
            getHandler().post(new Runnable() {
                @Override
                public void run() {
                    //LogUtil.e("FFApplication", "非主线程");
                    Toast.makeText(app, msg, Toast.LENGTH_LONG).show();
                }
            });
        }
    }

    public static void runOnUiThread(Runnable runnable) {
        if (Thread.currentThread() == mUiThread) {
            LogUtil.e("FFApplication", "主线程");
            runnable.run();
        } else {
            getHandler().post(runnable);
        }
    }

    public static Handler getHandler() {
        if (handler == null) {
            handler = new Handler(Looper.getMainLooper());
        }
        return handler;
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        // TODO Auto-generated method stub
        LogUtil.i(TAG, "Application uncaughtException。。。。。。");
        if (Constant.DebugConfig.LOG2SD_ENABLE) {
            exception2file(ex, false);
        }
        ex.printStackTrace();
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    private void exception2file(Throwable ex, boolean cause) {
        LogUtil.i(TAG, cause ? "Cause by: " + ex.toString() : ex.toString());
        StackTraceElement[] ss = ex.getStackTrace();
        for (StackTraceElement s : ss) {
            String info = s.toString();
            if (info.startsWith("android.app.ActivityThread.access")) {
                LogUtil.i(TAG, "...more");
                break;
            }
            LogUtil.i(TAG, info);
        }
        Throwable c = ex.getCause();
        if (c != null) {
            exception2file(c, true);
        }
    }
}
