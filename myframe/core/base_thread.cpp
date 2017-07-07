#include "base_thread.h"
#include "thread_helper.h"


base_thread::base_thread():_thread_id(0), _run_flag(true)
{
    thread_lock lock(&base_thread::_mutex);
    _thread_index_start++;
    _thread_index = _thread_index_start;
}

base_thread::~base_thread()
{
}

bool base_thread::start()
{
    if (_thread_id) {
        return false;
    }

    int ret = pthread_create(&_thread_id, NULL, base_thread_proc, this);
    if (ret != 0)
    {
        return false;
    }
    _thread_vec.push_back(this);
    return true;
}

void base_thread::join_thread()
{
    pthread_join(_thread_id, NULL);
}

bool base_thread::stop() 
{
    _run_flag = false;
    return true;
}


void * base_thread::base_thread_proc(void *arg)
{
    base_thread *p = (base_thread*)arg;
    return p->run();
}

void base_thread::stop_all_thread()
{
    vector<base_thread*>::iterator it;
    for (it = _thread_vec.begin(); it != _thread_vec.end(); it++){
        (*it)->stop();
    }
}

void base_thread::join_all_thread()
{
    vector<base_thread*>::iterator it;
    for (it = _thread_vec.begin(); it != _thread_vec.end(); it++){
        (*it)->join_thread();
    }
}


bool base_thread::get_run_flag()
{
    return _run_flag;
}

void base_thread::set_run_flag(bool flag)
{
    _run_flag = flag;
}

pthread_t base_thread::get_thread_id()
{
    return _thread_id;
}

uint32_t base_thread::get_thread_index()
{
    return _thread_index;
}


vector<base_thread*> base_thread::_thread_vec;

uint32_t base_thread::_thread_index_start;

thread_mutex_t base_thread::_mutex;
