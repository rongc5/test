package com.littleblackcat.util;

/**
 * Created by rong on 2017/11/13.
 */

public class Constant {

    public static final class DebugConfig {
        // 线上或测试环境
        public static boolean ONLINE_SERVER = true;

        public static String SERVER_URL = "www.baidu.com";

        public static long DEFAULT_CONNECT_TIMEOUT=15*1000;

        public static boolean SHOW_NET_INFO = true;

        // web端口
        public static String SERVER_PORT = ONLINE_SERVER ? "80" : "8081";

        public static String logDir = "DuoBaoDream";
        public static String logFile = "duoBaoAppLog.txt";

        // 调试控制台日志
        public static final boolean DEBUG = true;
        public static final boolean CLOG = false;

        // 是否打印log
        public static final boolean LOG_ENABLE = DEBUG;
        // 是否弹出测试toast
        public static final boolean TOAST_ENABLE = DEBUG;

        // 是否显示屏幕上的log控制器
        public static final boolean LOGVIEW_ENABLE = DEBUG;

        // 是否将LOG写到sd上
        public static final boolean LOG2SD_ENABLE = DEBUG;

    }

}
