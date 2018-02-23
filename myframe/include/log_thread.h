#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "base_thread.h"
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

class log_thread:public base_thread
{
    public:
        log_thread(log_conf & conf);
        virtual ~log_thread();

        static void log_write(log_prefix & prefix, const char *format, ...);

        void handle_msg(log_msg * p_msg);

        const log_conf & get_log_conf();

        virtual void *run();

        void obj_process();

        void put_msg(log_msg * p_msg);
        
     private:
        
        void log_thread_init();
        
        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

        int RECV(int fd, void *buf, size_t len);
           
        size_t process_recv_buf(const char *buf, const size_t len);

    private:
        log_write_name _log_name[LOGSIZE];
        log_conf _conf;
        int _epoll_fd; 
        thread_mutex_t _mutex[2];
        deque<log_msg *> _queue[2];
        int _channelid;
        struct epoll_event *_epoll_events;
        uint32_t _epoll_size;
        string _recv_buf;
        int _current;
};



#endif
