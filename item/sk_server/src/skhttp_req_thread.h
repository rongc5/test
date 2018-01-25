#ifndef __SKHTTP_REQ_THREAD_H_
#define __SKHTTP_REQ_THREAD_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "skhttp_req_data_process.h"

class skhttp_req_thread:public base_net_thread
{
    public:
        virtual void handle_msg(normal_msg * p_msg)
        {
            if (!p_msg) 
            {
                return;
            }

            if (p_msg->_msg_op == MSG_HTTP_REQ) 
            {
                out_connect<http_req_process> * connect = skhttp_req_data_process::gen_net_obj(req_msg);
                connect->set_net_container(_base_container);
                LOG_DEBUG("set http_req_process");
            }
            else
            {
                REC_OBJ<normal_msg> rc(p_msg);
                return;
            }

        }

};




#endif
