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
    LOG_DEBUG("begin RECV fd[%d]", _fd);
    int ret = recv(_fd, buf, len, MSG_DONTWAIT);
    if (ret == 0)
    {
        //LOG_DEBUG("the client close the socket %d", _fd);
        THROW_COMMON_EXCEPT("the client close the socket(" << _fd << ")");
    }
    else if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            //LOG_DEBUG("this socket occur fatal error %s", strerror(errno));
            THROW_COMMON_EXCEPT("this socket occur fatal error " << strerror(errno));
        }
        ret = 0;
    }

    return ret;
}



test_connect * test_connect::gen_connect(int fd, base_net_thread * thread)
{
    set_unblock(fd);
    test_connect * c_conn = new test_connect(fd, thread);
    c_conn->init_ev(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);

    return c_conn;
}


size_t test_connect::process_recv_buf(char *buf, size_t len)
{


    LOG_DEBUG("%s %d\n", "recv msg", len);
    size_t left_len = len;
    while(left_len > 0)
    {
        RECV_MSG_STATUS status = RECV_MSG_HEAD;
        size_t msg_body_len = 0;
        size_t _head_len = sizeof(int); 
        if (status == RECV_MSG_HEAD)
        {
            if (left_len > _head_len)
            {
                int *p_len = (int *)buf;
                msg_body_len = *p_len;
                LOG_DEBUG("msg_body_len[%d]", msg_body_len);
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

                LOG_DEBUG("_head_len[%d] msg_body_len[%d]", _head_len, msg_body_len);
                process_s(buf + _head_len, msg_body_len);
                left_len -= (_head_len + msg_body_len);
                buf = buf + _head_len + msg_body_len;
            } else {
                break;
            } 
        }               
    }

    return len - left_len;
}


size_t test_connect::process_s(char *buf, size_t len)
{

    CommonMsg recv_msg;
    
    string str(buf, len);
    recv_msg.ParseFromString(str);
    

    LOG_DEBUG("recv_msg:%d, str[%s]", recv_msg.obj_id(), recv_msg.str().c_str());

    write(_fd, "I recived", sizeof("I recived"));

    return len;
}



