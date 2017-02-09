#include "listen_data_process.h"
#include "listen_thread.h"
#include "base_thread_mgr.h"
#include "net_obj.h"
#include "myframe.pb.h"
#include "common_def.h"


listen_data_process::listen_data_process(void *p)
{
    _p_connect = (base_net_obj*)p;
    _thread = NULL;
}


size_t listen_data_process::process(int fd)
{

    if (_thread){


        PassMsg head;
        ObjId *src_id, *dest_id;

        src_id = head.mutable_src_id();
        *src_id = _p_connect->get_id();
        dest_id = head.mutable_dst_id();
        dest_id->set_thread_index(_thread->get_worker_id());
        PDEBUG("dest get_thread_index[%d], listen_thread[%d]\n", _thread->get_worker_id(), _thread->get_thread_index());
        dest_id->set_obj_id(0);
        head.set_cmd(ADD_NEW_SOCEKT);

        char tmp[SIZE_LEN_16];
        snprintf(tmp, sizeof(tmp), "%d", fd);
        head.set_str(tmp);

        char buf[SIZE_LEN_256] = {0};

        string out;
        head.SerializeToString(&out);

        int len = out.size();

        memcpy(buf, &len, sizeof(len));
        memcpy(buf + sizeof(len), out.c_str(), out.size());

        write(_thread->get_channelid(), buf, sizeof(len) + out.size());
        PDEBUG("write to fd [%d] fd size[%d]\n", _thread->get_channelid(), sizeof(len)+ out.size());
        //int ret = write(_thread->get_channelid(), "hello world", sizeof("hello world"));
    }

    return 0;
}

void listen_data_process::set_thread(listen_thread * thread)
{
    _thread = thread;    
}


