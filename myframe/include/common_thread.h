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

        void set_passing_type(int type)
        {
            //PDEBUG("%lu %d\n", get_thread_id(), type);
            _passing_type = type;
        }

        int get_passing_type()
        {
            return _passing_type;
        }

    protected:
        int _passing_type;
        int _channel_id;
};

#endif
