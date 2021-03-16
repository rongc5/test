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
        reload_thread();

        virtual void run_process();

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        void reload_timer_start();

        void destroy_idle_start();

        void dump_real_quotation();

        void dump_real_single();

        void update_quotation_dict();

        void update_single_dict();

    private:
        bool _is_first;
        int count;
};




#endif
