#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "common_epoll.h"
#include "base_net_container.h"
#include "net_obj.h"
#include "log_helper.h"
#include "common_exception.h"


namespace MZFRAME {

template<class PROCESS>
class base_connect:public NET_OBJ
{
    public:

        base_connect(const int32_t sock, EPOLL_TYPE epoll_type/*0:lt,  1:et*/)
        {
            init(epoll_type);
            _sock = sock;
            struct sockaddr_in sa;
            
            int len = sizeof(sa);
            if (getpeername(sock, (struct sockaddr *)&sa, &len)) {
                base_connect<PROCESS>::_peer_addr = sa;
            }
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

            _process = NULL;
            _p_epoll = NULL;
            _sock = 0;
        }


        void close()
        {
            if (_sock != 0)
            {
                if (_p_epoll != NULL)
                {
                    _p_epoll->del_from_epoll(this);
                }

                ::close(_sock);
                _sock = 0;
                _epoll_event = EPOLLIN | EPOLLERR | EPOLLHUP;
                if (_p_send_buf != NULL)
                {
                    delete _p_send_buf;
                    _p_send_buf = NULL;
                }
                _recv_buf_len = 0;
            }
        }

        void set_process(PROCESS *p)
        {
            if (_process != NULL)
                delete _process;
            _process = p;

            _process->set_para();
        }

        void event_process(const int32_t event)
        {
            if ((event & EPOLLERR) == EPOLLERR || (event & EPOLLHUP) == EPOLLHUP)
            {
                THROW_COMMON_EXCEPT("epoll error ");
            }

            if ((event & EPOLLIN) == EPOLLIN) //读
            {
                recv_process();
            }

            if ((event & EPOLLOUT) == EPOLLOUT ) //写
            {
                send_process();
            }		
        }	

        int32_t real_net_process()
        {
            int32_t ret = 0;

            real_recv();

            real_send();

            return ret;
        }

        size_t process_recv_buf(char *buf, size_t len)
        {
            size_t p_ret =_process->process_s((char*)_recv_buf.c_str(), _recv_buf_len);
            return p_ret;
        }

        void process_send_buf(string * buf)
        {
            _process->process_send_buf(buf);
        }

        int get_sock()
        {
            return _sock;
        }

        int get_event()
        {
            return _epoll_event;
        }

        int destroy()
        {
            delete this;
            return 0;
        }

        void get_local_addr(sockaddr_in &addr)
        {
            socklen_t len = 0;
            getsockname(_sock, (sockaddr*)&addr, &len);
        }

    protected:
        virtual ssize_t RECV(void *buf, size_t len)
        {
            ssize_t ret = recv(_sock, buf, len, MSG_DONTWAIT);
            if (ret == 0)
            {
                _process->peer_close();
                THROW_COMMON_EXCEPT("the client close the socket(" << _sock << ")");
            }
            else if (ret < 0)
            {
                if (errno != EAGAIN)
                {
                    THROW_COMMON_EXCEPT("this socket occur fatal error " << strerror(errno));
                }
            }

            return ret;
        }

        virtual ssize_t SEND(const void *buf, const size_t len)
        {
            if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
            {
                THROW_COMMON_EXCEPT("close the socket " << _sock);
            }

            ssize_t ret =  send(_sock, buf, len, MSG_DONTWAIT);
            if (ret < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    THROW_COMMON_EXCEPT("send data error " << strerror(errno));
                }
            }
            return ret;
        }


        virtual void recv_process()
        {	
            if ((_epoll_event & EPOLLET) != EPOLLET)
            {
                real_recv();
            }
        }

        virtual void send_process()
        {
            if((_epoll_event & EPOLLET) != EPOLLET)
            {
                real_send();
            }
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
                size_t p_ret = _process->process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);
                if (p_ret < _recv_buf_len)//表名上层处理能力跟不上,停止接受数据
                {
                    string tmp_str = _recv_buf.substr(p_ret, _recv_buf_len - p_ret);
                    memcpy((void*)_recv_buf.c_str(), tmp_str.c_str(), tmp_str.length());
                }

                _recv_buf_len = _recv_buf_len - p_ret;			
            }		
        }


        void real_send()
        {	
            int ii = 0; 	
            while (1)
            {
                if (ii >= max_send_num)
                    break;
                ii++;
                if (_p_send_buf == NULL)
                {
                    _p_send_buf = _process->get_send_buf();
                }

                if (_p_send_buf != NULL)
                {
                    bool is_break = false;
                    ssize_t ret = SEND(_p_send_buf->c_str(), _p_send_buf->length());				
                    if (ret == (ssize_t)_p_send_buf->length())
                    {
                        delete _p_send_buf;
                        _p_send_buf = NULL; 				
                    }
                    else if (ret >= 0)
                    {
                        _p_send_buf->erase(0, ret);
                        LOG_WARNING("_p_send_buf erase %d", ret);
                        is_break = true;
                    }
                    else //<0
                    {
                        is_break = true;
                    }
                    //限速
                    if (is_break)
                        break;
                }
            }
        }


    protected:
        int32_t _sock;
        PROCESS *_process;
        string _recv_buf;
        size_t _recv_buf_len;
        string* _p_send_buf;	
        uint64_t _connect_timeout;
        EPOLL_TYPE _epoll_type;
};

}

#endif
