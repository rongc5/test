#ifndef __NET_OBJ_H__
#define __NET_OBJ_H__

#include "common_def.h"


class common_epoll;
class base_net_container;
class base_net_obj: public normal_msg
{
    public:
        base_net_obj();

        virtual ~base_net_obj();

        virtual int real_net_process() = 0;
        virtual void event_process(const int32_t events) = 0;
        virtual int destroy() = 0;

        virtual void set_net_container(base_net_container *p_net_container);
        base_net_container * get_net_container();

        virtual int get_event();

        int get_sock();

        int set_id(const ObjId & id_str);

        const ObjId & get_id();

    protected:
        base_net_container *_p_net_container;
        common_epoll *_p_epoll;
        int _epoll_event;
        int _fd;	
        ObjId _id_str;
};

class NET_OBJ:public base_net_obj
{
    public:
        NET_OBJ();

        virtual ~NET_OBJ();
        virtual void close()=0;	
        virtual void get_local_addr(sockaddr_in &addr)=0;
        virtual size_t process_s(pass_msg* p_msg) = 0;

        /*******************************************************/	
        void get_peer_addr(sockaddr_in &addr);

    protected:
        sockaddr_in _peer_addr;	
};



#endif

