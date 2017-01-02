#ifndef __COMMON_NET_THREAD_H__
#define __COMMON_NET_THREAD_H__

#include "base_def.h"


namespace MZFRAME {


    class common_net_thread:public common_thread
    {
        public:
            common_net_thread():_base_container(NULL){
                _base_container = new common_obj_container();
            };
            virtual ~common_net_thread(){
                if (_base_container){
                    delete _base_container;
                }
            };

            virtual void obj_process();

        protected:
            const obj_id_str & gen_id_str()
            {
                id_str._obj_id++;
                return id_str;
            }

        protected:

            base_net_container * _base_container;
            obj_id_str _id_str;
    };

}
#endif
