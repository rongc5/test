#ifndef __BASE_THREAD_HPP_
#define __BASE_THREAD_HPP_

#include <pthread.h>
#include <vector>

namespace MZFRAME {

    using namespace std;

    class base_thread
    {
        public:
            base_thread();

            virtual ~base_thread();

            virtual bool start();

            void join_thread();

            virtual bool stop();

            virtual void* run() = 0;

            static void *base_thread_proc(void *arg);

            bool get_run_flag();

            void set_run_flag(bool flag);

            pthread_t get_thread_id();

        protected:
            pthread_t _thread_id;
            bool _run_flag;
            static vector<base_thread*> _thd_vec;
    };

}

#endif


