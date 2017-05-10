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
        LOG_COMMON(LOGWARNING, "[WARNING]", fmt, arg); \
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        LOG_COMMON(LOGFATAL, "[FATAL]", fmt, arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
    do { \
        LOG_COMMON(LOGNOTICE, "[NOTICE]", fmt, arg); \
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        LOG_COMMON(LOGTRACE, "[TRACE]", fmt, arg); \
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        LOG_COMMON(LOGDEBUG, "[DEBUG]", fmt, arg); \
    } while (0)


#define LOG_COMMON(TYPE, TYPESTR, fmt, arg...) \
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
            char log_common_tmp[SIZE_LEN_64]; \
            get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
            lgmsg->type = TYPE; \
            lgmsg->tid = pthread_self(); \
            char log_common_buf[SIZE_LEN_16384]; \
            snprintf(log_common_buf, sizeof(log_common_buf), TYPESTR":[%s]:[%lu]:[%d:%s():%s] "fmt, log_common_tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
            lgmsg->str.append(log_common_buf); \
            thread->add_msg(lgmsg); \
        }\
    } while (0)

#endif
