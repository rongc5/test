#ifndef __COMMON_QUEUE_H__
#define __COMMON_QUEUE_H__


#include "base_def.h"
#include "thread_helper.h"



template<class T> 
class common_queue
{
    public:
        common_queue(size_t num, bool is_lock);

        virtual ~common_queue(){}
        
        bool push(T *);
        T * pop();

        int length();

        void check_pop(common_queue<T> * res_queue);


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
bool common_queue<T>::push(T * t)
{
    if (!t)
        return false;

    if (_is_lock) {
        thread_lock lock(&_mutex);

        if (_max_size > 0 && _queue.size() >_max_size)
            return false;

        _queue.push_back(t);
    }else {
        if (_max_size > 0 && _queue.size() >_max_size)
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
    if (_is_lock) {
        thread_lock lock(&_mutex);
        return _queue.size();
    } else {
        return _queue.size();
    }
}

template<class T>
 void common_queue<T>::check_pop(common_queue<T> * res_queue)
{
    if (!res_queue) {
        return;
    }

    if (_is_lock) {
        thread_lock lock(&_mutex);
         
        QIT it;
        for (it = _queue.begin(); it != _queue.end();) {
            if ((*it)->check()) {
                if (!res_queue->push(*it)) {
                    (*it)->release();
                }
                it = _queue.erase(it);
            } else {
                ++it;
            }
        }

    }
}



#endif
