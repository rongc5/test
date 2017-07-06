#ifndef __EVENT_CHANNEL_MSG_H__
#define __EVENT_CHANNEL_MSG_H__

#include "common_def.h"

struct event_channel_msg{
    int _channelid;
    base_net_obj * _base_obj;

    event_channel_msg():_channelid(0), _base_obj(NULL){}
    ~event_channel_msg(){}
};


#endif
