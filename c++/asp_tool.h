/*
 * asp_tool.h
 *
 *  Created on: 2013-6-21
 *      Author: fengtian
 */

#ifndef LY_ASP_TOOL_H_
#define LY_ASP_TOOL_H_

#include<string.h>
#include <vector>
#include <string>
#include "zmq.h"

#define PATH_MAX_LEN 256

class zmq_base {
    
    public:
        virtual int init(char * path);
        virtual int do_send(char * buf, size_t len);
        virtual int do_recv(char * buf, size_t len);
        virtual ~zmq_base(){ 
            zmq_close (zmq_fd);
            zmq_ctx_destroy(context);
        }

    protected:
        char zeromq_address[PATH_MAX_LEN];
        void * context;
        void *zmq_fd;

};

class zmq_client : public zmq_base{
    public:
        virtual int init(char * path);
};


class zmq_server : public zmq_base{
    public:
        virtual int init(char * path);
};


#endif /* LY_ASP_TOOL_H_ */
