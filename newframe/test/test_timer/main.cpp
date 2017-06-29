#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"

int lasttime;  

    static void  
timeout_cb(int fd, short event, void *arg)  
{  
    struct timeval tv;  
    struct event *timeout = (struct event *)arg;  
    int newtime = time(NULL);  

    //printf("%s: called at %d: %d\n", __func__, newtime,  
    printf("%s: called at %d: %d\n", "timeout_cb", newtime,  
            newtime - lasttime);  
    lasttime = newtime;  

    evutil_timerclear(&tv);  
    tv.tv_sec = 6;  
    //重新注册event  
    event_add(timeout, &tv);  
}  

    int  
main (int argc, char **argv)  
{  
    struct event timeout;  
    struct timeval tv;  

    /* Initalize the event library */  
    //初始化event环境  
    event_init();  

    /* Initalize one event */  
    //设置事件  
    evtimer_set(&timeout, timeout_cb, &timeout);  

    evutil_timerclear(&tv);  
    tv.tv_sec = 2;  
    //注册事件  
    evtimer_add(&timeout, &tv);  

    lasttime = time(NULL);  

    //等待,分发,处理事件  
    event_dispatch();  

    return (0);  
}  
