#ifndef __BASE_THREAD_H__
#define __BASE_THREAD_H__

#include "common_def.h"

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


        static void stop_all_thread();


        static void join_all_thread();


        bool get_run_flag();


        void set_run_flag(bool flag);


        pthread_t get_thread_id();

        uint32_t get_thread_index();

    protected:
        uint32_t _thread_index;

        pthread_t _thread_id;
        bool _run_flag;

        static std::vector<base_thread*>	_thread_vec;

        static uint32_t _thread_index_start;
        static std::mutex _mutex;;
};


#endif


