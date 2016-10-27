#ifndef __THREAD_HELPER_H_
#define __THREAD_HELPER_H_

#include <pthread.h>


namespace MZFRAME {

class thread_mutex_t{
    public:
        thread_mutex_t();
        ~thread_mutex_t();
        void lock();
        void unlock();
    private:
        pthread_mutex_t _mutex;
};

class thread_lock {

    public:
        thread_lock(thread_mutex_t * mutex);
        ~thread_lock();
    private:
        thread_mutex_t * _mutex;
};

thread_rwlock_t{
    public:
        thread_rwlock_t();
        ~thread_rwlock_t();
        void read_lock();
        void write_lock();
        void unlock();

    private:
        pthread_rwlock_t _rwlock;
};


}

#endif
