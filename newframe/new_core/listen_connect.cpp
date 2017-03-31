#include "listen_connect.h"


listen_connect * listen_connect::gen_connect(const string &ip, unsigned short port, base_net_thread * thread)
{
    struct sockaddr_in address;
    int reuse_addr = 1;

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
        LOG_WARNING("socket error %s", strerror(errno));     
        return NULL;
    }
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr));

    if (::bind(_fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
    {        
        LOG_WARNING("bind error %s ip: %s port: %d", strerror(errno), ip.c_str(), port);
        return NULL;
    }        

    ret = listen(_fd, 250);
    if (ret == -1)
    {
        LOG_WARNING("listen error %s", strerror(errno));
        return NULL;
    }

    set_unblock(_fd);

    listen_connect * l_conn = new listen_connect(_fd, thread);    
    l_conn->init_ev(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(l_conn);

    return l_conn;
}


void listen_connect::call_back(int fd, short ev, void *arg)
{
    int tmp_sock = 0;
    sockaddr_in addr;   
    socklen_t len = 0;
    while((tmp_sock = accept(_fd, (sockaddr*)&addr, &len)) != -1)
    {               
        LOG_DEBUG("recv fd[%d]\n", tmp_sock);
        _thread->handle_new_fd(tmp_sock);
    }
}


