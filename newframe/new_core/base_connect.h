#ifndef __BASE_CONNECT_H__
#define __BASE_CONNECT_H__

#include "log_helper.h"
#include "common_def.h"
#include "common_exception.h"

class base_net_thread;
class base_connect
{
    public:

        base_connect(base_net_thread * thread);

        virtual ~base_connect()
        {
        }

        virtual int destroy();

        const ObjId & get_id();

    protected:
        base_net_thread * _thread;
        ObjId _id_str;
};


#endif
