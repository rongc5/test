#include "http_client_connect.h"
#include "base_net_thread.h"
#include "common_def.h"


void http_client_connect::http_request_done(struct evhttp_request *req, void *arg)
{
    if (!arg) {
        //WARNING_LOG("arg is NULL");
        return ;    
    }

    http_res_msg * rs_http = (http_res_msg *)arg;

    if (req) {
        struct evbuffer* buf = evhttp_request_get_input_buffer(req);
        size_t len = evbuffer_get_length(buf);
        unsigned char * str = evbuffer_pullup(req->input_buffer, len);

        rs_http->res_code = req->response_code;
        if (str)
            rs_http->res_buf.append((const char*)str, len);
        else 
            rs_http->res_buf.append("len: %d str is NULL", len);

        evbuffer_drain(buf, len);

    } else {

        rs_http->res_code = 2;
        rs_http->res_buf.append("req is NULL");
    }

    //DEBUG_LOG("sid: %s res_code[%d] res_buf[%s]", msg->sid.c_str(), rs_http.res_code, rs_http.res_buf.c_str());

    base_net_thread::put_msg_ObjId(rs_http);
}



void http_client_connect::do_request(base_passing_msg * b_msg, struct event_base* base)
{
    int ret = 0;
    map<string, string>::iterator it;
    char t_buf[SIZE_LEN_2048];
    int port = 0; 
    struct evhttp_request * req = NULL;
    const char *path = NULL;
    const char *query = NULL; 
    http_req_msg * msg = NULL;
    http_res_msg * rs_http = NULL;


    REC_OBJ<base_passing_msg> rc(b_msg);

    msg = dynamic_cast<http_req_msg *>(b_msg);
    if (!msg) {
        rs_http->res_code = 4;
        rs_http->res_buf.append("msg not http_req_msg");
        goto ERR;
    }

    rs_http = new http_res_msg;
    rs_http->_dst_id = msg->_src_id;
    rs_http->_op = PASSING_RES_HTTP;



    msg->uri = evhttp_uri_parse(msg->url.c_str());
    if (!msg->uri) {
        rs_http->res_code = 4;
        rs_http->res_buf.append("evhttp_uri_parse failed");
        goto ERR;
    }
    
    port = evhttp_uri_get_port(msg->uri); 
    if (port == -1){
        port = 80;
    }

    msg->cn = evhttp_connection_base_new(base,NULL, evhttp_uri_get_host(msg->uri), port);
    if (!msg->cn) {
        rs_http->res_code = 4;
        rs_http->res_buf.append("evhttp_connection_base_new failed");
        goto ERR;
    }

    req = evhttp_request_new(http_request_done, rs_http);
    if (!req) {
        rs_http->res_code = 4;
        rs_http->res_buf.append("evhttp_request_new failed");
        goto ERR;
    }

    evhttp_add_header(req->output_headers, "Host", evhttp_uri_get_host(msg->uri));

    evhttp_add_header(req->output_headers, "Connection", "close");


    for (it = msg->headers.begin(); it != msg->headers.end(); it++) {
        evhttp_add_header(req->output_headers, it->first.c_str(), it->second.c_str());
    }

    path = evhttp_uri_get_path(msg->uri);
    query = evhttp_uri_get_query(msg->uri); 

    if (query != NULL) {
        snprintf(t_buf, sizeof(t_buf), "%s?%s", path, query);
    }else {
        snprintf(t_buf, sizeof(t_buf), "%s", path);
    }


    if (msg->http_mode == HTTP_GET) {
            
        ret = evhttp_make_request(msg->cn, req, EVHTTP_REQ_GET, t_buf); 

    } else if (msg->http_mode == HTTP_POST) {
        
        evbuffer_add(req->output_buffer, msg->post_data.c_str(), msg->post_data.length());

        ret = evhttp_make_request(msg->cn, req, EVHTTP_REQ_POST, t_buf); 
    }

    if (ret) {

        rs_http->res_code = 4;
        rs_http->res_buf.append("evhttp_make_request failed");

        goto ERR;
    }

    //evhttp_connection_set_timeout(req->evcon, msg->time_out);
    struct timeval tv;
    //tv.tv_usec = base_singleton<dsp_conf>::get_instance()->http_connect_timeout;
    tv.tv_usec = 350 * 1000;
    tv.tv_sec = 0;
    evhttp_connection_set_timeout_tv(req->evcon, &tv);
    return ;

ERR:
    
    base_net_thread::put_msg_ObjId(rs_http);

    return ;
}


