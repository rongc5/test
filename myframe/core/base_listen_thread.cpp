#include "base_listen_thread.h"
#include "base_net_thread.h"
#include "base_net_container.h"
#include "common_obj_container.h"
#include "base_listen_connect.h"
#include "common_def.h"



base_listen_thread::base_listen_thread():_listen_connect(NULL){
};
virtual base_listen_thread::~base_listen_thread(){
};

void base_listen_thread::init(const string &ip, unsigned short port)
{
    _ip = ip;
    _port = port;


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

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)         
    {
        THROW_COMMON_EXCEPT("socket error " << strerror(errno));     
    }
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr)); 

    if (::bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
    {        
        THROW_COMMON_EXCEPT("bind error "  << strerror(errno) << " " << ip << ":" << port);
    }        

    ret = listen(fd, 250);
    if (ret == -1)
    {
        THROW_COMMON_EXCEPT("listen error "  << strerror(errno));
    }

    set_unblock(fd); 
    _listen_connect = gen_listen_obj();
    _listen_connect->set_net_container(_base_container);
}

int base_listen_thread::add_worker_thread(uint32_t thread_index)
{
    _listen_connect->get_process()->add_worker_thread(thread_index);

    return 0;
}

