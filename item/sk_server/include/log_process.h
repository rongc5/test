#ifndef __LOG__PROCESS_H__
#define __LOG__PROCESS_H__

#include "common_def.h"
#include "log_helper.h"

struct log_write_name
{
    char _name[SIZE_LEN_512];
    std::mutex _mutex;
};

class log_process
{
    public:
        log_process(log_conf & conf):_conf(conf);

        static void log_write(LogType type, const char *format, ...);

        const log_conf & get_log_conf();

    protected:
        void log_thread_init();

        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

    protected:
        log_write_name _log_name[LOGSIZE];
        log_conf _conf;
};





#endif