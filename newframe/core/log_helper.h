#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "log_thread.h"
#include "common_def.h"
#include "base_singleton.h"


#define LOG_INIT(log_conf) \
    do { \
        log_thread * thread = base_singleton<log_thread>::get_instance(); \
        if (thread) { \
            break; \
        } \
        thread = new log_thread(conf); \
        base_singleton<log_thread>::set_instance(thread); \
        thread->start(); \
    }  while (0)

#define LOG_WARNING(fmt, arg...) \
    do { \
        LOG_COMMON(LOG_WARNING, fmt, arg); \
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        LOG_COMMON(LOG_FATAL, fmt, arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
    do { \
        LOG_COMMON(LOG_NOTICE, fmt, arg); \
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        LOG_COMMON(LOG_TRACE, fmt, arg); \
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        LOG_COMMON(LOGDEBUG, fmt, arg); \
    } while (0)


#define LOG_COMMON(TYPE, fmt, arg...) \
    do { \
    log_thread * thread = base_singleton<log_thread>::get_instance(); \
    if (!thread) { \
        break; \
    } \
    if (TYPE > thread->get_log_conf().type) { \
        break; \
    } \
    {\
        log_msg * lgmsg =  new log_msg(); \
        char tmp[SIZE_LEN_64]; \
        lgmsg->st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        lgmsg->type = TYPE; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
    }\
} while (0)

#endif

