#ifndef _PASSING_MSG_THRAD_H__
#define _PASSING_MSG_THRAD_H__

#include "base_def.h"
#include "base_thread.h"
#include "common_def.h"
#include "net_obj.h"
#include "common_thread.h"


class base_net_container;
class passing_msg_thread : public base_thread
{
    public:
        passing_msg_thread();
        virtual ~passing_msg_thread();

        static int register_thread(common_thread *thread);

        virtual void* run();

        base_net_container * get_net_container();

        common_thread * get_dest_thread(uint32_t thread_index);
        base_net_obj * get_dest_obj(uint32_t thread_index);

        void set_dest_obj(common_thread * thread, base_net_obj * p_obj);

        NET_OBJ * gen_connect(const int fd, EPOLL_TYPE epoll_type);
        const ObjId & gen_id_str();

    protected:
        base_net_container * _net_container;
        ObjId _id_str;
        map<uint32_t, base_net_obj *> _net_obj_map;
        map<uint32_t, common_thread *> _thread_obj_map;
};




#endif
