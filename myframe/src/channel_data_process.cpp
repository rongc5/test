#include "common_msg_process.h"
#include "common_net_thread.h"
#include "common_thread.h"
#include "channel_data_process.h"
#include "common_thread.h"

namespace MZFRAME {

    channel_data_process::channel_data_process(void *p)
            {
                _p_msg_process = (channel_msg_process<channel_data_process>*)p;			
            }

            size_t channel_data_process::process_recv_buf(char *buf, size_t len)
            {
                if (!buf || !len || len < sizeof(_pass_msg_t)) {
                    return 0;
                } 

                _pass_msg_t *head = (_pass_msg_t *)buf;
                if (head->len < 4){
                    return 0;
                }

                int op = *(int *)(buf + sizeof(head));
                if (op == ADD_NEW_SOCEKT){
                    add_new_socket *stu = (add_new_socket *)(buf + sizeof(head));
                    int fd = stu->fd;
                    common_thread * thread =  _p_msg_process->get_common_thread();
                    common_net_thread<channel_msg_process<channel_data_process > > *net_thread = dynamic_cast<common_net_thread<channel_msg_process<channel_data_process > > * >(thread);
                    if (net_thread){
                        net_thread->gen_connect(fd);
                    }
                }

            }

            channel_data_process* channel_data_process::gen_process(void *p) 
            {   
                channel_data_process *p_tmp = new channel_data_process(p);
                return p_tmp;
            }   
}

