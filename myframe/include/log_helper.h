#ifndef __LOG_HELPER_H_
#define __LOG_HELPER_H_

#include "base_singleton.h"
#include "thread_helper.h"
#include "common_def.h"


enum LogType {
    LOGOFF,
    LOGFATAL,
    LOGWARNING,
    LOGNOTICE,
    LOGTRACE,
    LOGDEBUG,
    LOGSIZE
};

class log_helper;
class log_mgr;

//singleton_base<log_mgr>::get_instance()->log(LOGDEBUG, "["__FILE__":"__LINE__"]"fmt, ##__VA_ARGS__); 
//singleton_base<log_mgr>::get_instance()->log(LOGDEBUG, fmt, ##arg); 
#define LOG_DEBUG(fmt, arg...) \
    do { \
        char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        base_singleton<log_mgr>::get_instance_ex()->log(LOGDEBUG, "DEBUG: %s * %lu [%s:%s():%d] "fmt, tmp, pthread_self(), __FILE__, __FUNCTION__, __LINE__, ##arg); \
    } while (0)


#define LOG_NOTICE(fmt, arg...) \
    do { \
        char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        base_singleton<log_mgr>::get_instance_ex()->log(LOGNOTICE, "NOTICE: %s * %lu [%s:%s():%d] "fmt, tmp, pthread_self(), __FILE__, __FUNCTION__, __LINE__, ##arg); \
    } while (0)


#define LOG_WARNING(fmt, arg...) \
    do { \
        char tmp[SIZE_LEN_64]; \
        get_date_str(tmp, sizeof(tmp), LOG_DATE_FORMAT); \
        base_singleton<log_mgr>::get_instance_ex()->log(LOGWARNING, "WARNING: %s * %lu [%s:%s():%d] "fmt, tmp, pthread_self(), __FILE__, __FUNCTION__, __LINE__, ##arg); \
    } while (0)



#define LOG_MAX_SIZE 50*1024*1024
#define LOG_MAX_RECORD 10000

class log_mgr {
    public:
        log_mgr();
        ~log_mgr();
        void init(LogType type, const char *basename, off_t max_size=LOG_MAX_SIZE, size_t max_record=LOG_MAX_RECORD);
        void log(LogType type, const char * format, ...);
        void get_file_name(LogType type, char dest[], size_t dest_len);

    private:
        LogType _type;
        log_helper * _loger[LOGSIZE];
        char base_file_name[SIZE_LEN_64];
        off_t _max_size;
        size_t _max_record;
};

class log_helper {
    public:
        log_helper();
        void init(const char *filename, off_t max_size=LOG_MAX_SIZE, size_t max_record=LOG_MAX_RECORD);
        int log_write(const char *format, va_list &ap);
        void check_to_renmae();

    private:
        thread_mutex_t _mutex;
        char _file_name[SIZE_LEN_64];
        off_t _max_size;
        size_t _max_record;
        size_t _current_record;
};



#endif

