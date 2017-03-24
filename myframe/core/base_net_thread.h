#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "channel_data_process.h"
#include "base_thread.h"
#include "common_obj_container.h"
#include "net_obj.h"
#include "base_connect.h"


class base_net_thread:public base_thread
{
    public:
        base_net_thread():_channelid(0), _base_container(NULL){
            _base_container = new common_obj_container();
        };
        virtual ~base_net_thread(){
            if (_base_container){
                LOG_DEBUG("hello ");
                delete _base_container;
            }
        };

        virtual void *run();

        virtual void init();

        virtual void put_msg(pass_msg * msg);

        pass_msg* get_msg();

        virtual void deal_msg();

    protected:
        const ObjId & gen_id_str();

        void set_channelid(int fd);

    protected:

        int _channelid;
        typedef typename deque<pass_msg*>::iterator dItr;
        base_net_container * _base_container;
        ObjId _id_str;
        deque<pass_msg *> _queue;
        thread_mutex_t _mutex;
};

#endif
