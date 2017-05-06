#include "event_channel_msg.h"
#include "base_net_thread.h"


event_channel_msg::~event_channel_msg(){
}

void event_channel_msg::after_push()
{
    if (_channelid) {
        write(_channelid, CHANNEL_MSG_TAG, sizeof(CHANNEL_MSG_TAG));
    }
}

