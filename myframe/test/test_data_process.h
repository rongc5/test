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


        virtual size_t process_recv(pass_msg * p_msg)
        {
            REC_OBJ<pass_msg> rec(p_msg);


            LOG_DEBUG("_p_connect->get_id thread_index[%d]", _p_connect->get_id()._thread_index);
            string *str = new string();
            str->append("hello test");

            process_send(str);

            return 0;
        }

};

#endif

