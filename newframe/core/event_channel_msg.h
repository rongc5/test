#ifndef __EVENT_CHANNEL_MSG_H__
#define __EVENT_CHANNEL_MSG_H__

#include "common_def.h"

class base_net_thread;
struct event_channel_msg{
    base_net_thread * _net_thread;
    deque<base_passing_msg *> _queue;
    thread_mutex_t _mutex;
    bool _is_lock;
    int _channelid;

    event_channel_msg():_net_thread(NULL), _is_lock(false), _channelid(0){}
    ~event_channel_msg(){}
};



#endif
