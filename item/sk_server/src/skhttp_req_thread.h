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

        skhttp_req_thread()
        {
            memset(&_tranverse, 0, sizeof(_tranverse));
           
            _id_index = 0;
        }

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


                base_net_obj * connect = skhttp_req_data_process::gen_net_obj(req_msg, info);
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

        void real_req_start()
        {
            proc_data* p_data = proc_data::instance();
            if (is_real_time() && p_data && p_data->_id_dict)
            {
                id_dict * id_dic = p_data->_id_dict->current();
                if (id_dic && _id_index < id_vec->_id_dic.size())
                {
                    string id = id_vec->_id_dic[_id_index];
                    _id_index++;
                }
            }
        }

        void real_timer_start()
        {   
            proc_data* p_data = proc_data::instance();

            if (p_data && p_data->_conf)
            {   
                timer_msg t_msg;

                t_msg._timer_type = TIMER_TYPE_REAL_REQ;
                t_msg._time_length = p_data->_conf->reload_second;
                add_timer(timer_msg & t_msg);
            }
        }

        bool is_real_time()
        {
            char time_str[SIZE_LEN_64] = {'\0'};
            get_timestr(time_str, sizeof(time_str), "%H:%M:%S");

            proc_data* p_data = proc_data::instance();

            if (strlen(time_str) && p_data && p_data->_conf)
            {
                if (strncmp(time_str, p_data->_conf->real_morning_stime.c_str(), strlen("09:30")) > 0 &&
                        strncmp(time_str, p_data->_conf->real_morning_etime.c_str(), strlen("09:30")) < 0)
                    return true;

                if (strncmp(time_str, p_data->_conf->real_morning_etime.c_str(), strlen("09:30")) < 0 && 
                        strncmp(time_str, p_data->_conf->real_afternoon_etime.c_str(), strlen("09:30")) < 0)
                    return true;
            }

            return false;
        }

        void req_real_quotation(string & id)
        {
            char t_url[SIZE_LEN_64];
            snprintf(t_url, "http://web.sqt.gtimg.cn/q=%s", id.c_str());


        }

        void get_url_info(const char * url, string & ip)
        {
            bool flag = false; 
            url_info info;
            parse_url(req_msg->url, info);

            vector<string> vIp;

            get_domain(info.domain, vIp);
            if (!vIp.size())
            {
                parse_domain(info.domain, vIp);
                flag = true;
            }

            if (flag && vIp.size())
            {
                add_domain(info.domain, vIp);
            }

            if (!vIp.size())
            {
                REC_OBJ<normal_msg> rc(p_msg);
                return 0;
            }

            srand(p_msg);
            int index = rand() % vIp.size();
            info.ip = vIp[index];
        }

        virtual bool handle_timeout(timer_msg & t_msg)
        {
            if (t_msg._timer_type == TIMER_TYPE_REAL_REQ)
            {
                _id_index = 0;
                real_req_start();
                real_timer_start();
            }
        }

        virtual void run_process()
        {
            real_req_start();    
        }

    protected:
        map<string, vector<string> > _domain_cache;
        uint32_t _id_index;
};





#endif
