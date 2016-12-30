#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "base_def.h"


namespace MZFRAME {


    class common_thread:public base_thread
    {
        public:
            common_thread();
            ~common_thread();
            void init();
            int put_msg(pthread_t thd, base_obj *p_obj);
            void *run();
            void obj_process();
            void set_channelid(int fd)
            {
                _channel_id = fd;
            }

        protected:
            const obj_id_str & get_id_str()
            {
                id_str._obj_id++;
                return id_str;
            }


        protected:

            base_net_container * _base_container;
            obj_id_str _id_str;
            int _channel_id;
    };

}
#endif
