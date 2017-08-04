#ifndef __WS_DATA_PROCESS_H__
#define __WS_DATA_PROCESS_H__

#include "ws_data_process.h"
#include "web_socket_data_process.h"
#include "base_def.h"
#include "log_helper.h"

class ws_data_process:public web_socket_data_process
{
    public:
        
        virtual void msg_recv_finish()
        {

        }
};



#endif
