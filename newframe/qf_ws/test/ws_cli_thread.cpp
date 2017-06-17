#include "ws_cli_thread.h"
#include "ws_client_data_process.h"


void ws_cli_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;

    ws_client_data_process::gen_connect(ip, _port, this);
}


bool ws_cli_thread::handle_msg(base_passing_msg * p_msg)
{
    if (!p_msg) {
        return true;
    }

    REC_OBJ<base_passing_msg> rc(p_msg);

    return true;
}

