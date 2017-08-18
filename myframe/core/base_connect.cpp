#include "common_epoll.h"
#include "base_net_container.h"
#include "log_helper.h"
#include "common_exception.h"
#include "base_connect.h"
#include "base_data_process.h"


base_connect::base_connect(const int32_t sock):base_net_obj(sock)
{
    _p_send_buf = NULL;
    _process = NULL;
}

base_connect::~base_connect()
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

int base_connect::real_net_process()
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
            _recv_buf.append(t_buf, ret);
    }

    if (_recv_buf.length() > 0)
    {
        LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]", _recv_buf.length(), _fd);
        size_t p_ret = _process->process_recv_buf((char*)_recv_buf.c_str(), _recv_buf.length());
        if (p_ret && p_ret <= _recv_buf.length())
        {
            _recv_buf.erase(0, p_ret); 
        }
    }		
}


void base_connect::real_send()
{	
    if (!_p_send_buf)
    {
        _p_send_buf = _process->get_send_buf();
    }

    if (!_p_send_buf) {
        update_event(get_event() & ~EPOLLOUT);
        return;
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

    real_send();
}

bool base_connect::process_recv_msg(ObjId & id, normal_msg * p_msg)
{
    if (p_msg->_msg_op == MSG_TIMER) {
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

base_data_process * base_connect::get_process()
{
    return _process;
} 


