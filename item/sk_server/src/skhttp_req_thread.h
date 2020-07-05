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
    
        void do_single();

        void add_single_timer();

        void add_destroy_idle_timer();

        void do_quotation();

        void add_quotation_timer();

        void first_in_day();

        bool is_real_time();

        void req_real_quotation(const std::string & id);

        void req_real_single(const std::string & id);

        virtual void handle_timeout(std::shared_ptr<timer_msg> & t_msg);

        virtual void run_process();

    protected:
        time_t get_real_time(const char * date, const char * time);

        bool need_dump_real_quotation();
        bool need_dump_real_single();

        void dump_real_quotation();
        void dump_real_single();

        bool need_update_quotation_dict();
        bool need_update_single_dict();

        void update_quotation_dict();
        void update_single_dict();

        bool need_update_wquotation_dict();
        bool need_update_wsingle_dict();

        void update_wquotation_dict();
        void update_wsingle_dict();

        bool need_backup();
        void backup();

        void update_week_tdate_search_index();

        void update_block_set_search_index();

        void update_lrussr_search_index();

    protected:
        std::string _req_date;
        std::string _trade_date;
        uint32_t _quotation_index; //request id_index in id_dict
        bool _req_quotation;

        uint32_t _single_index;
        bool _req_single;

        uint32_t _quotation_destroy_num;//response id_index in id_dict
        uint32_t _single_destroy_num;


        time_t real_morning_stime;
        time_t real_morning_etime;
        
        time_t real_afternoon_stime;
        time_t real_afternoon_etime;

        time_t dump_real_time;

        uint32_t _history_quotation_num;
        uint32_t _history_single_num;

        uint32_t _history_wquotation_num;
        uint32_t _history_wsingle_num;

        time_t backup_stime;

        bool is_backuped;

        uint32_t _req_circle_times;//each day req circle times;

        id_dict * _id_dic;
        ua_dict * _ua_dic;
};





#endif
