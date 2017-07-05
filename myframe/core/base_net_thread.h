#ifndef __BASE_NET_THREAD_H__
#define __BASE_NET_THREAD_H__

#include "common_def.h"
#include "base_thread.h"
#include "common_obj_container.h"
#include "net_obj.h"
#include "base_connect.h"

class base_net_thread:public base_thread
{
    public:
        base_net_thread():_channelid(0), _base_container(NULL){
            _base_container = new common_obj_container(this);
        };
        virtual ~base_net_thread(){
            if (_base_container){
                delete _base_container;
            }

            close(_channelid);
        };

        virtual void *run();

        virtual void init();

        virtual void put_msg(normal_obj_msg * p_msg);

        static void put_obj_msg(normal_obj_msg * p_msg);

        virtual void routine_msg();

        /************* *****************/

        virtual void handle_new_fd(pass_msg * p_msg);

        virtual void handle_new_connect(pass_msg * p_msg);

        virtual void handle_new_msg(pass_msg * p_msg);

        static base_net_thread * get_base_net_thread_obj(uint32_t thread_index);
    protected:
        const ObjId & gen_id_str();

        void set_channelid(int fd);

    protected:

        int _channelid;
        typedef typename deque<pass_msg*>::iterator dItr;
        base_net_container * _base_container;
        ObjId _id_str;
        deque<pass_msg *> _queue;
        thread_mutex_t _base_net_mutex;

        typedef typename map<uint32_t, base_net_thread *>::iterator bntMapIter;
        static map<uint32_t, base_net_thread *> _base_net_thread_map;
};

#endif
