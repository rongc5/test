#ifndef __SKHTTP_RES_THREAD_H_
#define __SKHTTP_RES_THREAD_H_

#include "base_net_thread.h"
#include "common_util.h"
#include "log_helper.h"
#include "base_connect.h"
#include "skhttp_res_data_process.h"

class skhttp_res_thread:public base_net_thread
{
    public:
        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg)
        {
           if (!p_msg) 
               return;

           if (p_msg->_msg_op == NORMAL_MSG_CONNECT)
           {
               std::shared_ptr<content_msg> p=std::dynamic_pointer_cast<content_msg>(p_msg);
               if (p)
               {
                    skhttp_res_data_process::gen_listen_obj(p->fd, _base_container);
               }
           }
        }

        //virtual void run_process();
};




#endif
