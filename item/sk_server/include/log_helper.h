#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "log_thread.h"
#include "base_def.h"
#include "base_singleton.h"

#define LOG_INIT(log_conf) \
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

#define LOG_SET_TYPE(type) \
    do { \
        log_thread * thread = base_singleton<log_thread>::get_instance(); \
        if (thread) { \
            thread->set_type(LogType(type));\
        } \
    }  while (0)

#define LOG_WARNING(fmt, arg...) \
    do { \
        log_prefix prefix; \
        prefix.tid = pthread_self(); \
        prefix.line = __LINE__; \
        prefix.fun = __func__; \
        prefix.file = __FILE__; \
        prefix.typestr = "WARNING"; \
        prefix.type = LOGWARNING; \
        log_thread::log_write(prefix, fmt, ##arg); \
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        log_prefix prefix; \
        prefix.tid = pthread_self(); \
        prefix.line = __LINE__; \
        prefix.fun = __func__; \
        prefix.file = __FILE__; \
        prefix.typestr = "FATAL"; \
        prefix.type = LOGFATAL; \
        log_thread::log_write(prefix, fmt, ##arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
      do { \
        log_prefix prefix; \
        prefix.tid = pthread_self(); \
        prefix.line = __LINE__; \
        prefix.fun = __func__; \
        prefix.file = __FILE__; \
        prefix.typestr = "NOTICE"; \
        prefix.type = LOGNOTICE; \
        log_thread::log_write(prefix, fmt, ##arg); \
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        log_prefix prefix; \
        prefix.tid = pthread_self(); \
        prefix.line = __LINE__; \
        prefix.fun = __func__; \
        prefix.file = __FILE__; \
        prefix.typestr = "TRACE"; \
        prefix.type = LOGTRACE; \
        log_thread::log_write(prefix, fmt, ##arg); \
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        log_prefix prefix; \
        prefix.tid = pthread_self(); \
        prefix.line = __LINE__; \
        prefix.fun = __func__; \
        prefix.file = __FILE__; \
        prefix.typestr = "DEBUG"; \
        prefix.type = LOGDEBUG; \
        log_thread::log_write(prefix, fmt, ##arg); \
    } while (0)

#endif
