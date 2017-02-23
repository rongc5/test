#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__

#include "common_def.h"
#include "base_thread.h"



class common_thread:public base_thread
{
    public:
        common_thread();
        virtual ~common_thread(){};
        virtual void *run();
        virtual void obj_process() = 0;
        virtual void set_channelid(int fd)
        {
            _channel_id = fd;
        }

        int get_channelid()
        {
            return _channel_id;
        }

    protected:
        int _channel_id;
};

#endif
