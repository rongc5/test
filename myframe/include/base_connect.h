#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "common_def.h"
#include "base_net_obj.h"
#include "base_net_container.h"
#include "log_helper.h"
#include "common_exception.h"
#include "common_epoll.h"

class base_data_process;
template<class PROCESS>
class base_connect:public base_net_obj
{
    public:

        base_connect(const int32_t sock)
        {
            _fd = sock;
            _p_send_buf = NULL;
            _process = NULL;
        }

        base_connect()
        {
            _p_send_buf = NULL;
            _process = NULL;
        }

        virtual ~base_connect()
        {
            if (_p_send_buf != NULL){
                delete _p_send_buf;
                _p_send_buf = NULL;
            }

            if (_process) {
                delete _process;
                _process = NULL;
            }
        }


        virtual void event_process(int event)
        {
            if ((event & EPOLLERR) == EPOLLERR || (event & EPOLLHUP) == EPOLLHUP)
            {
                THROW_COMMON_EXCEPT("epoll error "<< strerror(errno));
            }

            if ((event & EPOLLIN) == EPOLLIN) //读
            {
                real_recv();
            }

            if ((event & EPOLLOUT) == EPOLLOUT ) //写
            {
                real_send();
            }	
        }
        virtual int real_net_process()
        {
            int32_t ret = 0;

            if ((get_event() & EPOLLIN) == EPOLLIN) {
                LOG_DEBUG("real_net_process real_recv");
                real_recv();
            }

            if ((get_event() & EPOLLOUT) == EPOLLOUT) {
                LOG_DEBUG("real_net_process real_send");
                real_send();
            }

            return ret;
        }

        void set_process(PROCESS *p)
        {
            if (_process != NULL)
                delete _process;
            _process = p;

            _process->set_para();
        }

        virtual void notice_send()
        {
            update_event(_epoll_event | EPOLLOUT);

            real_send();
        }

        virtual bool process_recv_msg(ObjId & id, normal_msg * p_msg)
        {
            if (p_msg->_msg_op == MSG_TIMER) 
            {
                timer_msg * t_msg = dynamic_cast<timer_msg *> (p_msg);
                if (!t_msg) {
                    REC_OBJ<normal_msg> rc(p_msg);
                    return true;
                }

                if (t_msg->_timer_type == DELAY_CLOSE_TIMER_TYPE) {
                    REC_OBJ<normal_msg> rc(p_msg);
                    THROW_COMMON_EXCEPT("the connect obj delay close, delete it");
                    return true;
                }
            }

            _process->process_recv_msg(id, p_msg);
            return true;
        }



    protected:
        virtual int RECV(void *buf, size_t len)
        {
            int ret = recv(_fd, buf, len, MSG_DONTWAIT);
            if (ret == 0)
            {
                _process->peer_close();
                THROW_COMMON_EXCEPT("the client close the socket(" << _fd << ")");
            }
            else if (ret < 0)
            {
                if (errno != EAGAIN)
                {
                    THROW_COMMON_EXCEPT("this socket occur fatal error " << strerror(errno));
                }
                ret = 0;
            }

            return ret;
        }

        virtual ssize_t SEND(const void *buf, const size_t len)
        {
            if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
            {
                THROW_COMMON_EXCEPT("close the socket " << _fd);
            }

            ssize_t ret =  send(_fd, buf, len, MSG_DONTWAIT | MSG_NOSIGNAL);
            if (ret < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    THROW_COMMON_EXCEPT("send data error " << strerror(errno));
                }
                LOG_WARNING("send error");
                ret = 0;
            }
            return ret;

        }

        void real_recv()
        {
            size_t _recv_buf_len = _recv_buf.length();
            size_t tmp_len = MAX_RECV_SIZE - _recv_buf_len; 	
            ssize_t ret = 0;
            size_t p_ret = 0;
            if (tmp_len > 0) //接收缓冲满了也可以先不接收
            {
                char t_buf[SIZE_LEN_32768];
                int r_len = tmp_len <= sizeof(t_buf) ? tmp_len:sizeof(t_buf);
                ret = RECV(t_buf, r_len);
                if (ret > 0){
                    _recv_buf.append(t_buf, ret);
                    _recv_buf_len += ret;
                }
            }

            if (_recv_buf_len > 0)
            {
                LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]", _recv_buf_len, _fd);
                p_ret = _process->process_recv_buf(_recv_buf.data(), _recv_buf_len);
                LOG_DEBUG("process_recv_buf p_ret[%d] fd[%d]", p_ret, _fd);
                if (p_ret && p_ret <= _recv_buf_len)
                {
                    _recv_buf.erase(0, p_ret); 
                }
            }		

        }

        void real_send()
        {
            if (!_p_send_buf)
            {
                _p_send_buf = _process->get_send_buf();
            }

            if (!_p_send_buf) {
                update_event(get_event() & ~EPOLLOUT);
                return;
            }

            size_t _send_buf_len = _p_send_buf->length();
            if (_p_send_buf && _send_buf_len)
            {
                ssize_t ret = SEND(_p_send_buf->c_str(), _send_buf_len);				
                if (ret == (ssize_t)_send_buf_len)
                {
                    delete _p_send_buf;
                    _p_send_buf = NULL; 				
                }
                else if (ret > 0)
                {
                    _p_send_buf->erase(0, ret);
                    LOG_WARNING("_p_send_buf erase %d", ret);
                }
            }
        }

    protected:
        string _recv_buf;
        string* _p_send_buf;	
        PROCESS *_process;
};


#endif
