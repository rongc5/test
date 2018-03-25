#ifndef __SKHTTP_REQ_THREAD_H_
#define __SKHTTP_REQ_THREAD_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "skhttp_req_data_process.h"
#include "http_req_process.h"
#include "out_connect.h"

class skhttp_req_thread:public base_net_thread
{
    public:
        virtual void handle_msg(normal_msg * p_msg)
        {
            if (!p_msg) 
            {
                return;
            }

            if (p_msg->_msg_op == MSG_HTTP_REQ) 
            {
                http_req_msg * req_msg = dynamic_cast<http_req_msg *>(p_msg);
                if (!req_msg) {
                    REC_OBJ<normal_msg> rc(p_msg);
                    return;
                }

                base_net_obj * connect = skhttp_req_data_process::gen_net_obj(req_msg);
                connect->set_net_container(_base_container);
                LOG_DEBUG("set http_req_process");
            }
            else
            {
                REC_OBJ<normal_msg> rc(p_msg);
                return;
            }
        }

        void get_domain(string & domain, vector<string> & vip)
        {
            map<string, vector<string> >::iterator it;
            it = _domain_cache.find(domain);
            if (it != _domain_cache.end())
            {
                vip = it->second;
            }
        }

        void add_domain(string & domain, vector<string> & vip)
        {
            _domain_cache[domain] = vip;
        }

        void delete_domain(string & domain)
        {
            _domain_cache.erase(domain);
        }

    protected:
        map<string, vector<string> > _domain_cache;
};





#endif
