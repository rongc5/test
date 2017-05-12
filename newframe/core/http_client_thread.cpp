#include "http_client_thread.h"
#include "http_client_connect.h"
#include "log_helper.h"

http_client_thread::http_client_thread()
{
    _http_thread_vec.push_back(this);
}

bool http_client_thread::handle_msg(base_passing_msg * msg)
{

    if (!msg) {
        return true;
    }   
    
    LOG_DEBUG("handle_msg");
    
    try {
        http_client_connect::do_request(msg, get_event_base());
    } catch (...) {
        REC_OBJ<base_passing_msg> rc(msg);
        //WARNING_LOG("do_request exception");
    }  

    return true;
}


void http_client_thread::put_msg(base_passing_msg * msg)
{
	int index = (unsigned long)msg  % _http_thread_vec.size();
	_http_thread_vec[index]->add_msg(msg);
}

vector<http_client_thread *>  http_client_thread::_http_thread_vec;
