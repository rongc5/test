#include "http_server_thread.h"
#include "log_helper.h"
#include "qf_msg_def.h"
#include "base_singleton.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

using namespace rapidjson;

http_server_thread::http_server_thread(): _nfd(0), _httpd(NULL)
{
    _httpser_thread_vec.push_back(this);
}

bool http_server_thread::handle_msg(base_passing_msg * msg)
{
    if (!msg) {
        return true;
    }   

    LOG_DEBUG("handle_msg");

    REC_OBJ<base_passing_msg> rc(msg);
    //WARNING_LOG("do_request exception");

    return true;
}

void http_server_thread::put_msg(base_passing_msg * msg)
{
    int index = (unsigned long)msg  % _httpser_thread_vec.size();
    _httpser_thread_vec[index]->add_msg(msg);
}

int http_server_thread::bind_port(int port)
{
    struct sockaddr_in address;
    int reuse_addr = 1;

    memset((char *) &address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    int ret = 0;

    address.sin_addr.s_addr = htonl(INADDR_ANY);


    int _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) 
    {
        //LOG_WARNING("socket error %s", strerror(errno));     
        return 0;
    }
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), sizeof(reuse_addr));

    if (::bind(_fd, (struct sockaddr *) &address, sizeof(address)) < 0) 
    {        
        //LOG_WARNING("bind error %s ip: %s port: %d", strerror(errno), ip.c_str(), port);
        return 0;
    }        

    ret = listen(_fd, 250);
    if (ret == -1)
    {
        //LOG_WARNING("listen error %s", strerror(errno));
        return 0;
    }

    set_unblock(_fd);

    return _fd;
}

void http_server_thread::set_nfd(int nfd)
{
    _nfd =  nfd;
}

void *http_server_thread::http_server_thread::run()
{

    _httpd = evhttp_new(_base);
    if (_httpd == NULL) return NULL;

    int r = evhttp_accept_socket(_httpd, _nfd);
    if (r != 0) return NULL;

    evhttp_set_gencb(_httpd, do_request_cb, this);
    //evhttp_set_cb(_http, "/dump", do_request_cb, NULL);

    base_net_thread::run();

    return NULL;
}

void http_server_thread::do_request_cb(struct evhttp_request *req, void *arg)
{
    http_server_thread * h_server = (http_server_thread *) arg;
    try {
        if (h_server) {
            h_server->do_call_back(req, arg);
        }
    } catch (std::exception & e) {
        LOG_WARNING("exception: %s", e.what());
    } catch (...) {
        LOG_WARNING("unknown error");
    }
}

//网页 发消息
void http_server_thread::do_sendmsg(struct evhttp_request *req)
{
    string query;
    struct evbuffer* buf = evhttp_request_get_input_buffer(req);
    size_t len = evbuffer_get_length(buf);
    unsigned char * str = evbuffer_pullup(req->input_buffer, len);

    query.append(str, len);
    evbuffer_drain(buf, len);


    vector<string> tmp_vec;
    SplitString(query, "&", tmp_vec);

    user_msg * msg = new user_msg;
    string cmd;

    for (uint32_t i = 0; i < tmp_vec.size(); i++) {
        vector<string> tt_vec;
        SplitString(tmp_vec[i].c_str(), "=", tt_vec);
        if (tt_vec.size() != 2) {
            break;
        }
        if (strstr(tt_vec[0].c_str(), "cmd")) {
            cmd = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "vid")) {
            msg->visitor_id = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "msg")) {
            msg->msg = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "to_sid")) {
            msg->to_id = tt_vec[1];
        }
    }

    int flag = 0;
    if (strstr(cmd.c_str(), "chat_msg")) {
        qf_msg_mgr<user_msg *> * to_sale = base_singleton<qf_msg_mgr<user_msg *> >::get_instance();        
        to_sale->push(atoi(msg->to_id.c_str()), msg);
        flag = 1;

        char t_buf[SIZE_LEN_256];
        snprintf(t_buf, sizeof(t_buf), "%ld", time(NULL));
        msg->stime.append(t_buf);
    }

    qf_msg_mgr<sale_msg *> * to_visitor = base_singleton<qf_msg_mgr<sale_msg *> >::get_instance();

    list<sale_msg *> to_visitor_list;
    to_visitor->pop(atoi(msg->visitor_id.c_str()), to_visitor_list);

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartArray();
    list<sale_msg *>::iterator it;
    for (it = to_visitor_list.begin() ; it != to_visitor_list.end(); it++) {
        writer.StartObject();

        writer.Key("sales_id");
        writer.String((*it)->sales_id.c_str());

        writer.Key("msg");
        writer.String((*it)->msg.c_str());

        writer.Key("stime");
        writer.String((*it)->stime.c_str());

        writer.EndObject();

        delete (*it);
    }
    writer.EndArray();

    struct evbuffer *evb = evbuffer_new();
    evbuffer_add_printf(evb, "%s", s.GetString());
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);

    if (!flag) {
        delete msg;
    }
}

void http_server_thread::do_sendVisitor(struct evhttp_request *req)
{
    string query;
    struct evbuffer* buf = evhttp_request_get_input_buffer(req);
    size_t len = evbuffer_get_length(buf);
    unsigned char * str = evbuffer_pullup(req->input_buffer, len);

    query.append(str, len);
    evbuffer_drain(buf, len);

    //char *decoded_query = evhttp_uridecode(query, 0, NULL);

    vector<string> tmp_vec;
    SplitString(query, "&", tmp_vec);

    sale_msg * msg = new sale_msg;
    string cmd;

    for (uint32_t i = 0; i < tmp_vec.size(); i++) {
        vector<string> tt_vec;
        SplitString(tmp_vec[i].c_str(), "=", tt_vec);
        if (tt_vec.size() != 2) {
            break;
        }
        if (strstr(tt_vec[0].c_str(), "cmd")) {
            cmd = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "sales_id")) {
            msg->sales_id = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "msg")) {
            msg->msg = tt_vec[1];
        } else if (strstr(tt_vec[0].c_str(), "to_id")) {
            msg->to_id = tt_vec[1];
        }
    }


    int flag = 0;
    if (strstr(cmd.c_str(), "chat_msg")) {
        qf_msg_mgr<sale_msg *> * to_visitor = base_singleton<qf_msg_mgr<sale_msg *> >::get_instance();        
        to_visitor->push(atoi(msg->to_id.c_str()), msg);
        flag = 1;

        char t_buf[SIZE_LEN_256];
        snprintf(t_buf, sizeof(t_buf), "%ld", time(NULL));
        msg->stime.append(t_buf);
    }

    qf_msg_mgr<user_msg *> * to_sale = base_singleton<qf_msg_mgr<user_msg *> >::get_instance();        

    list<user_msg *> to_sale_list;
    to_sale->pop(atoi(msg->sales_id.c_str()), to_sale_list);

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartArray();
    list<user_msg *>::iterator it;
    for (it = to_sale_list.begin() ; it != to_sale_list.end(); it++) {
        writer.StartObject();

        writer.Key("visitor_id");
        writer.String((*it)->visitor_id.c_str());

        writer.Key("msg");
        writer.String((*it)->msg.c_str());

        writer.Key("stime");
        writer.String((*it)->stime.c_str());

        writer.EndObject();
        delete (*it);
    }
    writer.EndArray();

    struct evbuffer *evb = evbuffer_new();
    evbuffer_add_printf(evb, "%s", s.GetString());
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);

    if (!flag) {
        delete msg;
    }
}


void http_server_thread::do_call_back(struct evhttp_request *req, void *arg)
{
    const char *cmdtype;
    struct evkeyvalq *headers;
    struct evkeyval *header;
    struct evhttp_uri *decoded = NULL;
    struct evbuffer * buf = NULL;
    struct evbuffer *evb = NULL;
    char *decoded_path = NULL;
    const char *path = NULL;
    const char *uri = NULL;
    char t_buf[SIZE_LEN_4096];
    int ret = 0;

    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        evhttp_send_error(req, HTTP_BADMETHOD, 0);
        goto done;
    }

    uri = evhttp_request_get_uri(req);
    decoded = evhttp_uri_parse(uri);
    if (!decoded) {
        printf("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        goto done;
    }

    path = evhttp_uri_get_path(decoded);
    if (!path) path = "/";

    decoded_path = evhttp_uridecode(path, 0, NULL);
    if (!decoded_path) {
        evhttp_send_error(req, HTTP_BADREQUEST, "bad request");
        goto done;
    }

    LOG_DEBUG("query:%s decoded_path:%s\n", evhttp_uri_get_query(decoded), decoded_path);

    if (strstr(decoded_path, SEND_MSG_URL)) {
        do_sendmsg(req);
    }else if (strstr(decoded_path, SEND_VISITOR_URL)) {
        do_sendVisitor(req);
    } else {
        evhttp_send_error(req, HTTP_BADREQUEST, "bad request");
    }

    goto done;

done:
    if (decoded)
        evhttp_uri_free(decoded);

    if (decoded_path)
        free(decoded_path);
}



vector<http_server_thread *>  http_server_thread::_httpser_thread_vec;
