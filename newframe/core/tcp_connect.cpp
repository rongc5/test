#include "tcp_connect.h"
#include "http_client_connect.h"
#include "log_helper.h"


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
    size_t tmp_len = max_recv_data - _recv_buf_len;     
    ssize_t ret = 0;
    if (tmp_len > 0)
    {
        ret = RECV((void*)(_recv_buf.c_str() + _recv_buf_len), tmp_len);
        _recv_buf_len += ret;
    }

    if (_recv_buf_len > 0)
    {
        //LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]\n", _recv_buf_len, _fd);
        size_t p_ret = process_recv_buf((char*)_recv_buf.c_str(), _recv_buf_len);
        if (p_ret < _recv_buf_len)
        {
            string tmp_str = _recv_buf.substr(p_ret, _recv_buf_len - p_ret);
            memcpy((void*)_recv_buf.c_str(), tmp_str.c_str(), tmp_str.length());
        }

        _recv_buf_len = _recv_buf_len - p_ret;          
        //PDEBUG("p_ret[%d] _recv_buf_len[%d]\n", p_ret, _recv_buf_len);
    }     
}

void tcp_connect::peer_close()
{
}

void tcp_connect::error_back()
{
}

int tcp_connect::RECV(void *buf, size_t len)
{
    //LOG_DEBUG("begin RECV fd[%d]", _fd);
    int ret = recv(_fd, buf, len, MSG_DONTWAIT);
    if (ret == 0)
    {
        //LOG_DEBUG("the client close the socket %d", _fd);
        //THROW_COMMON_EXCEPT("the client close the socket(" << _fd << ")");
        peer_close();
        destroy();
    }
    else if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            //LOG_DEBUG("this socket occur fatal error %s", strerror(errno));
            error_back();
            destroy();
        }
        ret = 0;
    }

    return ret;
}



size_t tcp_connect::process_send_buf(char *buf, size_t len)
{
    int ret = SEND(buf, len);
    if (ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        if (!update_event(EV_READ | EV_WRITE | EV_PERSIST)) {
            destroy();
        }

        _send_buf.append(buf, len);
    }

    return len;
}



ssize_t tcp_connect::SEND(const void *buf, const size_t len)
{
    if (len == 0) //上层抛一个长度为0的数据过来 ,直接关闭
    {
        error_back();
        destroy();
    }


    ssize_t ret =  send(_fd, buf, len, MSG_DONTWAIT);
    if (ret < 0)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            error_back();
            destroy();
        }
    }

    return ret;
}

void tcp_connect::real_send()
{
    int ii = 0;     

    while (1)
    {
        if (ii >= max_send_num)
            break;

        ii++;

        if (_send_buf.length())
        {
            bool is_break = false;

            ssize_t ret = SEND(_send_buf.c_str(), _send_buf.length());             

            if (ret == (ssize_t)_send_buf.length())
            {
                _send_buf.clear();
                if (!update_event(EV_READ | EV_PERSIST)) {
                    destroy();
                }

            }
            else if (ret >= 0)
            {
                _send_buf.erase(0, ret);

                LOG_WARNING("_p_send_buf erase %d", ret);
                is_break = true;
            }
            else //<0
            {
                is_break = true;
            }

            if (is_break)
                break;
        }
    }
}


