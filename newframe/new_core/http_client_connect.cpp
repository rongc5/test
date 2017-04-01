#include "http_client_connect.h"
#include "test_connect.h"

http_client_connect::http_client_connect(base_net_thread * thread):base_connect(thread), _t_conn(NULL)
{                                                                                                                                    
} 



void http_client_connect::http_request_done(struct evhttp_request *req, void *arg)
{
    http_client_connect * hcc = (http_client_connect *)arg;
    if (hcc){
        hcc->call_back(req, arg);
    }
}

void http_client_connect::close()
{  
    evhttp_uri_free(_uri);
    evhttp_connection_free(_cn);
}


http_client_connect * http_client_connect::gen_connect(string & url, test_connect * t_conn, base_net_thread * thread)
{
    http_client_connect * c_conn = new http_client_connect(thread);
    c_conn->init(url, t_conn);
    thread->add_connect_map(c_conn);

    return c_conn;
}

void http_client_connect::init(const string & url, test_connect * t_conn)
{
    _url = url;
    _t_conn = t_conn;

    _uri = evhttp_uri_parse(url.c_str());
    
    int port = evhttp_uri_get_port(_uri); 

    if (port == -1){
        port = 80;
    }

    LOG_DEBUG("url:%s", _url.c_str());
    _cn = evhttp_connection_base_new(_thread->get_event_base(),NULL, evhttp_uri_get_host(_uri), port);

    _req = evhttp_request_new(http_request_done, this);

    evhttp_add_header(_req->output_headers, "Host", evhttp_uri_get_host(_uri));

    const char *query = evhttp_uri_get_query(_uri); 
    const char *path = evhttp_uri_get_path(_uri);

    char t_buf[SIZE_LEN_2048];

    snprintf(t_buf, sizeof(t_buf), "%s?%s", path, query);
    evhttp_make_request(_cn, _req, EVHTTP_REQ_GET, t_buf); 
    evhttp_connection_set_timeout(_req->evcon, 600);

    //LOG_DEBUG("host:%s, path:%s, query:%s, port:%d", evhttp_uri_get_host(_uri), path, query, port);
    //LOG_DEBUG("t_buf:%s", t_buf);
}

void http_client_connect::call_back(struct evhttp_request *req, void *arg)
{
   switch(req->response_code)
   {
        case HTTP_OK:
            {
                struct evbuffer* buf = evhttp_request_get_input_buffer(req);
                size_t len = evbuffer_get_length(buf);
                unsigned char * str = evbuffer_pullup(req->input_buffer, len);
                string * sstr = new string((const char *)str, len);
                if (_t_conn) {
                    _t_conn->process_form_http(sstr);
                    destroy();
                }

            }

            break;
        default:
            break;
   }


}

