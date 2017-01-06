#ifndef __COMMON_THREAD_OBJ_H__
#define __COMMON_THREAD_OBJ_H__

#include "base_def.h"


namespace MZFRAME {


    class common_thread_obj
    {
        public:
            common_thread_obj():_base_container(NULL){};
            virtual ~common_thread_obj(){
                if (_base_container){
                    delete _base_container;
                }
            };

            virtual void* run()
            {
                while (get_run_flag()) {
                    real_run();
                }
            }
            virtual void real_run() = 0;

            void set_container(base_net_container * net_container)
            {
                _base_container = net_container;
            }
            
            NET_OBJ * gen_connect(const int fd) = 0;

            obj_id_str & gen_id_str() = 0;

        protected:
            base_net_container * _base_container;
            obj_id_str _id_str;
    };

}
#endif
