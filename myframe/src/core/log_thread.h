#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "common_def.h"
#include "base_thread.h"


class log_thread : public base_thread
{
    public:
        log_thread(log_conf & conf):_list(NULL), _mutex(NULL), _conf(conf)
    {
        _list = new list<log_msg *>[_conf.bucketlen];
        _mutex = new thread_mutex_t[_conf.bucketlen];
    }

        virtual ~log_thread()
        {
           if (_list) {
                clear();
           } 

           delete [] _list;
           delete [] _mutex;
        }
        
        int add(log_msg * msg);

        static int put_msg(log_msg *msg);

        virtual void* run();
    
        void log_write(log_msg * msg);

        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

        void clear();

    protected:
        list<log_msg *> * _list;
        thread_mutex_t * _mutex;
        log_conf _conf;
};


#endif


