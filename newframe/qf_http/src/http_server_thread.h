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

        void do_call_back(struct evhttp_request *req, void *arg);

        void do_sendmsg(struct evhttp_request *req);

        void do_sendmsg_begin(struct evhttp_request *req, user_msg * msg);

        void do_sendmsg_close(struct evhttp_request *req, user_msg * msg);

        void do_sendmsg_chat(struct evhttp_request *req, user_msg * msg);

        void do_sendmsg_invite(struct evhttp_request *req, user_msg * msg);

        void do_sendmsg_change(struct evhttp_request *req, user_msg * msg);


        void do_sendVisitor(struct evhttp_request *req);

    protected:
        int _nfd;
        struct evhttp *_httpd;
        static vector<http_server_thread *>  _httpser_thread_vec;
};



#endif
