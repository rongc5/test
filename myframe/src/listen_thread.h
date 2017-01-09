#ifndef __LISTEN_THREAD_H__
#define __LISTEN_THREAD_H__

#include "base_def.h"


namespace MZFRAME {


    class listen_thread:public common_thread
    {
        public:
            listen_thread():_base_container(NULL){
                _base_container = new common_obj_container();
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

            virtual void *run()
            {
                init();
                obj_process();
            }

            void init(const string &ip, unsigned short port)
            {
                listen_connect<>                

            }
            

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
