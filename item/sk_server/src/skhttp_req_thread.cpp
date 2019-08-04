#include "skhttp_req_thread.h"
#include "strategy_conf.h"
#include "common_util.h"
#include "sk_util.h"

#include <dirent.h>
#include "proc_data.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"


skhttp_req_thread::skhttp_req_thread()
{
    _quotation_index = 0;
    _single_index = 0;

    _quotation_destroy_num = 0;
    _single_destroy_num = 0;

    _req_quotation = true;
    _req_single = true;

    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");

    _req_date.assign(date);
    first_in_day();

    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    _history_quotation_num = strategy->history_quotation_num;
    _history_single_num = strategy->history_single_num;


    _id_dic = p_data->_id_dict->current();
    _ua_dic = p_data->_ua_dict->current();
}

void skhttp_req_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    if (p_msg == nullptr)
        return;

    proc_data* p_data = proc_data::instance();


    switch(p_msg->_msg_op)
    {
        case NORMAL_MSG_DESTROY_QT:
            {
                _quotation_destroy_num++;
                LOG_DEBUG("_quotation_destroy_num:%d, size:%d", _quotation_destroy_num, _id_dic->_id_vec.size());
                if (_quotation_destroy_num >= _id_dic->_id_vec.size())
                {
                    p_data->_hquoation_dict->update_search_index();
                    add_quotation_timer();
                }
            }

            break;
        case NORMAL_MSG_DESTROY_ST:
            {
                _single_destroy_num++;
                LOG_DEBUG("_single_destroy_num:%d, size:%d", _single_destroy_num, _id_dic->_id_vec.size());
                if (_single_destroy_num >= _id_dic->_id_vec.size())
                {
                    p_data->_hsingle_dict->update_search_index();
                    add_single_timer();
                }

            }
            break;
    }

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

void skhttp_req_thread::do_single()
{
    proc_data* p_data = proc_data::instance();

    if (!p_data || !p_data->_conf || !p_data->_id_dict)
    {   
        return;
    }

    if (_id_dic && _single_index < _id_dic->_id_vec.size())
    {
        std::string id = _id_dic->_id_vec[_single_index];
        LOG_DEBUG("single_index:%d id_vec.size:%d id:%s", _single_index, _id_dic->_id_vec.size(), id.c_str());
        req_real_single(id);
        _single_index++;

        if (_single_index >= _id_dic->_id_vec.size())
        {
            _req_single = false;
            //std::shared_ptr<timer_msg> t_msg(new timer_msg);

            //t_msg->_timer_type = TIMER_TYPE_SINGLE_IDLE_2_CURRENT;
            //t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
            //t_msg->_obj_id = OBJ_ID_THREAD;
            //add_timer(t_msg);
        }
    }
}

void skhttp_req_thread::add_single_timer()
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        std::shared_ptr<timer_msg> t_msg(new timer_msg);

        t_msg->_timer_type = TIMER_TYPE_REQ_SINGLE;
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_single_millisecond;
        t_msg->_obj_id = OBJ_ID_THREAD;
        add_timer(t_msg);
    }
}


void skhttp_req_thread::do_quotation()
{   
    proc_data* p_data = proc_data::instance();

    if (!p_data || !p_data->_conf || !p_data->_id_dict)
    {   
        return;
    }

    if (_id_dic && _quotation_index < _id_dic->_id_vec.size())
    {
        std::string id = _id_dic->_id_vec[_quotation_index];
        LOG_DEBUG("quotation_index: %d id_vec.size: %d id:%s", _quotation_index, _id_dic->_id_vec.size(), id.c_str());
        req_real_quotation(id);
        _quotation_index++;

        if (_quotation_index >= _id_dic->_id_vec.size())
        {
            _req_quotation = false;
            //std::shared_ptr<timer_msg> t_msg(new timer_msg);

            //t_msg->_timer_type = TIMER_TYPE_QUOTATION_IDLE_2_CURRENT;
            //t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
            //t_msg->_obj_id = OBJ_ID_THREAD;
            //add_timer(t_msg);
        }
    }
}

void skhttp_req_thread::add_quotation_timer()
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
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

        holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
        _holiday_dict->get_trade_date(_req_date, _trade_date);

        p_data->_trade_date = _trade_date;

        {
            std::unordered_set<std::string, str_hasher> t_block;
            p_data->_block_set->idle()->swap(t_block);
            p_data->_block_set->idle_2_current();
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

bool skhttp_req_thread::is_real_time()
{
    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");

    proc_data* p_data = proc_data::instance();
    if (p_data && p_data->_conf)
    {

        holiday_dict * _holiday_dict = p_data->_holiday_dict->current();

        if (strncmp(date, _req_date.c_str(), strlen(date)))
        {
            time_t stime = get_real_time(date, "09:15");
            if (now >= stime)
            {
                _req_date.assign(date);
                first_in_day();
            }
        }

        if (!_holiday_dict->is_trade_date(date))
            return false;

        if (now >= real_morning_stime && now <= real_morning_etime)
            return true;

        if (now >= real_afternoon_stime && now <= real_afternoon_etime)
            return true;
    }

    return false;
}


void skhttp_req_thread::req_real_quotation(const std::string & id)
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        if (p_data->_block_set->do_check_block(id))
        {
            std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
            net_obj->id = id;
            net_obj->_msg_op = NORMAL_MSG_DESTROY_QT;

            common_obj_container * net_container = get_net_container();
            ObjId oid; 
            oid._id = OBJ_ID_THREAD;
            oid._thread_index = net_container->get_thread_index();
            std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);
            base_net_thread::put_obj_msg(oid, ng);


            LOG_DEBUG("id: %s is blocked", id.c_str());
            return;
        }

        std::map<std::string, std::string> headers;

        headers.insert(std::make_pair("User-Agent", 
                    _ua_dic->_ua_vec[_quotation_index % _ua_dic->_ua_vec.size()]));

        rquotation_data_process::gen_net_obj(id, get_net_container(), headers);
    }
}

void skhttp_req_thread::req_real_single(const std::string & id)
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        if (p_data->_block_set->do_check_block(id))
        {

            std::shared_ptr<destroy_msg>  net_obj(new destroy_msg);
            net_obj->id = id;
            net_obj->_msg_op = NORMAL_MSG_DESTROY_ST;

            common_obj_container * net_container = get_net_container();
            ObjId oid; 
            oid._id = OBJ_ID_THREAD;
            oid._thread_index = net_container->get_thread_index();
            std::shared_ptr<normal_msg> ng = std::static_pointer_cast<normal_msg>(net_obj);
            base_net_thread::put_obj_msg(oid, ng);

            LOG_DEBUG("id: %s is blocked", id.c_str());
            return;
        }

        std::map<std::string, std::string> headers;

        headers.insert(std::make_pair("User-Agent", 
                    _ua_dic->_ua_vec[_single_index % _ua_dic->_ua_vec.size()]));

        rsingle_data_process::gen_net_obj(id, get_net_container(), headers);
    }
}

bool skhttp_req_thread::need_dump_real_quotation()
{
    char t_buf[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return false;

    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    if (!_holiday_dict->is_trade_date(_trade_date.c_str()))
        return false;
    
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), _trade_date.c_str());

    int d = access(t_buf, F_OK);
    if (!d)
        return false;

    if (_holiday_dict->is_trade_date(_req_date.c_str())){
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

    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    if (!_holiday_dict->is_trade_date(_trade_date.c_str()))
        return false;
    
    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_single_path.c_str(), _trade_date.c_str());

    int d = access(t_buf, F_OK);
    if (!d)
        return false;

    if (_holiday_dict->is_trade_date(_req_date.c_str())){
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

    auto & qt = p_data->_rquotation_index->current()->id_quotation;
    for (auto ii = qt.begin(); ii != qt.end(); ii++)
    {
        std::string tmp;

        if (p_data->_block_set->do_check_block(ii->first))
        {
            continue;
        }

        tmp.append(ii->first);
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->start));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->end));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->high));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->low));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->last_closed));
        tmp.append(1, '\t');

        tmp.append(int_2_str(ii->second.back()->vol));
        tmp.append(1, '\t');

        tmp.append(int_2_str(ii->second.back()->buy_vol));
        tmp.append(1, '\t');

        tmp.append(int_2_str(ii->second.back()->sell_vol));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->swing));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->change_rate));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->range_percent));
        tmp.append(1, '\t');

        tmp.append(float_2_str(ii->second.back()->total_price));
        tmp.append(1, '\t');

        FILE_WRITE(t_buf, "%s", tmp.c_str());
    }
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

    auto rsingle_dict_index = p_data->_rsingle_index->current();
    for (auto ii = rsingle_dict_index->id_single.begin(); ii != rsingle_dict_index->id_single.end(); ii++)
    {
        std::string t_str;

        std::deque<std::shared_ptr<single_vec> > & st = ii->second;
        if (!st.size())
            continue;

        if (p_data->_block_set->do_check_block(ii->first))
        {
            continue;
        }

        t_str.append(ii->first.c_str());
        t_str.append(1, '\t');

        for (uint32_t i =0; i < st.back()->size(); i++)
        {

            snprintf(tmp, sizeof(tmp), "%d\t%d\t%d", st.back()->at(i).in, st.back()->at(i).out, st.back()->at(i).diff);

            t_str.append(tmp);
            t_str.append(1, '\t');
        }

        FILE_WRITE(t_buf, "%s", t_str.c_str());
    }
}

void skhttp_req_thread::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    LOG_DEBUG("handle_timeout: timer_id:%u timer_type:%u", t_msg->_timer_id, t_msg->_timer_type);
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    switch (t_msg->_timer_type)
    {
        case TIMER_TYPE_REQ_QUOTATION:
            {
                bool flag = false;
                _quotation_index = 0;
                _quotation_destroy_num = 0;
                
                _id_dic = p_data->_id_dict->current();
                _ua_dic = p_data->_ua_dict->current();

                if (is_real_time())
                    _req_quotation = true;
                else
                    add_quotation_timer();

                if (need_dump_real_quotation())
                {
                    dump_real_quotation();
                    flag = true;
                }

                if (flag || _history_quotation_num != strategy->history_quotation_num) 
                {
                    update_quotation_dict(); 
                    _history_quotation_num = strategy->history_quotation_num;
                }

            }
            break;
        case TIMER_TYPE_REQ_SINGLE:
            {
                bool flag = false;
                _single_index = 0;
                _single_destroy_num = 0;
                if (is_real_time())
                    _req_single = true;
                else
                    add_single_timer();

                if (need_dump_real_single())
                {
                    dump_real_single();
                    flag = true;
                }

                if (flag || _history_single_num != strategy->history_single_num) 
                {
                    update_single_dict();
                    _history_single_num = strategy->history_single_num;
                }

            }
            break;
        case TIMER_TYPE_QUOTATION_IDLE_2_CURRENT:
            {
                LOG_DEBUG("QUOTATION_IDLE_2_CURRENT");
                p_data->_hquoation_dict->update_search_index();

                add_quotation_timer();
            }
            break;
        case TIMER_TYPE_SINGLE_IDLE_2_CURRENT:
            {
                //先更新再切换
                LOG_DEBUG("SINGLE_IDLE_2_CURRENT");
                p_data->_hsingle_dict->update_search_index();

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

