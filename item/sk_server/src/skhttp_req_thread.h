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
#include "common_obj_container.h"

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

        void real_req_start()
        {
            proc_data* p_data = proc_data::instance();
            if (_real_req && p_data && p_data->_id_dict)
            {
                if (!_id_index)
                {
                    req_real_quotation("sz002285");
                    _id_index += 1;
                }
                return;

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
                std::shared_ptr<timer_msg> t_msg(new timer_msg);

                t_msg->_timer_type = TIMER_TYPE_REAL_REQ;
                //t_msg._time_length = p_data->_conf->req_interval_second;
                t_msg->_time_length = 3;
                t_msg->_obj_id = OBJ_ID_THREAD;
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
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                auto it = p_data->_rquoation_dict->current()->_id_dict.find(id);
                if (it == p_data->_rquoation_dict->current->_id_dict.end())
                {
                    LOG_DEBUG("id: %s is not in _rquoation_dict", id.c_str());
                    return;
                }

                if (it->second.current()->blocked)
                {
                    LOG_DEBUG("id: %s is blocked", id.c_str());
                    return;
                }

                std::map<std::string, std::string> headers;
                ua_dict * ua_dic = p_data->_ua_dict->current();

                headers.insert(std::make_pair("User-Agent", ua_dic->_ua_vec[_id_index % ua_dic->_ua_vec.size()]));

                rquotation_data_process::gen_net_obj(id, get_net_container(), headers);
            }

        }

        virtual void handle_timeout(timer_msg & t_msg)
        {
            if (t_msg._timer_type == TIMER_TYPE_REAL_REQ)
            {
                _id_index = 0;
                if (is_real_time())
                    _real_req = true;

                LOG_DEBUG("handle_timeout: TIMER_TYPE_REAL_REQ");
                real_req_start();
            }
        }

        virtual void run_process()
        {
            real_req_start();    
        }

    protected:
        uint32_t _id_index;
        bool _real_req;
};





#endif
