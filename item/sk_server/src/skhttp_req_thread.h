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
#include "real_quotation_dict.h"
#include "rsingle_data_process.h"

class skhttp_req_thread:public base_net_thread
{
    public:

        skhttp_req_thread()
        {
            _id_index = 0;
            _real_req = true;
            _is_first = true;
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
                //if (!_id_index)
                //{
                    //req_real_quotation("sz002285");
                    //req_real_single("sz002285");
                    //_id_index += 1;
                //}
                //return;
                if (_base_container && _base_container->size() > 
                        p_data->_conf->_strategy->current()->max_reqhttp_num && 
                        p_data->_conf->_strategy->current()->max_reqhttp_num)
                {
                    //LOG_DEBUG("the _base_container size > %d", p_data->_conf->max_reqhttp_num);
                    return;
                }
                

                id_dict * id_dic = p_data->_id_dict->current();
                if (id_dic && _id_index < id_dic->_id_vec.size())
                {
                    std::string id = id_dic->_id_vec[_id_index];
                    LOG_DEBUG("%d %d %s", _id_index, id_dic->_id_vec.size(), id.c_str());
                    req_real_quotation(id);
                    req_real_single(id);
                    _id_index++;
                }
                else
                {
                    _is_first = false;
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
                t_msg->_time_length = 300000;
                t_msg->_obj_id = OBJ_ID_THREAD;
                add_timer(t_msg);
            }
        }

        void first_in_day()
        {
            proc_data* p_data = proc_data::instance();
            if (p_data && p_data->_conf)
            {
                std::unordered_set<std::string, str_hasher> t_block;
                p_data->_block_set.idle()->swap(t_block);
                p_data->_block_set.idle_2_current();
                _is_first = true;
            }

        }

        bool is_real_time()
        {
            char date[SIZE_LEN_64] = {'\0'};
            char time_str[SIZE_LEN_64] = {'\0'};

            get_timestr(time_str, sizeof(time_str), "%H:%M:%S");
            get_timestr(date, sizeof(date), "%Y%m%d");


            proc_data* p_data = proc_data::instance();

            if (strlen(time_str) && p_data && p_data->_conf)
            {
                if (strncmp(date, _req_date.c_str(), _req_date.size()) &&
                        strncmp(time_str, p_data->_conf->_strategy->current()->real_morning_stime.c_str(),
                            strlen("09:30")) > 0)
                {
                    _req_date.assign(date);
                    first_in_day();

                    return true;
                }

                if (strncmp(time_str, p_data->_conf->_strategy->current()->real_morning_stime.c_str(), 
                            strlen("09:30")) > 0 &&
                        strncmp(time_str, p_data->_conf->_strategy->current()->real_morning_etime.c_str(),
                            strlen("09:30")) < 0)
                    return true;

                if (strncmp(time_str, p_data->_conf->_strategy->current()->real_morning_etime.c_str(), 
                            strlen("09:30")) < 0 && 
                        strncmp(time_str, p_data->_conf->_strategy->current()->real_afternoon_etime.c_str(), 
                            strlen("09:30")) < 0)
                    return true;
            }

            return false;
        }

        void req_real_quotation(const std::string & id)
        {
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                {
                    auto it = p_data->_rquoation_dict->current()->_id_dict.find(id);
                    if (it == p_data->_rquoation_dict->current()->_id_dict.end())
                    {
                        LOG_DEBUG("id: %s is not in _rquoation_dict", id.c_str());
                        return;
                    }
                }

                {
                    auto it = p_data->_block_set.current()->find(id);
                    if (!_is_first && it != p_data->_block_set.current()->end())
                    {
                        LOG_DEBUG("id: %s is blocked", id.c_str());
                        return;
                    }
                }

                std::map<std::string, std::string> headers;
                ua_dict * ua_dic = p_data->_ua_dict->current();

                headers.insert(std::make_pair("User-Agent", ua_dic->_ua_vec[_id_index % ua_dic->_ua_vec.size()]));

                rquotation_data_process::gen_net_obj(id, get_net_container(), headers);
            }
        }

        void req_real_single(const std::string & id)
        {
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                {
                    auto it = p_data->_rsingle_dict->current()->_id_dict.find(id);
                    if (it == p_data->_rsingle_dict->current()->_id_dict.end())
                    {
                        LOG_DEBUG("id: %s is not in _rsingle_dict", id.c_str());
                        return;
                    }
                }

                {
                    auto it = p_data->_block_set.current()->find(id);
                    if (!_is_first && it != p_data->_block_set.current()->end())
                    {
                        LOG_DEBUG("id: %s is blocked", id.c_str());
                        return;
                    }
                }

                std::map<std::string, std::string> headers;
                ua_dict * ua_dic = p_data->_ua_dict->current();

                headers.insert(std::make_pair("User-Agent", ua_dic->_ua_vec[_id_index % ua_dic->_ua_vec.size()]));

                rsingle_data_process::gen_net_obj(id, get_net_container(), headers);
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
        std::string _req_date;
        uint32_t _id_index;
        bool _real_req;
        bool _is_first;
};





#endif
