#ifndef __LOG__PROCESS_H__
#define __LOG__PROCESS_H__

#include "base_def.h"
#include "thread_helper.h"
#include "common_util.h"
#include "common_def.h"

struct log_write_name
{
    char _name[SIZE_LEN_512];
    thread_mutex_t _mutex;
};

class log_process
{
    public:
        log_process(log_conf & conf):_conf(conf)
    {

        init();
    }


        void log_write(LogType type, const char *format, ...);

        const log_conf & get_log_conf();

    protected:
        void init();

        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);


    protected:
        log_write_name _log_name[LOGSIZE];
        log_conf _conf;
};





#endif
