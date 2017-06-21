#include "http_server_thread.h"
#include "log_helper.h"

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

    evhttp_set_gencb(_httpd, do_request_cb, NULL);
    //evhttp_set_cb(_http, "/dump", do_request_cb, NULL);

    base_net_thread::run();
    
    return NULL;
}

void http_server_thread::do_request_cb(struct evhttp_request *req, void *arg)
{
    	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	LOG_DEBUG("Received a %s request for %s\nHeaders:\n",
	    cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
	 header = header->next.tqe_next) {
		LOG_DEBUG("  %s: %s\n", header->key, header->value);
	}

	buf = evhttp_request_get_input_buffer(req);
	LOG_DEBUG("Input data: <<<\n");
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[128];
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	PDEBUG(">>>\n");

	evhttp_send_reply(req, 200, "OK", NULL);
}



vector<http_server_thread *>  http_server_thread::_httpser_thread_vec;
