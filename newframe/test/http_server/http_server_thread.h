#ifndef __HTTP_SERVER_THREAD_H__
#define __HTTP_SERVER_THREAD_H__

#include "base_net_thread.h"

class http_server_thread:public base_net_thread
{
    public:

        http_server_thread();
        virtual ~http_server_thread(){}

        static int bind_port(int port);

        virtual bool handle_msg(base_passing_msg * msg);
        
        static void put_msg(base_passing_msg * msg);

        virtual void *run();

        void set_nfd(int nfd);

        static void do_request_cb(struct evhttp_request *req, void *arg);

    protected:
        int _nfd;
        struct evhttp *_httpd;
        static vector<http_server_thread *>  _httpser_thread_vec;
};



#endif
