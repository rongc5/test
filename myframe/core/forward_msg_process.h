#ifndef _FORWARD_MSG_PROCESS_H_
#define _FORWARD_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "log_helper.h"
#include "base_msg_process.h"


class forward_obj_container;
class forward_msg_process:public base_msg_process
{
    public:
        forward_msg_process(void *p):base_msg_process(p),_container(NULL)
        {
        }

        virtual ~forward_msg_process()
        {
        }	

        virtual size_t process_recv_buf(char *buf, size_t len);


        size_t process_s(char *buf, size_t len);

        void put_msg(char *buf, size_t len);

        void set_forward_container(forward_obj_container * container);

    protected:
        forward_obj_container * _container;

};

#endif

