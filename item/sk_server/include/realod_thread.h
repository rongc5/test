#ifndef __RELOAD_THREAD_H_
#define __RELOAD_THREAD_H_

#include "base_def.h"
#include "base_net_thread.h"
#include "common_util.h"
#include "common_def.h"
#include "log_helper.h"
#include "proc_data.h"
#include "sk_def.h"

class reload_thread:public base_net_thread
{
    public:
        reload_thread()
        {
            _is_first =  false;
        }

        virtual void run_process()
        {
            if (!_is_first)
            {
                _is_first = true;
                reload_timer_start();
            }
        }

        virtual bool handle_timeout(timer_msg & t_msg)
        {
            LOG_DEBUG("handle_timeout timer_type:%d, time_length:%d", t_msg._timer_type, t_msg._time_length);
            proc_data* p_data = proc_data::instance();
            if (t_msg._timer_type == TIMER_TYPE_RELOAD_CONF && p_data)
            {
                p_data->reload();
                reload_timer_start();
            }

            return true;
        }

        void reload_timer_start()
        {
            proc_data* p_data = proc_data::instance();

            if (p_data && p_data->_conf)
            {
                timer_msg t_msg;

                t_msg._timer_type = TIMER_TYPE_RELOAD_CONF;
                t_msg._time_length = p_data->_conf->reload_second;
                add_timer(t_msg);
            }
        }

    private:
        bool _is_first;
};




#endif
