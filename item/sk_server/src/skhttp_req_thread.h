#ifndef __SKHTTP_REQ_THREAD_H_
#define __SKHTTP_REQ_THREAD_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "rquotation_data_process.h"
#include "proc_data.h"
#include "sk_def.h"
#include "ua_dict.h"
#include "id_dict.h"

class skhttp_req_thread:public base_net_thread
{
    public:

        skhttp_req_thread()
        {
            _id_index = 0;
            _real_req = true;
        }

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg)
        {
            return;
        }

        void get_domain(std::string & domain, std::vector<std::string> & vip)
        {
            std::map<std::string, std::vector<std::string> >::iterator it;
            it = _domain_cache.find(domain);
            if (it != _domain_cache.end())
            {
                vip = it->second;
            }
        }

        void add_domain(std::string & domain, std::vector<std::string> & vip)
        {
            _domain_cache[domain] = vip;
        }

        void delete_domain(std::string & domain)
        {
            _domain_cache.erase(domain);
        }

        void real_req_start()
        {
            proc_data* p_data = proc_data::instance();
            if (_real_req && p_data && p_data->_id_dict)
            {
                req_real_quotation("sz002285");
                id_dict * id_dic = p_data->_id_dict->current();
                if (id_dic && _id_index < id_dic->_id_vec.size())
                {
                    std::string id = id_dic->_id_vec[_id_index];
                    req_real_quotation(id);
                    _id_index++;
                }
                else
                {
                    real_timer_start();
                }
            }
        }

        void real_timer_start()
        {   
            proc_data* p_data = proc_data::instance();

            if (p_data && p_data->_conf)
            {   
                _real_req = false;
                timer_msg t_msg;

                t_msg._timer_type = TIMER_TYPE_REAL_REQ;
                t_msg._time_length = p_data->_conf->reload_second;
                add_timer(t_msg);
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

        void req_real_quotation(const std::string & id)
        {
            char t_url[SIZE_LEN_64];
            snprintf(t_url, sizeof(t_url), "http://web.sqt.gtimg.cn/q=%s", id.c_str());
            
            url_info info;
            get_url_info(t_url, info);
            http_req_msg req_msg;
            req_msg.url.append(t_url);
            
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                ua_dict * ua_dic = p_data->_ua_dict->current();
                req_msg.headers.insert(std::make_pair("User-Agent", 
                            ua_dic->_ua_vec[_id_index % ua_dic->_ua_vec.size()]));
            }

            std::shared_ptr<base_net_obj>  connect = 
                std::shared_ptr<base_net_obj>(rquotation_data_process::gen_net_obj(info, req_msg));

            connect->set_net_container(_base_container);
        }

        void get_url_info(const char * url, url_info & info)
        {
            bool flag = false; 
            parse_url(url, info);

            std::vector<std::string> vIp;

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
                return ;
            }

            srand((uint32_t)url);
            int index = rand() % vIp.size();
            info.ip = vIp[index];
        }

        virtual bool handle_timeout(timer_msg & t_msg)
        {
            if (t_msg._timer_type == TIMER_TYPE_REAL_REQ)
            {
                _id_index = 0;
                if (is_real_time())
                    _real_req = true;

                real_req_start();
            }
        }

        virtual void run_process()
        {
            real_req_start();    
        }

    protected:
        std::map<std::string, std::vector<std::string> > _domain_cache;
        uint32_t _id_index;
        bool _real_req;
};





#endif
