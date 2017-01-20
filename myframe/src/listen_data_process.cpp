#include "listen_data_process.h"
#include "listen_thread.h"
#include "base_thread_mgr.h"
#include "net_obj.h"


listen_data_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
    _thread = NULL;
}


size_t listen_data_process::process(int fd)
{

    if (_thread){

        string tmp_str;
        _pass_msg_t head;
        head._dst_obj._thread_id = _thread->get_worker_id();
        head._src_obj = _p_connect->get_id();
        head._op = ADD_NEW_SOCEKT;
        head.len = sizeof(fd);

        memcpy((void*)tmp_str.c_str(), &head, sizeof(head));
        memcpy((char *)tmp_str.c_str()+tmp_str.length(), &fd, sizeof(fd));

        int ret = write(_thread->get_channelid(), tmp_str.c_str(), tmp_str.length());
        PDEBUG("channelid[%d] fd[%d] %d\n", _thread->get_channelid(), fd, ret);
    }

    return 0;
}

void listen_data_process::set_thread(listen_thread * thread)
{
    _thread = thread;    
}


