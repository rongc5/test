#ifndef __EVENT_CHANNEL_MSG_H__
#define __EVENT_CHANNEL_MSG_H__

#include "common_def.h"
#include "common_queue.h"

class base_net_thread;
struct event_channel_msg{
    base_net_thread * _net_thread;
    common_queue<base_passing_msg> * _queue;

    event_channel_msg():_net_thread(NULL), _queue(NULL){}
};



#endif
