#include "tcp_connect.h"
#include "log_helper.h"
#include "common_exception.h"
#include "base_data_process.h"


tcp_connect::~tcp_connect()
{
}

void tcp_connect::call_back(int fd, short ev, void *arg)
{
    if (ev & EV_READ) {
        real_recv(); 
    } else if (ev & EV_WRITE) {
        real_send();
    }
}

void tcp_connect::real_recv()
{   
    size_t tmp_len = MAX_RECV_SIZE - _recv_buf.length();     
    ssize_t ret = 0;
    if (tmp_len > 0)
    {
        char t_buf[SIZE_LEN_32768];
        int r_len = tmp_len <= sizeof(t_buf) ? tmp_len:sizeof(t_buf);
        //ret = RECV((void*)(_recv_buf.c_str() + _recv_buf_len), tmp_len);
        ret = RECV(t_buf, r_len);
        if (ret > 0)
            _recv_buf.append(t_buf, ret);
    }

    if (_recv_buf.length() > 0)
    {
        //LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]\n", _recv_buf.length(), _fd);
        size_t p_ret = _process->process_recv_buf((char*)_recv_buf.c_str(), _recv_buf.length());
        if (p_ret <= _recv_buf.length())
        {
            //string tmp_str = _recv_buf.substr(p_ret, _recv_buf_len - p_ret);
            //memcpy((void*)_recv_buf.c_str(), tmp_str.c_str(), tmp_str.length());
            _recv_buf.erase(0, p_ret);
        }
    } 
}

size_t tcp_connect::process_recv_buf(char *buf, size_t len)
{
    return _process->process_recv_buf(buf, len);
}

int tcp_connect::RECV(void *buf, size_t len)
{
    //LOG_DEBUG("begin RECV fd[%d]", _fd);
    int ret = recv(_fd, buf, len, MSG_DONTWAIT);
    if (ret == 0)
    {
        //LOG_DEBUG("the client close the socket %d", _fd);
        //THROW_COMMON_EXCEPT("the client close the socket(" << _fd << ")");
        _process->err_close();
        THROW_COMMON_EXCEPT("peer close");
    }
    else if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            //LOG_DEBUG("this socket occur fatal error %s", strerror(errno));
            _process->err_close();
            THROW_COMMON_EXCEPT("recv errstr");
        }
        ret = 0;
    }

    return ret;
}

ssize_t tcp_connect::SEND(const void *buf, const size_t len)
{
    if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
    {
        _process->err_close();
        THROW_COMMON_EXCEPT("send length 0");
    }

    ssize_t ret =  send(_fd, buf, len, MSG_DONTWAIT);
    if (ret < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            _process->err_close();
            THROW_COMMON_EXCEPT("send errstr");
        }
    }

    return ret;
}

void tcp_connect::real_send()
{
    int ii = 0;     

    while (1)
    {
        if (ii >= MAX_SEND_NUM)
            break;

        ii++;

        if (!_send_buf) {
            _send_buf = _process->get_send_buf();
        }

        if (!_send_buf) {
            if (!update_event(_ev &~EV_WRITE)) {
                THROW_COMMON_EXCEPT("update_event(_ev &~EV_WRITE)");
            }

            break;
        }

        if (_send_buf && _send_buf->length())
        {
            ssize_t ret = SEND(_send_buf->c_str(), _send_buf->length());             
            if (ret == (ssize_t)_send_buf->length())
            {
                delete _send_buf;
                _send_buf = NULL;
            }
            else if (ret >= 0)
            {
                _send_buf->erase(0, ret);

                LOG_WARNING("_p_send_buf erase %d", ret);
            }
        }
    }
}

void tcp_connect::notice_send()
{
    if (!(_ev & EV_WRITE))
        update_event(_ev | EV_WRITE);
}
