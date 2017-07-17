#ifndef __QF_MSG_DEF_H_
#define __QF_MSG_DEF_H_

#include "common_def.h"
#include "thread_helper.h"
#include "log_helper.h"

#define SEND_MSG_URL "sendmsg"
#define SEND_VISITOR_URL "sendVisitor"

struct user_msg
{
    string to_id;
    string visitor_id;
    string msg;
    time_t stime;
};

struct sale_msg
{
    string to_id;
    string sales_id;
    string msg;
    time_t stime;
};

template<class T>
class qf_msg_mgr
{
    public:
        void init(uint32_t bucket=2000000)
        {
            _bucket = get_prime_num(bucket);
            LOG_DEBUG("_bucket:%d", _bucket);
            _mutex = new thread_mutex_t[_bucket]; 
            _list = new list<T>[_bucket];
        }

        void push(uint32_t id, T & t)
        {
            uint32_t index = id % _bucket;
            thread_lock lock(&_mutex[index]);
            _list[index].push_back(t);
        }
        void pop(uint32_t id, list<T> & msg_list) 
        {
            uint32_t index = id % _bucket;
            thread_lock lock(&_mutex[index]);
            msg_list = _list[index];
            _list[index].clear();
        }

    private:
        uint32_t _bucket;
        list<T> * _list;
        thread_mutex_t * _mutex;
};



#endif
