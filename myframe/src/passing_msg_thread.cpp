#include "passing_msg_thread.h"
#include "net_obj.h"
#include "common_thread.h"
#include "common_obj_container.h"
#include "passing_msg_process.h"
#include "passing_data_process.h"
#include "base_connect.h"
#include "log_helper.h"
#include "base_net_container.h"

namespace MZFRAME {


    passing_msg_thread::passing_msg_thread():_net_container(NULL){
        _net_container = new (std::nothrow)common_obj_container();
    }

    passing_msg_thread::~passing_msg_thread(){
        if (_net_container){
            delete _net_container;
        }
    }

    static int passing_msg_thread::register_thread(common_thread *thread)
    {
        if (!thread) {
            return -1;
        }

        passing_msg_thread * pass_thread = base_singleton<passing_msg_thread>::get_instance();
        if (!passing_msg_thread) {
            base_singleton<passing_msg_thread>::set_instance(new passing_msg_thread());

            pass_thread = base_singleton<passing_msg_thread>::get_instance();
        }

        int fd[2], ret;

        ret = socketpair(AF_UNIX, 0, SOCK_STREAM, fd);
        ASSERT_DO(ret != -1, LOG_WARNING("socketpair fail errstr[%s]", strerror(errno)));

        struct sockaddr_in sa;
        if(!getpeername(fd[0], (struct sockaddr *)&sa, &len))
        {
            LOG_WARNING("getpeername fail errstr[%s]", strerror(errno)); 
        }

        NET_OBJ *p_connect = gen_connect(fd[0], sa);
        if (p_connect){
            obj_id_str id_str;
            id_str._thread_id = thread->get_thread_id();
            base_net_container * net_container = passing_msg_thread->get_net_container();
            net_container->push_net_obj(p_connect);
            thread->set_channelid(fd[1]);
        }

        return 0;
    }


    void* msg_passing_thread::run()
    {
        while (get_run_flag())
        {
            _net_container->obj_process();
        }

        return NULL;
    }

    NET_OBJ * msg_passing_thread::gen_connect(const int fd, EPOLL_TYPE epoll_type)
    {
        NET_OBJ * p_connect = NULL;
        p_connect = new base_connect<passing_msg_process<passing_data_process> >(fd, epoll_type);
        passing_msg_process<passing_data_process> *process = new passing_msg_process<passing_data_process>((NET_OBJ*)p_connect);
        
        base_connect<passing_msg_process<passing_data_process> > * tmp_con = (base_connect<passing_msg_process<passing_data_process> > *)p_connect;
        tmp_con->set_process(process);

        return p_connect;
    }



}
