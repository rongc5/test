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
        LOG_WRITE(LOGWARNING, "WARNING:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        LOG_WRITE(LOGFATAL, "FATAL:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
      do { \
        LOG_WRITE(LOGNOTICE, "NOTICE:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg);\
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        LOG_WRITE(LOGTRACE, "TRACE:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        LOG_WRITE(LOGDEBUG, "DEBUG:[%lu]:[%d:%s:%s] " fmt, pthread_self(), __LINE__, __func__, __FILE__, ##arg); \
    } while (0)

#define LOG_WRITE(t, fmt...) log_thread::log_write(t, ##fmt)
#define FILE_WRITE(f, fmt...) log_thread::log_write(f, ##fmt)


#endif
