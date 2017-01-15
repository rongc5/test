#ifndef __NET_OBJ_H__
#define __NET_OBJ_H__

#include "base_def.h"
#include "common_def.h"


namespace MZFRAME {

class common_epoll;
class base_net_container;
class base_net_obj
    {
        public:
            base_net_obj();

            virtual ~base_net_obj();

            virtual int real_net_process() = 0;
            virtual void event_process(const int32_t events) = 0;
            virtual int destroy() = 0;

            virtual void set_net_container(base_net_container *p_net_container);
            virtual size_t process_recv_buf(char *buf, size_t len) = 0;
            virtual void process_send_buf(string * buf) = 0;
            base_net_container * get_net_container();

            virtual int get_event();

            int get_sock();

            int set_id(const obj_id_str & id_str);

            const obj_id_str & get_id();

        protected:
            base_net_container *_p_net_container;
            common_epoll *_p_epoll;
            int _epoll_event;
            int _fd;	
            obj_id_str _id_str;
    };

    class NET_OBJ:public base_net_obj
    {
        public:
            NET_OBJ();

            virtual ~NET_OBJ();
            virtual void close()=0;	
            virtual void get_local_addr(sockaddr_in &addr)=0;

            /*******************************************************/	
            void get_peer_addr(sockaddr_in &addr);

        protected:
            sockaddr_in _peer_addr;	
    };

}


#endif

