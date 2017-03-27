#ifndef _TEST_DATA_PROCESS_H_
#define _TEST_DATA_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "base_data_process.h"

class test_data_process:public base_data_process
{
    public:
        test_data_process(void *p):base_data_process(p)
        {
        }

        virtual ~test_data_process()
        {
        }	


        virtual size_t process_recv(pass_msg * p_msg);

};

#endif

