#ifndef __CHANNEL_DATA_PROCESS_H__
#define __CHANNEL_DATA_PROCESS_H__

#include "base_def.h"
#include "common_def.h"
#include "channel_msg_process.h"


class channel_data_process
{
    public:
        channel_data_process(void *p);

        virtual ~channel_data_process(){}

        virtual size_t process_recv_buf(char *buf, size_t len);

        static channel_data_process* gen_process(void *p);

    protected:	
        channel_msg_process<channel_data_process> *_p_msg_process;
};


#endif

