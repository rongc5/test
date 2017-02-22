#ifndef __FORWARD_OBJ_CONTAINER_H__
#define __FORWARD_OBJ_CONTAINER_H__

#include "common_def.h"
#include "common_epoll.h"
#include "common_obj_container.h"
#include "forward_msg_process.h"
#include "base_connect.h"
#include "common_thread.h"


class forward_obj_container:public common_obj_container
{
    public:
        forward_obj_container()
        {
        }

        virtual ~forward_obj_container()
        {
        }

        int add_forward_thread(common_thread *thread, const ObjId & id_str)
        {
            int fd[2], ret;

            ret = socketpair(AF_UNIX,SOCK_STREAM,0,fd);
            if (ret < 0) {
                LOG_WARNING("socketpair fail errstr[%s]", strerror(errno));
                return -1;
            }   

            NET_OBJ *p_connect = gen_forward_connect(fd[1], EPOLL_LT_TYPE);
            if (p_connect){
                p_connect->set_id(id_str);
                p_connect->set_net_container(this);
                set_dest_channelid(thread->get_thread_index(), fd[1]);
                thread->set_channelid(fd[0]);
            }

            return 0;
        }


        NET_OBJ * gen_forward_connect(const int fd, EPOLL_TYPE epoll_type)
        {
            NET_OBJ * p_connect = NULL;
            p_connect = new base_connect<forward_msg_process>(fd, epoll_type);
            forward_msg_process *process = new forward_msg_process((NET_OBJ*)p_connect);

            process->set_forward_container(this);
            base_connect<forward_msg_process> * tmp_con = (base_connect<forward_msg_process> *)p_connect;
            tmp_con->set_process(process);

            return p_connect;
        }

        void set_dest_channelid(uint32_t thread_index, uint32_t channelid)
        {
            _forward_channelid_map[thread_index] = channelid;
        }

        uint32_t get_dest_channelid(uint32_t thread_index)
        {
            map<uint32_t,uint32_t >::iterator it; 
            it = _forward_channelid_map.find(thread_index);
            if (it != _forward_channelid_map.end()){
                return it->second;
            }   

            return 0;            
        }


    protected:
        map<uint32_t, uint32_t> _forward_channelid_map;
};

#endif

