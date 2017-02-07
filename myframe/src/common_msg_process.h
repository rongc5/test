#ifndef _COMMON_MSG_PROCESS_H_
#define _COMMON_MSG_PROCESS_H_

#include "common_epoll.h"
#include "net_obj.h"
#include "common_def.h"
#include "base_msg_process.h"
#include "common_thread.h"



template<class DATA_PROCESS>
class common_msg_process:public base_msg_process
{
    public:
        common_msg_process(void *p):_thread(NULL), base_msg_process(p)
    {
        _data_process = DATA_PROCESS::gen_process((void*)this);

    }

        virtual ~common_msg_process()
        {
            if (_data_process){
                delete _data_process;
            }
        }	

        virtual size_t process_recv_buf(char *buf, size_t len)
        {
            LOG_DEBUG("recv buf %d", len);
            PDEBUG("recv buf %d", len);
            //size_t ret = 0;
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

                        PassMsg head;
                        ObjId *src_id, *dest_id;
                        src_id = head.mutable_dst_id();
                        *src_id = _p_connect->get_id();
                        dest_id = head.mutable_src_id(); 
                        *dest_id =  _p_connect->get_id();
                        head.set_cmd(COMMON_MSG);
                        head.set_str(buf + _head_len, msg_body_len);

                        string out;
                        head.SerializeToString(&out);

                        process_s(out.c_str(), out.size());

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

        void put_msg(char * buf, size_t len)
        {
            if (!buf || !len){
                //LOG_WARN
                return;
            }

            if (_thread) {
                write(_thread->get_channelid(), buf, len);
            }
        }

        void set_common_thread(common_thread *thread)
        {
            _thread = thread;
        }

    protected:
        common_thread * _thread;
        DATA_PROCESS *_data_process;

};

#endif

