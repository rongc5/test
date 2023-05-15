#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "log_thread.h"
#include "base_def.h"
#include "base_singleton.h"


#define LOG_INIT_OLD(log_conf) \
    do { \
        log_thread * thread = base_singleton<log_thread>::get_instance(); \
        if (thread) { \
            break; \
        } \
        thread = new log_thread(log_conf); \
        base_singleton<log_thread>::set_instance(thread); \
        thread->start(); \
        sleep(3); \
    }  while (0)

#define LOG_INIT(log_path) \
    do { \
        log_thread::init(log_path); \
        sleep(3); \
    }  while (0)

#define LOG_WARNING(fmt, arg...) \
    do { \
        LOG_WRITE(LOGTYPEWARNING, "WARNING:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        LOG_WRITE(LOGTYPEFATAL, "FATAL:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
      do { \
        LOG_WRITE(LOGTYPENOTICE, "NOTICE:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg);\
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        LOG_WRITE(LOGTYPETRACE, "TRACE:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        LOG_WRITE(LOGTYPEDEBUG, "DEBUG:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)

#define LOG_WRITE(t, fmt...) log_thread::log_write(t, ##fmt)
#define FILE_WRITE(f, fmt...) log_thread::log_write(f, ##fmt)

#define LOGWARNING log_stream(LOGTYPEWARNING, __LINE__, __func__, __FILE__)

#define LOGFATAL log_stream(LOGTYPEFATAL, __LINE__, __func__, __FILE__) 

#define LOGNOTICE log_stream(LOGTYPENOTICE, __LINE__, __func__, __FILE__)

#define LOGTRACE log_stream(LOGTYPETRACE, __LINE__, __func__, __FILE__)

#define LOGDEBUG log_stream(LOGTYPEDEBUG, __LINE__, __func__, __FILE__)

#endif
