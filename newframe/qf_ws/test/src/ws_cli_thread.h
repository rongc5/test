#ifndef __WS_CLI_THREAD_H__
#define __WS_CLI_THREAD_H__

#include "base_net_thread.h"
#include "common_def.h"

class ws_cli_thread: public base_net_thread
{
    public:
        ws_cli_thread(){
        };
        virtual ~ws_cli_thread(){
        };

        void init(const string &ip, unsigned short port);

        virtual bool handle_msg(base_passing_msg * msg);

    protected:

        string _ip;
        unsigned short _port;
};

#endif
