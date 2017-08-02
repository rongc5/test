#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "log_process.h"
#include "base_def.h"
#include "base_singleton.h"
#include "common_util.h"


#define LOG_INIT(log_conf) \
    do { \
        log_process * thread = base_singleton<log_process>::get_instance(); \
        if (thread) { \
            break; \
        } \
        thread = new log_process(conf); \
        base_singleton<log_process>::set_instance(thread); \
    }  while (0)

#define LOG_WARNING(fmt, arg...) \
    do { \
        log_process * thread = base_singleton<log_process>::get_instance(); \
        if (!thread) { \
            break; \
        } \
        if (LOGWARNING > thread->get_log_conf().type) { \
            break; \
        } \
        {\
            char log_common_tmp[SIZE_LEN_64]; \
            get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
            stringstream tmp_ss; \
            tmp_ss <<"[WARNING]:"<< "["<<log_common_tmp<<"]:" << "[" <<pthread_self()<< "]:" <<"["<< \
            __LINE__<< ":"<<__func__<<":" << __FILE__<<"]\t"<< fmt; \
            thread->log_write(LOGWARNING, tmp_ss.str().c_str(), ##arg);\
        }\
    } while (0)


#define LOG_FATAL(fmt, arg...) \
    do { \
        log_process * thread = base_singleton<log_process>::get_instance(); \
        if (!thread) { \
            break; \
        } \
        if (LOGFATAL > thread->get_log_conf().type) { \
            break; \
        } \
        {\
            char log_common_tmp[SIZE_LEN_64]; \
            get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
            stringstream tmp_ss; \
            tmp_ss <<"[FATAL]:"<< "["<<log_common_tmp<<"]:" << "[" <<pthread_self()<< "]:" <<"["<< \
            __LINE__<< ":"<<__func__<<":" << __FILE__<<"]\t"<<fmt; \
            thread->log_write(LOGFATAL, tmp_ss.str().c_str(), ##arg;\
        } \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
      do { \
            log_process * thread = base_singleton<log_process>::get_instance(); \
            if (!thread) { \
                    break; \
             } \
            if (LOGNOTICE > thread->get_log_conf().type) { \
                    break; \
             } \
             {\
                    char log_common_tmp[SIZE_LEN_64]; \
                    get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
                    stringstream tmp_ss; \
                    tmp_ss <<"[NOTICE]:"<< "["<<log_common_tmp<<"]:" << "[" <<pthread_self()<< "]:" <<"["<< \
                    __LINE__<< ":"<<__func__<<":" << __FILE__<<"]\t"<<fmt; \
                    thread->log_write(LOGNOTICE, tmp_ss.str().c_str(), ##arg);\
            }\
    } while (0)


#define LOG_TRACE(fmt, arg...) \
    do { \
        log_process * thread = base_singleton<log_process>::get_instance(); \
        if (!thread) { \
            break; \
        } \
        if (LOGTRACE > thread->get_log_conf().type) { \
            break; \
        } \
        {\
            char log_common_tmp[SIZE_LEN_64]; \
            get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
            stringstream tmp_ss; \
            tmp_ss <<"[TRACE]:"<< "["<<log_common_tmp<<"]:" << "[" <<pthread_self()<< "]:" <<"["<< \
            __LINE__<< ":"<<__func__<<":" << __FILE__<<"]\t"<<fmt; \
            thread->log_write(LOGTRACE, tmp_ss.str().c_str(), ##arg);\
        }\
    } while (0)


#define LOG_DEBUG(fmt, arg...) \
    do { \
        log_process * thread = base_singleton<log_process>::get_instance(); \
        if (!thread) { \
            break; \
        } \
        if (LOGDEBUG > thread->get_log_conf().type) { \
            break; \
        } \
        {\
            char log_common_tmp[SIZE_LEN_64]; \
            get_date_str(log_common_tmp, sizeof(log_common_tmp), LOG_DATE_FORMAT); \
            stringstream tmp_ss; \
            tmp_ss <<"[DEBUG]:"<< "["<<log_common_tmp<<"]:" << "[" <<pthread_self()<< "]:" <<"["<< \
            __LINE__<< ":"<<__func__<<":" << __FILE__<<"]\t"<<fmt; \
            thread->log_write(LOGDEBUG, tmp_ss.str().c_str(), ##arg);\
        }\
    } while (0)

#endif
