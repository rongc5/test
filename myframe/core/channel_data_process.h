#ifndef _CHANNEL_DATA_PROCESS_H_
#define _CHANNEL_DATA_PROCESS_H_

#include "common_def.h"
#include "base_data_process.h"


class channel_data_process:public base_data_process
{
    public:
        channel_data_process(void *p);

        virtual ~channel_data_process()
        {
        }	

        virtual size_t process_recv_buf(char *buf, size_t len);
};

#endif

