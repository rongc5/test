#include "common_epoll.h"
#include "base_net_container.h"
#include "log_helper.h"
#include "common_exception.h"
#include "base_connect.h"
#include "base_data_process.h"


base_connect::base_connect(const int32_t sock):base_net_obj(sock)
{
    _recv_buf.resize(max_recv_data);		
    _p_send_buf = NULL;
    _process = NULL;
}

base_connect::~base_connect()
{
    close();
}

void base_connect::close()
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

void base_connect::event_process(int event)
{
    if ((event & EPOLLERR) == EPOLLERR || (event & EPOLLHUP) == EPOLLHUP)
    {
        THROW_COMMON_EXCEPT("epoll error ");
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

int base_connect::RECV(void *buf, size_t len)
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

ssize_t base_connect::SEND(const void *buf, const size_t len)
{
    if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
    {
        THROW_COMMON_EXCEPT("close the socket " << _fd);
    }

    ssize_t ret =  send(_fd, buf, len, MSG_DONTWAIT);
    if (ret < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            THROW_COMMON_EXCEPT("send data error " << strerror(errno));
        }
        ret = 0;
    }
    return ret;
}

void base_connect::real_recv()
{	
    size_t tmp_len = MAX_RECV_SIZE - _recv_buf.length(); 	
    ssize_t ret = 0;
    if (tmp_len > 0) //接收缓冲满了也可以先不接收
    {
        char t_buf[SIZE_LEN_32768];
        int r_len = tmp_len <= sizeof(t_buf) ? tmp_len:sizeof(t_buf);
        ret = RECV(t_buf, r_len);
        if (ret > 0)
            recv_buf.append(t_buf, ret);
    }

    if (_recv_buf.length() > 0)
    {
        LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]\n", _recv_buf_len, _fd);
        size_t p_ret = _process->process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);
        if (p_ret <= _recv_buf.length())//表名上层处理能力跟不上,停止接受数据
        {
            _recv_buf.erase(0, p_ret); 
        }
    }		
}


void base_connect::real_send()
{	
    int ii = 0; 	
    while (1)
    {
        if (ii >= MAX_SEND_NUM)
            break;
        ii++;
        if (!_p_send_buf)
        {
            _p_send_buf = _process->get_send_buf();
        }

        if (!_p_send_buf) {
            update_event(get_event() & ~EPOLLOUT);
            break;
        }

        if (_p_send_buf && _p_send_buf->length())
        {
            ssize_t ret = SEND(_p_send_buf->c_str(), _p_send_buf->length());				
            if (ret == (ssize_t)_p_send_buf->length())
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
}

void base_connect::set_process(base_data_process *p) 
{
    if (_process != NULL && _process != p) {
        delete _process;
    }   

    _process = p;
    _process->set_para();
}

void base_connect::notice_send()
{
    update_event(_epoll_event | EPOLLOUT);
}

bool base_connect::process_recv_msg(normal_obj_msg* p_msg)
{
    size_t p_ret =_process->process_recv_msg(p_msg);
    return true;
}

void  base_connect::handle_timeout(const uint32_t timer_type)
{
    _process->handle_timeout(timer_type);
}

base_data_process * base_connect::get_process()
{
    return _process;
} 


