#include "thread_helper.h"


thread_mutex_t::thread_mutex_t()
{
    pthread_mutex_init(&_mutex, NULL);
}

thread_mutex_t::~thread_mutex_t()
{
    pthread_mutex_destroy(&_mutex);
}

void thread_mutex_t::lock()
{
    pthread_mutex_lock(&_mutex);
}

void thread_mutex_t::unlock()
{
    pthread_mutex_unlock(&_mutex);
}

thread_lock::thread_lock(thread_mutex_t * mutex)
{
    _mutex = mutex;
    if (_mutex != NULL){
        _mutex->lock();
    }
}

thread_lock::~thread_lock()
{
    if (_mutex !=NULL){
        _mutex->unlock();
    }
}

thread_rwlock_t::thread_rwlock_t()
{
    pthread_rwlock_init(&_rwlock, NULL);
}

thread_rwlock_t::~thread_rwlock_t()
{
    pthread_rwlock_destroy(&_rwlock);
}

void thread_rwlock_t::read_lock()
{
    pthread_rwlock_rdlock(&_rwlock); 
}

void thread_rwlock_t::write_lock()
{
    pthread_rwlock_wrlock(&_rwlock);
}

void thread_rwlock_t::unlock()
{
    pthread_rwlock_wrlock(&_rwlock);
}

