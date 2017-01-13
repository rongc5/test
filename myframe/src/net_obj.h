#ifndef __NET_OBJ_H__
#define __NET_OBJ_H__

#include "base_def.h"
#include "common_def.h"

class common_epoll;
class base_net_container;
namespace MZFRAME {

    class base_net_obj
    {
        public:
            base_net_obj()
            {
                _fd = 0;
                _epoll_event = 0;
                _p_net_container = NULL;
                _p_epoll = NULL;
            }

            virtual ~base_net_obj()
            {
            }

            virtual int real_net_process() = 0;
            virtual void event_process(const int32_t events) = 0;
            virtual int destroy() = 0;

            virtual void set_net_container(base_net_container *p_net_container)
            {
                _p_net_container = p_net_container;
                _id_str = p_net_container->get_id_str();
                _p_epoll = p_net_container->get_epoll();
                _p_epoll->add_to_epoll(this);
            }
            virtual size_t process_recv_buf(char *buf, size_t len) = 0;
            virtual void process_send_buf(string * buf) = 0;
            base_net_container * get_net_container()
            {
                return _p_net_container;
            }


            virtual int get_event()
            {
                return _epoll_event;
            }

            int get_sock()
            {
                return _fd;
            }

            int set_id(obj_id_str & id_str)
            {
                _id_str = id_str;
                return 0;
            }

            const obj_id_str & get_id()
            {
                return _id_str;
            }


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
            NET_OBJ()
            {
                memset(&_peer_addr, 0, sizeof(_peer_addr));
                _p_epoll = NULL;
                _epoll_event = 0;
            }
            virtual ~NET_OBJ()
            {		
            }
            virtual void close()=0;	
            virtual void get_local_addr(sockaddr_in &addr)=0;


            /*******************************************************/	
            void get_peer_addr(sockaddr_in &addr)
            {
                addr = _peer_addr;
            }

        protected:
            sockaddr_in _peer_addr;	
    };


}


#endif

