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
#include "rsingle_data_process.h"
#include "holiday_dict.h"

class skhttp_req_thread:public base_net_thread
{
    public:

        skhttp_req_thread()
        {
            _quotation_index = 0;
            _single_index = 0;

            _req_quotation = true;
            _req_single = true;
        }

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg)
        {
            return;
        }

        void real_req_start()
        {
            proc_data* p_data = proc_data::instance();
            if (!p_data || !p_data->_id_dict)
                return;

            if (_base_container && _base_container->size() > 
                    p_data->_conf->_strategy->current()->max_reqhttp_num && 
                    p_data->_conf->_strategy->current()->max_reqhttp_num)
            {
                //LOG_DEBUG("the _base_container size > %d", p_data->_conf->max_reqhttp_num);
                return;
            }

            if (_req_quotation)
            {
                do_quotation();
            }

            if (_req_single)
            {
                do_single();
            }
        }

        void single_reset()
        {
            proc_data* p_data = proc_data::instance();
            {
                std::vector<std::multimap<int, std::string> > t_map;
                p_data->_rsingle_diff_index.idle()->swap(t_map);
            }

            {
                std::vector<std::map<std::string, std::multimap<int, std::string> > > tmp;
                p_data->_hsingle_sum_diff_index.idle()->swap(tmp);
            }


            {
                std::unordered_map<std::string, std::deque<std::vector<single_t> >,str_hasher> tmp;
                p_data->_rsingle_dict_index.idle()->swap(tmp);
            }

        }
    
        void do_single()
        {
            proc_data* p_data = proc_data::instance();

            if (!p_data || !p_data->_conf || !p_data->_id_dict)
            {   
                return;
            }

            id_dict * id_dic = p_data->_id_dict->current();
            if (id_dic && _single_index < id_dic->_id_vec.size())
            {
                if (!_single_index)
                {
                    single_reset();
                }

                std::string id = id_dic->_id_vec[_single_index];
                LOG_DEBUG("single_index:%d id_vec.size:%d id:%s", _single_index, id_dic->_id_vec.size(), id.c_str());
                req_real_single(id);
                _single_index++;

                if (_single_index >= id_dic->_id_vec.size())
                {
                    std::shared_ptr<timer_msg> t_msg(new timer_msg);

                    t_msg->_timer_type = TIMER_TYPE_SINGLE_IDLE_2_CURRENT;
                    t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
                    t_msg->_obj_id = OBJ_ID_THREAD;
                    add_timer(t_msg);
                }
            }
        }

        void add_single_timer()
        {
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                _req_single = false;
                std::shared_ptr<timer_msg> t_msg(new timer_msg);

                t_msg->_timer_type = TIMER_TYPE_REQ_SINGLE;
                t_msg->_time_length = p_data->_conf->_strategy->current()->req_single_millisecond;
                t_msg->_obj_id = OBJ_ID_THREAD;
                add_timer(t_msg);
            }
        }

        void quotation_reset()
        {
            proc_data* p_data = proc_data::instance();
            {
                std::multimap<float, std::string> t_map;
                p_data->_end_index.idle()->swap(t_map);
            }

            {
                std::multimap<float, std::string> t_map;
                p_data->_change_rate_index.idle()->swap(t_map);
            }

            {
                std::multimap<float, std::string> t_map;
                p_data->_range_percent_index.idle()->swap(t_map);
            }

            {
                std::multimap<float, std::string> t_map;
                p_data->_down_pointer_index.idle()->swap(t_map);
            }

            {
                std::multimap<float, std::string> t_map;
                p_data->_up_pointer_index.idle()->swap(t_map);
            }

            {
                std::multimap<float, std::string> t_map;
                p_data->_end_avg_price_index.idle()->swap(t_map);
            }

            {
                std::unordered_map<std::string, quotation_t,str_hasher> tmp;
                p_data->_rquoation_dict_index.idle()->swap(tmp);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > t_map;
                p_data->_hq_sum_range_percent_index.idle()->swap(t_map);
            }

            {
                std::map<std::string, std::multimap<float, std::string> > t_map;
                p_data->_hq_sum_change_rate_index.idle()->swap(t_map);
            }
        }

        void do_quotation()
        {   
            proc_data* p_data = proc_data::instance();

            if (!p_data || !p_data->_conf || !p_data->_id_dict)
            {   
                return;
            }

            id_dict * id_dic = p_data->_id_dict->current();
            if (id_dic && _quotation_index < id_dic->_id_vec.size())
            {
                if (!_quotation_index)
                {
                    quotation_reset();
                }

                std::string id = id_dic->_id_vec[_quotation_index];
                LOG_DEBUG("quotation_index: %d id_vec.size: %d id:%s", _quotation_index, id_dic->_id_vec.size(), id.c_str());
                req_real_quotation(id);
                _quotation_index++;

                if (_quotation_index >= id_dic->_id_vec.size())
                {
                    std::shared_ptr<timer_msg> t_msg(new timer_msg);

                    t_msg->_timer_type = TIMER_TYPE_QUOTATION_IDLE_2_CURRENT;
                    t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
                    t_msg->_obj_id = OBJ_ID_THREAD;
                    add_timer(t_msg);
                }
            }
        }

        void add_quotation_timer()
        {
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                _req_quotation = false;
                std::shared_ptr<timer_msg> t_msg(new timer_msg);

                t_msg->_timer_type = TIMER_TYPE_REQ_QUOTATION;
                t_msg->_time_length = p_data->_conf->_strategy->current()->req_quotation_millisecond;
                t_msg->_obj_id = OBJ_ID_THREAD;
                add_timer(t_msg);
            }
        }


        void first_in_day()
        {
            proc_data* p_data = proc_data::instance();
            if (p_data && p_data->_conf)
            {
                //{
                    //std::unordered_map<std::string, std::vector<single_t>,str_hasher> t_dict;
                    //p_data->_rsingle_real_dict.swap(t_dict);
                //}

                {
                    std::unordered_set<std::string, str_hasher> t_block;
                    p_data->_block_set.idle()->swap(t_block);
                    p_data->_block_set.idle_2_current();
                }

                real_morning_stime = get_real_time(p_data->_conf->_strategy->current()->real_morning_stime.c_str());
                real_morning_etime = get_real_time(p_data->_conf->_strategy->current()->real_morning_etime.c_str());

                real_afternoon_stime = get_real_time(p_data->_conf->_strategy->current()->real_afternoon_stime.c_str());
                real_afternoon_etime = get_real_time(p_data->_conf->_strategy->current()->real_afternoon_etime.c_str());
            }

        }

        bool is_trade_date(const char * date)
        {
            if (!date)
                return false;

            int year, mon, day, weekday;
            int len = strlen(date);
            if (len < 8)
                return false;

            {
                std::string str(date, 0, 4);
                year = atoi(str.c_str());
            }

            {
                std::string str(date, 4, 2);
                mon = atoi(str.c_str());
            }
            
            
            {
                std::string str(date, 6, 2);
                day = atoi(str.c_str());
            }
            
            weekday = dayofweek(day, mon, year);
            if (weekday > 5 || weekday < 1)
                return false;

            proc_data* p_data = proc_data::instance();
            if (p_data) 
            {
                auto ii = p_data->_holiday_dict->current()->_date_dict.find(std::string(date));
                if (ii != p_data->_holiday_dict->current()->_date_dict.end())
                    return false;
            }
            return true;
        }

        bool is_real_time()
        {
            char date[SIZE_LEN_64] = {'\0'};
            time_t now = get_timestr(date, sizeof(date), "%Y%m%d");


            proc_data* p_data = proc_data::instance();

            if (p_data && p_data->_conf)
            {
                if (strncmp(date, _req_date.c_str(), strlen(date)))
                {
                    time_t stime = get_real_time("09:15");
                    if (now >= stime)
                    {
                        _req_date.assign(date);
                        first_in_day();

                        return true;
                    }
                }

                if (!is_trade_date(date))
                    return false;

                if (now >= real_morning_stime && now <= real_morning_etime)
                    return true;

                if (now >= real_afternoon_stime && now <= real_afternoon_etime)
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
                    auto it = p_data->_block_set.current()->find(id);
                    if (it != p_data->_block_set.current()->end())
                    {
                        LOG_DEBUG("id: %s is blocked", id.c_str());
                        return;
                    }
                }

                std::map<std::string, std::string> headers;
                ua_dict * ua_dic = p_data->_ua_dict->current();

                headers.insert(std::make_pair("User-Agent", 
                            ua_dic->_ua_vec[_quotation_index % ua_dic->_ua_vec.size()]));

                rquotation_data_process::gen_net_obj(id, get_net_container(), headers);
            }
        }

        void req_real_single(const std::string & id)
        {
            proc_data* p_data = proc_data::instance();
            if (p_data)
            {
                {
                    auto it = p_data->_block_set.current()->find(id);
                    if (it != p_data->_block_set.current()->end())
                    {
                        LOG_DEBUG("id: %s is blocked", id.c_str());
                        return;
                    }
                }

                std::map<std::string, std::string> headers;
                ua_dict * ua_dic = p_data->_ua_dict->current();

                headers.insert(std::make_pair("User-Agent", 
                            ua_dic->_ua_vec[_single_index % ua_dic->_ua_vec.size()]));

                rsingle_data_process::gen_net_obj(id, get_net_container(), headers);
            }
        }

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg)
        {
            LOG_DEBUG("handle_timeout: timer_id:%u timer_type:%u", t_msg->_timer_id, t_msg->_timer_type);
            proc_data* p_data = proc_data::instance();
            switch (t_msg->_timer_type)
            {
                case TIMER_TYPE_REQ_QUOTATION:
                    {
                        _quotation_index = 0;
                        if (is_real_time())
                            _req_quotation = true;
                        else
                            add_quotation_timer();
                    }
                    break;
                case TIMER_TYPE_REQ_SINGLE:
                    {
                        _single_index = 0;
                        if (is_real_time())
                            _req_single = true;
                        else
                            add_single_timer();
                    }
                    break;
                case TIMER_TYPE_QUOTATION_IDLE_2_CURRENT:
                    {
                        LOG_DEBUG("_block_set idle_2_current");
                        rquotation_data_process::update_all_index();

                        p_data->_block_set.idle_2_current();
                        p_data->_end_index.idle_2_current();
                        p_data->_change_rate_index.idle_2_current();
                        p_data->_range_percent_index.idle_2_current();
                        p_data->_down_pointer_index.idle_2_current();
                        p_data->_up_pointer_index.idle_2_current();
                        p_data->_end_avg_price_index.idle_2_current();

                        p_data->_hq_sum_range_percent_index.idle_2_current();
                        p_data->_hq_sum_change_rate_index.idle_2_current();
                        p_data->_rquoation_dict_index.idle_2_current();

                        add_quotation_timer();
                    }
                    break;
                case TIMER_TYPE_SINGLE_IDLE_2_CURRENT:
                    {
                        //先更新再切换
                        rsingle_data_process::update_all_index();

                        p_data->_rsingle_diff_index.idle_2_current();
                        p_data->_hsingle_sum_diff_index.idle_2_current();
                        p_data->_rsingle_dict_index.idle_2_current();

                        LOG_DEBUG("_rsingle_diff_index idle_2_current");

                        add_single_timer();
                    }
                    break;
            }
        }

        virtual void run_process()
        {
            real_req_start();    
        }

    protected:
        time_t get_real_time(const char * time)
        {
            struct tm tmp_time;
            memset(&tmp_time, 0, sizeof(tmp_time));
            char t_buf[SIZE_LEN_64];
            snprintf(t_buf, sizeof(t_buf), "%s %s", _req_date.c_str(), time);

            strptime(t_buf,"%Y%m%d %H:%M", &tmp_time);
            time_t t = mktime(&tmp_time);

            return t;
        }

    protected:
        std::string _req_date;
        uint32_t _quotation_index;
        bool _req_quotation;

        uint32_t _single_index;
        bool _req_single;

        time_t real_morning_stime;
        time_t real_morning_etime;
        
        time_t real_afternoon_stime;
        time_t real_afternoon_etime;

};





#endif
