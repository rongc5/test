#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"

struct log_prefix
{
    pthread_t tid;
    uint32_t line;
    string fun;
    string file;
    string typestr;
    LogType type;
};

struct log_write_name
{
    char _name[SIZE_LEN_512];
};

class log_thread:public base_net_thread
{
    public:
        log_thread(log_conf & conf);
        virtual ~log_thread(){}

        static void log_write(log_prefix & prefix, const char *format, ...);

        virtual void handle_msg(normal_msg * p_msg);

        const log_conf & get_log_conf();
        
     private:
        
        void log_thread_init();
        
        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

    private:
        log_write_name _log_name[LOGSIZE];
        log_conf _conf;
};



#endif
