#ifndef __LOG_THREAD_H__
#define __LOG_THREAD_H__

#include "base_thread.h"
#include "common_def.h"
#include "base_singleton.h"
#include "log_base.h"

class log_msg
{
    public:
        log_msg()
        {
            _buf = NULL;
            _logid = 0;
        }
        virtual ~log_msg()
        {
            if (_buf){
                delete _buf;
                _buf = NULL;
            }
        }

        LogType _type;
        std::string _fname;
        std::vector<char> * _buf;
        uint64_t _logid;
};

class log_thread:public base_thread
{
    public:
        log_thread();
        virtual ~log_thread();

        static void init(const char * path);

        static void log_write(LogType type, const char *format, ...);
        static void log_write(const char * filename, const char *format, ...);

        static uint64_t get_logid();

        void handle_msg(std::shared_ptr<log_msg> & p_msg);

        virtual void *run();

        void obj_process();

        void put_msg(std::shared_ptr<log_msg> & p_msg);

        bool check_type(LogType type);
        
     private:
        
        void log_thread_init(const char * path);

        void log_conf_init();

        void check_to_renmae(const char *filename, int max_size);

        int RECV(int fd, void *buf, size_t len);
           
        size_t process_recv_buf(const char *buf, const size_t len);


     public:
        std::string _proc_name;
    private:
        int _epoll_fd; 
        std::mutex _mutex;
        std::deque<std::shared_ptr<log_msg> > _queue[2];
        int _channelid;
        struct epoll_event *_epoll_events;
        uint32_t _epoll_size;
        std::string _recv_buf;
        volatile int _current;
        reload_mgr<log_conf> * _rlog_conf; 

        static uint64_t log_global_id;
};


#endif
