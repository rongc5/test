#ifndef __COMMON_QUEUE_H__
#define __COMMON_QUEUE_H__


#include "common_utils.h"
#include "thread_helper.h"
#include "log.h"



template<class T> 
class common_queue
{
    public:
        common_queue(size_t num, bool is_lock);

        virtual ~common_queue(){}

        T * pop(T2 *);
        
        template<class T2>
        bool push(T*, T2*);

        bool push(T*);

        int length();

        template<class T2>
            void check_pop(T2 * t);


    protected:


        typedef typename deque<T *>::iterator QIT;


        deque<T*> _queue;
        thread_mutex_t _mutex;
        bool _is_lock;
        size_t _max_size; // > 0 valid;
};

template<class T>
common_queue<T>::common_queue(size_t num, bool is_lock)
{
    _is_lock = is_lock;
    _max_size = num;
}

template<class T>
template<class T2>
bool common_queue<T>::push<T2>(T * t, T2 * t2)
{
    if (!t)
        return false;

    if (_is_lock) {
        thread_lock lock(&_mutex);

        if (_max_size > 0 && _queue.size() >=_max_size)
            return false;

        _queue.push_back(t);
        if (t2) {
            t2->after_push();
        }

    }else {
        if (_max_size > 0 && _queue.size() >=_max_size)
            return false;

        _queue.push_back(t);
        if (t2) {
            t2->after_push();
        }
    }

    return true;
}

template<class T>
bool common_queue<T>::push(T * t)
{
    if (!t)
        return false;

    if (_is_lock) {
        thread_lock lock(&_mutex);

        if (_max_size > 0 && _queue.size() >=_max_size)
            return false;

        _queue.push_back(t);

    }else {
        if (_max_size > 0 && _queue.size() >=_max_size)
            return false;

        _queue.push_back(t);
    }

    return true;
}

    template<class T>
T * common_queue<T>::pop()
{
    T * t = NULL;

    if (_is_lock) {
        thread_lock lock(&_mutex);

        if (_queue.begin() != _queue.end()) {
            t = _queue.front();
            _queue.pop_front();
        }

    } else {

        if (_queue.begin() != _queue.end()) {
            t = _queue.front();
            _queue.pop_front();
        }

    }

    return t;
}
    template<class T>
int common_queue<T>::length()
{
    return _queue.size();
}

template<class T>
    template<class T2>
void common_queue<T>::check_pop(T2 * t)
{
    if (!t) {
        return;
    }

    if (_is_lock) {
        thread_lock lock(&_mutex);

        QIT it;
        for (it = _queue.begin(); it != _queue.end();) {
            if (!t->handle_msg(*it)) {
                (*it)->release();
            }
        }
        it = _queue.erase(it);

    } else {

        QIT it;
        for (it = _queue.begin(); it != _queue.end();) {
            if (!t->handle_msg(*it)) {
                (*it)->release();
            }
        }
        it = _queue.erase(it);

    }
}



#endif
