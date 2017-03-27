#ifndef __TEST_NET_THREAD_H__
#define __TEST_NET_THREAD_H__

#include "common_def.h"
#include "base_net_thread.h"

class test_net_thread:public base_net_thread
{
    public:


        virtual void handle_new_msg(pass_msg * p_msg);

};

#endif
