#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "common_epoll.h"
#include "base_net_container.h"
#include "log_helper.h"
#include "common_exception.h"


class base_connect
{
    public:

        base_connect(const int32_t sock, EPOLL_TYPE epoll_type/*0:lt,  1:et*/)
        {
            init(epoll_type);
            _fd = sock;
        }

        ~base_connect()
        {
            //WRITE_WARN("delete the connect");
            close();

            if (_process != NULL)
                delete _process;

            if (_p_send_buf != NULL)
                delete _p_send_buf;
        }

        void init(EPOLL_TYPE epool_type = 0/*0:lt,  1:et*/)
        {
            _epoll_type = epool_type; //初始化

            _connect_timeout = CONNECT_TIME_OUT;

            _recv_buf.resize(max_recv_data);		
            _p_send_buf = NULL;

            _recv_buf_len = 0;

            _epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;	

            _p_epoll = NULL;
            _fd = 0;
        }


        void close()
        {
            if (_fd != 0)
            {
                
                


                ::close(_fd);
                _fd = 0;
                _epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;
                if (_p_send_buf != NULL)
                {
                    delete _p_send_buf;
                    _p_send_buf = NULL;
                }
                _recv_buf_len = 0;
            }
        }

        void set_process(event_fd_callback fd_cb)
        {
            _fd_cb = fd_cb;
        }


        int get_sock()
        {
            return _fd;
        }
        
        int destroy()
        {
            delete this;
            return 0;
        }

        void get_local_addr(sockaddr_in &addr)
        {
            socklen_t len = 0;
            getsockname(_fd, (sockaddr*)&addr, &len);
        }

    protected:
        virtual int RECV(void *buf, size_t len)
        {
            int ret = recv(_fd, buf, len, MSG_DONTWAIT);
            if (ret == 0)
            {
                _process->peer_close();
                PDEBUG("the client close the socket [%d]\n", _fd);
                THROW_COMMON_EXCEPT("the client close the socket(" << _fd << ")");
            }
            else if (ret < 0)
            {
                if (errno != EAGAIN)
                {
                    PDEBUG("this socket occur fatal error [%s]\n", strerror(errno));
                    THROW_COMMON_EXCEPT("this socket occur fatal error " << strerror(errno));
                }
                ret = 0;
            }

            return ret;
        }

        void real_recv()
        {	
            size_t tmp_len = max_recv_data - _recv_buf_len; 	
            ssize_t ret = 0;
            if (tmp_len > 0) //接收缓冲满了也可以先不接收
            {
                ret = RECV((void*)(_recv_buf.c_str() + _recv_buf_len), tmp_len);
                _recv_buf_len += ret;
            }

            if (_recv_buf_len > 0)
            {
                LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]\n", _recv_buf_len, _fd);
                size_t p_ret = _process->process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);

                if (p_ret < _recv_buf_len)//表名上层处理能力跟不上,停止接受数据
                {
                    string tmp_str = _recv_buf.substr(p_ret, _recv_buf_len - p_ret);
                    memcpy((void*)_recv_buf.c_str(), tmp_str.c_str(), tmp_str.length());
                }

                _recv_buf_len = _recv_buf_len - p_ret;			
                //PDEBUG("p_ret[%d] _recv_buf_len[%d]\n", p_ret, _recv_buf_len);
            }		
        }


    protected:
        event_fd_callback _fd_cb;
        string _recv_buf;
        size_t _recv_buf_len;
        string* _p_send_buf;	
        uint64_t _connect_timeout;
        
        int _fd;
        struct event_base* _base;
        struct event * ev;

};


#endif
