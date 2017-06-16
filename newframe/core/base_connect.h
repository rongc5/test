#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "log_helper.h"
#include "common_def.h"
#include "base_net_thread.h"

class base_data_process;
class base_net_thread;
class base_connect
{
    public:

        base_connect(int32_t sock, base_net_thread * thread);

        virtual ~base_connect()
        {
            close();
        }

        void close();

        int get_sock();

        void get_local_addr(sockaddr_in &addr);

        static void on_cb(int fd, short ev, void *arg);

        bool update_event(short ev);

        short get_ev_flags();

        virtual size_t process_recv_msg(base_passing_msg* p_msg);;

        const ObjId & get_id();

        void set_process(base_data_process *p);

    protected:

        virtual void call_back(int fd, short ev, void *arg) = 0;

    protected:
        int _fd;
        short _ev;
        struct event _event;			

        base_net_thread * _thread;
        ObjId _id_str;

        base_data_process *_process;
};


#endif
