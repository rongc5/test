#include "skhttp_req_thread.h"
#include "strategy_conf.h"
#include "common_util.h"
#include "sk_util.h"

#include <dirent.h>
#include "proc_data.h"
#include "history_single_dict.h"
#include "history_quotation_dict.h"
#include "lruSsr_search_index.h"
//#include "history_wsingle_dict.h"
//#include "history_wquotation_dict.h"
#include "worker_thread.h"
#include "history_main_funds_dict.h"


skhttp_req_thread::skhttp_req_thread()
{
    _quotation_index = 0;
    _single_index = 0;
    _main_funds_index = 0;
    _req_quotation_circle_times = 0;
    _req_single_circle_times = 0;
    _req_main_funds_circle_times = 0;

    _quotation_destroy_num = 0;
    _single_destroy_num = 0;
    _main_funds_destroy_num = 0;

    _req_quotation = true;
    _req_single = true;
    _req_main_funds = true;

    char date[SIZE_LEN_64] = {'\0'};
    time_t now = get_timestr(date, sizeof(date), "%Y%m%d");

    _req_date.assign(date);
    first_in_day();

    proc_data* p_data = proc_data::instance();

    _id_dic = p_data->_id_dict->current();
    _ua_dic = p_data->_ua_dict->current();

    _is_first =  false;
}

void skhttp_req_thread::handle_msg(shared_ptr<normal_msg> & p_msg)
{
    if (p_msg == nullptr)
        return;

    proc_data* p_data = proc_data::instance();


    switch(p_msg->_msg_op)
    {
        case NORMAL_MSG_DESTROY_QT:
            {
                auto p=dynamic_pointer_cast<destroy_msg>(p_msg);
                if (p)
                {
                    _quotation_destroy_num += p->id_vec->size();
                }

                LOG_DEBUG("_quotation_destroy_num:%d, size:%d", _quotation_destroy_num, _id_dic->_id_vec.size());
                if (_quotation_destroy_num >= _id_dic->_id_vec.size())
                {
                    _req_quotation_circle_times++;
                    if (_req_quotation_circle_times == p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
                    {
                        p_data->_block_set->destroy_idle();
                        p_data->_block_set->idle_2_current();
                        add_destroy_idle_timer();
                    }

                    LOG_DEBUG("QUOTATION_IDLE_2_CURRENT");
                    p_data->_hquoation_dict->update_search_index();
                    //p_data->_hwquoation_dict->update_search_index();

                    add_quotation_timer();
                }
            }

            break;
        case NORMAL_MSG_DESTROY_ST:
            {

                auto p=dynamic_pointer_cast<destroy_msg>(p_msg);
                if (p)
                {
                    _single_destroy_num += p->id_vec->size();
                }

                LOG_DEBUG("_single_destroy_num:%d, size:%d", _single_destroy_num, _id_dic->_id_vec.size());
                if (_single_destroy_num >= _id_dic->_id_vec.size())
                {
                    _req_single_circle_times++;
                    LOG_DEBUG("SINGLE_IDLE_2_CURRENT");
                    p_data->_hsingle_dict->update_search_index();
                    //p_data->_hwsingle_dict->update_search_index();


                    add_single_timer();
                }

            }
            break;
        case NORMAL_MSG_DESTROY_MF:
            {
                auto p=dynamic_pointer_cast<destroy_msg>(p_msg);
                if (p)
                {
                    _main_funds_destroy_num += p->id_vec->size();
                }

                LOG_DEBUG("_main_funds_destroy_num:%d, size:%d", _main_funds_destroy_num, _id_dic->_id_vec.size());
                if (_main_funds_destroy_num >= _id_dic->_id_vec.size())
                {
                    _req_main_funds_circle_times++;
                    LOG_DEBUG("main_funds_IDLE_2_CURRENT");
                    p_data->_hmain_funds_dict->update_search_index();
                    //p_data->_hwmain_funds_dict->update_search_index();

                    add_main_funds_timer();
                }
            }
            break;

        case NORMAL_MSG_UPDATE_TRADE_DATE:
            {
                LOG_DEBUG("update_trade date");
                first_in_day();    

                p_data->_hquoation_dict->update();
                p_data->_hsingle_dict->update();
                p_data->_hmain_funds_dict->update();

                //p_data->_hwquoation_dict->update();
                //p_data->_hwsingle_dict->update();

            }
            break;
        case NORMAL_MSG_REG_FOR_DATE_INFO:
            {
                LOG_DEBUG("recive msg NORMAL_MSG_UPDATE_DATE_INFO");
                
                shared_ptr<date_msg> p=dynamic_pointer_cast<date_msg>(p_msg);
                if (p && !_req_date.empty())
                {
                    //_reg_date_threadid_map[p->_thread_index] = 0;
                }
                
            }
            break;
        case NORMAL_MSG_RESPONSE_SINGLE:
            {    
                auto p=dynamic_pointer_cast<response_req_msg>(p_msg);
                if (p && p->_st)
                {    
                    LOG_DEBUG("NORMAL_MSG_RESPONSE_SINGLE:%s, size:%d", p->_id.c_str(),  p_data->_id_dict->current()->size());
                    p_data->_hsingle_dict->update_real_single(*p_data->get_trade_date(), p->_id, p->_st);
                    //p_data->_hwsingle_dict->update_real_wsingle(*p_data->get_trade_date(), p->_id, p->_st);
                }    
            }    
            break;
        case NORMAL_MSG_RESPONSE_QUOTATION:
            {
                auto p=dynamic_pointer_cast<response_req_msg>(p_msg);
                if (p && p->_qt)
                {
                    if (p->_qt->start <= 1)
                    {
                        p_data->_block_set->idle()->insert(p->_id);
                    }
                    else
                    {
                        LOG_DEBUG("NORMAL_MSG_RESPONSE_QUOTATION:%s", p->_id.c_str(), p_data->_id_dict->current()->size());
                        p_data->_hquoation_dict->update_real_quotation(*p_data->get_trade_date(), p->_id, p->_qt);
                        //p_data->_hwquoation_dict->update_real_wquotation(*p_data->get_trade_date(), p->_id, p->_qt);
                    }
                }
            }
            break;  
        case NORMAL_MSG_RESPONSE_MAIN_FUNDS:
            {
                auto p=dynamic_pointer_cast<response_req_msg>(p_msg);
                if (p && p->_mft)
                {
                    LOG_DEBUG("NORMAL_MSG_RESPONSE_MAIN_FUNDS:%s", p->_id.c_str(), p_data->_id_dict->current()->size());
                    p_data->_hmain_funds_dict->update_real_main_funds(*p_data->get_trade_date(), p->_id, p->_mft);
                }

            }
            break;
    }

    return;
}

#if 0
void skhttp_req_thread::notify_update_date()
{
    for (auto it = _reg_date_threadid_map.begin(); it != _reg_date_threadid_map.end(); it++)
    {
        if (!it->second)
        {
            shared_ptr<date_msg>  net_obj(new date_msg);
            net_obj->_msg_op = NORMAL_MSG_UPDATE_DATE_INFO;

            ObjId id; 
            id._id = OBJ_ID_THREAD;
            id._thread_index = it->first;

            p->req_date = _req_date;
            p->trade_date = _trade_date;

            base_net_thread::put_obj_msg(id, net_obj);
            it->second += 1;
        }
    }
}

void skhttp_req_thread::reset_threadid_map_flag()
{
    for (auto it = _reg_date_threadid_map.begin(); it != _reg_date_threadid_map.end(); it++)
    {
        it->second = 0;
    }
}
#endif

void skhttp_req_thread::real_req_start()
{
    if (!_is_first)
    {
        _is_first = true;
        reload_timer_start();
    } 

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

    //usleep(200000);

    if (_req_quotation)
    {
        do_quotation();
    }


    if (_req_single)
    {
        do_single();
    }

    if (_req_main_funds)
    {
        do_main_funds();
    }
}

void skhttp_req_thread::do_single()
{
    proc_data* p_data = proc_data::instance();

    if (!p_data || !p_data->_conf || !p_data->_id_dict)
    {   
        return;
    }

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    if (strategy->req_single_num_permillisecond)
    {
        time_t now = time(NULL);
        if (!_single_index)
            _single_circle_start_time = now; 

        if (llabs(_single_circle_start_time - now) && 
                _single_index/llabs(_single_circle_start_time - now) > strategy->req_single_num_permillisecond)
        {    
            return;
        }
    }

    if (_id_dic && _single_index < _id_dic->_id_vec.size())
    {
        string id = _id_dic->_id_vec[_single_index];
        LOG_DEBUG("single_index:%d id_vec.size:%d id:%s", _single_index, _id_dic->_id_vec.size(), id.c_str());
        req_real_single(id);
        _single_index++;

        if (_single_index >= _id_dic->_id_vec.size())
        {
            _req_single = false;
            //shared_ptr<timer_msg> t_msg(new timer_msg);

            //t_msg->_timer_type = TIMER_TYPE_SINGLE_IDLE_2_CURRENT;
            //t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
            //t_msg->_obj_id = OBJ_ID_THREAD;
            //add_timer(t_msg);
        }
    }
}

void skhttp_req_thread::do_main_funds()
{
    proc_data* p_data = proc_data::instance();

    if (!p_data || !p_data->_conf || !p_data->_id_dict)
    {   
        return;
    }

    strategy_conf * strategy = p_data->_conf->_strategy->current();

    if (strategy->req_main_funds_num_permillisecond)
    {
        time_t now = time(NULL);
        if (!_main_funds_index)
            _main_funds_circle_start_time = now; 

        if (llabs(_main_funds_circle_start_time - now) && 
                _main_funds_index/llabs(_main_funds_circle_start_time - now) > strategy->req_main_funds_num_permillisecond)
        {    
            return;
        }
    }

    if (_id_dic && _main_funds_index < _id_dic->_id_vec.size())
    {
        string id = _id_dic->_id_vec[_main_funds_index];
        LOG_DEBUG("main_funds_index:%d id_vec.size:%d id:%s", _main_funds_index, _id_dic->_id_vec.size(), id.c_str());
        req_real_main_funds(id);
        _main_funds_index++;

        if (_main_funds_index >= _id_dic->_id_vec.size())
        {
            _req_main_funds = false;
            //shared_ptr<timer_msg> t_msg(new timer_msg);

            //t_msg->_timer_type = TIMER_TYPE_main_funds_IDLE_2_CURRENT;
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
        shared_ptr<timer_msg> t_msg(new timer_msg);

        t_msg->_timer_type = TIMER_TYPE_REQ_SINGLE;
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_single_millisecond;
        t_msg->_obj_id = OBJ_ID_THREAD;
        add_timer(t_msg);
    }
}

void skhttp_req_thread::add_main_funds_timer()
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        shared_ptr<timer_msg> t_msg(new timer_msg);

        t_msg->_timer_type = TIMER_TYPE_REQ_MAIN_FUNDS;
        t_msg->_time_length = p_data->_conf->_strategy->current()->req_main_funds_millisecond;
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
    strategy_conf * strategy = p_data->_conf->_strategy->current();

    if (strategy->req_quotation_num_permillisecond)
    {
        time_t now = time(NULL);
        if (!_quotation_index)
            _quotation_circle_start_time = now;

        if (llabs(_quotation_circle_start_time - now) && 
                _quotation_index/llabs(_quotation_circle_start_time - now) > strategy->req_quotation_num_permillisecond)
        {
            return;
        }
    }

#if 0

    if (_id_dic && _quotation_index < _id_dic->_id_vec.size())
    {
        string id = _id_dic->_id_vec[_quotation_index];
        LOG_DEBUG("quotation_index: %d id_vec.size: %d id:%s", _quotation_index, _id_dic->_id_vec.size(), id.c_str());
        req_real_quotation(id);
        _quotation_index++;

        if (_quotation_index >= _id_dic->_id_vec.size())
        {
            _req_quotation = false;
            //shared_ptr<timer_msg> t_msg(new timer_msg);

            //t_msg->_timer_type = TIMER_TYPE_QUOTATION_IDLE_2_CURRENT;
            //t_msg->_time_length = p_data->_conf->_strategy->current()->req_http_timeout + 20;
            //t_msg->_obj_id = OBJ_ID_THREAD;
            //add_timer(t_msg);
        }
    }
#endif

    auto id_set = make_shared<set<string>>();
    int quotation_start = _quotation_index;
    while (_id_dic && _quotation_index < _id_dic->_id_vec.size()) 
    {
        string id = _id_dic->_id_vec[_quotation_index];
        LOG_DEBUG("quotation_index: %d id_vec.size: %d id:%s", _quotation_index, _id_dic->_id_vec.size(), id.c_str());
        id_set->insert(id);
        _quotation_index++;

        if (_quotation_index - quotation_start >= strategy->req_quotation_num_perreq)
            break;
    }

    if (!id_set->empty())
    {
        req_real_quotation(id_set);
    }

    if (_quotation_index >= _id_dic->_id_vec.size()) 
    {
        _req_quotation = false;
    }

}

void skhttp_req_thread::add_quotation_timer()
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        shared_ptr<timer_msg> t_msg(new timer_msg);

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

        {
            _req_quotation_circle_times = 0;
            _req_single_circle_times = 0;
            _req_main_funds_circle_times = 0;
            update_block_set_search_index();

            update_lrussr_search_index();

            update_week_tdate_search_index();

            add_destroy_idle_timer();

            is_backuped = true;


            strategy_conf * strategy = p_data->_conf->_strategy->current();

            _history_quotation_num = strategy->history_quotation_num;
            _history_technical_num = strategy->history_technical_num;
            _history_single_num = strategy->history_single_num;
            _history_main_funds_num = strategy->history_main_funds_num;

            _history_wquotation_num = strategy->history_wquotation_num;
            _history_wsingle_num = strategy->history_wsingle_num;

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

        backup_stime = get_real_time(_req_date.c_str(), 
                p_data->_conf->_strategy->current()->backup_stime.c_str());

        //reset_threadid_map_flag();
        
        p_data->update_req_date(_req_date);
        p_data->update_trade_date(_trade_date);
    }

}

bool skhttp_req_thread::is_real_time(uint32_t req_circle_times)
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

        if (now >= (real_morning_stime + 1200) && req_circle_times < p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
            return true;

        if (!_holiday_dict->is_trade_date(date))
            return false;

        if (now >= real_morning_stime && now <= real_morning_etime)
            return true;

        if (now >= real_afternoon_stime && now <= real_afternoon_etime)
            return true;
    }

    return false;
}


void skhttp_req_thread::req_real_quotation(shared_ptr<set<string>> id_vec)
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        strategy_conf * strategy = p_data->_conf->_strategy->current();

        auto id_block = make_shared<set<string>>();

        for (auto it = id_vec->begin(); it != id_vec->end();)
        {
            if (p_data->_block_set->do_check_block(*it))
            {
                id_block->insert(*it);
                LOG_DEBUG("id: %s is blocked", it->c_str());

                id_vec->erase(it++);
            }
            else
            {
                ++it;
            }
        }

        if (!id_block->empty())
        {
                shared_ptr<destroy_msg>  net_obj(new destroy_msg);
                net_obj->id_vec = id_block;
                net_obj->_msg_op = NORMAL_MSG_DESTROY_QT;

                common_obj_container * net_container = get_net_container();
                ObjId oid; 
                oid._id = OBJ_ID_THREAD;
                oid._thread_index = net_container->get_thread_index();
                shared_ptr<normal_msg> ng = static_pointer_cast<normal_msg>(net_obj);
                base_net_thread::put_obj_msg(oid, ng);
        }

        shared_ptr<req_msg> rmsg = make_shared<req_msg>(NORMAL_MSG_REQ_QUOTATION);

        rmsg->_headers.insert(make_pair("User-Agent", 
                    _ua_dic->_ua_vec[_quotation_index % _ua_dic->_ua_vec.size()]));

        rmsg->_headers.insert(make_pair("Accept", "*/*"));
        rmsg->_id_vec = id_vec;

        worker_thread::put_req_msg(rmsg, _quotation_index);
    }
}

void skhttp_req_thread::req_real_single(string id)
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        strategy_conf * strategy = p_data->_conf->_strategy->current();

        if (p_data->_block_set->do_check_block(id))
        {

            shared_ptr<destroy_msg>  net_obj(new destroy_msg);
            auto id_vec = make_shared<set<string>>();
            id_vec->insert(id);

            net_obj->id_vec = id_vec;
            net_obj->_msg_op = NORMAL_MSG_DESTROY_ST;

            common_obj_container * net_container = get_net_container();
            ObjId oid; 
            oid._id = OBJ_ID_THREAD;
            oid._thread_index = net_container->get_thread_index();
            shared_ptr<normal_msg> ng = static_pointer_cast<normal_msg>(net_obj);
            base_net_thread::put_obj_msg(oid, ng);

            LOG_DEBUG("id: %s is blocked", id.c_str());
            return;
        }

        shared_ptr<req_msg> rmsg = make_shared<req_msg>(NORMAL_MSG_REQ_SINGLE);


        rmsg->_headers.insert(make_pair("User-Agent", 
                    _ua_dic->_ua_vec[_single_index % _ua_dic->_ua_vec.size()]));

        rmsg->_headers.insert(make_pair("Accept", "*/*"));
        rmsg->_id_vec = make_shared<set<string>>();
        rmsg->_id_vec->insert(id);

        worker_thread::put_req_msg(rmsg, _single_index);
    }
}

void skhttp_req_thread::req_real_main_funds(string id)
{
    proc_data* p_data = proc_data::instance();
    if (p_data)
    {
        strategy_conf * strategy = p_data->_conf->_strategy->current();

        if (p_data->_block_set->do_check_block(id))
        {

            shared_ptr<destroy_msg>  net_obj(new destroy_msg);
            auto id_vec = make_shared<set<string>>();
            id_vec->insert(id);

            net_obj->id_vec = id_vec;
            net_obj->_msg_op = NORMAL_MSG_DESTROY_MF;

            common_obj_container * net_container = get_net_container();
            ObjId oid; 
            oid._id = OBJ_ID_THREAD;
            oid._thread_index = net_container->get_thread_index();
            shared_ptr<normal_msg> ng = static_pointer_cast<normal_msg>(net_obj);
            base_net_thread::put_obj_msg(oid, ng);

            LOG_DEBUG("id: %s is blocked", id.c_str());
            return;
        }

        shared_ptr<req_msg> rmsg = make_shared<req_msg>(NORMAL_MSG_REQ_MAIN_FUNDS);


        rmsg->_headers.insert(make_pair("User-Agent", 
                    _ua_dic->_ua_vec[_main_funds_index % _ua_dic->_ua_vec.size()]));

        rmsg->_headers.insert(make_pair("Accept", "*/*"));
        rmsg->_id_vec = make_shared<set<string>>();
        rmsg->_id_vec->insert(id);

        worker_thread::put_req_msg(rmsg, _main_funds_index);
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

    if (_req_quotation_circle_times < p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
        return false;


    time_t now = time(NULL);
    if (strategy->per_day_perdump_with_circles && _req_quotation_circle_times % strategy->per_day_perdump_with_circles == 0 && now < dump_real_time)
    {
        return true;
    }

    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_quotation_path.c_str(), _trade_date.c_str());
    int d = access(t_buf, F_OK);
    struct stat st;
    stat(t_buf, &st);

    if (!d && st.st_mtime > dump_real_time) {
        return false;
    } 

    if (_holiday_dict->is_trade_date(_req_date.c_str())){
        if (now > dump_real_time)
            return true;
    }else {
        return true;
    }

    return false;
}


bool skhttp_req_thread::need_update_wquotation_dict()
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char path_buf[SIZE_LEN_512]; 

    if (_history_wquotation_num != strategy->history_wquotation_num)
    {
        _history_wquotation_num = strategy->history_wquotation_num;
        return true;
    }

    path_buf[0] = '\0';
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_wquotation_path.c_str(), strategy->history_wquotation_file.c_str());
    int d = access(path_buf, F_OK);
    if (!d)
    {   
        return false;
    }   

    return true;
}

bool skhttp_req_thread::need_update_wsingle_dict()
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char path_buf[SIZE_LEN_512]; 

    if (_history_wsingle_num != strategy->history_wsingle_num)
    {
        _history_wsingle_num = strategy->history_wsingle_num;
        return true;
    }

    path_buf[0] = '\0';
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_wsingle_path.c_str(), strategy->history_wsingle_file.c_str());
    int d = access(path_buf, F_OK);
    if (!d)
    {   
        return false;
    }   

    return true;
}

bool skhttp_req_thread::need_update_quotation_dict()
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char path_buf[SIZE_LEN_512]; 

    if (_history_quotation_num != strategy->history_quotation_num)
    {
        _history_quotation_num = strategy->history_quotation_num;
        return true;
    }

    path_buf[0] = '\0';
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_quotation_path.c_str(), strategy->history_quotation_file.c_str());
    int d = access(path_buf, F_OK);
    if (!d)
    {   
        return false;
    }   

    return true;
}

bool skhttp_req_thread::need_update_single_dict()
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char path_buf[SIZE_LEN_512]; 

    if (_history_single_num != strategy->history_single_num)
    {
        _history_single_num = strategy->history_single_num;
        return true;
    }

    path_buf[0] = '\0';
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_single_path.c_str(), strategy->history_single_file.c_str());
    int d = access(path_buf, F_OK);
    if (!d)
    {   
        return false;
    }   

    return true;
}

bool skhttp_req_thread::need_update_main_funds_dict()
{
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    char path_buf[SIZE_LEN_512]; 

    if (_history_main_funds_num != strategy->history_main_funds_num)
    {
        _history_main_funds_num = strategy->history_main_funds_num;
        return true;
    }

    path_buf[0] = '\0';
    snprintf(path_buf, sizeof(path_buf), "%s/%s", strategy->history_main_funds_path.c_str(), strategy->history_main_funds_file.c_str());
    int d = access(path_buf, F_OK);
    if (!d)
    {   
        return false;
    }   

    return true;
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

    if (_req_single_circle_times < p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
        return false;


    time_t now = time(NULL);
    if (strategy->per_day_perdump_with_circles && _req_single_circle_times % strategy->per_day_perdump_with_circles == 0 && now < dump_real_time)
    {
        return true;
    }

    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_single_path.c_str(), _trade_date.c_str());
    int d = access(t_buf, F_OK);
    struct stat st;
    stat(t_buf, &st);

    if (!d && st.st_mtime > dump_real_time) {
        return false;
    }

    if (_holiday_dict->is_trade_date(_req_date.c_str())){
        if (now > dump_real_time)
            return true;
    }else {
        return true;
    }

    return false;
}

bool skhttp_req_thread::need_dump_real_main_funds()
{
    char t_buf[SIZE_LEN_512];

    proc_data* p_data = proc_data::instance();
    if (!p_data)
        return false;

    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    if (!_holiday_dict->is_trade_date(_trade_date.c_str()))
        return false;

    if (_req_main_funds_circle_times < p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
        return false;


    time_t now = time(NULL);
    if (strategy->per_day_perdump_with_circles && _req_main_funds_circle_times % strategy->per_day_perdump_with_circles == 0 && now < dump_real_time)
    {
        return true;
    }

    snprintf(t_buf, sizeof(t_buf), "%s/%s", strategy->real_main_funds_path.c_str(), _trade_date.c_str());
    int d = access(t_buf, F_OK);
    struct stat st;
    stat(t_buf, &st);

    if (!d && st.st_mtime > dump_real_time) {
        return false;
    } 

    if (_holiday_dict->is_trade_date(_req_date.c_str())){
        if (now > dump_real_time)
            return true;
    }else {
        return true;
    }

    return false;
}

bool skhttp_req_thread::need_update_holiday_dict()
{
   proc_data* p_data = proc_data::instance();
   time_t now = time(NULL);

   if (p_data && p_data->_conf)
   {
       holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
       if (now < real_morning_stime)
           return false;

       if (!_holiday_dict->is_trade_date(_trade_date.c_str()))
           return false;

       if (!_req_quotation_circle_times || _req_quotation_circle_times > p_data->_conf->_strategy->current()->per_day_min_req_circle_times)
           return false;


        // 获取比率
        hquotation_search_item * search_index = p_data->_hquotation_index->current();
        int index = 0;
        int len = 0;
        int count = 0;
        int sum = search_index->id_quotation.size();
        if (!sum)
            return false;

        for (auto ii = search_index->id_quotation.begin(); ii != search_index->id_quotation.end(); ii++)
        {
            const string & id = ii->first;
            const deque< shared_ptr<quotation_t>> & tt = ii->second;
            len = ii->second.size();
            index = search_index->get_index(id, _trade_date);
            if (index < 0 || len < 2)
                continue;

            if (tt[len - 1 - 1]->end == tt[len - 1]->end) 
                count++;

            if (count *1.0/sum > p_data->_conf->_strategy->current()->is_not_trade_date_ratio)
                return true;
        }


       return false;
   }

   return false;
}


void skhttp_req_thread::handle_timeout(shared_ptr<timer_msg> & t_msg)
{
    LOG_DEBUG("handle_timeout: timer_id:%u timer_type:%u", t_msg->_timer_id, t_msg->_timer_type);
    proc_data* p_data = proc_data::instance();
    strategy_conf * strategy = p_data->_conf->_strategy->current();
    time_t now = time(NULL);
    switch (t_msg->_timer_type)
    {
        case TIMER_TYPE_REQ_QUOTATION:
            {
                bool flag = false;

                _id_dic = p_data->_id_dict->current();
                _ua_dic = p_data->_ua_dict->current();

                if (is_real_time(_req_quotation_circle_times)) {
                    _req_quotation = true;
                    _quotation_index = 0;
                    _quotation_destroy_num = 0;
                } else
                    add_quotation_timer();

                if (need_dump_real_quotation())
                {
                    p_data->_hquoation_dict->dump();
                    //p_data->_hwquoation_dict->dump();

                    flag = true;
                }


                if ((flag && now > dump_real_time ) || need_update_quotation_dict()) 
                {
                    p_data->_hquoation_dict->update();

                    update_lrussr_search_index();
                }

                // if ((flag && now > dump_real_time ) || need_update_wquotation_dict())
                // {
                //     p_data->_hwquoation_dict->update();
                // }

                if (need_update_holiday_dict())
                {
                    p_data->_holiday_dict->current()->update();
                }

                //if (need_backup())
                //{
                    //backup();
                //}

                //notify_update_date();

            }
            break;
        case TIMER_TYPE_REQ_SINGLE:
            {
                bool flag = false;
                if (is_real_time(_req_single_circle_times)) {
                    _req_single = true;
                    _single_index = 0;
                    _single_destroy_num = 0;
                } else
                    add_single_timer();

                if (need_dump_real_single())
                {
                    p_data->_hsingle_dict->dump();
                    //p_data->_hwsingle_dict->dump();

                    flag = true;
                }

                if ((flag && now > dump_real_time ) || need_update_single_dict()) 
                {
                    p_data->_hsingle_dict->update();
                }

                // if ((flag && now > dump_real_time ) || need_update_wsingle_dict())
                // {
                //     p_data->_hwsingle_dict->update();
                // }
            }
            break;
        case TIMER_TYPE_REQ_MAIN_FUNDS:
            {
                bool flag = false;
                if (is_real_time(_req_main_funds_circle_times)) {
                    _req_main_funds = true;
                    _main_funds_index = 0;
                    _main_funds_destroy_num = 0;

                } else
                    add_main_funds_timer();

                if (need_dump_real_main_funds())
                {
                    p_data->_hmain_funds_dict->dump();
                    //p_data->_hwmain_funds_dict->dump();

                    flag = true;
                }

                if ((flag && now > dump_real_time ) || need_update_main_funds_dict()) 
                {
                    p_data->_hmain_funds_dict->update();
                }

                //if (flag || need_update_wmain_funds_dict())
                //{
                 //   p_data->_hwmain_funds_dict->update();
                //}
            }
            break;
        case TIMER_TYPE_QUOTATION_IDLE_2_CURRENT:
            {
                LOG_DEBUG("QUOTATION_IDLE_2_CURRENT");
                p_data->_hquoation_dict->update_search_index();
                //p_data->_hwquoation_dict->update_search_index();

                add_quotation_timer();
            }
            break;
        case TIMER_TYPE_SINGLE_IDLE_2_CURRENT:
            {
                //先更新再切换
                LOGDEBUG << "SINGLE_IDLE_2_CURRENT" << "stream test";
                p_data->_hsingle_dict->update_search_index();
                //p_data->_hwsingle_dict->update_search_index();

                add_single_timer();
            }
            break;
        case TIMER_TYPE_MAIN_FUNDS_IDLE_2_CURRENT:
            {
                //先更新再切换
                //LOG_DEBUG("main_funds_IDLE_2_CURRENT");
                LOGDEBUG << "main_funds_IDLE_2_CURRENT" << "stream test";
                p_data->_hmain_funds_dict->update_search_index();
                //p_data->_hwmain_funds_dict->update_search_index();

                add_main_funds_timer();
            }
            break;
        case TIMER_TYPE_DESTROY_IDLE:
            {
                p_data->_wtdate_set->destroy_idle();
                p_data->_block_set->destroy_idle();
                p_data->_lrussr_index->destroy_idle();
                p_data->destroy_idle();
                p_data->_conf->destroy_idle();
            }
            break;
        case TIMER_TYPE_RELOAD_CONF:
            {
                if (p_data->reload() || p_data->_conf->reload())
                {
                    add_destroy_idle_timer();
                }

                reload_timer_start();

                malloc_trim(0);
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

void skhttp_req_thread::update_week_tdate_search_index()
{
    proc_data* p_data = proc_data::instance();

    p_data->_wtdate_set->destroy_idle();

    strategy_conf * strategy = p_data->_conf->_strategy->current();
    int num = strategy->history_wsingle_num > strategy->history_wquotation_num? strategy->history_wsingle_num:strategy->history_wquotation_num;

    struct dirent *r;
    DIR *p;
    int i = 0;
    set<string> files;

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
            files.insert(r->d_name);
        }
    } 
    closedir(p);


    i = 0;
    string yw;
    for (auto ii = files.rbegin(); ii != files.rend(); ii++)
    {
        string date = basename((char *)ii->c_str());
        string yw_tmp;
        get_yearweek(date, yw_tmp);
        if (yw.empty())
        {
            if (i >= num)
            {
                break;
            }

            p_data->_wtdate_set->add_week_date(yw_tmp, date);
            i++;
        }
        else if (yw != yw_tmp)
        {
            if (i >= num)
            {
                break;
            }

            p_data->_wtdate_set->add_week_date(yw_tmp, date);
            i++;
        }
        else
        {
            p_data->_wtdate_set->add_week_date(yw_tmp, date);
        }

        yw = yw_tmp;
    }

    string date;
    holiday_dict * _holiday_dict = p_data->_holiday_dict->current();
    _holiday_dict->get_trade_date(date);
    string yw_tmp;
    get_yearweek(date, yw_tmp);
    p_data->_wtdate_set->add_week_date(yw_tmp, date);

    p_data->_wtdate_set->idle_2_current();
}

void skhttp_req_thread::add_destroy_idle_timer()
{
    proc_data* p_data = proc_data::instance();
    shared_ptr<timer_msg> t_msg(new timer_msg);

    t_msg->_timer_type = TIMER_TYPE_DESTROY_IDLE;
    t_msg->_time_length = p_data->_conf->_strategy->current()->destroy_idle_millisecond;
    t_msg->_obj_id = OBJ_ID_THREAD;
    add_timer(t_msg);
}

void skhttp_req_thread::update_block_set_search_index()
{
    proc_data* p_data = proc_data::instance();
    p_data->_block_set->destroy_idle();
    p_data->_block_set->idle_2_current();
}

void skhttp_req_thread::update_lrussr_search_index()
{
    proc_data* p_data = proc_data::instance();
    p_data->_lrussr_index->destroy_idle();
    p_data->_lrussr_index->idle_2_current();
}


void skhttp_req_thread::reload_timer_start()
{
    proc_data* p_data = proc_data::instance();

    if (p_data && p_data->_conf)
    {
        std::shared_ptr<timer_msg> t_msg(new timer_msg);

        t_msg->_timer_type = TIMER_TYPE_RELOAD_CONF;
        t_msg->_time_length = p_data->_conf->_strategy->current()->reload_millisecond;
        t_msg->_obj_id = OBJ_ID_THREAD;
        add_timer(t_msg);
    }
}
