#ifndef __HTTP_CLIENT_CONNECT_H__
#define __HTTP_CLIENT_CONNECT_H__

#include "base_net_thread.h"
#include "common_def.h"
#include "base_connect.h"

class job_connect;
class base_net_thread;
class http_client_connect:public base_connect
{
    public:

        http_client_connect(base_net_thread * thread);

        virtual ~http_client_connect()
        {
            close();
        }

        void close();

        void init(const string & url, job_connect * t_conn);

        void call_back(struct evhttp_request *req, void *arg);

        static void http_request_done(struct evhttp_request *req, void *arg);

        static http_client_connect * gen_connect(string & url, job_connect * t_conn, base_net_thread * thread);

    protected:
        struct evhttp_uri *_uri;
        struct evhttp_connection *_cn;
        struct evhttp_request *_req;
        job_connect * _t_conn;
        string _url;

        char _res_buf[SIZE_LEN_65536];
};


#endif
