#include "reload_thread.h"
#include <malloc.h>
#include "history_quotation_dict.h"
#include "history_single_dict.h"
#include "history_wquotation_dict.h"
#include "history_wsingle_dict.h"
#include "holiday_dict.h"
#include "id_dict.h"

reload_thread::reload_thread()
{
    _is_first =  false;
}

void reload_thread::run_process()
{
    if (!_is_first)
    {
        _is_first = true;
        reload_timer_start();
    }
}

void reload_thread::handle_msg(std::shared_ptr<normal_msg> & p_msg)
{
    
    proc_data* p_data = proc_data::instance();
    if (!p_data || p_msg == nullptr)
        return;

#if 0
    switch(p_msg->_msg_op)
    {
        case NORMAL_MSG_QUOTATION_IDLE_2_CURRENT:
            {
                //LOG_DEBUG("QUOTATION_IDLE_2_CURRENT");
                //p_data->_hquoation_dict->update_search_index();
                //p_data->_hwquoation_dict->update_search_index();
            }
            break;
        case NORMAL_MSG_SINGLE_IDLE_2_CURRENT:
            {
                //LOG_DEBUG("SINGLE_IDLE_2_CURRENT");
                //p_data->_hsingle_dict->update_search_index();
                //p_data->_hwsingle_dict->update_search_index();
            }
            break;
        case NORMAL_MSG_QUOTATION_DUMP:
            {
                //p_data->_hquoation_dict->dump();
                //p_data->_hwquoation_dict->dump();
            }
            break;
        case NORMAL_MSG_SINGLE_DUMP:
            {
                //p_data->_hsingle_dict->dump();
                //p_data->_hwsingle_dict->dump();
            }
            break;
        case NORMAL_MSG_QUOTATION_DICT_UPDATE:
            {
                //p_data->_hquoation_dict->update();
            }
            break;
        case NORMAL_MSG_SINGLE_DICT_UPDATE:
            {
                //p_data->_hsingle_dict->update();
            }
            break;
        case NORMAL_MSG_UPDATE_TRADE_DATE:
            {
                p_data->_hquoation_dict->update();
                p_data->_hsingle_dict->update();

                p_data->_hwquoation_dict->update();
                p_data->_hwsingle_dict->update();
            }
            break;
        case NORMAL_MSG_HOLIDAY_DICT_UPDATE:
            {
                //p_data->_holiday_dict->current()->update();
            }
            break;
        //case NORMAL_MSG_RESPONSE_SINGLE:
            //{    
                //auto p=std::dynamic_pointer_cast<response_req_msg>(p_msg);
                //if (p && p->_st)
                //{    
                    //LOG_DEBUG("NORMAL_MSG_RESPONSE_SINGLE:%s, size:%d", p->_id.c_str(),  p_data->_id_dict->current()->size());
                    //p_data->_hsingle_dict->update_real_single(*p_data->get_trade_date(), p->_id, p->_st);
                    //p_data->_hwsingle_dict->update_real_wsingle(*p_data->get_trade_date(), p->_id, p->_st);
                //}    
            //}    
            //break;
        //case NORMAL_MSG_RESPONSE_QUOTATION:
            //{
                //auto p=std::dynamic_pointer_cast<response_req_msg>(p_msg);
                //if (p && p->_qt)
                //{
                    //if (p->_qt->start <= 1)
                    //{
                        //p_data->_block_set->idle()->insert(p->_id);
                    //}
                    //else
                    //{
                        //LOG_DEBUG("NORMAL_MSG_RESPONSE_QUOTATION:%s", p->_id.c_str(), p_data->_id_dict->current()->size());
                        //p_data->_hquoation_dict->update_real_quotation(*p_data->get_trade_date(), p->_id, p->_qt);
                        //p_data->_hwquoation_dict->update_real_wquotation(*p_data->get_trade_date(), p->_id, p->_qt);
                    //}
                //}
            //}
            //break;
    }
#endif
}

void reload_thread::handle_timeout(std::shared_ptr<timer_msg> & t_msg)
{
    LOG_DEBUG("handle_timeout timer_type:%d, time_length:%d", t_msg->_timer_type, t_msg->_time_length);
    proc_data* p_data = proc_data::instance();
    if (t_msg->_timer_type == TIMER_TYPE_RELOAD_CONF && p_data)
    {
        if (p_data->reload() || p_data->_conf->reload())
        {
            destroy_idle_start();
        }
        else
        {
            reload_timer_start();
        }

        malloc_trim(0);
    }
    else if (t_msg->_timer_type == TIMER_TYPE_DESTROY_IDLE && p_data)
    {
        p_data->destroy_idle();
        p_data->_conf->destroy_idle();

        reload_timer_start();
    }
}

void reload_thread::reload_timer_start()
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

void reload_thread::destroy_idle_start()
{
    proc_data* p_data = proc_data::instance();

    if (p_data && p_data->_conf)
    {
        std::shared_ptr<timer_msg> t_msg(new timer_msg);

        t_msg->_timer_type = TIMER_TYPE_DESTROY_IDLE;
        t_msg->_time_length = p_data->_conf->_strategy->current()->destroy_idle_millisecond;
        t_msg->_obj_id = OBJ_ID_THREAD;
        add_timer(t_msg);
    }
}




