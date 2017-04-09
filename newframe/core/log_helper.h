#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "log_thread.h"
#include "common_def.h"


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
                       lgmsg->type = LOGWARNING; \
                       char tmp[SIZE_LEN_64]; \
        time_t st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[WARNING]:[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
} while (0)


#define LOG_FATAL(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        time_t st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * lgmsg =  new log_msg(); \
        lgmsg->type = LOGFATAL; \
        lgmsg->st = st; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[FATAL]:[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
} while (0)


#define LOG_NOTICE(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        time_t st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * lgmsg =  new log_msg(); \
        lgmsg->type = LOGNOTICE; \
        lgmsg->st = st; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[NOTICE]:[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
} while (0)



#define LOG_TRACE(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        time_t st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * lgmsg =  new log_msg(); \
        lgmsg->type = LOGTRACE; \
        lgmsg->st = st; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[TRACE]:[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
} while (0)


#define LOG_DEBUG(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        time_t st = get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * lgmsg =  new log_msg(); \
        lgmsg->type = LOGDEBUG; \
        lgmsg->st = st; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "[DEBUG]:[%s]:[%lu]:[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        lgmsg->str.append(buf); \
        log_thread::put_msg(lgmsg); \
} while (0)


#define LOG_COMMON(TYPE, fmt, ...) \
    log_thread * thread = base_singleton<log_thread>::get_instance(); \
    if (!thread) { \
        break; \
    } \
    if (LOGWARNING > thread->get_log_conf.type) { \
        break; \
    } \
    log_msg * lgmsg =  new log_msg(); \

#endif

