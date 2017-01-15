#ifndef __NET_THREAD_MGR_H__
#define __NET_THREAD_MGR_H__

#include "base_def.h"
#include "passing_msg_thread.h"
#include "common_net_thread.h"

namespace MZFRAME {

    template<class MSG_PROCESS>
        class net_thread_mgr
        {
            public:
                net_thread_mgr(int num):_thread_num(num), _count(0), _worker_thread(NULL){}
                ~net_thread_mgr(){
                    delete [] _worker_thread;
                }


                void start()
                {
                    _worker_thread = new common_net_thread<MSG_PROCESS>[_thread_num];
                    for(int i=0; i < _thread_num; i++) {
                        passing_msg_thread::register_thread(&_worker_thread[i]);
                        _worker_thread[i].start(); 
                    }
                }

                common_net_thread<MSG_PROCESS> * get_one_worker()
                {
                    int i = 0;
                    i = _count % _thread_num;
                    return &_worker_thread[i];
                }

            protected:
                int _thread_num;
                unsigned int _count;
                common_net_thread<MSG_PROCESS> *_worker_thread;
        };
}

#endif

