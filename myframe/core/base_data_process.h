#ifndef __BASE_DATA_PROCESS_H__
#define __BASE_DATA_PROCESS_H__

#include "common_msg_process.h"
#include "common_def.h"
#include "log_helper.h"


class base_data_process
{
    public:
        base_data_process(void *p)
        {
            _p_msg_process = (common_msg_process<base_data_process>*)p;			
        }

        virtual ~base_data_process(){}

        virtual size_t process_recv_buf(char *buf, size_t len)
        {
            PassMsg pass_msg;
            pass_msg.ParseFromArray(buf, len);
            
            CommonMsg msg;
            char * ptr = (char *)pass_msg.str().c_str();
            msg.ParseFromArray(ptr + 4, *ptr);

            LOG_DEBUG("%s\n", msg.str().c_str());

            //write(1, buf, len);
            return len;
        }

        static base_data_process* gen_process(void *p) 
        {   
            base_data_process *p_tmp = new base_data_process(p);
            return p_tmp;
        }   

    protected:	
        common_msg_process<base_data_process> *_p_msg_process;
};

#endif

