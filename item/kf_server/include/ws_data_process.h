#ifndef __WS_DATA_PROCESS_H__
#define __WS_DATA_PROCESS_H__

#include "web_socket_data_process.h"
#include "base_def.h"
#include "log_helper.h"
#include "web_socket_process.h"

class ws_data_process:public web_socket_data_process
{
    public:

        ws_data_process(web_socket_process *p);
        
        virtual void msg_recv_finish();
};



#endif
