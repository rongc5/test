#ifndef _COMMON_MSG_PROCESS_H_
#define _COMMON_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"

class common_msg_process:public base_msg_process
{
    public:
        common_msg_process(void *p)
        {
        }

        virtual ~common_msg_process()
        {
        }	


        size_t process_s(char *buf, size_t len)
        {
            return len;
        }

        void put_msg(string *p_msg)
        {
            if (!p_msg || p_msg->length() < sizeof(_pass_msg_t)){
                //LOG_WARN
                delete p_msg;
                return;
            }

            obj_id_str obj_id = _p_connect->get_id();
            obj_id._obj_id = 1;
            base_net_container * net_container = _p_connect->get_net_container();
            base_net_obj * net_obj = net_container->find(&obj_id);
            net_obj->process_send_buf(p_msg);
        }

};
#endif

