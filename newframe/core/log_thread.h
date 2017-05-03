#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "common_def.h"
#include "base_net_thread.h"

class log_thread : public base_net_thread
{
    public:
        log_thread(log_conf & conf):base_net_thread(LOG_QUEUE_NUM), _conf(conf)
    {
        if (_conf.log_path[0] != '0') {
            char buf[SIZE_LEN_256];
            snprintf(buf, sizeof(buf), "mkdir -p %s", _conf.log_path);
            system(buf);
        }
    }

        
        virtual void add_msg(base_passing_msg * p_msg);

        virtual bool handle_msg(base_passing_msg * msg);
    
        void log_write(log_msg * msg);

        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

        const log_conf & get_log_conf();

    protected:
        log_conf _conf;
};


#endif


