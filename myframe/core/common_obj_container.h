#ifndef __COMMON_OBJ_CONTAINER_H__
#define __COMMON_OBJ_CONTAINER_H__

#include "common_def.h"
#include "common_epoll.h"

class base_timer;
class common_domain;
class common_obj_container
{
    public:
        common_obj_container(uint32_t thread_index, uint32_t epoll_size=DAFAULT_EPOLL_SIZE);

        ~common_obj_container();

        bool push_real_net(std::shared_ptr<base_net_obj> & p_obj);
        bool remove_real_net(std::shared_ptr<base_net_obj> & p_obj);

        std::shared_ptr<base_net_obj> find(uint32_t obj_id);

        bool insert(std::shared_ptr<base_net_obj> & p_obj);
        void erase(uint32_t obj_id);

        void handle_msg(uint32_t obj_id, std::shared_ptr<normal_msg> & p_msg);

        void obj_process();

        common_epoll *get_epoll();

        base_timer * get_timer();

        common_domain * get_domain();

        void add_timer(std::shared_ptr<timer_msg> & t_msg);

        void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        uint32_t get_thread_index();

        uint32_t size();

    protected:
        const ObjId & gen_id_str();

    protected:
        std::unordered_map<uint32_t, std::shared_ptr<base_net_obj> > _obj_map;
        std::unordered_map<uint32_t, std::shared_ptr<base_net_obj> > _obj_net_map;

        common_epoll *_p_epoll;
        base_timer * _timer;
        common_domain * _domain;
        ObjId _id_str;
};

#endif

