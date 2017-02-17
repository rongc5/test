#include "common_def.h"


int get_date_str(char dest[], size_t dest_len, const char * format)
{
    if (!dest || !dest_len || !format){
        return -1;
    }   

    time_t now;
    struct tm *tm_now;

    time(&now);
    tm_now = localtime(&now);
    strftime(dest, dest_len, format, tm_now);

    return 0;
}


uint64_t GetTickCount()
{
    timeval tm; 
    gettimeofday(&tm, NULL);
    uint64_t now = tm.tv_sec * 1000 + tm.tv_usec/1000;
    return now;
}

bool operator<(const ObjId & oj1, const ObjId & oj2)
{
    if (oj1.obj_id() != oj2.obj_id()){
        return oj1.obj_id() < oj2.obj_id();
    } else if (oj1.thread_index() != oj2.thread_index()) {
        return oj1.thread_index() < oj2.thread_index();
    }else 
        return false;
}

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

    int num = zmq_recv(zmq_fd, buf, len, 0);
    if (num > 0){
        buf[num] = '\0';     
    }

    return num;
}

int zmq_client::init(char *path)
{
    zmq_base::init(path);

    context = zmq_ctx_new();
    zmq_fd = zmq_socket(context, ZMQ_DEALER);
    zmq_connect(zmq_fd, path);

    return 0;
}


int zmq_server::init(char * path)
{
    zmq_base::init(path);

    context = zmq_ctx_new();
    zmq_fd = zmq_socket(context, ZMQ_ROUTER);
    zmq_bind(zmq_fd, path);

    return 0;
}

