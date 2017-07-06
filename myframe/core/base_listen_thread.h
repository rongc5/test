#ifndef __BASE_LISTEN_THREAD_H__
#define __BASE_LISTEN_THREAD_H__

#include "base_net_thread.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_data_process.h"


class base_listen_thread: public base_net_thread
{
    public:
        base_listen_thread();

        virtual ~base_listen_thread();

        void init(const string &ip, unsigned short port);

        int add_worker_thread(uint32_t thread_index);

        virtual base_listen_connect * gen_listen_obj(int fd) = 0;

    protected:

        string _ip;
        unsigned short _port;
        base_listen_connect *_listen_connect;
};

#endif
