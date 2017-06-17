#include "ws_client_data_process.h"
#include "base_net_thread.h"
#include "log_helper.h"


ws_client_data_process * ws_client_data_process::gen_connect(const string &ip, unsigned short port, base_net_thread * thread)
{
    struct sockaddr_in address;

    memset((char *) &address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    int ret = 0;

    if (ip != "")
    {
        inet_aton(ip.c_str(), &address.sin_addr);
    }
    else
    {
        address.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    int _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) 
    {
        //LOG_WARNING("socket error %s", strerror(errno));     
        return NULL;
    }
    
    int len =  sizeof(address);
    ret = connect(_fd, (struct sockaddr *)&address, len);
    if (-1 == ret)
    {   //LOG_WARNING("connect"); 
        exit(1);
    }   

    set_unblock(_fd);

    ws_client_data_process * l_conn = new ws_client_data_process(_fd, thread);    
    l_conn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(l_conn);
    
    ws_req_head_para req_para;

    req_para._s_path = "/";
    req_para._s_host = "www.yiyi.com";
    req_para._s_websocket_key = "zEkljJfDq59X6c+8BZx7yw==";
    req_para._origin = "yiyi.com";
    req_para._version = 13;

    l_conn->set_req_para(req_para);

    l_conn->notice_send();

    return l_conn;
}

void ws_client_data_process::send_request()
{
    string *tmp = new string;
    tmp->append("hello, this is test !!!!!!!!!!!!!!");
    ws_msg_type msg;
    msg._p_msg = tmp;
    msg._con_type = 0x01;

    put_send_msg(msg);

    LOG_DEBUG("send_request");
}

void ws_client_data_process::on_handshake_ok()
{
    send_request();
}

void ws_client_data_process::msg_recv_finish()
{
    LOG_DEBUG("recv: %s", _recent_msg.c_str());
    
    sleep(3);

    send_request();
}


