#include "skhttp_req_thread.h"
#include "strategy_conf.h"
#include "common_util.h"

#include <dirent.h>


skhttp_req_thread::skhttp_req_thread()
{
    _quotation_index = 0;
    _single_index = 0;

    _req_quotation = true;
    _req_single = true;
}

void skhttp_req_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    return;
}

void skhttp_req_thread::real_req_start()
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

void skhttp_req_thread::single_reset()
{
    proc_data* p_data = proc_data::instance();
    {
        std::vector<std::multimap<int, std::string> > t_map;
        p_data->_rsingle_diff_index.idle()->swap(t_map);
    }

    {
        std::vector<std::multimap<int, std::string> > t_map;
        p_data->_rsingle_diff2_index.idle()->swap(t_map);
    }


    {
        std::unordered_map<std::string, std::deque<std::vector<single_t> >,str_hasher> tmp;
        p_data->_rsingle_dict_index.idle()->swap(tmp);
    }

}

void skhttp_req_thread::do_single()
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

void skhttp_req_thread::add_single_timer()
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

void skhttp_req_thread::quotation_reset()
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
        std::unordered_map<std::string, std::shared_ptr<quotation_t>,str_hasher> tmp;
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

void skhttp_req_thread::do_quotation()
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

void skhttp_req_thread::add_quotation_timer()
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


void skhttp_req_thread::first_in_day()
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

        real_morning_stime = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->real_morning_stime.c_str());
        real_morning_etime = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->real_morning_etime.c_str());

        real_afternoon_stime = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->real_afternoon_stime.c_str());
        real_afternoon_etime = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->real_afternoon_etime.c_str());

        dump_real_time = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->dump_real_time.c_str());
    }

}

bool skhttp_req_thread::is_trade_date(const char * date)
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

bool skhttp_req_thread::is_real_time()
{
    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");


    proc_data* p_data = proc_data::instance();

    if (p_data && p_data->_conf)
    {
        if (strncmp(date, _req_date.c_str(), strlen(date)))
        {
            time_t stime = get_real_time(date, "09:15");
            if (now >= stime)
            {
                _req_date.assign(date);
                get_trade_date();

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

void skhttp_req_thread::get_trade_date()
{
    std::string tmp_date;

    _trade_date = _req_date;
    int diff = 0;

    while (!is_trade_date(_trade_date.c_str()))
    {
        diff++;
        if (diff > 30)// 说明有bug
            break;

        date_change(_req_date, -1 * diff, _trade_date);
    }
}


void skhttp_req_thread::req_real_quotation(const std::string & id)
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

void skhttp_req_thread::req_real_single(const std::string & id)
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

bool skhttp_req_thread::need_dump_real_quotation()
{
    char t_buf[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return false;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), _trade_date.c_str());

    int d = access(t_buf, F_OK);
    if (!d)
        return false;

    if (_trade_date == _req_date){
        time_t now = time(NULL);
        if (now > dump_real_time)
            return true;
    }else {
        return true;
    }
    
    return false;
}

bool skhttp_req_thread::need_dump_real_single()
{
    char t_buf[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return false;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_single_path.c_str(), _trade_date.c_str());

    int d = access(t_buf, F_OK);
    if (!d)
        return false;

    if (_trade_date == _req_date){
        time_t now = time(NULL);
        if (now > dump_real_time)
            return true;
    }else {
        return true;
    }
    
    return false;
}

void skhttp_req_thread::dump_real_quotation()
{
    char t_buf[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), _trade_date.c_str());

    for (auto ii = p_data->_rquoation_real_dict.begin(); ii != p_data->_rquoation_real_dict.end(); ii++)
    {
        std::string tmp;

        tmp.append(ii->first);
        tmp.append(1, '\t');

        tmp.append(ii->second->start);
        tmp.append(1, '\t');

        tmp.append(ii->second->end);
        tmp.append(1, '\t');

        tmp.append(ii->second->high);
        tmp.append(1, '\t');

        tmp.append(ii->second->low);
        tmp.append(1, '\t');

        tmp.append(ii->second->last_closed);
        tmp.append(1, '\t');

        tmp.append(ii->second->vol);
        tmp.append(1, '\t');

        tmp.append(ii->second->buy_vol);
        tmp.append(1, '\t');

        tmp.append(ii->second->sell_vol);
        tmp.append(1, '\t');

        tmp.append(ii->second->swing);
        tmp.append(1, '\t');

        tmp.append(ii->second->change_rate);
        tmp.append(1, '\t');

        tmp.append(ii->second->range_percent);
        tmp.append(1, '\t');

        tmp.append(ii->second->total_price);
        tmp.append(1, '\t');

        tmp.append(ii->second->down_pointer);
        tmp.append(1, '\t');

        tmp.append(ii->second->up_pointer);
        tmp.append(1, '\t');

        tmp.append(ii->second->avg_price);
        tmp.append(1, '\t');

        FILE_WRITE(t_buf, "%s", tmp.c_str());
    }

    update_quotation_dict();
}

void skhttp_req_thread::update_quotation_dict()
{
    char t_buf[SIZE_LEN_512];
    char path_buf[SIZE_LEN_512];
    std::set<std::string> files;

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    struct dirent *r;
    DIR *p;
    uint32_t i = 0;

    p = opendir(strategy->real_quotation_path.c_str());
    if (p == NULL)
    {   
        LOG_WARNING("opendir:%s", strategy->real_quotation_path.c_str());

        return;
    } 

    while ((r= readdir(p)) != NULL)
    {   
        if (r->d_type == DT_REG && strstr(r->d_name, "20"))
        {
            snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), r->d_name);
            files.insert(t_buf);
        }
    } 
    closedir(p);


    snprintf(t_buf, sizeof(t_buf), "%s/.tmp_quotation", strategy->history_quotation_path.c_str());
    FILE * fp = fopen(t_buf, "a");
    if (!fp){ 
        return;
    }

    i = 0;
    for (auto ii = files.rbegin(); ii != files.rend() && i < strategy->history_quotation_num; ii++, i++)
    {
        fprintf(fp, "%s\n", ii->c_str());
    }

    fclose(fp);
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_quotation_path.c_str(), strategy->history_quotation_file.c_str());

    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }
}

void skhttp_req_thread::update_single_dict()
{
    char t_buf[SIZE_LEN_512];
    char path_buf[SIZE_LEN_512];
    std::set<std::string> files;

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    struct dirent *r;
    DIR *p;
    uint32_t i = 0;

    p = opendir(strategy->real_single_path.c_str());
    if (p == NULL)
    {   
        LOG_WARNING("opendir:%s", strategy->real_single_path.c_str());

        return;
    } 

    while ((r= readdir(p)) != NULL)
    {   
        if (r->d_type == DT_REG && strstr(r->d_name, "20"))
        {
            snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_single_path.c_str(), r->d_name);
            files.insert(t_buf);
        }
    } 
    closedir(p);


    snprintf(t_buf, sizeof(t_buf), "%s/.tmp_single", strategy->history_single_path.c_str());
    FILE * fp = fopen(t_buf, "a");
    if (!fp){ 
        return;
    }

    i = 0;
    for (auto ii = files.rbegin(); ii != files.rend() && i < strategy->history_single_num; ii++, i++)
    {
        fprintf(fp, "%s\n", ii->c_str());
    }

    fclose(fp);
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_single_path.c_str(), strategy->history_single_file.c_str());

    int ret = rename(t_buf, path_buf);
    if (ret < 0)
    {
        LOG_WARNING("rename t_buf:%s to path_buf:%s, err:%s", t_buf, path_buf, strerror(errno));
    }
}

void skhttp_req_thread::dump_real_single()
{
    char t_buf[SIZE_LEN_512];
    char tmp[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return ;

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_single_path.c_str(), _trade_date.c_str());

    for (auto ii = p_data->_rsingle_dict_index.current()->begin(); ii != p_data->_rsingle_dict_index.current()->end(); ii++)
    {
        std::string t_str;

        std::deque<std::vector<single_t> > & st = ii->second;
        if (!st.size())
            continue;

        t_str.append(ii->first.c_str());
        t_str.append(1, '\t');

        for (uint32_t i =0; i < st.back().size(); i++)
        {

            snprintf(tmp, sizeof(tmp), "%d\t%d\t%d", st.back().at(i).in, st.back().at(i).out, st.back().at(i).diff);

            t_str.append(tmp);
            t_str.append(1, '\t');
        }

        FILE_WRITE(t_buf, "%s", t_str.c_str());
    }

    update_single_dict();
}

void skhttp_req_thread::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
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

                if (need_dump_real_quotation())
                {
                    dump_real_quotation();
                }

            }
            break;
        case TIMER_TYPE_REQ_SINGLE:
            {
                _single_index = 0;
                if (is_real_time())
                    _req_single = true;
                else
                    add_single_timer();

                if (need_dump_real_single())
                {
                    dump_real_single();
                }

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
                p_data->_rsingle_diff2_index.idle_2_current();
                p_data->_rsingle_dict_index.idle_2_current();

                LOG_DEBUG("_rsingle_diff_index idle_2_current");

                add_single_timer();

            }
            break;
    }
}

void skhttp_req_thread::run_process()
{
    real_req_start();    
}

time_t skhttp_req_thread::get_real_time(const char * date, const char * time)
{
    struct tm tmp_time;
    memset(&tmp_time, 0, sizeof(tmp_time));
    char t_buf[SIZE_LEN_64];
    snprintf(t_buf, sizeof(t_buf), "%s %s", date, time);

    strptime(t_buf,"%Y%m%d %H:%M", &tmp_time);
    time_t t = mktime(&tmp_time);

    return t;
}

