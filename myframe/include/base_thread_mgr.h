#ifndef __BASE_THREAD_MGR_H__
#define __BASE_THREAD_MGR_H__

#include "common_def.h"
#include "base_thread.h"


class base_thread_mgr
{
    public:
        base_thread_mgr(){
            _it_count = _thread_mgr.begin();
        }
        virtual ~base_thread_mgr(){
        }

        
        void add_thread(base_thread * thread)
        {
            PDEBUG("add threadid[%lu] index[%lu]\n", thread->get_thread_id(), thread->get_thread_index());
            _thread_mgr.insert(thread);
        }


        void start()
        {
            set<base_thread *>::iterator it;
            for(it = _thread_mgr.begin(); it != _thread_mgr.end(); it++) {
                (*it)->start(); 
            }
        }

        void stop()
        {
            set<base_thread *>::iterator it;
            for(it = _thread_mgr.begin(); it != _thread_mgr.end(); it++) {
                (*it)->stop(); 
            }
        }

        virtual base_thread * get_idle_worker()
        {
            if (_thread_mgr.begin() == _thread_mgr.end()){
                return NULL;
            }

            if (_it_count == _thread_mgr.end()){
                _it_count = _thread_mgr.begin();
            }
            PDEBUG("size:[%d] threadid[%lu]\n", _thread_mgr.size(), (*_it_count)->get_thread_id());

            set<base_thread *>::iterator tmp_it = _it_count;
            _it_count++;
            return *tmp_it;
        }

    protected:
        set<base_thread *>::iterator _it_count;
        set<base_thread *> _thread_mgr;
};

#endif

