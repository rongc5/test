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
    char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * msg =  new log_msg(); \
        msg->type = LOGWARNING; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "WARNING\t%s\t[%lu]\t[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        msg->str.append(buf); \
        log_thread::put_msg(msg); \
} while (0)


#define LOG_FATAL(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * msg =  new log_msg(); \
        msg->type = LOGFATAL; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "FATAL\t%s\t[%lu]\t[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        msg->str.append(buf); \
        log_thread::put_msg(msg); \
} while (0)


#define LOG_NOTICE(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * msg =  new log_msg(); \
        msg->type = LOGNOTICE; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "NOTICE\t%s\t[%lu]\t[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        msg->str.append(buf); \
        log_thread::put_msg(msg); \
} while (0)



#define LOG_TRACE(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * msg =  new log_msg(); \
        msg->type = LOGTRACE; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "TRACE\t%s\t[%lu]\t[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        msg->str.append(buf); \
        log_thread::put_msg(msg); \
} while (0)


#define LOG_DEBUG(fmt, arg...) \
do { \
    char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        log_msg * msg =  new log_msg(); \
        msg->type = LOGDEBUG; \
        char buf[SIZE_LEN_8192]; \
        snprintf(buf, sizeof(buf), "DEBUG\t%s\t[%lu]\t[%d:%s():%s] "fmt, tmp,pthread_self(), __LINE__, __func__, __FILE__,     ##arg); \
        msg->str.append(buf); \
        log_thread::put_msg(msg); \
} while (0)



#endif

