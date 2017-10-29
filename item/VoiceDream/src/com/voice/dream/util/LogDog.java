package com.voice.dream.util;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

import android.os.Environment;

import com.voice.dream.util.Constant.DebugConfig;

public class LogDog {
	public static final SimpleDateFormat DATA_FORMAT_TIME = new SimpleDateFormat(
			"MM-dd HH:mm:ss.SSS");

	private static LogDog dog = new LogDog();

	private LogDog() {
		init();
	}

	private static Logger logger;

	private static void init() {
		try {
			if (!Environment.getExternalStorageState().equals(
					Environment.MEDIA_MOUNTED)) {
				return;
			}
			DATA_FORMAT_TIME.setTimeZone(TimeZone.getDefault());
			logger = Logger.getLogger(LogDog.class.getName());
			logger.setLevel(Level.INFO);
			FileHandler fileHandler = null;
			try {
				String logPath = getLogFilePath(true);
				fileHandler = new FileHandler(logPath, true);
				fileHandler.setLevel(Level.INFO);
				fileHandler.setFormatter(new MyLogFormatter());
			} catch (IOException e) {
				e.printStackTrace();
			}
			logger.addHandler(fileHandler);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static class MyLogFormatter extends Formatter {
		@Override
		public String format(LogRecord record) {
			return DATA_FORMAT_TIME.format(new Date()) + "| "
					+ record.getMessage() + "\n";
		}
	}

	public static void i(String message) {
		if (dog.logger == null) {
			init();
		}
		if (dog.logger == null) {
			return;
		}
		dog.logger.info(message);
	}

	@Override
	protected void finalize() throws Throwable {
		if (dog.logger != null) {
			Handler[] hs = dog.logger.getHandlers();
			if (hs != null) {
				for (Handler h : hs) {
					h.close();
				}
			}
		}
		super.finalize();
	}

	public static void clean() {
		String logPath = getLogFilePath(false);
		if (logPath == null || "".equals(logPath.trim())) {
			return;
		}
		File f = new File(logPath);
		if (f.exists()) {
			try {
				FileWriter wf = new FileWriter(f);
				wf.write("");
				wf.flush();
				wf.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private static String getLogFilePath(boolean creatIfNotExist) {
		if (!Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED)) {
			return null;
		}
		File logDir = new File(Environment.getExternalStorageDirectory()
				+ "/"+ DebugConfig.logDir);
		if (!logDir.exists() || !logDir.isDirectory()) {
			if (creatIfNotExist) {
				logDir.mkdir();
			} else {
				return null;
			}
		}
		File appFile = new File(Environment.getExternalStorageDirectory()
				+ "/"+ DebugConfig.logDir + "/"+ DebugConfig.logFile);
		if (!appFile.exists()) {
			try {
				if (creatIfNotExist) {
					appFile.createNewFile();
					return appFile.getAbsolutePath();
				} else {
					return null;
				}
			} catch (IOException e) {
				e.printStackTrace();
				return null;
			}
		} else {
			return appFile.getAbsolutePath();
		}
	}
}
