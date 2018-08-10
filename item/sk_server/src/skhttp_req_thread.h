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

        skhttp_req_thread();

        virtual void handle_msg(std::shared_ptr<normal_msg> & p_msg);

        void real_req_start();

        void single_reset();
    
        void do_single();

        void add_single_timer();

        void quotation_reset();

        void do_quotation();

        void add_quotation_timer();

        void first_in_day();

        bool is_trade_date(const char * date);

        bool is_real_time();

        void req_real_quotation(const std::string & id);

        void req_real_single(const std::string & id);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void run_process();

    protected:
        time_t get_real_time(const char * time);

        void get_trade_date();

        bool need_dump_real_quotation();

        bool need_dump_real_single();

        void dump_real_quotation();
        void dump_real_single();

    protected:
        std::string _req_date;
        std::string _trade_date;
        uint32_t _quotation_index;
        bool _req_quotation;

        uint32_t _single_index;
        bool _req_single;

        time_t real_morning_stime;
        time_t real_morning_etime;
        
        time_t real_afternoon_stime;
        time_t real_afternoon_etime;

        time_t dump_real_time;

};





#endif
