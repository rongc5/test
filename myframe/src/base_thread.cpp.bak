#include "log_helper.h"
#include "base_thread.h"

namespace MZFRAME {

    base_thread::base_thread():_thread_id(0), _run_flag(true)
    {
    }

    base_thread::~base_thread()
    {
    }

    bool base_thread::start()
    {
        int ret = pthread_create(&_thread_id, NULL, base_thread_proc, this);
        if (ret != 0)
        {
            LOG_WARNING("start thread fail");
                return false;
        }
        _thd_vec.push_back(this);
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

    vector<base_thread*> base_thread::_thd_vec;
}

