#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_def.h"


namespace MZFRAME {

    template<class WORKER_MGR>
    class listen_thread:public common_thread
    {
        public:
            listen_thread():_base_container(NULL){
            };
            virtual ~listen_thread(){
                if (_base_container){
                    delete _base_container;
                }
            };

            virtual void obj_process()
            {
                _base_container->obj_process();
            }

            virtual void set_channelid(int fd)
            {
                common_thread::set_channelid(fd);

            }

            void init(const string &ip, unsigned short port)
            {
                _ip = ip;
                _port = port;
                _count = 0;
                _base_container = new common_obj_container();
                listen_connect<listen_data_process> * p_connect
                    = new listen_connect<listen_connect>();
                listen_data_process * data_process = new listen_data_process(p_connect);
                p_connect->set_id(gen_id_str());
                p_connect->init(ip, port);
                data_process->set_thread(this);
                p_connect->set_process(data_process);
                p_connect->set_net_container(_base_container);

                passing_msg_thread::register_thread(_listen_obj_thread);
            }

            pthread_t get_worker_id()
            {
                _worker_mgr->get_one_worker()->get_thread_id();
            }

            void set_worker_mgr(WORKER_MGR * worker_mgr)
            {
                if (_worker_mgr && _worker_mgr != worker_mgr){
                    delete _worker_mgr;
                }
                
                _worker_mgr = worker_mgr;
            }

        protected:
            const obj_id_str & gen_id_str()
            {
                _id_str._thread_id = get_thread_id();
                _id_str._obj_id++;
                return _id_str;
            }

        protected:
            string _ip;
            unsigned short _port;
            WORKER_MGR * _worker_mgr;
            base_net_container * _base_container;
            obj_id_str _id_str;
    };

}
#endif
