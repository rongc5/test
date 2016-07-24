#include <string.h>
#include "asp_tool.h"


int zmq_base::init(char * path)
{
    if (!path || !strlen(path)){
        return -1; 
    }

    snprintf(zeromq_address, PATH_MAX_LEN, "%s", path);

    context = NULL;
    zmq_fd = NULL;

    return 0;
}

int zmq_base::do_send(char * buf, size_t len)
{
     int rc = zmq_send(zmq_fd, buf, len, 0);

     return rc;
}

int zmq_base::do_recv(char * buf, size_t len)
{

    int num = zmq_recv(zmq_fd, buf, 128, 0);
    if (num > 0){
        buf[num] = '\0';
    }

    return num;
}

int zmq_client::init(char *path)
{
    zmq_base::init(path);

    context = zmq_ctx_new();
    zmq_fd = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(zmq_fd, path);

    return 0;
}

int zmq_server::init(char * path)
{
    zmq_base::init(path);

    context = zmq_ctx_new();
    zmq_fd = zmq_socket(context, ZMQ_PAIR);
    zmq_bind(zmq_fd, path);

    return 0;
}

