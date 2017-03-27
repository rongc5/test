#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"

class base_net_thread:public base_thread
{
    public:
        base_net_thread():_channelid(0){
            _base = event_base_new()
        };

        virtual ~base_net_thread(){
            close(_channelid);
        };

        virtual void *run();

        virtual void init();

        virtual void put_msg(int fd);

        virtual void routine_msg();

        static void channel_cb(int fd, short ev, void *arg);

        /************* *****************/

        virtual void handle_new_fd(int fd);

    protected:
        void set_channelid(int fd);

    protected:
        struct event_base* _base;
        int _channelid;
        deque<fd> _queue;
        thread_mutex_t _base_net_mutex;
};

#endif
