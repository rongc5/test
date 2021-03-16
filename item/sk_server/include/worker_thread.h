#ifndef __worker_thread_H__
#define __worker_thread_H__

#include "base_net_thread.h"
#include "common_obj_container.h"
#include "listen_connect.h"
#include "common_def.h"
#include "listen_process.h"
#include "sk_def.h"

#include "curl_req.h"

class worker_thread: public base_net_thread
{
    public:
        worker_thread(int channel_num = 1):base_net_thread(channel_num)
    {
        //_curl.init();
    }
        void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        static void put_req_msg(std::shared_ptr<req_msg> & p_msg, int index = 0);

        virtual ~worker_thread()
        {
        }

        curl_req & get_curl();


    protected:

        curl_req _curl;
};

#endif
