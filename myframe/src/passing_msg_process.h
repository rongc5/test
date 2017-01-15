#ifndef _CHANNEL_MSG_PROCESS_H_
#define _CHANNEL_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "log_helper.h"
#include "base_msg_process.h"
#include "base_net_container.h"
#include "passing_msg_thread.h"


namespace MZFRAME {

    template<class DATA_PROCESS>
        class passing_msg_process:public base_msg_process
    {
        public:
            passing_msg_process(void *p):base_msg_process(p), _thread(NULL)
        {
            _data_process = DATA_PROCESS::gen_process((void*)this);
        }

            virtual ~passing_msg_process()
            {
                if (_data_process){
                    delete _data_process;
                }
            }	

            virtual size_t process_recv_buf(char *buf, size_t len)
            {
                LOG_DEBUG("recv buf %d", len);
                //size_t ret = 0;
                size_t left_len = len;
                while(left_len > 0)
                {
                    RECV_MSG_STATUS status = RECV_MSG_HEAD;
                    size_t msg_body_len = 0;
                    if (status == RECV_MSG_HEAD)
                    {
                        if (left_len > _head_len)
                        {
                            const char *ptr = "";

                            if (_head_len >= MSG_HEAD_BODY_LENTH_LEN) 
                                ptr = buf + _head_len - MSG_HEAD_BODY_LENTH_LEN;
                            else {
                                ptr = buf;
                            }

                            msg_body_len = ntohl(*ptr);

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
                            process_s(buf, _head_len + msg_body_len);

                            left_len -= _head_len + msg_body_len;
                            buf = buf + left_len;
                        } else {
                            break;
                        } 
                    }				
                }

                return len - left_len;
            }	


            size_t process_s(char *buf, size_t len)
            {
                return _data_process->process_recv_buf(buf, len);
            }

            void put_msg(string *p_msg)
            {
                if (!p_msg || p_msg->length() < sizeof(_pass_msg_t)){
                    //LOG_WARN
                    delete p_msg;
                    return;
                }

                _pass_msg_t * ptr = (_pass_msg_t *)p_msg;


                if (!_thread){
                    return;
                }
                
                const base_net_obj * net_obj =_thread->get_dest_obj(ptr->_dst_obj._thread_id);
                if (net_obj) {
                    net_obj->process_send_buf(p_msg);
                }
            }
            
            void set_passing_thread(passing_msg_thread * thread)
            {
                _thread = thread;
            }

            passing_msg_thread * get_passing_thread()
            {
                return _thread;
            }

        protected:
            DATA_PROCESS *_data_process;
            passing_msg_thread * _thread;

    };


}
#endif

