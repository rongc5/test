#ifndef __BASE_NET_OBJ_H__
#define __BASE_NET_OBJ_H__

#include "common_util.h"

class base_data_process;
class common_obj_container;
class base_net_obj: public std::enable_shared_from_this<base_net_obj>
{
    public:
        base_net_obj();

        virtual ~base_net_obj();

        virtual void event_process(int events) = 0;

        bool get_real_net();
        void set_real_net(bool real_net);

        virtual int real_net_process()=0;

        virtual void set_net_container(common_obj_container *p_net_container);

        common_obj_container * get_net_container();

        virtual void update_event(int event);    
        int get_event();

        virtual void notice_send();

        int get_sfd();

        void set_id(const ObjId & id_str);
        const ObjId & get_id();

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        void add_timer(std::shared_ptr<timer_msg> & t_msg);
        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void destroy();

        net_addr & get_peer_addr();

    protected:
        void add_timer();

    protected:
        common_obj_container *_p_net_container;
        int _epoll_event;
        int _fd;	
        ObjId _id_str;
        bool _real_net;
        std::vector<std::shared_ptr<timer_msg> > _timer_vec;

        net_addr _peer_net;
};



#endif

