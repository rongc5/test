#ifndef __SKHTTP_REQ_MASTER_H_
#define __SKHTTP_REQ_MASTER_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "skhttp_req_data_process.h"
#include "http_req_process.h"
#include "out_connect.h"

class skhttp_req_master:public base_net_thread
{
    public:
        virtual void handle_msg(normal_msg * p_msg);

        int add_worker_thread(uint32_t thread_index);

    protected:
        vector<uint32_t> _req_thread_vec;
};




#endif