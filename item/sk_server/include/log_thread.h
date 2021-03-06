#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "base_thread.h"
#include "common_def.h"

struct log_write_name
{
    char _name[SIZE_LEN_512];
};

class log_thread:public base_thread
{
    public:
        log_thread(log_conf & conf);
        virtual ~log_thread();

        static void log_write(LogType type, const char *format, ...);
        static void log_write(const char * filename, const char *format, ...);

        void handle_msg(std::shared_ptr<log_msg> & p_msg);

        const log_conf & get_log_conf();

        void set_type(LogType type);

        virtual void *run();

        void obj_process();

        void put_msg(std::shared_ptr<log_msg> & p_msg);
        
     private:
        
        void log_thread_init();
        
        void get_file_name(LogType type, char dest[], size_t dest_len);

        void check_to_renmae(const char *filename, int max_size);

        int RECV(int fd, void *buf, size_t len);
           
        size_t process_recv_buf(const char *buf, const size_t len);

     public:
        std::string _proc_name;
    private:
        log_write_name _log_name[LOGSIZE];
        log_conf _conf;
        int _epoll_fd; 
        std::mutex _mutex;
        std::deque<std::shared_ptr<log_msg> > _queue[2];
        int _channelid;
        struct epoll_event *_epoll_events;
        uint32_t _epoll_size;
        std::string _recv_buf;
        volatile int _current;
};



#endif
