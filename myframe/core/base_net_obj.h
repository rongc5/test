#ifndef __BASE_NET_OBJ_H__
#define __BASE_NET_OBJ_H__

#include "common_def.h"

class base_data_process;
class base_net_container;
class base_net_obj: public normal_msg
{
    public:
        base_net_obj(const int32_t sock);

        virtual ~base_net_obj();

        virtual void event_process(int events) = 0;
        virtual void close() = 0;

        virtual void set_net_container(base_net_container *p_net_container);
        base_net_container * get_net_container();

        void update_event(int event);    
        int get_event();

        int get_sfd();

        void set_id(const ObjId & id_str);
        const ObjId & get_id();

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg);
        virtual void  handle_timeout(const uint32_t timer_type);

    protected:
        base_net_container *_p_net_container;
        int _epoll_event;
        int _fd;	
        ObjId _id_str;
};



#endif

