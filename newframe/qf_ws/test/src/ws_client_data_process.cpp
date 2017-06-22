#include "ws_client_data_process.h"
#include "base_net_thread.h"
#include "log_helper.h"
#include "tcp_connect.h"
#include "base_singleton.h"
#include "user_info.h"

ws_client_data_process::ws_client_data_process(tcp_connect * t_cn):ws_req_data_process(t_cn)
{
}

void ws_client_data_process::gen_connect(const string &ip, unsigned short port, base_net_thread * thread)
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
        return ;
    }
    
    int len =  sizeof(address);
    ret = connect(_fd, (struct sockaddr *)&address, len);
    if (-1 == ret)
    {   //LOG_WARNING("connect"); 
        exit(1);
    }   

    set_unblock(_fd);

    tcp_connect * t_cn = new tcp_connect(_fd, thread);
    ws_client_data_process * qf_process = new ws_client_data_process(t_cn);
    t_cn->set_process(qf_process);
    t_cn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(t_cn);
    
    ws_req_head_para req_para;

    req_para._s_path = "/";
    req_para._s_host = "www.yiyi.com";
    req_para._s_websocket_key = "zEkljJfDq59X6c+8BZx7yw==";
    req_para._origin = "yiyi.com";
    req_para._version = 13;

    qf_process->set_req_para(req_para);

    qf_process->notice_send();
}

void ws_client_data_process::send_request(const char * buf)
{
    if (!buf || !strlen(buf)){
        LOG_DEBUG("sned buf is empty");
        return;
    }

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
    user_info * user = base_singleton<user_info>::get_instance();
    if (!user) {
        LOG_WARNING("user_info is NULL");
        exit(1);
    }

    if (user->op == 1) {
        printf("登录成功， 等待用户到来吧\n");
    }else {
        char buf[SIZE_LEN_1024] = {'\0'};
        printf("你好， 有什么能帮你的吗?\n");
        printf("输入你想说的话?\n:");
        scanf("%s", buf);
        send_request(buf);
    }
}

void ws_client_data_process::msg_recv_finish()
{
    
    LOG_DEBUG("recv: %s", _recent_msg.c_str());

    user_info * user = base_singleton<user_info>::get_instance();
    if (!user) {
        LOG_WARNING("user_info is NULL");
        exit(1);
    }
    
    printf("新消息到来: %s\n", _recent_msg.c_str());

    //sleep(3);
    //send_request();

    char buf[SIZE_LEN_1024] = {'\0'};
    printf("输入你想说的话?\n:");
    scanf("%s", buf);
    send_request(buf);

}


