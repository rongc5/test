#include "client_connect.h"


client_connect * client_connect::gen_connect(const string &ip, unsigned short port, base_net_thread * thread)
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

    client_connect * l_conn = new client_connect(_fd, thread);    
    l_conn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(l_conn);

    l_conn->send_request();

    return l_conn;
}

void client_connect::send_request()
{
    char buf[SIZE_LEN_8192];

    CommonMsg msg;

    msg.set_obj_id(1);
    msg.set_obj_op(1);
    msg.set_version(1);
    msg.set_reserved(0);

    sprintf(buf, "%s", "hello, this is test !!!!!!!!!!!!!!");
    msg.set_str(buf);

    string out;
    msg.SerializeToString(&out);

    int length = out.size();
    memset(buf, 0, sizeof(buf));

    memcpy(buf, &length, sizeof(length));
    memcpy(buf+sizeof(length), out.c_str(), out.size());

    write(_fd, buf, sizeof(length) + length);

    LOG_DEBUG("send_request");
}

void client_connect::call_back(int fd, short ev, void *arg)
{
    char tmp[SIZE_LEN_8192];

    memset(tmp, 0, sizeof(tmp));
    read(_fd, tmp, sizeof(tmp));

    //LOG_DEBUG("recv: %s", tmp);
    printf("recv: %s\n", tmp);
    
    sleep(3);

    send_request();
}


