#include "reload_thread.h"

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

}

void reload_thread::handle_timeout(timer_msg & t_msg)
{
    LOG_DEBUG("handle_timeout timer_type:%d, time_length:%d", t_msg._timer_type, t_msg._time_length);
    proc_data* p_data = proc_data::instance();
    if (t_msg._timer_type == TIMER_TYPE_RELOAD_CONF && p_data)
    {
        p_data->_conf->reload();
        p_data->reload();
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




