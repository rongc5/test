#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "base_def.h"


namespace MZFRAME {


    class common_thread:public base_thread
    {
        public:
            common_thread();
            virtual ~common_thread(){};
            virtual void *run();
            virtual void obj_process() = 0;
            void set_channelid(int fd)
            {
                _channel_id = fd;
            }

        protected:

            int _channel_id;
    };

}
#endif
