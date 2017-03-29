#include "test_connect.h"



void test_connect::call_back(int fd, short ev, void *arg)
{
    real_recv(); 
}

void test_connect::real_recv()
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
        LOG_DEBUG("process_recv_buf _recv_buf_len[%d] fd[%d]\n", _recv_buf_len, _fd);
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

int test_connect::RECV(void *buf, size_t len)
{
    int ret = recv(_fd, buf, len, MSG_DONTWAIT);
    if (ret == 0)
    {
        PDEBUG("the client close the socket [%d]\n", _fd);
        base_connect::destroy();
    }
    else if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            PDEBUG("this socket occur fatal error [%s]\n", strerror(errno));
            base_connect::destroy();
        }
        ret = 0;
    }

    return ret;
}



test_connect * test_connect::gen_connect(int fd, base_net_thread * thread)
{
    set_unblock(fd);
    test_connect * c_conn = new test_connect(_fd, thread);
    c_conn->init_ev(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);
}


size_t test_connect::process_recv_buf(char *buf, size_t len)
{


    return len;
}



