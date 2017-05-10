#include "job_connect.h"
#include "http_client_thread.h"
#include "log_helper.h"


job_connect::~job_connect()
{
}

job_connect * job_connect::gen_connect(int fd, base_net_thread * thread)
{
    //set_unblock(fd);
    job_connect * c_conn = new job_connect(fd, thread);
    c_conn->update_event(EV_TIMEOUT | EV_READ | EV_PERSIST);
    thread->add_connect_map(c_conn);

    return c_conn;
}

void job_connect::recv_passing_msg(base_passing_msg * p_msg)
{
    http_res_msg * rs_http = dynamic_cast<http_res_msg *>(p_msg);

    REC_OBJ<base_passing_msg> rc(p_msg);
    
    if (rs_http) {
        process_send_buf((char *)rs_http->res_buf.c_str(), rs_http->res_buf.length());
    }
}

size_t job_connect::process_recv_buf(char *buf, size_t len)
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


size_t job_connect::process_s(char *buf, size_t len)
{
    CommonMsg recv_msg;
    
    string str(buf, len);
    recv_msg.ParseFromString(str);
    
    string url;
    url.append("http://open.adview.cn/agent/openRequest.do?n=1&pt=0&at=0&html5=1&w=320&h=50&sw=640&sh=1136&ip=124.193.184.2&os=1&bdr=10.0.2&tp=iPhone+5S&brd=iPhone&pack=com.easou.esbook&appid=SDK20161026100933450xiouxiz91j0i&idfa=F580C676-8270-4FDB-8E5A-0838A79708D3&idfv=2127C7B2-7F2A-4187-9560-0A1D68EC8113&openudid=F580C676-8270-4FDB-8E5A-0838A79708D3&tab=0&sn=F580C676-8270-4FDB-8E5A-0838A79708D3&nop=&mc=&nt=&ua=iPhone__iPhone+5S__10.0.2&tm=0&time=1490776774349&token=9d18f508d30f933732a13d8829efe4dc");


    http_req_msg * http_msg = new http_req_msg();
    http_msg->http_mode = HTTP_GET;
    http_msg->url = url;
    http_msg->_src_id = get_id(); 
    http_msg->_op = PASSING_REQ_HTTP;
    
    http_client_thread::put_msg(http_msg);

    //LOG_DEBUG("recv_msg:%d, str[%s]", recv_msg.obj_id(), recv_msg.str().c_str());
    //printf("recv_msg:%d, str[%s]\n", recv_msg.obj_id(), recv_msg.str().c_str());

    //write(_fd, "I recived", sizeof("I recived"));

    //write(_fd, "987654321", sizeof("987654321"));
    return len;
}

