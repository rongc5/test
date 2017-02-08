#include "common_epoll.h"
#include "net_obj.h"
#include "base_net_container.h"
#include "common_net_thread.h"
#include "channel_msg_process.h"


size_t channel_msg_process::process_recv_buf(char *buf, size_t len)
{
    size_t left_len = len;
    while(left_len > 0)
    {
        RECV_MSG_STATUS status = RECV_MSG_HEAD;
        size_t msg_body_len = 0;
        _head_len = sizeof(int);
        if (status == RECV_MSG_HEAD)
        {
            if (left_len > _head_len)
            {
                int *p_len = (int *)buf;
                msg_body_len = ntohl(*p_len);

                status = RECV_MSG_BODY;
            }
            else
            {
                break;
            }
        }

        if (status == RECV_MSG_BODY)
        {
            if (left_len >= _head_len + msg_body_len) {
                process_s(buf + _head_len, msg_body_len);

                left_len -= _head_len + msg_body_len;
                buf = buf + left_len;
            } else {
                break;
            } 
        }				
    }

    return len - left_len;
}	


size_t channel_msg_process::process_s(char *buf, size_t len)
{
    if (!buf || !len) {
        return 0;
    } 

	put_msg(buf, len);

    return len;
}

void channel_msg_process::put_msg(char * buf, size_t len)
{
    if (!buf || !len){
        //LOG_WARN
        return;
    }


    base_net_container * net_container = _p_connect->get_net_container();
    if (!net_container) {
        return;
    }


	 PassMsg pass_msg;
    pass_msg.ParseFromArray(buf, len);

    switch (pass_msg.cmd())
    {
        case ADD_NEW_SOCEKT:
            {
                common_net_thread<channel_msg_process> *net_thread = dynamic_cast<common_net_thread<channel_msg_process> * >(_thread);
                int fd = atoi(pass_msg.str().c_str());
                PDEBUG("recv fd[%d]\n", fd);

                if (net_thread){
                    net_thread->gen_connect(fd);
                }
            }

            break;
        default:
        		{
				 	NET_OBJ * net_obj = dynamic_cast<NET_OBJ *>(net_container->find(&pass_msg.dst_id()));
					if (net_obj) {
						net_obj->process_recv_buf(buf, len);
					}
				}
    }

}

void channel_msg_process::set_common_thread(common_thread *thread)
{
    _thread = thread;
}

common_thread* channel_msg_process::get_common_thread()
{
    return _thread;
}




