#include "http_client_thread.h"
#include "http_client_connect.h"

http_client_thread::http_client_thread()
{
    _http_thread_vec.push_back(this);
}

bool http_client_thread::handle_msg(base_passing_msg * msg)
{

    if (!msg) {
        return;
    }   

    //DEBUG_LOG("sid: %s", msg->sid.c_str());

    try {
        http_client_connect::do_request(msg, get_event_base());
    } catch (...) {
        REC_OBJ<base_passing_msg> rc(msg);
        WARNING_LOG("do_request exception");
    }  

    return true;
}


static void put_msg(base_passing_msg * msg)
{
	int index = (unsigned long)msg  % _http_thread_vec.size();
	_http_thread_vec[index]->add_msg(msg);
}
