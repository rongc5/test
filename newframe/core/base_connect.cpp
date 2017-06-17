#include "base_connect.h"
#include "base_net_thread.h"
#include "common_exception.h"
#include "log_helper.h"
#include "base_data_process.h"


base_connect::base_connect(int32_t sock, base_net_thread * thread):_fd(sock), _ev(0),_thread(thread), _process(NULL)
{   
    _id_str = thread->gen_id_str();
}

const ObjId & base_connect::get_id()
{
    return _id_str;
}

void base_connect::set_process(base_data_process *p)
{
    if (_process != NULL)
        delete _process;

    _process = p;
    _process->set_para();
}

size_t base_connect::process_recv_msg(base_passing_msg* p_msg)
{
    if (_process) {
        _process->process_recv_msg(p_msg);
    }else {
        REC_OBJ<base_passing_msg> rc(p_msg);
    }

    return 0;
}

bool base_connect::update_event(short ev)
{
    if (_ev == ev) {
        return true;
    }

    //if (_ev) 
        //event_del(&_event);

    if (_ev && event_del(&_event) == -1) 
        return false;

    event_set(&_event, _fd, ev, on_cb, this);
    event_base_set(_thread->get_event_base(), &_event);
    _ev = ev;
    if (event_add(&_event, 0) == -1) 
        return false;

    return true;
}

short base_connect::get_ev_flags()
{
    return _ev;
}

void base_connect::close()
{
    _thread->destory_connect(_id_str); 

    if (_fd != 0)
    {
        event_del(&_event);
        ::close(_fd);
        _fd = 0;
    }
}

int base_connect::get_sock()
{
    return _fd;
}

void base_connect::get_local_addr(sockaddr_in &addr)
{
    socklen_t len = 0;
    getsockname(_fd, (sockaddr*)&addr, &len);
}

void base_connect::on_cb(int fd, short ev, void *arg)
{
    base_connect * conn = (base_connect *) arg;
    if (conn) {
        try {
            conn->call_back(fd, ev, arg);
        }catch (std::exception & e) {
            LOG_WARNING("destory connect: %s", e.what());
            delete conn;
        }catch (...) {
            LOG_WARNING("unknown destory connect");
            delete conn;
        }
    }
}
