#ifndef __HTTP_CLIENT_CONNECT_H__
#define __HTTP_CLIENT_CONNECT_H__

#include "base_net_thread.h"
#include "common_def.h"
#include "base_connect.h"

class http_client_connect
{
    public:

        http_client_connect();

        ~http_client_connect()
        {   
            //DEBUG_LOG("destory http_client_connect\n");
        }   

        static void do_request(base_passing_msg * p_msg, struct event_base* base);


        static void http_request_done(struct evhttp_request *req, void *arg);
};




#endif
