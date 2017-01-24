#include "listen_data_process.h"
#include "listen_thread.h"
#include "base_thread_mgr.h"
#include "net_obj.h"
#include "myframe.pb.h"


listen_data_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
    _thread = NULL;
}


size_t listen_data_process::process(int fd)
{

    if (_thread){


        com::mingz::frame::PassMsgHead head;
        com::mingz::frame::ObjId *src_id, *dest_id;

        src_id = head.mutable_dst_id();
        *src_id = _p_connect->get_id();
        dest_id = head.mutable_src_id();
        *dest_id = _thread_id->get_worker_id();
        head.set_cmd(ADD_NEW_SOCEKT);

        char tmp[SIZE_LEN_16];
        snprintf(tmp, sizeof(tmp), "%d", fd);

        head.set_str(tmp);

        head.body_len = sizeof(fd);

        
        memcpy((void*)tmp_str.c_str(), &head, sizeof(head));
        PDEBUG("str: %s, [%d]\n", tmp_str.c_str(), sizeof(head));
        memcpy((char *)tmp_str.c_str()+tmp_str.length(), &fd, sizeof(fd));

        int ret = write(_thread->get_channelid(), tmp_str.c_str(), tmp_str.length());
        //int ret = write(_thread->get_channelid(), "hello world", sizeof("hello world"));
        PDEBUG("channelid[%d] fd[%d] %d\n", _thread->get_channelid(), fd, ret);
    }

    return 0;
}

void listen_data_process::set_thread(listen_thread * thread)
{
    _thread = thread;    
}


