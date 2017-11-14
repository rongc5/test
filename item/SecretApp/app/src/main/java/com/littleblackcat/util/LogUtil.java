package com.littleblackcat.util;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Administrator on 2017/11/14.
 */

public class LogUtil {
    public static void i(String tag, String message) {
        if (Constant.DebugConfig.LOG_ENABLE) {
            Log.i(tag, message);
            if (Constant.DebugConfig.LOG2SD_ENABLE) {
                LogDog.i("[" + tag + "] " + message);
            }

        }
    }

    public static void e(String tag, String message) {
        if (Constant.DebugConfig.LOG_ENABLE) {
            Log.e(tag, message);
            if (Constant.DebugConfig.LOG2SD_ENABLE) {
                LogDog.i("[" + tag + "] " + message);
            }
        }
    }

    public static void e(String tag, String message, Exception e){
        if(Constant.DebugConfig.LOG_ENABLE){
            Log.e(tag, message, e);
            if(Constant.DebugConfig.LOG2SD_ENABLE){
                StackTraceElement[] ss = e.getStackTrace();
                for (StackTraceElement s : ss) {
                    String info = s.toString();
                    LogDog.i("[" + tag + "] " + info);
                }
            }
        }
    }

    public static void e(String tag, Exception ex) {
        e(tag, ex, false);
    }

    private static void e(String tag, Throwable ex, boolean cause) {
        if (Constant.DebugConfig.LOG_ENABLE) {

            LogUtil.i(tag, cause ? "Cause by: " + ex.toString() : ex.toString());
            StackTraceElement[] ss = ex.getStackTrace();
            for (StackTraceElement s : ss) {
                String info = s.toString();
                if (info.startsWith("android.app.ActivityThread.access")) {
                    LogUtil.i(tag, "...more");
                    break;
                }
                LogUtil.i(tag, info);
            }
            Throwable c = ex.getCause();
            if (c != null) {
                e(tag, c, true);
            }
        }
    }

    public static void d(String tag, String message) {
        if (Constant.DebugConfig.LOG_ENABLE) {
            Log.d(tag, message);
            if (Constant.DebugConfig.LOG2SD_ENABLE) {
                LogDog.i("[" + tag + "] " + message);
            }
        }
    }

    public static void toast(Context context, String message) {
        if (Constant.DebugConfig.TOAST_ENABLE) {
            Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
        }
    }

    public static void msg(String message) {
        // Log.i(TEST_TAG, message);
        // logs.add(message);
    }

    private static List<String> logs1 = new ArrayList<String>();

    public static synchronized void log2view(String message) {


    }

    public static synchronized void log2view(String tag, String message) {
        // if(DebugConfig.LOG2SD_ENABLE){
        // LogDog.i("["+tag+"] "+message);
        // }

    }

//	private static final int log_limit = 10 * 1024 * 1024;

    public static void Log2SD() {
    }

    public static synchronized void clearLogs() {
        LogUtil.i("LogUtil", "娓呯┖鏃ュ織");
        logs1.clear();
    }

    public static synchronized List<String> getLogs() {
        LogUtil.i("LogUtil", "鑾峰彇鏃ュ織");
        return logs1;
    }

    public static synchronized void cleanAppLog() {
        LogDog.clean();
    }

}
